#pragma once
#include <string>
#include <fstream>
#include <utility>
#include "global_definitions.hpp"

class splitfile
{
   public:
        splitfile(std::string  folder, std::string  filename, size_t size) : _folder{std::move(folder)}, _filename{std::move(filename)}, _size{size}{}
        ~splitfile() {_current_file.close(); }
        splitfile& operator<<(const std::string& msg);

        void init();

   private:
        const std::string _folder;
        const std::string _filename;
        const size_t _size;

        size_t _current_file_index{0};
        size_t _current_file_size{0};
        std::ofstream _current_file{};

        std::string get_current_file_path();


};