cmake_minimum_required(VERSION 3.9)
project(cpp_integrator)

if(APPLE)
  set(ROOT_DIR "/Users/alexaksentyev/REPOS/CPP_INTEGRATOR")
else(APPLE)
  set(ROOT_DIR "/home/alexa/REPOS/CPP_INTEGRATOR")
endif(APPLE)
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
    "/usr/local/include/eigen3")
else(APPLE)
  include_directories("/usr/local/boost_1_66_0"
    "/usr/include/eigen3")
endif(APPLE)

if(APPLE)
   link_directories(/usr/local/Cellar/boost/1.66.0/lib)
else(APPLE)
   link_directories(/usr/local/boost_1_66_0/stage/lib)
endif(APPLE)

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
