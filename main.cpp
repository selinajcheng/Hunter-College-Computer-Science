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

#include <iostream>
#include <string>
#include <vector>
#include "Item.hpp"
#include "Compare.hpp"
#include "ItemGenerator.hpp"
#include "HashInventory.hpp"
#include "TreeInventory.hpp"

// Test utility functions
void printTestResult(const std::string& testName, bool passed) {
    std::cout << (passed ? "\033[32m[PASS] " : "\033[31m[FAIL] ") 
              << testName << "\033[0m" << std::endl;
}

void runItemTests() {
    std::cout << "\n=== Item Class Tests ===\n";
    
    // Test 1: Basic construction
    Item sword("Excalibur", 5.5, ItemType::WEAPON);
    bool passed = (sword.name_ == "Excalibur") && 
                 (sword.weight_ == 5.5f) && 
                 (sword.type_ == ItemType::WEAPON);
    printTestResult("Basic Construction", passed);

    // Test 2: Equality operator
    Item sword2("Excalibur", 6.0, ItemType::ARMOR); // Same name, different attributes
    Item axe("Battle Axe", 7.0, ItemType::WEAPON);
    passed = (sword == sword2) && !(sword == axe);
    printTestResult("Equality Operator", passed);

    // Test 3: Hash functionality
    std::unordered_set<Item> items;
    items.insert(sword);
    items.insert(sword2); // Should not insert (same name)
    passed = (items.size() == 1);
    printTestResult("Hash Uniqueness", passed);
}

void runComparatorTests() {
    std::cout << "\n=== Comparator Tests ===\n";
    
    Item apple("Apple", 1.0, ItemType::ACCESSORY);
    Item banana("Banana", 2.0, ItemType::ACCESSORY);
    Item appleHeavy("Apple", 3.0, ItemType::WEAPON);

    // Test 1: Name comparison
    bool namePassed = CompareItemName::lessThan(apple, banana) &&
                     !CompareItemName::lessThan(banana, apple);
    printTestResult("Name Comparison", namePassed);

    // Test 2: Weight comparison
    bool weightPassed = CompareItemWeight::lessThan(apple, banana) &&
                        CompareItemWeight::equal(apple, appleHeavy);
    printTestResult("Weight Comparison", weightPassed);

    // Test 3: Type comparison
    bool typePassed = CompareItemType::lessThan(apple, appleHeavy) &&
                      !CompareItemType::lessThan(appleHeavy, apple);
    printTestResult("Type Comparison", typePassed);
}

void runHashInventoryTests() {
    std::cout << "\n=== HashInventory Tests ===\n";
    
    Inventory<CompareItemName, std::unordered_set<Item>> inv;
    Item sword("Sword", 5.0, ItemType::WEAPON);
    Item shield("Shield", 8.0, ItemType::ARMOR);

    // Test 1: Basic operations
    bool pickupPassed = inv.pickup(sword) && 
                       !inv.pickup(Item("Sword", 6.0, ItemType::WEAPON)) &&
                       inv.pickup(shield);
    printTestResult("Pickup Items", pickupPassed);

    // Test 2: Weight tracking
    bool weightPassed = (inv.getWeight() == 13.0f);
    printTestResult("Weight Tracking", weightPassed);

    // Test 3: Query functionality
    auto result = inv.query(Item("Shield", 0, ItemType::NONE), 
                   Item("Sword", 0, ItemType::NONE));
    bool queryPassed = (result.size() == 2);
    printTestResult("Range Query", queryPassed);
}

void runTreeInventoryTests() {
    std::cout << "\n=== TreeInventory Tests ===\n";
    
    Inventory<CompareItemWeight, Tree> inv;
    Item light("Feather", 0.5, ItemType::ACCESSORY);
    Item medium("Sword", 5.0, ItemType::WEAPON);
    Item heavy("Anvil", 20.0, ItemType::ARMOR);

    // Test 1: Ordered insertion
    inv.pickup(medium);
    inv.pickup(light);
    inv.pickup(heavy);
    bool orderPassed = (inv.getItems().root()->value_.weight_ == 5.0f); // Root should be medium
    printTestResult("AVL Ordering", orderPassed);

    // Test 2: Weight-based query
    auto result = inv.query(Item("", 1.0, ItemType::NONE), 
                   Item("", 10.0, ItemType::NONE));
    bool queryPassed = (result.size() == 1) && 
                      (result.begin()->name_ == "Sword");
    printTestResult("Weight Query", queryPassed);
}

void runItemGeneratorTests() {
    std::cout << "\n=== ItemGenerator Tests ===\n";
    
    ItemGenerator gen(42);
    auto items = gen.getRandomItems(100);

    // Test 1: Unique names
    std::unordered_set<std::string> names;
    for (const auto& item : items) names.insert(item.name_);
    bool uniquePassed = (names.size() == 100);
    printTestResult("Unique Names", uniquePassed);

    // Test 2: Weight range
    bool weightPassed = true;
    for (const auto& item : items) {
        if (item.weight_ < ItemGenerator::MIN_WEIGHT || 
            item.weight_ > ItemGenerator::MAX_WEIGHT) {
            weightPassed = false;
            break;
        }
    }
    printTestResult("Weight Range", weightPassed);
}

void runIntegrationTest() {
    std::cout << "\n=== Integration Test ===\n";
    
    // Create inventory with 1000 random items
    ItemGenerator gen(123);
    Inventory<CompareItemName, Tree> inv;
    auto items = gen.getRandomItems(1000);
    for (const auto& item : items) inv.pickup(item);

    // Verify size and weight
    bool sizePassed = (inv.size() == 1000);
    printTestResult("Large Inventory Size", sizePassed);

    // Query test
    Item start("Item_0", 0, ItemType::NONE);
    Item end("Item_999", 0, ItemType::NONE);
    auto result = inv.query(start, end);
    bool queryPassed = (result.size() == 1000);
    printTestResult("Large Query", queryPassed);
}

int main() {
    std::cout << "=== Inventory System Test Suite ===\n";
    
    runItemTests();
    runComparatorTests();
    runHashInventoryTests();
    runTreeInventoryTests();
    runItemGeneratorTests();
    runIntegrationTest();

    std::cout << "\n=== All Tests Completed ===\n";
    return 0;
}