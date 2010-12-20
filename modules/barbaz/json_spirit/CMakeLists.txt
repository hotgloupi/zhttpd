
if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-DZHTTPD_DEBUG -DDEBUG -ggdb3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -UDEBUG -UZHTTPD_DEBUG -DNDEBUG")
endif(UNIX)

include(CPack)
cmake_minimum_required(VERSION 2.6)
project(json_spirit)
include_directories(.)

SET(JSON_SPIRIT_SRCS
    json_spirit_reader.cpp
    json_spirit_value.cpp
    json_spirit_writer.cpp)

FIND_PACKAGE(Boost 1.34 REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

ADD_LIBRARY(json_spirit STATIC ${JSON_SPIRIT_SRCS})

