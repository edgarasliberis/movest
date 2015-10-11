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
            --enable-nonfree
            --enable-libx264
            --enable-libfdk-aac
            --disable-doc
            --disable-programs
            --enable-shared
        BUILD_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" make -j8
        INSTALL_COMMAND make install
)

set(X264_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/x264)

ExternalProject_Add(
        libx264_proj
        BINARY_DIR ${X264_DIR}
        SOURCE_DIR ${X264_DIR}
        URL libs/x264
        CONFIGURE_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" ./configure
        --prefix="<BINARY_DIR>"
        --bindir="<BINARY_DIR>/bin"
        --enable-shared
        BUILD_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" make -j8
        INSTALL_COMMAND make install
)

add_library(libavcodec SHARED IMPORTED)
add_library(libavformat SHARED IMPORTED)
add_library(libavfilter SHARED IMPORTED)
add_library(libavutil SHARED IMPORTED)
add_library(libx264 SHARED IMPORTED)

ExternalProject_Get_Property(ffmpeg_proj BINARY_DIR)
set_property(TARGET libavcodec PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavcodec.so)
set_property(TARGET libavformat PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavformat.so)
set_property(TARGET libavfilter PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavfilter.so)
set_property(TARGET libavutil PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavutil.so)
ExternalProject_Get_Property(libx264_proj BINARY_DIR)
set_property(TARGET libx264 PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libx264.so)

add_dependencies(libavcodec ffmpeg_proj)
add_dependencies(libavformat ffmpeg_proj)
add_dependencies(libavfilter ffmpeg_proj)
add_dependencies(libavutil ffmpeg_proj)
add_dependencies(libx264 libx264_proj)

add_dependencies(movest libavcodec)
add_dependencies(movest libavformat)
add_dependencies(movest libavfilter)
add_dependencies(movest libavutil)
add_dependencies(movest libx264)

ExternalProject_Get_Property(ffmpeg_proj SOURCE_DIR)
include_directories(movest ${SOURCE_DIR})
ExternalProject_Get_Property(libx264_proj SOURCE_DIR)
include_directories(movest ${SOURCE_DIR})

target_link_libraries(movest libavcodec)
target_link_libraries(movest libavformat)
target_link_libraries(movest libavfilter)
target_link_libraries(movest libavutil)

