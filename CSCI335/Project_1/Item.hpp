#pragma once

#include <iostream>
#include <string>

enum ItemType { NONE=0, WEAPON=1, ACCESSORY=2, ARMOR=3};

struct Item {
    std::string name_; // The name of the Item
    float weight_;     // A float representing the weight of the Item
    ItemType type_;    // An enum representing the type of Item
    
     /**
     * @brief Constructs a new Item object.
     * @param name A const. ref to a string representing the name of the item.
     * @param weight A const. ref to a float representing the weight of the item.
     * @param type A const ref. to an ItemType specifying the type of the item.
     */
    Item(const std::string& name="", const float& weight = 0, const ItemType& type = NONE);

    /**
     * @brief Checks if two Item objects are equal
     * NOTE: Two Item objects are considered equal if 
     *       their names, weights, and types are equal.
     * 
     * @param rhs The other Item object to compare with.
     * @return True if the two Item objects are equal, false otherwise.
     */
    bool operator==(const Item& rhs) const;
};