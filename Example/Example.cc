#include "Laser.cc"

using namespace std;

int main(int argc, char * argv[]) {
  double sizeX = 10e-2;
  double sizeY = 10e-2;
  double sizeZ = 5e-3;

  double laserWavelength = 266e-9;
  double laserWaistRadius = 25e-6;

  Laser *laser = new Laser(laserWavelength, laserWaistRadius);
  laser->SetBeamDirection(1., 0., 0.);
  laser->SetWaistPosition(0., 0., 0.);
  laser->SetGasVolume(-0.5*sizeX, -0.5*sizeY, -0.5*sizeZ, +0.5*sizeX, +0.5*sizeY, +0.5*sizeZ);
  laser->SetPulseEnergy(51e-6);
  laser->SetDebugging(true);

  double nprimaries = laser->PulseMC();

  cout << nprimaries << endl;
}
