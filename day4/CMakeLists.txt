cmake_minimum_required(VERSION 3.2)
project(aoc_day4)

# Set C++20 as the required standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost REQUIRED)

# Check if Boost is found
if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
else ()
    message(FATAL_ERROR "Boost not found!")
endif ()


add_executable(aoc_day4 src/day4.cpp)