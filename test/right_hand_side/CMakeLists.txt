cmake_minimum_required(VERSION 3.9)
project(rhs_test)
set(ROOT_DIR "/home/alexa/REPOS/CPP_INTEGRATOR")
set(SRC_DIR "${ROOT_DIR}/src")
include_directories("${ROOT_DIR}/include")
# file(GLOB ...) allows for wildcard additions
# alternative to listing sources in 
# set(SOURCES src/file1.cc src/file2.cc ...)
set(SOURCES rhs_test.cc "${SRC_DIR}/right_hand_side.cc" "${SRC_DIR}/particle.cc")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x")


add_executable(rhs_test ${SOURCES})
