#include "Inventory.hpp"

// Constructor
Inventory::Inventory(const std::vector<std::vector<Item>>& items, Item* equipped)
    : inventory_grid_(items), equipped_(equipped), weight_(0), item_count_(0)
{
    // Calculate initial weight and item count
    for (const auto& row : inventory_grid_)
    {
        for (const auto& item : row)
        {
            if (item.type_ != NONE)
            {
                weight_ += item.weight_;
                item_count_++;
            }
        }
    }
}

// Copy constructor
Inventory::Inventory(const Inventory& rhs)
    : inventory_grid_(rhs.inventory_grid_), equipped_(nullptr), weight_(rhs.weight_), item_count_(rhs.item_count_)
{
    if (rhs.equipped_)
    {
        equipped_ = new Item(*rhs.equipped_);
    } else
    {
        equipped_ = nullptr;
    }
}

// Move constructor
Inventory::Inventory(Inventory&& rhs)
    : inventory_grid_(std::move(rhs.inventory_grid_)), equipped_(rhs.equipped_), weight_(rhs.weight_), item_count_(rhs.item_count_)
{
    rhs.equipped_ = nullptr;
    rhs.weight_ = 0;
    rhs.item_count_ = 0;
}

// Copy assignment operator
Inventory& Inventory::operator=(const Inventory& rhs)
{
    if (this != &rhs)
    {
        inventory_grid_ = rhs.inventory_grid_;
        weight_ = rhs.weight_;
        item_count_ = rhs.item_count_;

        if (equipped_)
        {
            delete equipped_;
            equipped_ = nullptr;
        }

        if (rhs.equipped_)
        {
            equipped_ = new Item(*rhs.equipped_);
        } else
        {
            equipped_ = nullptr;
        }
    }
    return *this;
}

// Move assignment operator
Inventory& Inventory::operator=(Inventory&& rhs)
{
    if (this != &rhs)
    {
        inventory_grid_ = std::move(rhs.inventory_grid_);
        weight_ = rhs.weight_;
        item_count_ = rhs.item_count_;

        if (equipped_)
        {
            delete equipped_;
            equipped_ = nullptr;
        }

        equipped_ = rhs.equipped_;
        rhs.equipped_ = nullptr;

        rhs.weight_ = 0;
        rhs.item_count_ = 0;
    }
    return *this;
}

// Destructor
Inventory::~Inventory()
{
    if (equipped_)
    {
        delete equipped_;
        equipped_ = nullptr;
    }
}

// ACCESSORS

Item* Inventory::getEquipped() const
{
    return equipped_;
}

std::vector<std::vector<Item>> Inventory::getItems() const
{
    return inventory_grid_;
}

float Inventory::getWeight() const
{
    return weight_;
}

size_t Inventory::getCount() const
{
    return item_count_;
}

Item Inventory::at(const size_t& row, const size_t& col) const
{
    if (row >= inventory_grid_.size() || col >= inventory_grid_[row].size())
    {
        throw std::out_of_range("Row or column out of bounds.");
    }
    return inventory_grid_[row][col];
}

// MUTATORS

void Inventory::equip(Item* itemToEquip)
{
    if (equipped_)
    {
        delete equipped_;
        equipped_ = nullptr;
    }
    equipped_ = itemToEquip;
}

void Inventory::discardEquipped()
{
    if (equipped_)
    {
        delete equipped_;
        equipped_ = nullptr;
    }
}

bool Inventory::store(const size_t& row, const size_t& col, const Item& pickup)
{
    if (row >= inventory_grid_.size() || col >= inventory_grid_[row].size())
    {
        throw std::out_of_range("Row or column out of bounds.");
    }

    if (inventory_grid_[row][col].type_ != NONE)
    {
        return false;
    }

    inventory_grid_[row][col] = pickup;
    weight_ += pickup.weight_;
    item_count_++;
    return true;
}