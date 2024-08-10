cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
include(Version)

project(tick-tack-toe
        DESCRIPTION "tick-tack-toe"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(tick)

target_sources(tick
               PRIVATE
               main.cpp)

target_include_directories(tick PRIVATE ${CMAKE_BINARY_DIR}/include)

file(CONFIGURE OUTPUT include/version.h CONTENT "\
#pragma once

#define PROJECT_VERSION \"${THIS_PROJECT_VERSION}\"
#define PROJECT_SHA \"${THIS_PROJECT_SHA}\"
")

include(CPack)
include(GNUInstallDirs)
install(TARGETS tick
        RUNTIME)