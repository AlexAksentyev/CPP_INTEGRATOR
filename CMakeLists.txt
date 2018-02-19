cmake_minimum_required(VERSION 3.9)
project(cpp_integrator)

if(UNIX)
  set(ROOT_DIR "/home/alexa/REPOS/CPP_INTEGRATOR")
elseif(APPLE)
  set(ROOT_DIR "/home/alexaksentyev/REPOS/CPP_INTEGRATOR")
endif(UNIX)
set(SRC_DIR "${ROOT_DIR}/src")

# project headers
include_directories(
  "${ROOT_DIR}/include" 
  "${ROOT_DIR}/include/elements"
  )
  
find_package(Boost 1.66.0 COMPONENTS system iostreams REQUIRED)
# external headers
if(UNIX)
  include_directories("/usr/local/boost_1_66_0"
    "/usr/include/eigen3")
elseif(APPLE)
  set(CELLAR "/usr/local/Cellar")
  include_directories("${CELLAR}/boost/1.66.0/boost"
    "${CELLAR}/eigen/3.3.4/include/eigen3")
endif(UNIX)

link_directories(/usr/local/boost_1_66_0/stage/lib)

set(SOURCES 
  "${SRC_DIR}/main.cc" 
  "${SRC_DIR}/particle.cc" 
  "${SRC_DIR}/data_log.cc"
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

add_executable(cpp_integrator ${SOURCES})

target_link_libraries(cpp_integrator ${Boost_LIBRARIES})
