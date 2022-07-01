/**
 * @file logger.hpp
 * @author Giacomo Bruno
 * @brief file for the logger class
 * @version 0.1
 * @date 2022-06-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <fstream>  //std::fstream
#include <map>      //std::map
#include <iostream> //std::cout
/**
 * @brief project namespace
 */
namespace twins
{

    /**
     * @brief file size limit for twins and primes
     */
    static constexpr unsigned long file_size = 20000000;

    /**
     * @brief path used for output
     */
    static constexpr const char *output_path = "./result/output";

    /**
     * @brief name of the log file
     */
    static constexpr const char *log_file_name = "log";

    /**
     * @brief name of the twinsmooths file
     */
    static constexpr const char *twins_file_name = "twinsmooths";

    /**
     * @brief name of primes file
     */
    static constexpr const char *primes_file_name = "primes";

    /**
     * @brief class holding the files for the outputs of a certain smoothness
     */
    class logger_files
    {
    public:
        /**
         * @brief normal constructor
         * @param smoothness used for path name
         */
        explicit logger_files(int smoothness);

        /**
         * @brief copy constructor is deleted
         */
        logger_files(const logger_files &) = delete;

        /**
         * @brief move constructor is deleted
         */
        logger_files(logger_files &&) = delete;

        /**
         * @brief destructor, closes the files
         */
        ~logger_files();

        /**
         * @brief copy operator is deleted
         */
        logger_files &operator=(const logger_files &) = delete;

        /**
         * @brief move operator is deleted
         */
        logger_files &operator=(logger_files &&) = delete;

        /**
         * @brief file stream used to log
         */
        std::fstream *log{new std::fstream};

        /**
         * @brief file stream used to save twins found
         */
        std::fstream *twins{new std::fstream};

        /**
         * @brief file stream used to save primes
         */
        std::fstream *primes{new std::fstream};

        /**
         * @brief function used to avoid going over the file size limit for twin
         * file, create a new file if necessary
         */
        void twin_size_control();

        /**
         * @brief function used to avoid going over the file size limit for prime
         * file, create a new file if necessary
         */
        void prime_size_control();

    private:
        int _smoothness;

        int twins_fcounter{0};
        int primes_fcounter{0};

        unsigned long long twins_fbytes{0};
        unsigned long long primes_fbytes{0};
    };

    /**
     * @brief singleton class responsible for managing logs. each smoothness has
     * ONE set of files associated for logging purposes
     */
    class logger
    {
    public:
        /**
         * Static getter for the singleton class
         * @return reference to the only instance of this class
         */
        static logger &get();

        /**
         * @brief set the logger in silent mode, no standard output (used for tests)
         *
         * @param mode true for no standard output
         */
        static void set_silent_mode(bool mode) { get().is_silent = mode; }

        /**
         * @brief method to print on log file + standard output
         * @tparam T type of the value to be logged, needs to implement stream
         * operator <<
         * @param v value to be logged
         * @param N smoothness, used for choosing where to log
         * @param width when using a non string object, the logger will use this
         * value for padding and enclose the object in square brackets e.g. [  10  ]
         */
        template <class T>
        void print(const T &v, int N, int width = 10);

        /**
         * @brief method to print c string literals on log file + standard output
         * @tparam SIZE length of the c string to log
         * @param s the string to log
         * @param N smoothness, used for choosing where to log
         */
        template <int SIZE>
        void print(char const (&s)[SIZE], int N);

        /**
         * @brief method to save a twin in the twins file
         * @tparam T type of the number to save
         * @param v value to save
         * @param N smoothness, used for choosing where to log
         */
        template <class T>
        void save_twin(const T &v, int N);

        /**
         * method to save a prime in the primes file
         * @tparam T type of number to save
         * @param v value to save
         * @param N smoothness, used for choosing where to log
         */
        template <class T>
        void save_prime(const T &v, int N);

    private:
        template <class T>
        void _print(const T &, logger_files *, int width = 10);

        template <int SIZE>
        void _print(char const (&s)[SIZE], logger_files *);

        template <class T>
        void _save_twin(const T &, logger_files *);

        template <class T>
        void _save_prime(const T &, logger_files *);

        std::map<int, logger_files *> log_files;

        logger() = default;
        ~logger();

        bool is_silent{false};
    };

    /**
     * @brief simple way to use the logger without having to give the smoothness on
     * each call to print
     */
    class simple_logger
    {
    public:
        /**
         * constructor
         * @param s smoothness, used for choosing where to log
         */
        explicit simple_logger(int s) : N{s}, logger_instance{logger::get()} {}

        /**
         * @brief method to print on log file + standard output
         * @tparam T type of the value to be logged, needs to implement stream
         * operator <<
         * @param v value to be logged
         * @param width when using a non string object, the logger will use this
         * value for padding and enclose the object in square brackets e.g. [  10  ]
         */
        template <class T>
        void print(const T &v, int width = 10);

        /**
         * method to print c string literals on log file + standard output
         * @tparam SIZE length of the c string
         * @param s string to log
         */
        template <int SIZE>
        void print(char const (&s)[SIZE]);

        /**
         * method to save a twin in the twins file
         * @tparam T type of number to save
         * @param v value to save
         */
        template <class T>
        void save_twin(const T &v);

        /**
         * method to save a prime in the primes file
         * @tparam T type of number to save
         * @param v value to save
         */
        template <class T>
        void save_prime(const T &v);

    private:
        int N{0};
        logger &logger_instance;
    };
} // namespace twins

//#############################################################################################################
//# GENERIC IMPLEMENTATION
//#############################################################################################################

namespace twins
{
    namespace logger_utilities
    {

        /**
         * @brief class for centering a string during stream operation
         *
         * @tparam charT string type
         * @tparam traits string traits
         */
        template <typename charT, typename traits>
        class center_helper
        {
            std::basic_string<charT, traits> str_;

        public:
            /**
             * @brief construct a center_helper
             *
             * @param str
             */
            explicit center_helper(std::basic_string<charT, traits> str) : str_(std::move(str)) {}

            /**
             * @brief stack_overflow stream operator for centering a string
             *
             * @tparam a stream type 1
             * @tparam b stream type 2
             * @param s stream
             * @param c center_helper
             * @return std::basic_ostream<a, b>& stream
             */
            template <typename a, typename b>
            friend std::basic_ostream<a, b> &operator<<(std::basic_ostream<a, b> &s, const center_helper<a, b> &c);
        };

        /**
         * @brief function that center a string
         *
         * @tparam charT string char type
         * @tparam traits string trait
         * @param str string to center
         * @return center_helper<charT, traits> a center_helper responsible for centering the string
         */
        template <typename charT, typename traits>
        [[maybe_unused]] center_helper<charT, traits> centered(std::basic_string<charT, traits> str);

        /**
         * @brief function that center a std::string
         *
         * @param str string to center
         * @return center_helper<std::string::value_type, std::string::traits_type> a center_helper responsible for centering the string
         */
        center_helper<std::string::value_type, std::string::traits_type> centered(const std::string &str);
        // fine codice trovato
    } // namespace logger_utilities

    template <class T>
    void logger::print(const T &v, int N, int width)
    {
        if (log_files.count(N) == 0)
            log_files[N] = new logger_files(N);

        _print(v, log_files[N], width);
    }

    template <int SIZE>
    void logger::print(char const (&s)[SIZE], int N)
    {
        if (log_files.count(N) == 0)
            log_files[N] = new logger_files(N);

        _print(s, log_files[N]);
    }

    template <class T>
    void logger::save_twin(const T &v, int N)
    {
        if (log_files.count(N) == 0)
            log_files[N] = new logger_files(N);

        _save_twin(v, log_files[N]);
    }

    template <class T>
    void logger::save_prime(const T &v, int N)
    {
        if (log_files.count(N) == 0)
            log_files[N] = new logger_files(N);

        _save_prime(v, log_files[N]);
    }

    template <class T>
    void simple_logger::print(const T &v, int width) { logger_instance.print(v, N, width); }

    template <int SIZE>
    void simple_logger::print(const char (&s)[SIZE]) { logger_instance.print<SIZE>(s, N); }

    template <class T>
    void simple_logger::save_twin(const T &v) { logger_instance.save_twin(v, N); }

    template <class T>
    void simple_logger::save_prime(const T &v) { logger_instance.save_prime(v, N); }

    template <int SIZE>
    void logger::_print(const char (&s)[SIZE], logger_files *files)
    {
        *files->log << s;
        if (!is_silent)
            std::cout << s;
    }
} // namespace twins