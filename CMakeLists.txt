cmake_minimum_required(VERSION 3.16)
project(twinsmooth_searcher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")

add_executable(twinsmooth_searcher )

target_sources(twinsmooth_searcher
    PRIVATE ${PROJECT_SOURCE_DIR}/src/global_definitions.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/range_searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/global_k_searcher.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/benchmark.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/main.cpp
)
target_include_directories(twinsmooth_searcher
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PUBLIC  ${PROJECT_SOURCE_DIR}/third_party
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GMP_INCLUDE_DIR "/opt/homebrew/include" )
    set(GMP_LIBRARY_DIR "/opt/homebrew/lib")
    link_directories(${GMP_LIBRARY_DIR})
    include_directories(twinsmooth_searcher ${GMP_INCLUDE_DIR})
    target_link_libraries(twinsmooth_searcher gmp)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    target_link_libraries(twinsmooth_searcher gmp pthread)
endif()