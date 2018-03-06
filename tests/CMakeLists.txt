
cmake_minimum_required(VERSION 3.9)
project(TESTS)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# test headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# adding executables
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_SOURCES ${SRC_DIR}/operator_plus.cc)

foreach(test_src_filename ${TEST_SOURCES})
  get_filename_component(test_name ${test_src_filename} NAME_WE)
  add_executable( ${test_name} ${test_src_filename} )
endforeach(test_src_filename ${TEST_SOURCES})
