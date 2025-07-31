cmake_minimum_required(VERSION 3.15)

# Setup

project(tiff_writer LANGUAGES C CXX)
include(CTest)

# Dependencies

find_package(TIFF REQUIRED)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    include(GoogleTest)
    find_package(ZLIB REQUIRED)
    find_package(LibXml2 REQUIRED)
endif()

# Source

add_subdirectory(core)

# Tests

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
