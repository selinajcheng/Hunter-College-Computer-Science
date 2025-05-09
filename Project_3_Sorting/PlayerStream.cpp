#include "PlayerStream.hpp"

// class PlayerStream is abstract, no implementation needed

VectorPlayerStream::VectorPlayerStream(const std::vector<Player>& players)
    : players_(players), index_(0) {}

Player VectorPlayerStream::nextPlayer()
{
    if (index_ >= players_.size()) {
        throw std::runtime_error("No players left in stream.");
    }

    return players_[index_++];
}

size_t VectorPlayerStream::remaining() const
{
    return players_.size() - index_;
}