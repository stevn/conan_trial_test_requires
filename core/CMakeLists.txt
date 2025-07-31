add_library(tiff_core STATIC)

target_sources(tiff_core
    PUBLIC
        FILE_SET publicheaders
            TYPE HEADERS
            BASE_DIRS
                include
            FILES
                include/tiff_writer_api.h
    PRIVATE
        src/tiff_writer_api.cpp

        src/tiff_writer.hpp
        src/tiff_writer.cpp
)

target_include_directories(tiff_core
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        include
        src
)

target_link_libraries(tiff_core PRIVATE TIFF::TIFF)

install(TARGETS tiff_core)
