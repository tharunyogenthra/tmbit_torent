#ifndef tmbit_torrent_bencode_h
#define tmbit_torrent_bencode_h

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <any>

class torrent_info
{
public:
    struct file;

    auto set_name(const std::string &name) -> void { name_ = name; }
    auto set_piece_length(int piece_length) -> void { piece_length_ = piece_length; }
    auto set_pieces(const std::vector<std::string> &pieces) -> void { pieces_ = pieces; }
    auto add_file(const file &f) -> void { files_.push_back(f); }

    auto get_name() const -> const std::string & { return name_; }
    auto get_piece_length() const -> int { return piece_length_; }
    auto get_pieces() const -> const std::vector<std::string> & { return pieces_; }
    auto get_files() const -> const std::vector<file> & { return files_; }

    std::string to_string() const;
    friend std::ostream &operator<<(std::ostream &os, const torrent_info &ti);

    struct file
    {
        int length;
        std::vector<std::string> path;

        auto to_string() const -> std::string;
        friend std::ostream &operator<<(std::ostream &os, const file &f);
    };

private:
    std::vector<file> files_;
    std::string name_;
    int piece_length_;
    std::vector<std::string> pieces_;
};

class bencode
{
public:
    void set_announce(const std::string &announce) { announce_ = announce; }
    void set_announce_list(const std::vector<std::string> &announce_list) { announce_list_ = announce_list; }
    void set_comment(const std::string &comment) { comment_ = comment; }
    void set_created_by(const std::string &created_by) { created_by_ = created_by; }
    void set_creation_date(int creation_date) { creation_date_ = creation_date; }
    void set_url_list(const std::vector<std::string> &url_list) { url_list_ = url_list; }
    void set_info(const torrent_info &info) { info_ = info; }
    void set_encoding(const std::string &encoding) { encoding_ = encoding; }

    auto get_announce() const -> const std::string & { return announce_; }
    auto get_announce_list() const -> const std::vector<std::string> & { return announce_list_; }
    auto get_comment() const -> const std::string & { return comment_; }
    auto get_created_by() const -> const std::string & { return created_by_; }
    auto get_creation_date() const -> int { return creation_date_; }
    auto get_encoding() const -> const std::string & { return encoding_; }
    auto get_url_list() const -> const std::vector<std::string> & { return url_list_; }
    auto get_info() const -> const torrent_info & { return info_; }

    std::string to_string() const;
    friend std::ostream &operator<<(std::ostream &os, const bencode &b);

private:
    std::string announce_;
    std::vector<std::string> announce_list_;
    std::string comment_;
    std::string created_by_;
    int creation_date_;
    std::string encoding_;
    std::vector<std::string> url_list_;
    torrent_info info_;
};

#endif // tmbit_torrent_bencode_h
