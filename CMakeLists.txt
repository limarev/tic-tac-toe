cmake_minimum_required(VERSION 3.22)

project(tick-tack-toe
        VERSION 0.0.1
        DESCRIPTION "tick-tack-toe"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(tick)

target_sources(tick
               PRIVATE
               main.cpp)