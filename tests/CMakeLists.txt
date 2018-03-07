cmake_minimum_required(VERSION 3.9)
project(TEST)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -Os")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(INTEGRATOR_ROOT_DIR "$ENV{HOME}/REPOS/CPP_INTEGRATOR")

# Cpp_int Core headers
include_directories("${INTEGRATOR_ROOT_DIR}/Core/include")

# test headers
include_directories(${INTEGRATOR_ROOT_DIR}/tests/include)

find_package(Boost 1.66.0 COMPONENTS REQUIRED system iostreams)
# external headers
if(APPLE)
  set(CELLAR "/usr/local/Cellar")
  include_directories("${CELLAR}/boost/1.66.0/boost"
    "/usr/local/include/eigen3"
    "$ENV{HOME}/REPOS/gnuplot-iostream")
  link_directories("${CELLAR}/boost/1.66.0/lib")
else(APPLE)
  include_directories("/usr/local/boost_1_66_0"
    "/usr/include/eigen3")
  link_directories(/usr/local/boost_1_66_0/stage/lib)
endif(APPLE)

link_directories(${INTEGRATOR_ROOT_DIR}/Core/lib)

# adding executables
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_SOURCES ${SRC_DIR}/operator_plus.cc)

link_directories(${ROOT_DIR}/Core/lib)
foreach(test_src_filename ${TEST_SOURCES})
  get_filename_component(test_name ${test_src_filename} NAME_WE)
  add_executable( ${test_name} ${test_src_filename})
  target_link_libraries( ${test_name} Cpp_int ${Boost_LIBRARIES})
endforeach(test_src_filename ${TEST_SOURCES})
