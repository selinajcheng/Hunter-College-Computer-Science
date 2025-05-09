#include "PlayerStream.hpp"

// class PlayerStream is abstract, no implementation needed

VectorPlayerStream::VectorPlayerStream(const std::vector<Player>& players)
    : players_(players), index_(0) {}

Player VectorPlayerStream::nextPlayer()
{
    // stub
    return Player();
}

size_t VectorPlayerStream::remaining() const
{
    // stub
    return 0;
}