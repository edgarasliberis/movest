cmake_minimum_required(VERSION 3.3)
project(movestlib)
include(ExternalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")
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

find_package(Crypto++ REQUIRED)
include_directories(${CRYPTO++_INCLUDE_DIR})
target_link_libraries(movestlib ${CRYPTO++_LIBRARIES})
