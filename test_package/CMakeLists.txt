cmake_minimum_required(VERSION 3.15)
project(TiffWriterPackageTest CXX)

find_package(tiff_writer CONFIG REQUIRED)

add_executable(test_tiff_writer_pkg src/test_tiff_writer_pkg.cpp)
target_link_libraries(test_tiff_writer_pkg tiff_writer::tiff_core)
