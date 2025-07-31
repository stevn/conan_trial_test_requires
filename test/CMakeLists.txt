add_executable(test_tiff_writer)

target_sources(test_tiff_writer PRIVATE
    test_tiff_writer.cpp
)

target_link_libraries(test_tiff_writer
    tiff_core
    GTest::gtest_main
    LibXml2::LibXml2
    ZLIB::ZLIB
)

target_compile_definitions(test_tiff_writer PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

gtest_discover_tests(test_tiff_writer)
