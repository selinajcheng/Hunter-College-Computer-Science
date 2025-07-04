#include "Leaderboard.hpp"

/**
 * @brief Constructor for RankingResult with top players, cutoffs, and elapsed time.
 *
 * @param top Vector of top-ranked Player objects, in sorted order.
 * @param cutoffs Map of player count thresholds to minimum level cutoffs.
 *   NOTE: This is only ever non-empty for Online::rankIncoming().
 *         This parameter & the corresponding member should be empty
 *         for all Offline algorithms.
 * @param elapsed Time taken to calculate the ranking, in seconds.
 */
RankingResult::RankingResult(const std::vector<Player>& top, const std::unordered_map<size_t, size_t>& cutoffs, double elapsed)
    : top_ { top }
    , cutoffs_ { cutoffs }
    , elapsed_ { elapsed }
{
}

namespace Offline
{
    // size_t partition(std::vector<Player>& players, size_t left, size_t right)
    // {
    //     // using Hoare's algorithm
    //     Player pivot = players[left + (right - left) / 2];
    //     size_t i = left - 1;
    //     size_t j = right + 1;

    //     while (true)
    //     {
    //         do { i++; } while (players[i] < pivot);
    //         do { j--; } while (players[j] > pivot);

    //         if (i >= j) return j;   // as the new pivot

    //         std::swap(players[i], players[j]);
    //     }
    // }

    // void quickSelect(std::vector<Player>& players, size_t left, size_t right, size_t target_position)
    // {
    //     if (left < right)
    //     {
    //         size_t pivot = partition(players, left, right);

    //         if (pivot - left + 1 < target_position)
    //         {
    //             quickSelect(players, pivot + 1, right, target_position - pivot - left + 1);
    //         }
    //         else if (pivot - left + 1 > target_position)
    //         {
    //             quickSelect(players, left, pivot, target_position);
    //         }
    //     }
    // }

    RankingResult quickSelectRank(std::vector<Player>& players)
    {
        auto start = std::chrono::high_resolution_clock::now();
        const size_t top_10_percent = players.size() / 10;
        if (top_10_percent == 0) return RankingResult();

        std::nth_element(players.begin(), players.begin() + top_10_percent, players.end(), std::greater<Player>());
        std::sort(players.begin(), players.begin() + top_10_percent);

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();

        return RankingResult(std::vector<Player>(players.begin(), players.begin() + top_10_percent), {}, elapsed);
    }

    RankingResult heapRank(std::vector<Player>& players)
    {
        auto start = std::chrono::high_resolution_clock::now();

        const size_t top_10_percent = players.size() / 10;
        if (top_10_percent == 0) return RankingResult();

        // make min-heap for top 10%
        std::make_heap(players.begin(), players.begin() + top_10_percent, std::greater<Player>());

        // iterate through remaining players
        for (size_t i = top_10_percent; i < players.size(); i++)
        {
            if (players[i] > players.front())
            {
                std::pop_heap(players.begin(), players.begin() + top_10_percent, std::greater<Player>());
                players[top_10_percent - 1] = players[i];
                std::push_heap(players.begin(), players.begin() + top_10_percent, std::greater<Player>());
            }
        }

        // sort in ascending order
        std::sort(players.begin(), players.begin() + top_10_percent);

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();

        return RankingResult(std::vector<Player>(players.begin(), players.begin() + top_10_percent), {}, elapsed);
    }

}

namespace Online
{
    void replaceMin(PlayerIt first, PlayerIt last, Player& target)
    {
        if (first == last) return;  // if empty

        Player tmp = std::move(target); // move target to root
        PlayerIt hole = first;
        size_t heap_size = last - first;

        // percolate down starts
        while (true)
        {
            size_t child = 2 * (hole - first) + 1;

            // first breakpoint: if at leaf
            if (child >= heap_size) break;

            PlayerIt child_it = first + child;
            
            // increment only if
            if (child + 1 < heap_size && *(child_it + 1) < *child_it)
            {
                child++;
                child_it++;
            }

            // second breakpoint: if successfully heapified
            if (!(*child_it < tmp)) break;

            // continue percolating down otherwise
            *hole = std::move(*child_it); // move child to hole
            hole = child_it;
        }

        *hole = std::move(tmp);
    }

    RankingResult rankIncoming(PlayerStream& stream, const size_t& reporting_interval)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<Player> top_players;            // instantiate min-heap
        std::unordered_map<size_t, size_t> cutoffs; // track cutoffs
        size_t count = 0;                           // track processed

        // while there are still players to rank
        while (stream.remaining() > 0)
        {
            // get the next player
            Player player = stream.nextPlayer();
            count++;

            if (top_players.size() < reporting_interval)
            {
                top_players.push_back(player);

                // if heap is full
                if (top_players.size() == reporting_interval)
                {
                    std::make_heap(top_players.begin(), top_players.end(), std::greater<Player>());
                }
            }

            else if (player > top_players.front())
            {
                replaceMin(top_players.begin(), top_players.end(), player);
            }

            if (count % reporting_interval == 0)
            {
                cutoffs[count] = top_players.front().level_;
            }
        }

        if (cutoffs.find(count) == cutoffs.end())
        {
            cutoffs[count] = top_players.front().level_;
        }

        std::sort(top_players.begin(), top_players.end());

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();

        return RankingResult(top_players, cutoffs, elapsed);
    }
}