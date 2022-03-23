//
// Created by Giacomo Bruno on 18/03/22.
//

#include "logger.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <utility>

template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
    std::basic_string<charT, traits> str_;
public:
    explicit center_helper(std::basic_string<charT, traits> str) : str_(std::move(str)) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);
};

template<typename charT, typename traits = std::char_traits<charT> >
center_helper<charT, traits> centered(std::basic_string<charT, traits> str) {
    return center_helper<charT, traits>(str);
}

// redeclare for std::string directly so we can support anything that implicitly converts to std::string
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

std::string get_path(const std::string& fl, const std::string& fd, uint64_t sm)
{
    return (fd + "/" + fl +"_"+std::to_string(sm)+".txt");
}

logger::logger(const std::string& filename, const std::string& folder, uint64_t smoothness)
{
    path = get_path(filename, folder, smoothness);
    std::filesystem::create_directories(folder);
    std::cout.precision(5);
    log_file.open(path, std::fstream::out);
}

void logger::log(const std::string& s) {
    log_file << s;
    std::cout << s;
}

void logger::log(const std::string& s, bigint n)
{
    log(s); log(n);
}
void logger::log(const std::string& s, double n)
{
    log(s); log(n);
}
void logger::log(const std::string& s, float n)
{
    log(s); log(n);
}
void logger::log(const std::string& s, int n)
{
    log(s); log(n);
}
void logger::log(const std::string& s, size_t n)
{
    log(s); log(n);
}

void logger::logl(const std::string& s)
{
    log(s); newline();
}
void logger::logl(const std::string& s, bigint n)
{
    log(s); log(n); newline();
}
void logger::logl(const std::string& s, double n)
{
    log(s); log(n); newline();
}
void logger::logl(const std::string& s, float n)
{
    log(s); log(n); newline();
}
void logger::logl(const std::string& s, int n)
{
    log(s); log(n); newline();
}
void logger::logl(const std::string& s, size_t n)
{
    log(s); log(n); newline();
}

void logger::log(bigint n) {
    auto cs = mpz_get_str(nullptr, 10, *n);
    std::string s(cs);
    log_file << "[" << std::setw(30) << centered(s) << "]";
    std::cout << "[" <<std::setw(30) << centered(s) << "]";
    free(cs);
}

void logger::log(double d) {
    std::string s = std::to_string(d);
    log_file  <<  std::fixed << "[" << std::setw(10) << centered(s) << "]";
    std::cout << std::fixed << "[" << std::setw(10) << centered(s) << "]";

}

void logger::log(float f) {
    std::string s = std::to_string(f);
    log_file << std::fixed << "[" << std::setw(6)<< centered(s) << "]";
    std::cout << std::fixed << "[" << std::setw(6) << centered(s) << "]";
}

void logger::log(int n) {
    std::string s = std::to_string(n);
    log_file << "["<< std::setw(5) << centered(s) << "]";
    std::cout << "[" << std::setw(5) << centered(s) << "]";
}

void logger::log(size_t n) {
    std::string s = std::to_string(n);
    log_file  << "[" <<std::setw(10)<< centered(s) << "]";
    std::cout  <<"[" <<std::setw(10) << centered(s) << "]";
}

void logger::newline() {
    log_file << std::endl;
    std::cout << std::endl;
}

logger* lg;