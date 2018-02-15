cmake_minimum_required(VERSION 3.9)
project(element_test)

set(ROOT_DIR "/Users/alexaksentyev/REPOS/CPP_INTEGRATOR")
set(SRC_DIR "${ROOT_DIR}/src")
set(INCLUDE_DIR "/usr/local/Cellar")

include_directories("${ROOT_DIR}/include"
"${INCLUDE_DIR}/boost/1.66.0"
"${INCLUDE_DIR}/eigen/3.3.4")

set(SOURCES element_test.cc "${SRC_DIR}/quadrupole.cc" "${SRC_DIR}/element.cc"
 "${SRC_DIR}/dipole.cc" "${SRC_DIR}/particle.cc" "${SRC_DIR}/sextupole.cc"
 "${SRC_DIR}/wien_filter.cc")

set(CMAKE_CXX_STANDARD 11)


add_executable(element_test ${SOURCES})
