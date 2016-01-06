cmake_minimum_required(VERSION 3.3)
project(movestlib)
include(ExternalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

file(GLOB_RECURSE SOURCE_FILES
    "*.h"
    "*.cpp"
)

link_directories(${EP_INSTALL}/lib)
include_directories(${EP_INSTALL}/include)
add_library(movestlib SHARED ${SOURCE_FILES})

install(TARGETS movestlib
        LIBRARY DESTINATION lib)
install(FILES movest_connector.h DESTINATION include/movest)

set(RSCODE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../libs/rscode)
ExternalProject_Add(
        rscode_proj
        BINARY_DIR ${RSCODE_DIR}
        SOURCE_DIR ${RSCODE_DIR}
        INSTALL_DIR ${EP_INSTALL}
        URL ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/rscode
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

add_library(librscode STATIC IMPORTED)
set_property(TARGET librscode PROPERTY IMPORTED_LOCATION ${EP_INSTALL}/lib/librscode.a)
add_dependencies(librscode rscode_proj)
add_dependencies(movestlib librscode)
target_link_libraries(movestlib librscode)

#set(FFMPEG_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/ffmpeg)
#set(FFMS2_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/ffms2)

#ExternalProject_Add(
#        ffmpeg_proj
#        BINARY_DIR ${FFMPEG_DIR}
#        SOURCE_DIR ${FFMPEG_DIR}
#        URL libs/ffmpeg
#        CONFIGURE_COMMAND <SOURCE_DIR>/configure
#            --prefix=<BINARY_DIR>
#            --extra-cflags=-I<BINARY_DIR>/include
#            --extra-ldflags="-L<BINARY_DIR>/lib"
#            --enable-gpl
#            --enable-nonfree
#            --enable-libfdk-aac
#            --disable-doc
#            --disable-pthreads
#            --enable-shared
#            --enable-static
#            --enable-runtime-cpudetect
#        BUILD_COMMAND make -j3
#        INSTALL_COMMAND make install
#)

#ExternalProject_Add(
#        ffms2_proj
#        BINARY_DIR ${FFMS2_DIR}
#        SOURCE_DIR ${FFMS2_DIR}
#        URL libs/ffms2
#        CONFIGURE_COMMAND ./configure --prefix=<BINARY_DIR> PKG_CONFIG_PATH=<BINARY_DIR>/lib/pkgconfig AVRESAMPLE_LIBS=-lswresample
#        BUILD_COMMAND make
#        INSTALL_COMMAND make install
#)


#set(X264_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/x264)
#
#ExternalProject_Add(
#        libx264_proj
#        BINARY_DIR ${X264_DIR}
#        SOURCE_DIR ${X264_DIR}
#        URL libs/x264
#        CONFIGURE_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" ./configure
#        --prefix=${FFMPEG_DIR}
#        --extra-ldflags=-ljpeg\ -lpng\ -lm
#        --enable-shared
#        --enable-static
#        BUILD_COMMAND env PATH="<BINARY_DIR>/bin:$ENV{PATH}" make -j3
#        INSTALL_COMMAND make install
#)

#add_library(libavcodec SHARED IMPORTED)
#add_library(libavformat SHARED IMPORTED)
#add_library(libavfilter SHARED IMPORTED)
#add_library(libavutil SHARED IMPORTED)
#add_library(libx264 SHARED IMPORTED)

#ExternalProject_Get_Property(ffmpeg_proj BINARY_DIR)
#set_property(TARGET libavcodec PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavcodec.so)
#set_property(TARGET libavformat PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavformat.so)
#set_property(TARGET libavfilter PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavfilter.so)
#set_property(TARGET libavutil PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libavutil.so)
#ExternalProject_Get_Property(libx264_proj BINARY_DIR)
#set_property(TARGET libx264 PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/lib/libx264.so)

#add_dependencies(libavcodec ffmpeg_proj)
#add_dependencies(libavformat ffmpeg_proj)
#add_dependencies(libavfilter ffmpeg_proj)
#add_dependencies(libavutil ffmpeg_proj)
#add_dependencies(ffmpeg_proj libx264)

#add_dependencies(ffms2_proj ffmpeg_proj)
#add_dependencies(libx264_proj ffms2_proj)
#add_dependencies(libx264 libx264_proj)

#add_dependencies(movest libavcodec)
#add_dependencies(movest libavformat)
#add_dependencies(movest libavfilter)
#add_dependencies(movest libavutil)
#add_dependencies(movest libx264)

#ExternalProject_Get_Property(ffmpeg_proj SOURCE_DIR)
#include_directories(movest ${SOURCE_DIR})
#ExternalProject_Get_Property(libx264_proj SOURCE_DIR)
#include_directories(movest ${SOURCE_DIR})

#target_link_libraries(movest libavcodec)
#target_link_libraries(movest libavformat)
#target_link_libraries(movest libavfilter)
#target_link_libraries(movest libavutil)
#target_link_libraries(movest libx264)
#target_link_libraries(movest m)
#target_link_libraries(movest gpac)

#add_subdirectory(libs/ffmpeg)