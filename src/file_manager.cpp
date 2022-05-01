#include <sys/stat.h>
#include <filesystem>

#include "../include/file_manager.h"



bool file_exists(const string& path)
{
    struct stat buffer{};
    return (stat(path.c_str(), &buffer) == 0);
}

CappedFile::CappedFile(string filename, string folder, std::ios_base::openmode open_mode, uint64_t _smoothness)
{
    smoothness = _smoothness;
    _filename = std::move(filename);
    _folder = std::move(folder);
    openmode = open_mode;
    _index = 0;
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

void CappedFile::open()
{
    current_file.open(get_c_path(_index),get_openmode());
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
        n++;
    }
}

void CappedFile::printn(const char* fmt, mpz_t n)
{
    if (openmode & std::fstream::in) return;

    char buffer[1024];

    gmp_sprintf(buffer, fmt, n);
    if(written_bytes + strlen(buffer) > MAX_FILE_SIZE)
    {
        current_file.flush();
        current_file.close();
        _index++;
        written_bytes = 0;
        current_file.open(get_c_path(_index), get_openmode());
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
            current_file.open(get_c_path(_index),get_openmode());
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
    current_file.open(get_c_path(_index), get_openmode());
    
}

std::ios_base::openmode CappedFile::get_openmode() {
    if(openmode & std::fstream::in) return openmode;
    if(openmode & std::fstream::app && file_exists(get_c_path(_index))) return openmode;
    return std::fstream::out;
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
        current_file.open(get_c_path(_index), get_openmode());
    }
}





bool CappedFile::exists() {
    return file_exists(get_c_path(_index));
}

/*
bigint1 CappedFile::get_bigint() {
    //assumption file is open
    string line;
    if(this->getline(line))
    {
        auto val = bigint_new;
        mpz_init2(*val, MPZ_INIT_BITS);
        int fail = mpz_set_str(*val, line.c_str(), 10);
        if(!fail) {
            return val;
        }
        else
        {
            bigint_free(val);
        }
    }
    return nullptr;
}
*/

CappedFile::CappedFile()= default;