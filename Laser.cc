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
  this->waistX = x;
  this->waistY = y;
  this->waistZ = z;
}

void Laser::SetBeamDirection(double dx, double dy, double dz) {
  this->directionX = dx;
  this->directionY = dy;
  this->directionZ = dz;
}

void Laser::SetGasVolume(double x1, double y1, double z1, double x2, double y2, double z2) {
  gasX1 = x1;
  gasY1 = y1;
  gasZ1 = z1;
  gasX2 = x2;
  gasY2 = y2;
  gasZ2 = z2;
}

double Laser::GetRayleighRange() {
  return pi*pow(this->waistRadius,2)/this->wavelength;
}

double Laser::GetIonizationDensityAt(double r, double x) {
  double beamRadius = beamQuality*waistRadius*sqrt(1+pow(x/GetRayleighRange(),2));
  //cout << "radius " << 1.e6*beamRadius << " um" << endl;
  double beamIntensity = 2*pulseEnergy/(pi*pow(waistRadius,2))*pow(waistRadius/beamRadius,2)*exp(-2*pow(r/beamRadius,2));
  return ionizationRateConstant*pow(beamIntensity,2);
}

double Laser::GetIonizationDensityAt(double x, double y, double z) {
  return GetIonizationDensityAt(sqrt(pow(y,2)+pow(z,2)), x);
}

int Laser::PulseMC() {
  // calculate average primary ionization by monte carlo integration
  int nSamples = 100000; // n of monte carlo samples
  double x, y, z;
  double averagePrimaryIonizationMC = 0.;
  cout << "Calculating primary ionization...";
  for (int i=0; i<nSamples; i++) {
    x = gasX1+((double)rand())/RAND_MAX*(gasX2-gasX1);
    y = gasY1+((double)rand())/RAND_MAX*(gasY2-gasY1);
    z = gasZ1+((double)rand())/RAND_MAX*(gasZ2-gasZ1);
    averagePrimaryIonizationMC += GetIonizationDensityAt(x, y, z);
    //cout << averagePrimaryIonizationMC << endl;
    cout << "\rCalculating primary ionization... " << (float)i/nSamples*100 << "%";
  }
  cout << "\rCalculating primary ionization....... Done" << endl;
  
  averagePrimaryIonizationMC /= nSamples;
  averagePrimaryIonizationMC *= (gasX2-gasX1)*(gasY2-gasY1)*(gasZ2-gasZ1);
  this->averagePrimaryIonization = (int) averagePrimaryIonizationMC;
  //cout << "Average primary ionization " << averagePrimaryIonization << endl;
  return averagePrimaryIonization;

  // create poisson distribution generator
  /*fPoissonGenerator = default_random_engine();
  fPoisson = poisson_distribution<int>(fAveragePrimaryIonization);
  fMaxIonizationDensity = GetIonizationDensityAt(0., 0.);

  return fPoisson(fPoissonGenerator);*/
}

/*void Laser::GetPrimaryElectron(double &x, double &y, double &z) {
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
}*/