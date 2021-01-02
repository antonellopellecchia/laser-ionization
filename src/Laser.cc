#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

#include "Laser.hh"

#include "IntegrableFunction.hh"

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
  double beamRadius = beamQuality*waistRadius*sqrt(1+pow((x-waistX)/GetRayleighRange(),2));
  //cout << "radius " << 1.e6*beamRadius << " um" << endl;
  double beamIntensity = 2*pulseEnergy/(pi*pow(waistRadius,2))*pow(waistRadius/beamRadius,2)*exp(-2*pow(r/beamRadius,2));
  return ionizationRateConstant*pow(beamIntensity,2);
}

double Laser::GetIonizationDensityAt(double x, double y, double z) {
  return GetIonizationDensityAt(sqrt(pow(y-waistY,2)+pow(z-waistZ,2)), x-waistX);
}

void Laser::Initialize() {
  // calculate average primary ionization by monte carlo integration
  IntegrableFunction *ionizationDensity = new IntegrableFunction(3);
  ionizationDensity->SetDomain(gasX1, gasY1, gasZ1, gasX2, gasY2, gasZ2);

  double rayleighRange = this->GetRayleighRange();
  auto ionizationDensityLambda = [=](double x, double y, double z) {
    double beamRadius = beamQuality*waistRadius*sqrt(1+pow(x-waistX/rayleighRange,2));
    double r = sqrt(pow(y-waistY,2)+pow(z-waistZ,2));
    double beamIntensity = 2*pulseEnergy/(pi*pow(waistRadius,2))*pow(waistRadius/beamRadius,2)*exp(-2*pow(r/beamRadius,2));
    return ionizationRateConstant*pow(beamIntensity,2);
  };
  ionizationDensity->SetExpression(ionizationDensityLambda);

  int nsamples = 100000;
  this->averagePrimaryIonization = (int) ionizationDensity->IntegrateMonteCarlo(nsamples);
  this->maxIonizationDensity = GetIonizationDensityAt(waistX, waistY, waistZ);

  // create random poisson variable
  // with average value just calculated
  this->randomGenerator = default_random_engine();
  this->poissonDistribution = poisson_distribution<int>(averagePrimaryIonization);
}

int Laser::Pulse() {
  // sample from Poisson distribution with average calculated in Initialize()
  if (this->averagePrimaryIonization<0) this->Initialize();
  return poissonDistribution(randomGenerator);
}

void Laser::GetPrimaryElectron(double &x, double &y, double &z) {
  // sample primary electron position
  // from ionization density function

  double sampledIonizationDensity = 0.;
  double calculatedIonizationDensity = 0.;
  do {
    x = gasX1 + ((double)rand())/RAND_MAX * (gasX2-gasX1);
    y = gasY1 + ((double)rand())/RAND_MAX * (gasY2-gasY1);
    z = gasZ1 + ((double)rand())/RAND_MAX * (gasZ2-gasZ1);
    sampledIonizationDensity = ((double)rand())/RAND_MAX*maxIonizationDensity;
    calculatedIonizationDensity = GetIonizationDensityAt(x, y, z);
  } while (calculatedIonizationDensity<sampledIonizationDensity);
  //x+=WaistX; y+=WaistY; z+=WaistZ; // translate to waist position
  //x*=1.e2; y*=1.e2; z*=1.e2; // convert to garfield units
}