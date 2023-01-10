cmake_minimum_required(VERSION 3.21)
project(Project8_starter_files)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(Headers Parse.h)

add_executable(Project8_starter_files main.cpp Input.cpp Interpret.cpp Interpret.h test1.blip)
