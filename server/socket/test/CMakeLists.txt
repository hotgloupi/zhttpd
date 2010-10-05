cmake_minimum_required(VERSION 2.6)

include_directories(
    ../
    ../../utils
    ../../thread
    ../../../unitester
    ../../../api
)

file(GLOB test_parser_main "main.cpp")
file(GLOB test_class "TestSocket.hpp")

list(APPEND
    srcs
        ${server_socket_src}
        ${server_utils_src}
        ${server_thread_src}
        ${test_parser_main}
        ${test_class}
)

add_executable(test_socket ${srcs})

if(WIN32)
        target_link_libraries(test_socket ws2_32)
else(WIN32)
        target_link_libraries(test_socket pthread)
endif(WIN32)

