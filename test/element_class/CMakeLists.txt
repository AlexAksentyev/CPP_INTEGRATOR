cmake_minimum_required(VERSION 3.9)
project(element_test)

if(UNIX)
  set(ROOT_DIR "/home/alexa/REPOS/CPP_INTEGRATOR")
elseif(APPLE)
  set(ROOT_DIR "/home/alexaksentyev/REPOS/CPP_INTEGRATOR")
endif(UNIX)
set(SRC_DIR "${ROOT_DIR}/src")
set(INCLUDE_DIR "/usr/local/Cellar")

include_directories("${ROOT_DIR}/include" "${ROOT_DIR}/include/elements")
if(UNIX)
  include_directories("/usr/local/boost_1_66_0"
    "/usr/include/eigen3")
elseif(APPLE)
  include_directories("/usr/local/Cellar/boost/1.66.0/boost"
    "/usr/local/Cellar/eigen/3.3.4/include/eigen3")
endif(UNIX)

set(SOURCES element_test.cc 
  "${SRC_DIR}/particle.cc" 
  "${SRC_DIR}/right_hand_side.cc"
  "${SRC_DIR}/data_log.cc"
  "${SRC_DIR}/elements/element.cc" 
  "${SRC_DIR}/elements/dipole.cc" 
  "${SRC_DIR}/elements/quadrupole.cc" 
  "${SRC_DIR}/elements/sextupole.cc" 
  "${SRC_DIR}/elements/wien_filter.cc"
  "${SRC_DIR}/elements/rf_element.cc"
  )

set(CMAKE_CXX_STANDARD 11)


add_executable(element_test ${SOURCES})
