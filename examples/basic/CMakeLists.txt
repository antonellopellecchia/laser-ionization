
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(basic)

include_directories($ENV{LASER_HOME}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for the laser ionization library
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources $ENV{LASER_HOME}/src/*.cc)
file(GLOB headers $ENV{LASER_HOME}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable
#
add_executable(basic basic.cc ${sources} ${headers})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS basic DESTINATION bin)


