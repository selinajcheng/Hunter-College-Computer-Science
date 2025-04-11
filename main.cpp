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
#include <list>
#include <chrono>
#include <vector>
#include <algorithm>
#include "ItemGenerator.hpp"
#include "Inventory.hpp"
#include "HashInventory.hpp"
#include "TreeInventory.hpp"

using namespace std::chrono;

// Modified timeContains
template <typename InventoryType>
float timeContains(InventoryType& inv, const std::vector<std::string>& names) {
    volatile size_t found = 0; // Prevent optimization
    auto start = high_resolution_clock::now();
    for (const auto& name : names) {
        found += inv.contains(name); // Use result
    }
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count() / 1000.0f;
}

// Timing function for query()
template <typename InventoryType, typename Comparator>
float timeQuery(InventoryType& inv, const Item& start, const Item& end) {
    auto start_time = high_resolution_clock::now();
    inv.query(start, end);
    auto end_time = high_resolution_clock::now();
    return duration_cast<milliseconds>(end_time - start_time).count();
}

void runContainsTests() {
    const std::vector<size_t> n_values = {1000, 2000, 4000, 8000};
    const int test_items = 100;
    
    std::cout << "=== Part A: contains() Performance ===\n";
    std::cout << "n\tVector\tList\tHash\tTree\n";
    
    for (auto n : n_values) {
        // Setup
        ItemGenerator gen(42);
        
        // Create inventory with n items
        Inventory<CompareItemName, std::vector<Item>> vecInv;
        Inventory<CompareItemName, std::list<Item>> listInv;
        Inventory<CompareItemName, std::unordered_set<Item>> hashInv;
        Inventory<CompareItemName, Tree> treeInv;
        
        for (size_t i = 0; i < n; i++) {
            auto item = gen.randomItem();
            vecInv.pickup(item);
            listInv.pickup(item);
            hashInv.pickup(item);
            treeInv.pickup(item);
        }
        
        // Get test names
        std::vector<std::string> contained;
        for (int i = 0; i < test_items; i++) {
            contained.push_back(gen.randomUsedName());
        }
        
        std::vector<std::string> missing;
        for (int i = 0; i < test_items; i++) {
            Item temp = gen.randomItem(); // New item not in inventory
            missing.push_back(temp.name_);
        }
        
        // Time contains()
        float vecTime = (timeContains(vecInv, contained) + timeContains(vecInv, missing)) / 200.0f;
        float listTime = (timeContains(listInv, contained) + timeContains(listInv, missing)) / 200.0f;
        float hashTime = (timeContains(hashInv, contained) + timeContains(hashInv, missing)) / 200.0f;
        float treeTime = (timeContains(treeInv, contained) + timeContains(treeInv, missing)) / 200.0f;
        
        // Output results
        std::cout << n << "\t" 
                  << vecTime << "\t" 
                  << listTime << "\t"
                  << hashTime << "\t"
                  << treeTime << "\n";
    }
}

void runQueryTests() {
    const std::vector<size_t> n_values = {1000, 2000, 4000, 8000};
    const int trials = 10;
    
    std::cout << "\n=== Part B: query() Performance ===\n";
    
    // Name-based queries
    std::cout << "\nName-based queries:\n";
    std::cout << "n\tHash\tTree\n";
    for (auto n : n_values) {
        ItemGenerator gen(42);
        
        Inventory<CompareItemName, std::unordered_set<Item>> hashInv;
        Inventory<CompareItemName, Tree> treeInv;
        
        for (size_t i = 0; i < n; i++) {
            auto item = gen.randomItem();
            hashInv.pickup(item);
            treeInv.pickup(item);
        }
        
        float hashTotal = 0;
        float treeTotal = 0;
        
        for (int i = 0; i < trials; i++) {
            // Get two random names
            std::string name1 = gen.randomUsedName();
            std::string name2 = gen.randomUsedName();
            
            // Ensure proper ordering
            if (name1 > name2) std::swap(name1, name2);
            
            Item start(name1, 0, ItemType::NONE);
            Item end(name2, 0, ItemType::NONE);
            
            hashTotal += timeQuery<decltype(hashInv), CompareItemName>(hashInv, start, end);
            treeTotal += timeQuery<decltype(treeInv), CompareItemName>(treeInv, start, end);
        }
        
        std::cout << n << "\t"
                  << hashTotal/trials << "\t"
                  << treeTotal/trials << "\n";
    }
    
    // Weight-based queries
    std::cout << "\nWeight-based queries:\n";
    std::cout << "n\tHash\tTree\n";
    for (auto n : n_values) {
        ItemGenerator gen(42);
        
        Inventory<CompareItemWeight, std::unordered_set<Item>> hashInv;
        Inventory<CompareItemWeight, Tree> treeInv;
        
        for (size_t i = 0; i < n; i++) {
            auto item = gen.randomItem();
            hashInv.pickup(item);
            treeInv.pickup(item);
        }
        
        float hashTotal = 0;
        float treeTotal = 0;
        
        for (int i = 0; i < trials; i++) {
            float start_weight = gen.randomFloat(ItemGenerator::MIN_WEIGHT, 
                                               ItemGenerator::MAX_WEIGHT - 0.1f);
            float end_weight = start_weight + 0.1f;
            
            Item start("", start_weight, ItemType::NONE);
            Item end("", end_weight, ItemType::NONE);
            
            hashTotal += timeQuery<decltype(hashInv), CompareItemWeight>(hashInv, start, end);
            treeTotal += timeQuery<decltype(treeInv), CompareItemWeight>(treeInv, start, end);
        }
        
        std::cout << n << "\t"
                  << hashTotal/trials << "\t"
                  << treeTotal/trials << "\n";
    }
}

int main() {
    std::cout << "=== Inventory Performance Tests ===\n";
    
    // Part A: contains() tests
    runContainsTests();
    
    // Part B: query() tests
    runQueryTests();
    
    return 0;
}

// #include <iostream>
// #include <string>
// #include <vector>
// #include "Item.hpp"
// #include "Compare.hpp"
// #include "ItemGenerator.hpp"
// #include "HashInventory.hpp"
// #include "TreeInventory.hpp"

// // Test utility functions
// void printTestResult(const std::string& testName, bool passed) {
//     std::cout << (passed ? "\033[32m[PASS] " : "\033[31m[FAIL] ") 
//               << testName << "\033[0m" << std::endl;
// }

// void runItemTests() {
//     std::cout << "\n=== Item Class Tests ===\n";
    
//     // Test 1: Basic construction
//     Item sword("Excalibur", 5.5, ItemType::WEAPON);
//     bool passed = (sword.name_ == "Excalibur") && 
//                  (sword.weight_ == 5.5f) && 
//                  (sword.type_ == ItemType::WEAPON);
//     printTestResult("Basic Construction", passed);

//     // Test 2: Equality operator
//     Item sword2("Excalibur", 6.0, ItemType::ARMOR); // Same name, different attributes
//     Item axe("Battle Axe", 7.0, ItemType::WEAPON);
//     passed = (sword == sword2) && !(sword == axe);
//     printTestResult("Equality Operator", passed);

//     // Test 3: Hash functionality
//     std::unordered_set<Item> items;
//     items.insert(sword);
//     items.insert(sword2); // Should not insert (same name)
//     passed = (items.size() == 1);
//     printTestResult("Hash Uniqueness", passed);
// }

// void runComparatorTests() {
//     std::cout << "\n=== Comparator Tests ===\n";
    
//     Item apple("Apple", 1.0, ItemType::ACCESSORY);
//     Item banana("Banana", 2.0, ItemType::ACCESSORY);
//     Item appleHeavy("Apple", 3.0, ItemType::WEAPON);

//     // Test 1: Name comparison
//     bool namePassed = CompareItemName::lessThan(apple, banana) &&
//                      !CompareItemName::lessThan(banana, apple);
//     printTestResult("Name Comparison", namePassed);

//     // Test 2: Weight comparison
//     bool weightPassed = CompareItemWeight::lessThan(apple, banana) &&
//                         CompareItemWeight::equal(apple, appleHeavy);
//     printTestResult("Weight Comparison", weightPassed);

//     // Test 3: Type comparison
//     bool typePassed = CompareItemType::lessThan(apple, appleHeavy) &&
//                       !CompareItemType::lessThan(appleHeavy, apple);
//     printTestResult("Type Comparison", typePassed);
// }

// void runHashInventoryTests() {
//     std::cout << "\n=== HashInventory Tests ===\n";
    
//     Inventory<CompareItemName, std::unordered_set<Item>> inv;
//     Item sword("Sword", 5.0, ItemType::WEAPON);
//     Item shield("Shield", 8.0, ItemType::ARMOR);

//     // Test 1: Basic operations
//     bool pickupPassed = inv.pickup(sword) && 
//                        !inv.pickup(Item("Sword", 6.0, ItemType::WEAPON)) &&
//                        inv.pickup(shield);
//     printTestResult("Pickup Items", pickupPassed);

//     // Test 2: Weight tracking
//     bool weightPassed = (inv.getWeight() == 13.0f);
//     printTestResult("Weight Tracking", weightPassed);

//     // Test 3: Query functionality
//     auto result = inv.query(Item("Shield", 0, ItemType::NONE), 
//                    Item("Sword", 0, ItemType::NONE));
//     bool queryPassed = (result.size() == 2);
//     printTestResult("Range Query", queryPassed);
// }

// void runTreeInventoryTests() {
//     std::cout << "\n=== TreeInventory Tests ===\n";
    
//     Inventory<CompareItemWeight, Tree> inv;
//     Item light("Feather", 0.5, ItemType::ACCESSORY);
//     Item medium("Sword", 5.0, ItemType::WEAPON);
//     Item heavy("Anvil", 20.0, ItemType::ARMOR);

//     // Test 1: Ordered insertion
//     inv.pickup(medium);
//     inv.pickup(light);
//     inv.pickup(heavy);
//     bool orderPassed = (inv.getItems().root()->value_.weight_ == 5.0f); // Root should be medium
//     printTestResult("AVL Ordering", orderPassed);

//     // Test 2: Weight-based query
//     auto result = inv.query(Item("", 1.0, ItemType::NONE), 
//                    Item("", 10.0, ItemType::NONE));
//     bool queryPassed = (result.size() == 1) && 
//                       (result.begin()->name_ == "Sword");
//     printTestResult("Weight Query", queryPassed);
// }

// void runItemGeneratorTests() {
//     std::cout << "\n=== ItemGenerator Tests ===\n";
    
//     ItemGenerator gen(42);
//     auto items = gen.getRandomItems(100);

//     // Test 1: Unique names
//     std::unordered_set<std::string> names;
//     for (const auto& item : items) names.insert(item.name_);
//     bool uniquePassed = (names.size() == 100);
//     printTestResult("Unique Names", uniquePassed);

//     // Test 2: Weight range
//     bool weightPassed = true;
//     for (const auto& item : items) {
//         if (item.weight_ < ItemGenerator::MIN_WEIGHT || 
//             item.weight_ > ItemGenerator::MAX_WEIGHT) {
//             weightPassed = false;
//             break;
//         }
//     }
//     printTestResult("Weight Range", weightPassed);
// }

// void runIntegrationTest() {
//     std::cout << "\n=== Integration Test ===\n";
    
//     // Create inventory with 1000 random items
//     ItemGenerator gen(123);
//     Inventory<CompareItemName, Tree> inv;
//     auto items = gen.getRandomItems(1000);
//     for (const auto& item : items) inv.pickup(item);

//     // Verify size and weight
//     bool sizePassed = (inv.size() == 1000);
//     printTestResult("Large Inventory Size", sizePassed);

//     // Query test
//     Item start("Item_0", 0, ItemType::NONE);
//     Item end("Item_999", 0, ItemType::NONE);
//     auto result = inv.query(start, end);
//     bool queryPassed = (result.size() == 1000);
//     printTestResult("Large Query", queryPassed);
// }

// int main() {
//     std::cout << "=== Inventory System Test Suite ===\n";
    
//     runItemTests();
//     runComparatorTests();
//     runHashInventoryTests();
//     runTreeInventoryTests();
//     runItemGeneratorTests();
//     runIntegrationTest();

//     std::cout << "\n=== All Tests Completed ===\n";
//     return 0;
// }