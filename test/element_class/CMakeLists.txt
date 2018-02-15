cmake_minimum_required(VERSION 3.9)
project(element_test)

set(ROOT_DIR "/home/alexa/REPOS/CPP_INTEGRATOR")
set(SRC_DIR "${ROOT_DIR}/src")

include_directories("${ROOT_DIR}/include" "/usr/local/boost_1_66_0"
  "/usr/include/eigen3"
)

set(SOURCES element_test.cc "${SRC_DIR}/element.cc" 
  "${SRC_DIR}/particle.cc" "${SRC_DIR}/right_hand_side.cc"
 "${SRC_DIR}/dipole.cc" "${SRC_DIR}/quadrupole.cc" 
 "${SRC_DIR}/sextupole.cc" 
 "${SRC_DIR}/wien_filter.cc"
 "${SRC_DIR}/rf_element.cc"
 "${SRC_DIR}/data_log.cc"
 )

set(CMAKE_CXX_STANDARD 11)


add_executable(element_test ${SOURCES})
