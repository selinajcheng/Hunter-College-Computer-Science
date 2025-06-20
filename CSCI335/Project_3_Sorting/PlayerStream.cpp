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

APIPlayerStream::APIPlayerStream(const size_t& expected_length, const size_t& seed, const size_t& batch_size)
    : expected_length_(expected_length),
      seed_(seed),
      batch_size_(batch_size),
      batch_index_(0),
      local_batch_(),
      cursor_(1)
{}

Player APIPlayerStream::nextPlayer()
{
    if (remaining() == 0)
    {
        throw std::runtime_error("No players left in stream.");
    }

    if (batch_index_ >= local_batch_.size())
    {
        std::string url = SOCKET + "/api?seed=" + std::to_string(seed_) +
                          "&cursor=" + std::to_string(cursor_) +
                          "&batch=" + std::to_string(batch_size_);
        
        auto response = cpr::Get(cpr::Url{url});
        if (response.status_code != 200)
        {
            throw std::runtime_error("API request failed.");
        }

        auto json = nlohmann::json::parse(response.text);
        cursor_ = json["cursor"].template get<size_t>();

        local_batch_.clear();
        for (auto level : json["levels"])
        {
            local_batch_.emplace_back("Player", level.template get<size_t>());
        }

        batch_index_ = 0;
    }

    return local_batch_[batch_index_++];
}

size_t APIPlayerStream::remaining() const
{
    // return expected_length_ - (cursor_ - 1) * batch_size_ - (local_batch_.size() - batch_index_);
    size_t fetched = cursor_ > 1 ? (cursor_ - 1) : 0;
    size_t unread = (local_batch_.size() >= batch_index_) ? (local_batch_.size() - batch_index_) : 0;
    size_t consumed = fetched - unread;

    return (expected_length_ > consumed) ? (expected_length_ - consumed) : 0;
}