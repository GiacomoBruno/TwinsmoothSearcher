//
// Created by Giacomo Bruno on 18/03/22.
//

#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include "type_definitions.h"

std::string get_path(const std::string& fl, const std::string& fd, uint64_t sm);

class logger {
private:
    std::fstream log_file;
    std::string path;
public:
    logger(const std::string& filename, const std::string& folder, uint64_t smoothness);

    void newline();
    void log(const std::string& s);
    void log(const std::string& s, bigint n);
    void log(const std::string& s, double n);
    void log(const std::string& s, float n);
    void log(const std::string& s, int n);
    void log(const std::string& s, size_t n);

    void logl(const std::string& s);
    void logl(const std::string& s, bigint n);
    void logl(const std::string& s, double n);
    void logl(const std::string& s, float n);
    void logl(const std::string& s, int n);
    void logl(const std::string& s, size_t n);




    void log(bigint n);
    void log(double d);
    void log(float f);
    void log(int n);
    void log(size_t n);
};

extern logger* lg;