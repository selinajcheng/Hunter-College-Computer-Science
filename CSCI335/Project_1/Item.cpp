#include "Item.hpp"

/**
 * @brief Constructs a new Item object.
 * @param name A const. ref to a string representing the name of the item.
 * @param weight A const. ref to a float representing the weight of the item.
 * @param type A const ref. to an ItemType specifying the type of the item.
 */
Item::Item(const std::string& name, const float& weight, const ItemType& type) 
    : name_{name}, weight_{weight}, type_{type}{}

/**
 * @brief Checks if two Item objects are equal
 * NOTE: Two Item objects are considered equal if 
 *       their names, weights, and types are equal.
 * 
 * @param rhs The other Item object to compare with.
 * @return True if the two Item objects are equal, false otherwise.
 */
bool Item::operator==(const Item& rhs) const {
    return name_ == rhs.name_ && weight_ == rhs.weight_ && type_ == rhs.type_;
}