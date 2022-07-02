cmake_minimum_required(VERSION 3.16)
project(twinsmooth_searcher)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(twinsmooth_searcher)

target_sources(twinsmooth_searcher
    PRIVATE ${PROJECT_SOURCE_DIR}/src/logger.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/avl_tree.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/twins/base_searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/twins/standard_searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/twins/range_searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/twins/k_searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp
)
    

target_include_directories(twinsmooth_searcher
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(twinsmooth_searcher gmp)