cmake_minimum_required(VERSION 3.16)
project(twinsmooth_searcher)
set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GMP_INCLUDE_DIR "/opt/homebrew/include" )
    set(GMP_LIBRARY_DIR "/opt/homebrew/lib")
    link_directories(${GMP_LIBRARY_DIR})
    include_directories(twinsmooth_searcher ${GMP_INCLUDE_DIR})
endif()

add_executable(twinsmooth_searcher src/main.cpp include/list.h src/logger.cpp include/logger.h src/bigint.cpp include/bigint.h src/bigint_tree.cpp include/bigint_tree.h src/twinsmooth/generators.cpp include/twinsmooth/generators.h src/twinsmooth/iterators.cpp include/twinsmooth/iterators.h src/twinsmooth/chunkers.cpp include/twinsmooth/chunkers.h src/bigfloat.cpp include/bigfloat.h src/utilities.cpp include/utilities.h src/twinsmooth/searcher.cpp include/twinsmooth/searcher.h src/twinsmooth/standard_searcher.cpp include/twinsmooth/standard_searcher.h src/twinsmooth/range_searcher.cpp include/twinsmooth/range_searcher.h src/twinsmooth/k_searcher.cpp include/twinsmooth/k_searcher.h src/twinsmooth/all_searchers.cpp include/twinsmooth/all_searchers.h src/benchmark.cpp include/benchmark.h src/twinsmooth/growing_k_searcher.cpp include/twinsmooth/growing_k_searcher.h)

target_link_libraries(twinsmooth_searcher gmp)