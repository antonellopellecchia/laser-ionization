#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

#include "Laser.hh"

Laser::Laser(double wavelength, double pulseEnergy, double waistRadius, Point3D waistPosition) {
  this->wavelength = wavelength;
  this->pulseEnergy=pulseEnergy;
  this->waistRadius = waistRadius;
  this->waistPosition = waistPosition;
  this->ionizationDensity.SetExpression(this, &Laser::GetIonizationDensityAt);
  this->ionizationDensity.SetCodomain(0., GetIonizationDensityAt(waistPosition));
  CalculateAveragePrimaries();
}

void Laser::SetWaistPosition(Point3D waistPosition) {
  this->waistPosition = waistPosition;
  CalculateAveragePrimaries();
}

void Laser::SetGasVolume(Point3D vertex1, Point3D vertex2) {
  gasVertex1 = vertex1;
  gasVertex2 = vertex2;
  this->ionizationDensity.SetDomain(gasVertex1, gasVertex2);
  CalculateAveragePrimaries();
}

void Laser::SetWavelength(double wavelength) {
  this->wavelength=wavelength;
  CalculateAveragePrimaries();
}

void Laser::SetWaistRadius(double waistRadius) {
  this->waistRadius=waistRadius;
  CalculateAveragePrimaries();
}

void Laser::SetPulseEnergy(double pulseEnergy) {
  this->pulseEnergy=pulseEnergy;
  CalculateAveragePrimaries();
}

double Laser::GetRayleighRange() const {
  return pi*pow(this->waistRadius,2)/this->wavelength;
}

double Laser::GetIonizationDensityAt(Point3D position) const {
  double r = sqrt(pow(position.y-waistPosition.y,2)+pow(position.z-waistPosition.z,2));
  double beamRadius = beamQuality*waistRadius*sqrt(1+pow((position.x-waistPosition.x)/GetRayleighRange(),2));
  double beamIntensity = 2*pulseEnergy/(pi*pow(waistRadius,2))*pow(waistRadius/beamRadius,2)*exp(-2*pow(r/beamRadius,2));
  return ionizationRateCrossSection*pow(beamIntensity,2);
}

void Laser::CalculateAveragePrimaries() {
  // calculate average primary ionization by monte carlo integration
  int nsamples = 100000;
  this->averagePrimaryIonization = (int) this->ionizationDensity.IntegrateMonteCarlo(nsamples);

  // create random poisson variable
  // with average value just calculated
  this->randomGenerator = std::default_random_engine();
  this->poissonDistribution = std::poisson_distribution<int>(averagePrimaryIonization);
}

int Laser::Pulse() {
  // sample from Poisson distribution with average calculated in Initialize()
  if (this->averagePrimaryIonization<0) CalculateAveragePrimaries();
  return poissonDistribution(randomGenerator);
}

Point3D Laser::GetPrimaryElectron() {
  // sample primary electron position
  // from ionization density function
  return this->ionizationDensity.SamplePoint();
}