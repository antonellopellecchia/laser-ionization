#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

#ifndef LASER_DEFINITION
#define LASER_DEFINITION

#include "IntegrableFunction.hh"
#include "Point3D.hh"

class Laser {
public:
  Laser(double wavelength, double pulseEnergy, double waistRadius, Point3D waistPosition);

  double GetIonizationDensityAt(Point3D) const;
  
  void SetWavelength(double wavelength);
  void SetWaistRadius(double waistRadius);
  void SetPulseEnergy(double pulseEnergy);
  void SetWaistPosition(Point3D);
  void SetGasVolume(Point3D, Point3D);

  double GetWavelength() {return this->wavelength;}
  double GetPulseEnergy() {return this->pulseEnergy;}
  double GetWaistRadius() {return this->waistRadius;}
  double GetRayleighRange() const;

  void CalculateAveragePrimaries();
  int Pulse();
  Point3D GetPrimaryElectron();

  void SetDebugging(bool debugging) {this->debugging=debugging;}
    
private:
  IntegrableFunction<Laser> ionizationDensity;
  std::poisson_distribution<int> poissonDistribution;
  std::default_random_engine randomGenerator;

  double wavelength;
  double pulseEnergy;
  double waistRadius;
  Point3D waistPosition;
  Point3D gasVertex1;
  Point3D gasVertex2;

  const double pi = atan(1)*4;
  const double ionizationRateCrossSection = 2948437307.082676;
  const double beamQuality = 1.5;

  int averagePrimaryIonization = -1;

  bool debugging = false;
};

#endif