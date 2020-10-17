LASERLIB = $(LASER_HOME)

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long \
	`root-config --cflags` \
	-O3 -fno-common -c \
	-I$(LASERLIB)

LDFLAGS = -L$(LIBDIR) -lGarfield
LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm

Example: Example.cc 
	$(CXX) $(CFLAGS) Example.cc
	$(CXX) `root-config --cflags` -o Example Example.o $(LDFLAGS)
	rm Example.o
