#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <gmp.h>

constexpr unsigned long long MEGABYTE = 1000000;
constexpr unsigned long long MAX_FILE_SIZE = MEGABYTE*100;

typedef std::string string;

bool file_exists(const string& path);

class CappedFile {
    private:
        std::ios_base::openmode openmode{};
        size_t written_bytes{};
        std::fstream current_file;
        int _index{};
        std::string _filename;
        std::string _folder;
        std::vector<std::string> _files;
        uint64_t smoothness{};
        std::ios_base::openmode get_openmode();
    public:
        CappedFile(string filename, string folder, std::ios_base::openmode open_mode, uint64_t _smoothness);
        CappedFile();
        void printn(const char* fmt, mpz_t n);
        bool getline(std::string& line);

        bool is_open();
        void open();
        bool has_successor();
        void flush();
        void close();
        void force_next_file();
        void load_all_subfiles();
        void delete_all_subfiles();

        bool exists();
        std::string get_c_path(int n);
};
