cmake_minimum_required(VERSION 3.3)
project(rscode)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -fPIC")
add_library(rscode STATIC rs.c galois.c berlekamp.c crcgen.c)
install(TARGETS rscode
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ecc.h DESTINATION include/rscode)
