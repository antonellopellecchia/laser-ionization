
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gem)

#---Find Garfield and ROOT packages-------------------------------------------------------
find_package(Garfield REQUIRED)
find_package(ROOT REQUIRED)

include(${ROOT_USE_FILE})
include_directories($ENV{LASER_HOME}/include)
include_directories($ENV{GARFIELD_HOME}/include)

link_directories($ENV{GARFIELD_HOME}/lib)

#----------------------------------------------------------------------------
# Locate sources and headers for the laser ionization library
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources $ENV{LASER_HOME}/src/*.cc)
file(GLOB headers $ENV{LASER_HOME}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable
#
add_executable(gem gem.cc ${sources} ${headers})
target_link_libraries(gem Garfield)
target_link_libraries(gem ${ROOT_LIBRARIES})

# Copy electric field files calculated with Ansys to build directory
file(COPY field DESTINATION .)