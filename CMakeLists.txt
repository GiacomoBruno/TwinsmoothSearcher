cmake_minimum_required(VERSION 3.21)
project(BSmooth)

set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.3")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 20)

set(GMP_INCLUDE_DIR "/opt/homebrew/include" )
set(GMP_LIBRARY_DIR "/opt/homebrew/lib")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

link_directories(${GMP_LIBRARY_DIR})
add_executable(BSmooth main.cpp tree.cpp file_manager.cpp file_manager.h tree.h nb.h twinsmooth.h twinsmooth.cpp list.cpp list.h twinsmooth_range.cpp twinsmooth_range.h nb.cpp twinsmooth_k_growing.cpp twinsmooth_k_growing.h twinsmooth_k.cpp twinsmooth_k.h logger.cpp logger.h static_twinsmooth.cpp static_twinsmooth.h type_definitions.h prime_calculator.cpp prime_calculator.h prime_calculator_top_n.cpp prime_calculator_top_n.h treelist.cpp treelist.h)



include_directories(BSmooth ${GMP_INCLUDE_DIR})
target_link_libraries(BSmooth gmp)