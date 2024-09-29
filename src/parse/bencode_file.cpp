#include "bencode_file.hpp"

auto torrent_info::file::to_string() const -> std::string
{
    std::ostringstream oss;
    oss << "{ \"length\": " << length << ", \"path\": [";
    for (size_t i = 0; i < path.size(); ++i)
    {
        oss << "\"" << path[i] << "\"";
        if (i < path.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "] }";
    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const torrent_info::file &f)
{
    os << f.to_string();
    return os;
}

auto torrent_info::to_string() const -> std::string
{
    std::ostringstream oss;
    oss << "{ \"files\": [";
    for (size_t i = 0; i < files_.size(); ++i)
    {
        oss << files_[i].to_string();
        if (i < files_.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "], \"name\": \"" << name_ << "\", \"piece length\": " << piece_length_ << ", \"pieces\": [";
    for (size_t i = 0; i < pieces_.size(); ++i)
    {
        oss << "\"" << pieces_[i] << "\"";
        if (i < pieces_.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "] }";
    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const torrent_info &ti)
{
    os << ti.to_string();
    return os;
}

auto bencode::to_string() const -> std::string
{
    std::ostringstream oss;
    oss << "{ \"announce\": \"" << announce_ << "\", \"announce-list\": [";
    for (size_t i = 0; i < announce_list_.size(); ++i)
    {
        oss << "[\"" << announce_list_[i] << "\"]";
        if (i < announce_list_.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "], \"comment\": \"" << comment_ << "\", \"created by\": \"" << created_by_ << "\", "
        << "\"creation date\": " << creation_date_ << ", \"url-list\": [";
    for (size_t i = 0; i < url_list_.size(); ++i)
    {
        oss << "\"" << url_list_[i] << "\"";
        if (i < url_list_.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << "], \"info\": " << info_.to_string() << " }";
    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const bencode &b)
{
    os << b.to_string();
    return os;
}
