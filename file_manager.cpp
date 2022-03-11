#include <utility>
#include "file_manager.h"



bool file_exists(const string& path)
{
    struct stat buffer{};
    return (stat(path.c_str(), &buffer) == 0);
}

CappedFile::CappedFile(string filename, string folder, bool _read_mode, uint64_t _smoothness)
: _index(0), written_bytes(0), smoothness(_smoothness)
{
    _filename = std::move(filename);
    _folder = std::move(folder);
    read_mode = _read_mode;
    current_file.open(get_c_path(_index),read_mode ? std::fstream::in : std::fstream::out);
}

std::string CappedFile::get_c_path(int n)
{
    return (_folder + "/" + _filename +"_"+std::to_string(smoothness)+ "_" + std::to_string(n) + ".txt");
}

void CappedFile::flush()
{
    current_file.flush();
}

bool CappedFile::is_open()
{
    return current_file.is_open();
}

bool CappedFile::has_successor()
{
    return file_exists(get_c_path(_index+1));
}

void CappedFile::load_all_subfiles()
{
    int n = 0;
    while(file_exists(get_c_path(n)))
    {
        _files.push_back(get_c_path(n));
    }
}

void CappedFile::printn(const char* fmt, mpz_t n)
{
    if (read_mode) return;

    char buffer[1024];

    gmp_sprintf(buffer, fmt, n);
    if(written_bytes + strlen(buffer) > MAX_FILE_SIZE)
    {
        current_file.flush();
        current_file.close();
        _index++;
        written_bytes = 0;
        current_file.open(get_c_path(_index), std::fstream::out);
        current_file << "\n";
    
    }
    else written_bytes = written_bytes + strlen(buffer);

    current_file << buffer;

}

bool CappedFile::getline(std::string& line)
{
    if(!std::getline(current_file, line))
    {
        current_file.close();
        _index++;
        if(file_exists(get_c_path(_index)))
        {
            _files.push_back(get_c_path(_index));
            current_file.open(get_c_path(_index),std::fstream::in);
            return getline(line);
        }
        else
        {
            return false;
        }
    }
    return true;
}

void CappedFile::force_next_file()
{
    current_file.flush();
    current_file.close();
    _index++;
    written_bytes = 0;

    _files.push_back(get_c_path(_index));
    current_file.open(get_c_path(_index), read_mode ? std::fstream::in : std::fstream::out);
    
}

void CappedFile::close()
{
    current_file.close();
}

void CappedFile::delete_all_subfiles()
{
    auto was_open = is_open();
    for(const auto& f : _files)
    {
        remove(f.c_str());
    }
    _files.clear();
    _index = 0;
    if(was_open)
    {   
        _files.push_back(get_c_path(_index));
        current_file.open(get_c_path(_index), read_mode ? std::fstream::in : std::fstream::out);
    }
}

CappedFile::CappedFile()= default;
