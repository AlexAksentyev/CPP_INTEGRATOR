cmake_minimum_required(VERSION 3.9)
project(eigen_test)

set(SOURCES eigen_test.cc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x")


add_executable(eigen_test ${SOURCES})
