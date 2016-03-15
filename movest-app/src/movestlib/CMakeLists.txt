cmake_minimum_required(VERSION 3.3)
project(movestlib)
include(ExternalProject)

set(COMMON_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 ${COMMON_FLAGS}")

file(GLOB SOURCE_FILES
    "algo/*.h"
    "algo/*.cpp"
    "crypto/*.h"
    "crypto/*.cpp"
    "*.cpp"
    "*.h"
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

# Test
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(PROJECT_TEST_NAME movestlib_test)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} movestlib ${CRYPTO++_LIBRARIES})

add_test(MovestTests ${PROJECT_TEST_NAME})
