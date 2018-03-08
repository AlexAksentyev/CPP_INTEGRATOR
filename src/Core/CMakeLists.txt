cmake_minimum_required(VERSION 3.9)
set(LIB_NAME TrackerCore)
project(${LIB_NAME} VERSION 1.0.1 DESCRIPTION "Integrator core")


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -Os -DNDEBUG -fopenmp -march=native")

set(PROJ_ROOT_DIR "$ENV{HOME}/REPOS/CPP_INTEGRATOR")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJ_ROOT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJ_ROOT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_ROOT_DIR}/bin)
set(SRC_DIR ${CMAKE_SOURCE_DIR})

# project headers
include_directories("${PROJ_ROOT_DIR}/include")
  
find_package(Boost 1.66.0 COMPONENTS system iostreams REQUIRED)
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

set(LIB_SOURCES
  # elements
  "${SRC_DIR}/elements/element.cc" 
  "${SRC_DIR}/elements/dipole.cc" 
  "${SRC_DIR}/elements/quadrupole.cc" 
  "${SRC_DIR}/elements/sextupole.cc" 
  "${SRC_DIR}/elements/wien_filter.cc"
  "${SRC_DIR}/elements/rf_element.cc"
  # other
  "${SRC_DIR}/particle.cc" 
  "${SRC_DIR}/data_log.cc"
  "${SRC_DIR}/lattice.cc"
  "${SRC_DIR}/tilt.cc"
  "${SRC_DIR}/right_hand_side.cc"
  )

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

add_library(${LIB_NAME} ${LIB_SOURCES})
target_link_libraries( ${LIB_NAME} ${Boost_LIBRARIES})
set_target_properties(${LIB_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
