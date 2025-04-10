// #include "Compare.hpp"
// #include "HashInventory.hpp"
// #include "Inventory.hpp"
// #include "Item.hpp"
// #include "ItemAVL.hpp"
// #include "TreeInventory.hpp"

// int main()
// {
//     return 0;
// }

#include <gtest/gtest.h>
#include "Item.hpp"
#include "ItemGenerator.hpp"
#include "HashInventory.hpp"
#include "TreeInventory.hpp"
#include "Compare.hpp"

// -------------------- Item Tests --------------------
TEST(ItemTest, BasicConstruction) {
    Item sword("Excalibur", 5.5, ItemType::WEAPON);
    EXPECT_EQ(sword.name_, "Excalibur");
    EXPECT_FLOAT_EQ(sword.weight_, 5.5);
    EXPECT_EQ(sword.type_, ItemType::WEAPON);
}

TEST(ItemTest, EqualityOperator) {
    Item a("Phoenix Feather", 0.1, ItemType::ACCESSORY);
    Item b("Phoenix Feather", 0.2, ItemType::ARMOR);
    Item c("Dragon Scale", 0.1, ItemType::ACCESSORY);
    
    EXPECT_TRUE(a == b);  // Same name
    EXPECT_FALSE(a == c); // Different name
}

TEST(ItemTest, HashCollision) {
    std::unordered_set<Item> items;
    items.insert(Item("A", 1.0, ItemType::WEAPON));
    items.insert(Item("A", 2.0, ItemType::ARMOR)); // Same name
    
    EXPECT_EQ(items.size(), 1);
}

// -------------------- Comparator Tests --------------------
TEST(ComparatorTest, NameComparison) {
    Item a("Apple", 1.0, ItemType::WEAPON);
    Item b("Banana", 1.0, ItemType::WEAPON);
    
    EXPECT_TRUE(CompareItemName::lessThan(a, b));
    EXPECT_FALSE(CompareItemName::lessThan(b, a));
}

TEST(ComparatorTest, WeightComparison) {
    Item a("Sword", 5.0, ItemType::WEAPON);
    Item b("Sword", 5.1, ItemType::WEAPON);
    
    EXPECT_TRUE(CompareItemWeight::lessThan(a, b));
    EXPECT_TRUE(CompareItemWeight::equal(a, a));
}

// -------------------- HashInventory Tests --------------------
class HashInventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        inv.pickup(Item("Sword", 5.0, ItemType::WEAPON));
        inv.pickup(Item("Shield", 8.0, ItemType::ARMOR));
    }
    
    Inventory<CompareItemName, std::unordered_set<Item>> inv;
};

TEST_F(HashInventoryTest, BasicOperations) {
    EXPECT_EQ(inv.size(), 2);
    EXPECT_FLOAT_EQ(inv.getWeight(), 13.0);
    
    EXPECT_TRUE(inv.contains("Sword"));
    EXPECT_FALSE(inv.contains("Potion"));
}

TEST_F(HashInventoryTest, QueryFunction) {
    Item start("Shield", 0, ItemType::NONE);
    Item end("Sword", 0, ItemType::NONE);
    auto result = inv.query(start, end);
    
    ASSERT_EQ(result.size(), 2);
    EXPECT_TRUE(result.count(Item("Shield")));
    EXPECT_TRUE(result.count(Item("Sword")));
}

// -------------------- TreeInventory Tests --------------------
class TreeInventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        inv.pickup(Item("Zebra", 3.0, ItemType::ARMOR));
        inv.pickup(Item("Apple", 1.0, ItemType::WEAPON));
        inv.pickup(Item("Mango", 2.0, ItemType::ACCESSORY));
    }
    
    Inventory<CompareItemName, Tree> inv;
};

TEST_F(TreeInventoryTest, OrderedQuery) {
    Item start("Apple", 0, ItemType::NONE);
    Item end("Mango", 0, ItemType::NONE);
    auto result = inv.query(start, end);
    
    ASSERT_EQ(result.size(), 2);
    EXPECT_TRUE(result.count(Item("Apple")));
    EXPECT_TRUE(result.count(Item("Mango")));
    EXPECT_FALSE(result.count(Item("Zebra")));
}

TEST_F(TreeInventoryTest, WeightBasedQuery) {
    Inventory<CompareItemWeight, Tree> weightInv;
    weightInv.pickup(Item("A", 1.0, ItemType::WEAPON));
    weightInv.pickup(Item("B", 2.0, ItemType::WEAPON));
    weightInv.pickup(Item("C", 3.0, ItemType::WEAPON));
    
    Item start("", 1.5, ItemType::NONE);
    Item end("", 2.5, ItemType::NONE);
    auto result = weightInv.query(start, end);
    
    ASSERT_EQ(result.size(), 1);
    EXPECT_TRUE(result.count(Item("B")));
}

// -------------------- ItemAVL Tests --------------------
TEST(ItemAVLTest, ContainsCheck) {
    ItemAVL<CompareItemWeight> tree;
    tree.insert(Item("A", 1.0, ItemType::WEAPON));
    tree.insert(Item("B", 2.0, ItemType::ARMOR));
    
    EXPECT_TRUE(tree.contains("A"));
    EXPECT_FALSE(tree.contains("C"));
}

TEST(ItemAVLTest, BalanceCheck) {
    ItemAVL<CompareItemName> tree;
    for(char c = 'Z'; c >= 'A'; c--) {
        tree.insert(Item(std::string(1, c), 1.0, ItemType::WEAPON));
    }
    
    EXPECT_LE(tree.height(tree.root()), 5); // Height should be O(log n)
}

// -------------------- ItemGenerator Tests --------------------
TEST(ItemGeneratorTest, UniqueNames) {
    ItemGenerator gen(42);
    auto items = gen.getRandomItems(100);
    
    std::unordered_set<std::string> names;
    for(const auto& item : items) {
        names.insert(item.name_);
    }
    
    EXPECT_EQ(names.size(), 100); // All names should be unique
}

TEST(ItemGeneratorTest, WeightRange) {
    ItemGenerator gen(42);
    Item item = gen.randomItem();
    
    EXPECT_GE(item.weight_, ItemGenerator::MIN_WEIGHT);
    EXPECT_LE(item.weight_, ItemGenerator::MAX_WEIGHT);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}