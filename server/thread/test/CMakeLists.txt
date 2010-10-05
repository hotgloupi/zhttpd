cmake_minimum_required(VERSION 2.6)

include_directories(
    ../
    ../../../unitester
    ../../utils
)

file(GLOB thread_src "../*.cpp")
file(GLOB thread_hpp "../*.hpp")

file(GLOB test_thread_main "main.cpp")
file(GLOB test_class "TestThreadPool.hpp")

list(APPEND
    test_parser_src
        ${thread_src}
        ${thread_hpp}
        ${test_thread_main}
        ${test_class}
)

add_definitions(-DTIXML_USE_TICPP)

add_executable(test_threads ${test_parser_src})

if(WIN32)
        target_link_libraries(test_threads ws2_32)
else(WIN32)
        target_link_libraries(test_threads pthread)
endif(WIN32)
