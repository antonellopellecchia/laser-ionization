INCDIR = $(GARFIELD_HOME)/include
#LIBDIR = $(GARFIELD_HOME)/Library
LIBDIR = $(GARFIELD_HOME)/build/install/lib
LASERLIB = $(LASER_HOME)
#STYLELIB = $(HOME)/code/root-style

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long \
	`root-config --cflags` \
	-O3 -fno-common -c \
	-I$(INCDIR) \
	-I$(LASERLIB)
	#-I$(STYLELIB) \

LDFLAGS = -L$(LIBDIR) -lGarfield
LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm

Example: Example.cc 
	$(CXX) $(CFLAGS) Example.cc
	$(CXX) `root-config --cflags` -o Example Example.o $(LDFLAGS)
	rm Example.o
