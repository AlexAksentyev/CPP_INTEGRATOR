cmake_minimum_required(VERSION 3.9)
set(CODE_NAME FODO_lattice)
project(TEST_PROJECT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Os")

set(ROOT_DIR "$ENV{HOME}/REPOS/CPP_INTEGRATOR")
set(SRC_DIR "${ROOT_DIR}/src")

# project headers
include_directories(
  "${ROOT_DIR}/include" 
  "${ROOT_DIR}/include/elements"
  )
  
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

set(SOURCES 
  "${SRC_DIR}/${CODE_NAME}.cc" 
  "${SRC_DIR}/particle.cc" 
  "${SRC_DIR}/data_log.cc"
  "${SRC_DIR}/lattice.cc"
  "${SRC_DIR}/tilt.cc"
  # elements
  "${SRC_DIR}/elements/element.cc" 
  "${SRC_DIR}/elements/dipole.cc" 
  "${SRC_DIR}/elements/quadrupole.cc" 
  "${SRC_DIR}/elements/sextupole.cc" 
  "${SRC_DIR}/elements/wien_filter.cc"
  "${SRC_DIR}/elements/rf_element.cc"
  "${SRC_DIR}/right_hand_side.cc"
  )

set(CMAKE_CXX_STANDARD 11)

add_executable(${CODE_NAME} ${SOURCES})

target_link_libraries(${CODE_NAME} ${Boost_LIBRARIES})
