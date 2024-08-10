cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
include(Version)

project(tic-tac-toe
        DESCRIPTION "tic-tac-toe"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(tic)

target_sources(tic
               PRIVATE
               main.cpp)

target_include_directories(tic PRIVATE ${CMAKE_BINARY_DIR}/include)

file(CONFIGURE OUTPUT include/version.h CONTENT "\
#pragma once

#define PROJECT_VERSION \"${THIS_PROJECT_VERSION}\"
#define PROJECT_SHA \"${THIS_PROJECT_SHA}\"
")

set(CPACK_PACKAGE_VERSION ${THIS_PROJECT_VERSION})

include(CPack)
include(GNUInstallDirs)
install(TARGETS tic
        RUNTIME)