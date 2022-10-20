#include "splitfile.hpp"
#include <filesystem>
splitfile &splitfile::operator<<(const std::string &msg)
{
    _current_file << msg;
    _current_file_size += msg.length();

    if(_current_file_size > _size)
    {
        _current_file.close();
        _current_file_index++;
        _current_file = std::ofstream(get_current_file_path());
        _current_file_size = 0;
    }

    return *this;
}

void splitfile::init()
{
    std::string path{};
    do
    {
        path = get_current_file_path();
        _current_file_index++;
    }while(std::filesystem::exists(path));

    _current_file_index--;
    if(_current_file_index == 0) {
        _current_file = std::ofstream(get_current_file_path());
        return;
    }

    _current_file_index--;

    _current_file_size = std::filesystem::file_size(get_current_file_path());

    if(_current_file_size > _size) {
        _current_file_index++;
        _current_file_size = 0;
        _current_file = std::ofstream(get_current_file_path());
        return;
    }

    _current_file = std::ofstream(get_current_file_path(), std::ios::app);

}
std::string splitfile::get_current_file_path()
{
    return _folder + _filename + "_" + std::to_string(_current_file_index) + ".txt";
}
