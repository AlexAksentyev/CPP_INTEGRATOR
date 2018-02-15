cmake_minimum_required(VERSION 3.9)
project(particle_test)
include_directories(../../include "/usr/include/eigen3")
# file(GLOB ...) allows for wildcard additions
# alternative to listing sources in 
# set(SOURCES src/file1.cc src/file2.cc ...)
set(SOURCES particle_test.cc ../../src/particle.cc)


set(CMAKE_CXX_STANDARD 11)

add_executable(particle_test ${SOURCES})
