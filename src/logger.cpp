//
// Created by Giacomo Bruno on 18/03/22.
//

#include "../include/logger.h"

#include <string>
#include <iostream>

constexpr unsigned long long MEGABYTE = 1000000;
constexpr unsigned long long MAX_FILE_SIZE = MEGABYTE*50;

template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
    std::basic_string<charT, traits> str_;
public:
    explicit center_helper(std::basic_string<charT, traits> str) : str_(std::move(str)) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);
};

template<typename charT, typename traits = std::char_traits<charT> >
[[maybe_unused]] center_helper<charT, traits> centered(std::basic_string<charT, traits> str) {
    return center_helper<charT, traits>(str);
}

center_helper<std::string::value_type, std::string::traits_type> centered(const std::string& str) {
    return center_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const center_helper<charT, traits>& c) {
    std::streamsize w = s.width();
    if (w > c.str_.length()) {
        std::streamsize left = (w + c.str_.length()) / 2;
        s.width(left);
        s << c.str_;
        s.width(w - left);
        s << "";
    } else {
        s << c.str_;
    }
    return s;
}

std::string logger::get_path(const std::string& fl, const std::string& fd, uint64_t sm) const
{
    if(file_number > 0) return (fd + "/" + fl +"_"+std::to_string(sm)+"_" + std::to_string(file_number) + ".txt");
    else return (fd + "/" + fl +"_"+std::to_string(sm)+".txt");
}

logger::logger(const std::string& filename, const std::string& folder, uint64_t smoothness)
    : _filename(filename), _folder(folder), _smoothness(smoothness)
{
    path = get_path(filename, folder, smoothness);
    std::filesystem::create_directories(folder);
    std::cout.precision(5);
    log_file.precision(5);
    log_file.open(path, std::fstream::out);
}

void logger::log_top_of_tree(bigint_tree* tree, int n)
{
    auto iter = tree->end();
    for(int i = 0; i < n && iter != nullptr; i++)
    {
        log(*iter->val);
        newline();
        iter = iter->prev;
    }
}

void logger::log(const std::string& s) {
    log_file << s;
    std::cout << s;
}

void logger::log(double d) {
    std::string s = std::to_string(d);
    log_file << std::fixed << "[" << std::setw(10) << centered(s) << "]";
    std::cout << std::fixed << "[" << std::setw(10) << centered(s) << "]";

}

void logger::log(int n) {
    std::string s = std::to_string(n);
    log_file << "[" <<std::setw(10) << centered(s) << "]";
    std::cout << "[" << std::setw(5) << centered(s) << "]";
}

void logger::log(unsigned long long n) {
    std::string s = std::to_string(n);
    log_file << "[" <<std::setw(10) << centered(s) << "]";
    std::cout  << "[" <<std::setw(10) << centered(s) << "]";
}

void logger::log(const bigint& n)
{
    std::string s = mpz_get_str(nullptr, 10, n.number);
    log_file <<"[" << std::setw(50) << centered(s) << "]";
    std::cout  <<"[" << std::setw(50) << centered(s) << "]";
}

void logger::log(const bigfloat& n)
{
    char buf[1024];
    gmp_sprintf(buf, "%Ff", n.number);
    log_file << "[" << std::setw(10) << centered(buf) << "]";
    std::cout << "[" << std::setw(10) << centered(buf) << "]";
}

void logger::newline() {
    log_file << std::endl;
    std::cout << std::endl;
}

void logger::f_log(const std::string& s) {
    log_file << s;
    bytes_written += s.length();
    check_file_size();

}

void logger::f_log(double d) {
    log_file  <<  d;
    bytes_written += sizeof(double);
    check_file_size();


}

void logger::f_log(int n) {
    log_file << n;
    bytes_written += sizeof(int);
    check_file_size();

}

void logger::f_log(unsigned long long n) {
    log_file  << n;
    bytes_written += sizeof(unsigned long long);
    check_file_size();

}

void logger::f_log(const bigint& n)
{
    std::string s = mpz_get_str(nullptr, 10, n.number);
    log_file  << s;
    bytes_written += s.length();
    check_file_size();
}

void logger::f_log(const bigfloat& n)
{
    char buf[1024];
    gmp_sprintf(buf, "%Ff", n.number);
    log_file << buf;
    bytes_written += std::string(buf).length();
    check_file_size();
}

void logger::f_log(bigint_tree* tree)
{
    auto iter = tree->begin();
    while(iter != nullptr)
    {
        f_log(*iter->val);
        f_log("\n");
        iter = iter->next;
    }
}


void logger::check_file_size() {
    if(bytes_written > MAX_FILE_SIZE)
    {
        bytes_written = 0;
        log_file.close();
        file_number++;
        auto cur_path = get_path(_filename, _folder, _smoothness);
        log_file.open(cur_path, std::fstream::out);
    }
}

