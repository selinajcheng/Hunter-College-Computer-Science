#pragma once

#include "Player.hpp"
#include <vector>
#include <algorithm>
#include <iterator>

class Guild {
    private: 
        /**
        * @brief A vector containing the players currently enlisted in the guild.
        */
        std::vector<Player> enlisted_players;
    public:
        /**
         * @brief Constructs a new Guild object. Initializes the enlisted_players vector.
         */
        Guild();

        /**
        * @brief Searches for a player in the guild by name
        * 
        * @param playerName A const reference to the player's name to search for
        * @return std::vector<Player>::iterator An iterator pointing to the found player, or enlisted_players.end() if not found
        */
        std::vector<Player>::iterator findPlayer(const std::string& playerName);

        /**
        * @brief Attempts to enlist a player into the guild
        * 
        * @param player An l-value reference to a Player object whose contents will be moved into the guild
        * @return True if the player was successfully enlisted, false if a player with the same name already exists
        * 
        * @post If successful, player's contents are moved into the guild's enlisted_players 
        *       vector and the original player object is left in a valid but unspecified state.
        *       If unsuccessful, player remains unchanged.
        */
        bool enlistPlayer(Player& player);

        /**
        * @brief Moves a player from this guild to another guild
        * 
        * @param playerName A const reference to the name of the player to move
        * @param target An l-value reference to the destination Guild
        * @return True if the player was successfully moved, 
        *       false if the player doesn't exist in this guild
        *       or if a player with the same name already exists in the target guild
        * 
        * @post If successful, the specified player is moved from 
        *       this guild to the target guild, and their content is erased from the source guild
        *       If unsuccessful, both guilds remain unchanged.
        */
        bool movePlayerTo(const std::string& playerName, Guild& target);

        /**
        * @brief Copies a player from this guild to another guild
        * 
        * @param playerName A const reference to the name of the player to copy
        * @param target An l-value reference to the destination Guild
        * @return True if the player was successfully copied, 
        *         false if the player doesn't exist in this guild
        *         or if a player with the same name already exists in the target guild
        * 
        * @post If successful, a copy of the specified player is added to the target guild.
        *       If unsuccessful, both guilds remain unchanged.
        *       In either case, the original player in this guild remains unchanged.
        */
        bool copyPlayerTo(const std::string& playerName, Guild& target);
};