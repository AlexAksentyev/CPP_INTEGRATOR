cmake_minimum_required(VERSION 3.9)
project(quad_test)
set(ROOT_DIR "/home/alexa/REPOS/CPP_INTEGRATOR")
set(SRC_DIR "${ROOT_DIR}/src")
include_directories("${ROOT_DIR}/include" "/usr/local/boost_1_66_0")
# file(GLOB ...) allows for wildcard additions
# alternative to listing sources in 
# set(SOURCES src/file1.cc src/file2.cc ...)
set(SOURCES quadrupole_test.cc "${SRC_DIR}/quadrupole.cc" "${SRC_DIR}/element.cc")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x")


add_executable(quad_test ${SOURCES})
