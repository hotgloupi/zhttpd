
cmake_minimum_required(VERSION 2.6)

if(CMAKE_INSTALL_PREFIX)
    MESSAGE("Installation path is ${CMAKE_INSTALL_PREFIX}")
    MESSAGE("${PREFIX}")
endif(CMAKE_INSTALL_PREFIX)

project(zhttpd)
set (zhttpd_VERSION_MAJOR 0)
set (zhttpd_VERSION_MINOR 1)
set(CONF_DIR "${CMAKE_INSTALL_PREFIX}/etc/zhttpd")
set(CONF_FILE "default.xml")
configure_file("server/core/config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/config.hpp")

set(MODULES_PATH "${CMAKE_INSTALL_PREFIX}/lib/zhttpd/modules/")
set(WWW_ROOT "/var/www/")
set(LOG_FILE "/var/log/zhttpd.log")
configure_file("server/configuration/template.xml" "${CMAKE_CURRENT_BINARY_DIR}/${CONF_FILE}")

set(EXECUTABLE_OUTPUT_PATH bin)


if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -DZHTTPD_DEBUG -DDEBUG -ggdb3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -UDEBUG -UZHTTPD_DEBUG -DNDEBUG")
else(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D ZHTTPD_DEBUG")
endif(UNIX)

set(CMAKE_BUILD_TYPE "Debug")

include_directories(
    .
    server/
    unitester/
    ${CMAKE_CURRENT_BINARY_DIR}/
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

add_executable(zhttpd ${server_src} ${server_main})

find_library(DL_LIBRARY dl)

if(WIN32)
        target_link_libraries(zhttpd ws2_32)
else(WIN32)
    if (DL_LIBRARY)
        target_link_libraries(zhttpd ${DL_LIBRARY})
    endif (DL_LIBRARY)
    target_link_libraries(zhttpd pthread)
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

install(TARGETS zhttpd RUNTIME DESTINATION bin)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/default.xml" DESTINATION ${CONF_DIR})
install(FILES "htdocs/index.zhttpd.html" DESTINATION ${WWW_ROOT})


