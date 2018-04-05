cmake_minimum_required(VERSION 3.9)
project(LATTICE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O2 -DNDEBUG -march=native")

# if (APPLE)
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Rpass-analysis=loop-vectorize")
# else(APPLE)
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopt-info-vec-optimized")
# endif(APPLE)

set(PROJ_ROOT_DIR "$ENV{HOME}/REPOS/CPP_INTEGRATOR")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_ROOT_DIR}/bin/Lattices)

# project headers
include_directories("${PROJ_ROOT_DIR}/include")

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

link_directories(${PROJ_ROOT_DIR}/lib)

## adding executables
set(APP_SOURCES "${CMAKE_SOURCE_DIR}/FODO_lattice.cc"
  "${CMAKE_SOURCE_DIR}/BNL.cc"
  "${CMAKE_SOURCE_DIR}/test_lattice.cc"
  )

foreach(app_src_filename ${APP_SOURCES})
  get_filename_component(app_name ${app_src_filename} NAME_WE)
  add_executable( ${app_name} ${app_src_filename} )
  target_link_libraries( ${app_name} TrackerCore ${Boost_LIBRARIES})
endforeach(app_src_filename ${APP_SOURCES})
