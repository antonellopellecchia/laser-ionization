#include "Laser.hh"
#include "Point3D.hh"

int main(int argc, char * argv[]) {
  double sizeX = 10e-2;
  double sizeY = 10e-2;
  double sizeZ = 5e-3;

  double laserWavelength = 266e-9;
  double laserPulseEnergy = 51e-8;
  double laserWaistRadius = 25e-6;

  Laser laser{laserWavelength, laserPulseEnergy, laserWaistRadius, Point3D(0., 0., 0.)};
  Point3D gasVolume1{-0.5*sizeX, -0.5*sizeY, -0.5*sizeZ}, gasVolume2{+0.5*sizeX, +0.5*sizeY, +0.5*sizeZ};
  laser.SetGasVolume(gasVolume1, gasVolume2);

  double nprimaries;
  for (int i=0; i<100; i++) {
    nprimaries = laser.Pulse();
    std::cout << nprimaries << " ";
  }
  std::cout << std::endl;
}
