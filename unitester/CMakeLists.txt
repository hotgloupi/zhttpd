cmake_minimum_required(VERSION 2.6)

include_directories(.)

file(GLOB unitester_main "main.cpp")

add_executable(unitester ${unitester_main})

