cmake_minimum_required(VERSION 3.22)

project(tick-tack-toe
        VERSION 1.0.0
        DESCRIPTION "tick-tack-toe"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(tick)

target_sources(tick
        PRIVATE
        main.cpp)