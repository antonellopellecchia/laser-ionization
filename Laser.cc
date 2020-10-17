#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

#include "Laser.hh"

using namespace std;

Laser::Laser(double wavelength, double waistRadius) {
  this->wavelength = wavelength;
  this->waistRadius = waistRadius;
}

Laser::~Laser() {}

void Laser::SetWaistPosition(double x, double y, double z) {
  fWaistX = x;
  fWaistY = y;
  fWaistZ = z;
}

void Laser::SetBoundaries(double x, double y, double z) {
  fSizeX = x;
  fSizeY = y;
  fSizeZ = z;
}

double Laser::GetIonizationDensityAt(double r, double x) {
  double beamRadius = fBeamQuality*fWaistRadius*sqrt(1+pow(x/fRayleighRange,2));
  double beamIntensity = 2*fPulseEnergy*1e-6/(pi*pow(fWaistRadius,2))*pow(fWaistRadius/beamRadius,2)*exp(-2*pow(r/beamRadius,2));
  return ionizationRateConstant*pow(beamIntensity,2);
}

double Laser::GetIonizationDensityAt(double x, double y, double z) {
  return GetIonizationDensityAt(sqrt(pow(y,2)+pow(z,2)), x);
}

double Laser::Pulse() {
  // calculate average primary ionization by monte carlo integration
  fPulseEnergy = pulseEnergy;
  fAveragePrimaryIonization = 0;
  double averagePrimaryIonizationDouble = 0.;
  int nSamples = 100000; // n of monte carlo samples
  double x, y, z;
  cout << "Calculating primary ionization...";
  for (int i=0; i<nSamples; i++) {
    x = -0.5*fSizeX+((double)rand())/RAND_MAX*fSizeX;
    y = -0.5*fSizeY+((double)rand())/RAND_MAX*fSizeY;
    z = -0.5*fSizeZ+((double)rand())/RAND_MAX*fSizeZ;
    averagePrimaryIonizationDouble += GetIonizationDensityAt(x, y, z);
    cout << "\rCalculating primary ionization... " << (float)i/nSamples*100 << "%";
  }
  cout << "\rCalculating primary ionization....... Done" << endl;
  
  averagePrimaryIonizationDouble /= nSamples;
  averagePrimaryIonizationDouble *= fSizeX*fSizeY*fSizeZ;
  fAveragePrimaryIonization = (int) averagePrimaryIonizationDouble;
  cout << "Average primary ionization " << fAveragePrimaryIonization << endl;

  // create poisson distribution generator
  fPoissonGenerator = default_random_engine();
  fPoisson = poisson_distribution<int>(fAveragePrimaryIonization);
  fMaxIonizationDensity = GetIonizationDensityAt(0., 0.);

  return fPoisson(fPoissonGenerator);
}

void Laser::GetPrimaryElectron(double &x, double &y, double &z) {
  // sample primary electron position
  // from ionization density function

  double sampledIonizationDensity = 0.;
  double calculatedIonizationDensity = 0.;
  do {
    x = -0.5*fSizeX+((double)rand())/RAND_MAX*fSizeX;
    y = -0.5*fSizeY+((double)rand())/RAND_MAX*fSizeY;
    z = -0.5*fSizeZ+((double)rand())/RAND_MAX*fSizeZ;
    sampledIonizationDensity = ((double)rand())/RAND_MAX*fMaxIonizationDensity;
    calculatedIonizationDensity = GetIonizationDensityAt(x, y, z);
  }
  while (calculatedIonizationDensity<sampledIonizationDensity);
  x+=fWaistX; y+=fWaistY; z+=fWaistZ; // translate to waist position
  x*=1.e2; y*=1.e2; z*=1.e2; // convert to garfield units
}
