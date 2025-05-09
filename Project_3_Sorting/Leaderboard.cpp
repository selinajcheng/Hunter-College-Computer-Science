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

    RankingResult quickSelectRank(std::vector<Player>& players)
    {
        // stub
        return RankingResult();
    }

    RankingResult heapRank(std::vector<Player>& players)
    {
        // stub
        return RankingResult();
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
            if (child + 1 < last && *(child_it + 1) < *child_it)
            {
                child++;
            }

            // second breakpoint: if heap property's satisfied
            if (!(*child_it < tmp)) break;

            *hole = std::move(*child_it); // move child to hole
            hole = child_it;
        }

        *hole = std::move(tmp);
    }

    RankingResult rankIncoming(PlayerStream& stream, const size_t& reporting_interval)
    {
        // stub
        return RankingResult();
    }

}