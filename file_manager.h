#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <cstdarg>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include "nb.h"

typedef std::string string;

bool file_exists(const string& path);

class CappedFile {
    private:
        bool read_mode{};
        size_t written_bytes{};
        std::fstream current_file;
        int _index{};
        std::string _filename;
        std::string _folder;
        std::vector<std::string> _files;
        uint64_t smoothness;
    public:
        CappedFile(string filename, string folder, bool _read_mode, uint64_t _smoothness);
        CappedFile();
        void printn(const char* fmt, mpz_t n);
        bool getline(std::string& line);
        bool is_open();
        bool has_successor();
        void flush();
        void close();
        void force_next_file();
        void load_all_subfiles();
        void delete_all_subfiles();
        std::string get_c_path(int n);
};


#endif
