#ifndef tmbit_torrent_bencode_decoder_h
#define tmbit_torrent_bencode_decoder_h

#include "bencode_file.hpp"
#include <string>

class bencode_decoder
{
public:
    bencode_decoder(const std::string &path)
    {
        auto t = bencode();
        parse_file(path, t);
        bencode_file = t;
    }

    bencode_decoder() = default;

    bencode get_bencode() { return bencode_file; }

    auto decode_bencode_with_pos(size_t startPos) -> std::pair<std::any, size_t>;

    auto decode_bencode(const std::string &input) -> std::any;

    auto populate_bencode_object(bencode &obj, const std::map<std::string, std::any> &dict) -> void;

    auto populate_torrent_info(torrent_info &info, const std::map<std::string, std::any> &dict) -> void;

    static auto convert_pieces(const std::string &pieces) -> std::vector<std::string>;

    auto parse_file(const std::string &filename, bencode &torrentBencode) -> bool;

private:
    std::string data;
    size_t pos = 0;
    bencode bencode_file;

    auto read_until(char delimiter) -> std::string;

    auto decode_string() -> std::pair<std::string, size_t>;

    auto decode_integer() -> std::pair<int64_t, size_t>;

    auto decode_list() -> std::pair<std::vector<std::any>, size_t>;

    auto decode_dictionary() -> std::pair<std::map<std::string, std::any>, size_t>;

};

#endif // tmbit_torrent_bencode_decoder_h