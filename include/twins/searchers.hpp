#pragma once
#include "base_searcher.hpp"

namespace twins
{
    class std_searcher : public base_searcher
    {
    public:
        std_searcher(int s) : base_searcher{s} {}

    protected:
        void run() override;
    };

    class rng_searcher : public base_searcher
    {
    public:
        rng_searcher(int s) : base_searcher{s} {}

    protected:
        void run() override;
    };

    class kvl_searcher : public base_searcher
    {
    public:
        kvl_searcher(int s) : base_searcher{s} {}

    protected:
        void run() override;
    };
}