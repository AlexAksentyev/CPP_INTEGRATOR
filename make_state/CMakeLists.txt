cmake_minimum_required(VERSION 3.9)
project(make_state)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -Os")

set(ROOT_DIR "$ENV{HOME}/REPOS/CPP_INTEGRATOR")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# project headers
include_directories(
  "${CMAKE_SOURCE_DIR}/include" 
  "${CMAKE_SOURCE_DIR}/include/ensemble"
  )
  
find_package(Boost 1.66.0 COMPONENTS system iostreams REQUIRED)
# external headers
if(APPLE)
  set(CELLAR "/usr/local/Cellar")
  include_directories("${CELLAR}/boost/1.66.0/boost"
    "/usr/local/include/eigen3")
  link_directories("${CELLAR}/boost/1.66.0/lib")
else(APPLE)
  include_directories("/usr/local/boost_1_66_0"
    "/usr/include/eigen3")
  link_directories(/usr/local/boost_1_66_0/stage/lib)
endif(APPLE)

set(SOURCES "${SRC_DIR}/ensemble/make_state.cc" "${SRC_DIR}/right_hand_side.cc")

add_executable(make_state ${SOURCES})
target_link_libraries( make_state ${Boost_LIBRARIES})
