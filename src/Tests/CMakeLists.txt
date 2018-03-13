cmake_minimum_required(VERSION 3.9)
project(TEST)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -Os -fopenmp")

set(PROJ_ROOT_DIR "$ENV{HOME}/REPOS/CPP_INTEGRATOR")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJ_ROOT_DIR}/bin/Tests")

# project headers
include_directories("${PROJ_ROOT_DIR}/include")

set(BOOST_LIBRARYDIR "/usr/local/boost_1_66_0/stage/lib")
find_package(Boost 1.66.0 COMPONENTS REQUIRED timer system iostreams)
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
  link_directories("/usr/local/boost_1_66_0/stage/lib")
endif(APPLE)

link_directories(${PROJ_ROOT_DIR}/lib)

# adding executables
set(TEST_SOURCES ${CMAKE_SOURCE_DIR}/odeint_parallel.cc)

foreach(test_src_filename ${TEST_SOURCES})
  get_filename_component(test_name ${test_src_filename} NAME_WE)
  add_executable( ${test_name} ${test_src_filename})
  target_link_libraries( ${test_name} TrackerCore ${Boost_LIBRARIES})
endforeach(test_src_filename ${TEST_SOURCES})
