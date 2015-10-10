cmake_minimum_required(VERSION 3.3)
project(movest)
include(ExternalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SOURCE_FILES
    "src/*.c"
    "src/*.h"
    "src/*.cpp"
)

add_executable(movest ${SOURCE_FILES})

set(FFMPEG_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/ffmpeg)

ExternalProject_Add(
        ffmpeg_proj
        BINARY_DIR ${FFMPEG_DIR}
        SOURCE_DIR ${FFMPEG_DIR}
        URL libs/ffmpeg
        CONFIGURE_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" PKG_CONFIG_PATH="<BINARY_DIR>/lib/pkgconfig" ./configure
            --prefix="<BINARY_DIR>"
            --extra-cflags="-I<BINARY_DIR>/include"
            --extra-ldflags="-L<BINARY_DIR>/lib"
            --bindir="<BINARY_DIR>/bin"
            --enable-gpl
            --enable-libx264
            --disable-doc
            --disable-programs
            --enable-shared
        BUILD_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" make -j8
        INSTALL_COMMAND make install
)

add_library(libavcodec SHARED IMPORTED)
add_library(libavformat SHARED IMPORTED)
add_library(libavfilter SHARED IMPORTED)
add_library(libavutil SHARED IMPORTED)

ExternalProject_Get_Property(ffmpeg_proj BINARY_DIR)
set_property(TARGET libavcodec PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavcodec.so)
set_property(TARGET libavformat PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavformat.so)
set_property(TARGET libavfilter PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavfilter.so)
set_property(TARGET libavutil PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavutil.so)

add_dependencies(libavcodec ffmpeg_proj)
add_dependencies(libavformat ffmpeg_proj)
add_dependencies(libavfilter ffmpeg_proj)
add_dependencies(libavutil ffmpeg_proj)

add_dependencies(movest libavcodec)
add_dependencies(movest libavformat)
add_dependencies(movest libavfilter)
add_dependencies(movest libavutil)

ExternalProject_Get_Property(ffmpeg_proj SOURCE_DIR)
include_directories(movest ${SOURCE_DIR})

target_link_libraries(movest libavcodec)
target_link_libraries(movest libavformat)
target_link_libraries(movest libavfilter)
target_link_libraries(movest libavutil)

