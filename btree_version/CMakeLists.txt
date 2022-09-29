cmake_minimum_required(VERSION 3.16)
project(twinsmooth_searcher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GMP_INCLUDE_DIR "/opt/homebrew/include" )
    set(GMP_LIBRARY_DIR "/opt/homebrew/lib")
    link_directories(${GMP_LIBRARY_DIR})
    include_directories(twinsmooth_searcher ${GMP_INCLUDE_DIR})
endif()

add_executable(twinsmooth_searcher include/range_searcher.hpp include/variable_range_searcher.hpp)

target_sources(twinsmooth_searcher
    PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/benchmark.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/include/searcher.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/include/BS/BS_thread_pool_light.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/include/BS/BS_thread_pool.hpp
    PRIVATE ${PROJECT_SOURCE_DIR}/include/variable_range_searcher.hpp

    
)
    

target_include_directories(twinsmooth_searcher
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(twinsmooth_searcher gmp)
