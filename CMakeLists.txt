cmake_minimum_required(VERSION 3.9)
project(integrate)
include_directories(include "/usr/local/boost_1_66_0")
# file(GLOB ...) allows for wildcard additions
# alternative to listing sources in 
# set(SOURCES src/file1.cc src/file2.cc ...)
file(GLOB SOURCES "src/*.cc")

add_executable(integrate ${SOURCES})
