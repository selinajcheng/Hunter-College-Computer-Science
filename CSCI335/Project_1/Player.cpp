#include "Player.hpp"

// Constructor
Player::Player(const std::string& name, const Inventory& inventory)
    : inventory_(inventory), name_(name)
{}

// Copy constructor
Player::Player(const Player& rhs)
    : inventory_(rhs.inventory_), name_(rhs.name_)
{}

// Move constructor
Player::Player(Player&& rhs)
    : inventory_(std::move(rhs.inventory_)), name_(std::move(rhs.name_))
{}

// Copy assignment operator
Player& Player::operator=(const Player& rhs)
{
    if (this != &rhs)
    {
        inventory_ = rhs.inventory_;
        name_ = rhs.name_;
    }
    return *this;
}

// Move assignment operator
Player& Player::operator=(Player&& rhs)
{
    if (this != &rhs)
    {
        inventory_ = std::move(rhs.inventory_);
        name_ = std::move(rhs.name_);
    }
    return *this;
}

// ACCESSORS

std::string Player::getName() const
{
    return name_;
}

Inventory& Player::getInventoryRef()
{
    return inventory_;
}