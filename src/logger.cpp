/**
 * @file logger.cpp
 * @author Giacomo Bruno
 * @brief logger implementation (not including generic methods)
 * @version 0.1
 * @date 2022-06-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "logger.hpp"

#include <gmpxx.h> //mpz_class

#include <filesystem> //std::create_directories
#include <iomanip>    //std::setw
#include <iostream>   //std::cout
#include <string>     //std::to_string

namespace twins
{

    namespace logger_utilities
    {
        static std::string get_path(const std::string &path, uint64_t smoothness, int n)
        {
            if (n >= 0)
                return (std::string(output_path) + "_" + std::to_string(smoothness) + "/" + path + "_" + std::to_string(n) +
                        ".txt");
            else
                return (std::string(output_path) + "_" + std::to_string(smoothness) + "/" + path + ".txt");
        }

        template <typename charT, typename traits = std::char_traits<charT>>
        [[maybe_unused]] center_helper<charT, traits> centered(std::basic_string<charT, traits> str)
        {
            return center_helper<charT, traits>(str);
        }

        center_helper<std::string::value_type, std::string::traits_type> centered(const std::string &str)
        {
            return center_helper<std::string::value_type, std::string::traits_type>(str);
        }

        template <typename charT, typename traits>
        std::basic_ostream<charT, traits> &operator<<(std::basic_ostream<charT, traits> &s,
                                                      const center_helper<charT, traits> &c)
        {
            std::streamsize w = s.width();
            if ((unsigned long)w > c.str_.length())
            {
                std::streamsize left = (w + c.str_.length()) / 2;
                s.width(left);
                s << c.str_;
                s.width(w - left);
                s << "";
            }
            else
            {
                s << c.str_;
            }
            return s;
        }

    } // namespace logger_utilities

    logger_files::logger_files(int smoothness) : _smoothness(smoothness)
    {
        std::filesystem::create_directories(std::string(output_path) + "_" +
        std::to_string(smoothness) + "/");
        log->open(logger_utilities::get_path(log_file_name, smoothness, -1), std::fstream::out);
        twins->open(logger_utilities::get_path(twins_file_name, smoothness, 0), std::fstream::out);
        primes->open(logger_utilities::get_path(primes_file_name, smoothness, 0), std::fstream::out);
    }

    void logger_files::twin_size_control()
    {
        if (twins_fbytes > file_size)
        {
            twins_fbytes = 0;
            twins_fcounter++;
            twins->close();
            twins->open(logger_utilities::get_path(twins_file_name, _smoothness, twins_fcounter), std::fstream::out);
        }
    }

    void logger_files::prime_size_control()
    {
        if (primes_fbytes > file_size)
        {
            primes_fbytes = 0;
            primes_fcounter++;
            primes->close();
            primes->open(logger_utilities::get_path(primes_file_name, _smoothness, primes_fcounter), std::fstream::out);
        }
    }

    logger_files::~logger_files()
    {
        log->flush();
        primes->flush();
        twins->flush();

        log->close();
        primes->close();
        twins->close();

        delete log;
        delete primes;
        delete twins;
    }

    logger &logger::get()
    {
        static logger instance; // gets freed at the end of program execution
        return instance;
    }

    logger::~logger()
    {
        for (auto &file : log_files)
        {
            delete file.second;
            file.second = nullptr;
        }

        log_files.clear();
    }

    template <>
    void logger::_print(const mpz_class &n, logger_files *files, int width)
    {
        *files->log << "[" << std::setw(width) << logger_utilities::centered(n.get_str()) << "]";
        std::cout << "[" << std::setw(width) << logger_utilities::centered(n.get_str()) << "]";
    }

    template <>
    void logger::_print<std::string>(const std::string &str, logger_files *files, int)
    {
        *files->log << str;
        std::cout << str;
    }

    template <class T>
    void logger::_save_twin(const T &n, logger_files *files) { *files->twins << n << "\n"; }

    template <>
    void logger::_save_twin<mpz_class>(const mpz_class &n, logger_files *files)
    {
        *files->twins << n.get_str() << "\n";
    }

    template <class T>
    void logger::_save_prime(const T &n, logger_files *files) { *files->primes << n << "\n"; }

    template <>
    void logger::_save_prime<mpz_class>(const mpz_class &n, logger_files *files)
    {
        *files->primes << n.get_str() << "\n";
    }

    template <class T>
    void logger::_print(const T &n, logger_files *files, int width)
    {
        *files->log << "[" << std::setw(width) << logger_utilities::centered(std::to_string(n)) << "]";
        if (!is_silent)
            std::cout << "[" << std::setw(width) << logger_utilities::centered(std::to_string(n)) << "]";
    }



    template void logger::_save_twin<int>(const int &, logger_files *);
    template void logger::_save_twin<unsigned long long>(const unsigned long long &, logger_files *);
    template void logger::_save_prime<int>(const int &, logger_files *);
    template void logger::_save_prime<unsigned long long>(const unsigned long long &, logger_files *);
} // namespace twins