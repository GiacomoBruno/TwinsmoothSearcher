//
// Created by Giacomo Bruno on 18/03/22.
//

#include "logger.h"

std::string get_path(const std::string& fl, const std::string& fd, uint64_t sm)
{
    return (fd + "/" + fl +"_"+std::to_string(sm)+".txt");
}

logger::logger(const std::string& filename, const std::string& folder, uint64_t smoothness)
{
    path = get_path(filename, folder, smoothness);
    std::filesystem::create_directories(folder);

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
void logger::log(const std::string& s, uint64_t n)
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
void logger::logl(const std::string& s, uint64_t n)
{
    log(s); log(n); newline();
}
void logger::logl(const std::string& s, size_t n)
{
    log(s); log(n); newline();
}

void logger::log(bigint n) {
    std::string s(mpz_get_str(nullptr, 10, *n));
    log_file << "{" << s << "}";
    std::cout << "{" << s << "}";
}

void logger::log(double d) {
    log_file << "{" << d << "}";
    std::cout << "{" << d << "}";

}

void logger::log(float f) {
    log_file << "{" << f << "}";
    std::cout << "{" << f << "}";
}

void logger::log(int n) {
    log_file << "{" << n << "}";
    std::cout << "{" << n << "}";
}

void logger::log(uint64_t n) {
    log_file << "{" << n << "}";
    std::cout << "{" << n << "}";
}

void logger::log(size_t n) {
    log_file << "{" << n << "}";
    std::cout << "{" << n << "}";
}

void logger::newline() {
    log_file << std::endl;
    std::cout << std::endl;
}

logger* log;