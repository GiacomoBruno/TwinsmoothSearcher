//
// Created by Giacomo Bruno on 18/03/22.
//

#pragma once
#include <fstream>
#include "bigint.h"
#include "bigfloat.h"
#include "bigint_tree.h"


class logger {
private:
    std::fstream log_file;
    std::string path;
    unsigned long long bytes_written{0};
    int file_number{0};

    void check_file_size();
    std::string get_path(const std::string& fl, const std::string& fd, uint64_t sm) const;
    std::string _filename;
    std::string _folder;
    unsigned long long _smoothness;
public:
    logger(const std::string& filename, const std::string& folder, unsigned long long smoothness);
    ~logger() {std::cout.flush(); log_file.flush(); log_file.close(); }
    void newline();

    void log_top_of_tree(bigint_tree* tree, int n);
    void f_log_top_of_tree(bigint_tree* tree, int n);
    void f_log_tree(bigint_tree* tree);

    void log(const std::string& s);
    void log(int n);
    void log(double n);
    void log(unsigned long long n);
    void log(const bigint& n);
    void log(const bigfloat& n);

    void f_log(const std::string& s);
    void f_log(int n);
    void f_log(double n);
    void f_log(unsigned long long n);
    void f_log(const bigint& n);
    void f_log(const bigfloat& n);
    void f_log(bigint_tree* tree);

    template<typename T>
    void log(const std::string& s, T n);

    template<typename T>
    void logl(const std::string& s, T n);

    template<typename T>
    void f_log(const std::string& s, T n);

    template<typename T>
    void f_logl(const std::string& s, T n);
};

template<typename T>
void logger::log(const std::string& s, T n)
{
    log(s);
    log(n);
}

template<typename T>
void logger::logl(const std::string& s, T n)
{
    log(s);
    log(n);
    newline();
}

template<typename T>
void logger::f_log(const std::string& s, T n)
{
    f_log(s);
    f_log(n);
}

template<typename T>
void logger::f_logl(const std::string& s, T n)
{
    f_log(s);
    f_log(n);
    f_log("\n");
}





