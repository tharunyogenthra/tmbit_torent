#include "bencode_decoder.hpp"

auto bencode_decoder::decode_bencode_with_pos(size_t start_pos) -> std::pair<std::any, size_t>
{
    pos = start_pos;
    if (data[pos] == 'i')
    {
        return decode_integer();
    }
    else if (data[pos] == 'l')
    {
        return decode_list();
    }
    else if (data[pos] == 'd')
    {
        return decode_dictionary();
    }
    else
    {
        return decode_string();
    }
}

auto bencode_decoder::decode_bencode(const std::string &input) -> std::any
{
    data = input;
    pos = 0;
    // structured binding shit from comp6771
    auto [result, _] = decode_bencode_with_pos(0);
    return result;
}

auto bencode_decoder::populate_bencode_object(bencode &obj, const std::map<std::string, std::any> &dict) -> void
{

    if (auto it = dict.find("announce"); it != dict.end())
    {
        try
        {
            obj.set_announce(std::any_cast<std::string>(it->second));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for announce" << std::endl;
        }
    }

    if (auto it = dict.find("announce-list"); it != dict.end())
    {
        try
        {
            
            const auto &announce_list = std::any_cast<std::vector<std::any>>(it->second);
            std::vector<std::string> announce_list_strings;

            for (const auto &inner_list : announce_list)
            {
                
                const auto &inner_vector = std::any_cast<std::vector<std::any>>(inner_list);
                for (const auto &item : inner_vector)
                {
                    if (item.type() == typeid(std::string))
                    {
                        announce_list_strings.push_back(std::any_cast<std::string>(item));
                    }
                    else
                    {
                        std::cerr << "Invalid type in announce-list, expected std::string but got "
                                  << item.type().name() << std::endl;
                    }
                }
            }
            obj.set_announce_list(announce_list_strings);
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for announce-list" << std::endl;
        }
    }

    if (auto it = dict.find("comment"); it != dict.end())
    {
        try
        {
            obj.set_comment(std::any_cast<std::string>(it->second));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for comment" << std::endl;
        }
    }

    if (auto it = dict.find("created by"); it != dict.end())
    {
        try
        {
            obj.set_created_by(std::any_cast<std::string>(it->second));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for created by" << std::endl;
        }
    }

    if (auto it = dict.find("creation date"); it != dict.end())
    {
        try
        {
            obj.set_creation_date(static_cast<int>(std::any_cast<int64_t>(it->second)));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for creation date" << std::endl;
        }
    }

    if (auto it = dict.find("encoding"); it != dict.end())
    {
        try
        {
            obj.set_encoding(std::any_cast<std::string>(it->second));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for encoding" << std::endl;
        }
    }

    if (auto it = dict.find("url-list"); it != dict.end())
    {
        try
        {
            const auto &url_list = std::any_cast<std::vector<std::any>>(it->second);
            std::vector<std::string> url_list_strings;
            for (const auto &url : url_list)
            {
                url_list_strings.push_back(std::any_cast<std::string>(url));
            }
            obj.set_url_list(url_list_strings);
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for url-list" << std::endl;
        }
    }

    if (auto it = dict.find("info"); it != dict.end())
    {
        try
        {
            const auto &info_dict = std::any_cast<std::map<std::string, std::any>>(it->second);
            torrent_info info_obj;
            populate_torrent_info(info_obj, info_dict);
            obj.set_info(info_obj);
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for info" << std::endl;
        }
    }
}

auto bencode_decoder::populate_torrent_info(torrent_info &info, const std::map<std::string, std::any> &dict) -> void
{
    if (auto it = dict.find("name"); it != dict.end())
    {
        try
        {
            info.set_name(std::any_cast<std::string>(it->second));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for name" << std::endl;
        }
    }

    if (auto it = dict.find("piece length"); it != dict.end())
    {
        try
        {
            info.set_piece_length(static_cast<int>(std::any_cast<int64_t>(it->second)));
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for piece length" << std::endl;
        }
    }

    if (auto it = dict.find("pieces"); it != dict.end())
    {
        try
        {
            const auto &pieces_str = std::any_cast<std::string>(it->second);
            std::vector<std::string> pieces = convert_pieces(pieces_str);
            info.set_pieces(pieces);
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for pieces" << std::endl;
        }
    }

    if (auto it = dict.find("files"); it != dict.end())
    {
        try
        {
            const auto &files = std::any_cast<std::vector<std::any>>(it->second);
            for (const auto &file_any : files)
            {
                const auto &file_dict = std::any_cast<std::map<std::string, std::any>>(file_any);
                torrent_info::file f;
                if (auto length_it = file_dict.find("length"); length_it != file_dict.end())
                {
                    f.length = static_cast<int>(std::any_cast<int64_t>(length_it->second));
                }
                if (auto path_it = file_dict.find("path"); path_it != file_dict.end())
                {
                    const auto &path_list = std::any_cast<std::vector<std::any>>(path_it->second);
                    for (const auto &p : path_list)
                    {
                        f.path.push_back(std::any_cast<std::string>(p));
                    }
                }
                info.add_file(f);
            }
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for files" << std::endl;
        }
    }
}

auto bencode_decoder::convert_pieces(const std::string &pieces) -> std::vector<std::string>
{
    std::vector<std::string> hex_pieces;
    const size_t piece_length = 20;
    for (size_t i = 0; i < pieces.length(); i += piece_length)
    {
        std::stringstream ss;
        for (size_t j = 0; j < piece_length && i + j < pieces.length(); ++j)
        {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(static_cast<uint8_t>(pieces[i + j]));
        }
        hex_pieces.push_back(ss.str());
    }
    return hex_pieces;
}

auto bencode_decoder::parse_file(const std::string &filename, bencode &torrent_bencode) -> bool
{
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Failed to open input file: " << filename << std::endl;
        return false;
    }

    std::string raw_data((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();

    std::any decoded_data = decode_bencode(raw_data);

    if (auto *dict = std::any_cast<std::map<std::string, std::any>>(&decoded_data))
    {
        populate_bencode_object(torrent_bencode, *dict);
        return true;
    }
    else
    {
        std::cerr << "Error decoding the data from file: " << filename << std::endl;
        return false;
    }
}

auto bencode_decoder::read_until(char delim) -> std::string
{
    size_t start = pos;
    while (pos < data.length() && data[pos] != delim)
    {
        pos++;
    }
    return data.substr(start, pos - start);
}

auto bencode_decoder::decode_string() -> std::pair<std::string, size_t>
{
    std::string length_str = read_until(':');
    pos++; 
    size_t length = static_cast<size_t>(std::stoul(length_str));
    std::string value = data.substr(pos, length);
    pos += length;
    return {value, pos};
}

auto bencode_decoder::decode_integer() -> std::pair<int64_t, size_t>
{
    pos++; 
    std::string number_str = read_until('e');
    pos++; 
    return {std::stoll(number_str), pos};
}

auto bencode_decoder::decode_list() -> std::pair<std::vector<std::any>, size_t>
{
    pos++; 
    std::vector<std::any> list;
    while (pos < data.length() && data[pos] != 'e')
    {
        auto [item, new_pos] = decode_bencode_with_pos(pos);
        list.push_back(item);
        pos = new_pos;
    }
    pos++; 
    return {list, pos};
}

auto bencode_decoder::decode_dictionary() -> std::pair<std::map<std::string, std::any>, size_t>
{
    pos++; 
    std::map<std::string, std::any> dict;
    while (pos < data.length() && data[pos] != 'e')
    {
        auto [key, key_pos] = decode_string();
        pos = key_pos;
        auto [value, value_pos] = decode_bencode_with_pos(pos);
        dict[key] = value;
        pos = value_pos;
    }
    pos++; 
    return {dict, pos};
}