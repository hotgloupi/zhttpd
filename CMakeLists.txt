
cmake_minimum_required(VERSION 2.6)

project(zhttpd)
set(EXECUTABLE_OUTPUT_PATH bin)

if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -DZHTTPD_DEBUG -DDEBUG -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -UDEBUG -UZHTTPD_DEBUG -DNDEBUG")
else(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D ZHTTPD_DEBUG")
endif(UNIX)

set(CMAKE_BUILD_TYPE "Debug")

include_directories(
    .
    server/
    unitester/
)

file(GLOB api_hpp "api/*.hpp")
set_source_files_properties(
    api_hpp
        ABSTRACT
)

## SERVER ##

file(GLOB server_core_src "server/core/*.[ch]pp")
file(GLOB server_configuration_src "server/configuration/*.[ch]pp")
file(GLOB server_modules_src "server/modules/*/*.[ch]pp")
file(GLOB server_socket_src "server/socket/*.[ch]pp")
file(GLOB server_thread_src "server/thread/*.[ch]pp")
file(GLOB server_utils_src "server/utils/*.[ch]pp")
file(GLOB server_parser_src "server/parser/*.[ch]pp")
file(GLOB server_parser_ticpp "server/parser/ticpp/*.cpp")

add_definitions(-DTIXML_USE_TICPP)

file(GLOB server_main "server/main.cpp")

list(APPEND
    server_src
        ${api_hpp}
        ${server_core_src}
        ${server_configuration_src}
        ${server_socket_src}
        ${server_thread_src}
        ${server_utils_src}
        ${server_parser_src}
        ${server_parser_ticpp}
        ${server_modules_src}
        ${server_rcon_src}
)

add_executable(server ${server_src} ${server_main})

find_library(DL_LIBRARY dl)

if(WIN32)
        target_link_libraries(server ws2_32)
else(WIN32)
    if (DL_LIBRARY)
        target_link_libraries(server ${DL_LIBRARY})
        message("found libdl !")
    endif (DL_LIBRARY)
    target_link_libraries(server pthread)
endif(WIN32)

### TEST PARSER ##
#
#file(GLOB test_parser_main "server/parser/test/main.cpp")
#
#add_executable(test_parser ${server_src} ${test_parser_main})
#
#if(WIN32)
#        target_link_libraries(test_parser ws2_32)
#else(WIN32)
#    if (DL_LIBRARY)
#        target_link_libraries(test_parser ${DL_LIBRARY})
#        message("found libdl !")
#    endif (DL_LIBRARY)
#    target_link_libraries(test_parser pthread)
#endif(WIN32)
#
### TEST UTILS ##
#
#file(GLOB test_utils_main "server/utils/test/main.cpp")
#
#list(APPEND
#    test_utils_src
#        ${server_utils_src}
#        ${server_thread_src}
#        ${test_utils_main}
#)
#
#add_executable(test_utils ${test_utils_src})
#if (UNIX)
#    target_link_libraries(test_utils pthread)
#else(UNIX)
#    target_link_libraries(test_utils ws2_32)
#endif(UNIX)
#
### FOR MAKE TEST ##
#
#enable_testing()
#add_test(Parser_test ${EXECUTABLE_OUTPUT_PATH}/test_parser)
#add_test(Utils_test ${EXECUTABLE_OUTPUT_PATH}/test_utils)

## MODULES ##

add_subdirectory(modules)

