#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

using namespace std;

class Laser;

class Laser {
public:
  Laser(double wavelength, double waistRadius);
  virtual ~Laser();

  double GetIonizationDensityAt(double r, double z);
  double GetIonizationDensityAt(double x, double y, double z);
  
  // setter methods
  void SetWavelength(double wavelength) {this->wavelength=wavelength;}
  void SetWaistRadius(double waistRadius) {this->waistRadius=waistRadius;}
  void SetPulseEnergy(double pulseEnergy) {this->pulseEnergy=pulseEnergy;}
  void SetWaistPosition(double x, double y, double z);
  void SetBoundaries(double x, double y, double z);

  // getter methods
  double GetWavelength() {return this->wavelength;}
  double GetPulseEnergy() {return this->pulseEnergy;}
  double GetWaistRadius() {return this->waistRadius;}
  double GetRayleighRange() {return pi*pow(waistRadius,2)/wavelength;}

  double Pulse();
  void GetPrimaryElectron(double &x, double &y, double &z);

  void SetDebugging(bool debugging) {this->debugging=debugging;}
    
private:
  void UpdateRayleighRange()
  poisson_distribution<int> poisson;
  default_random_engine poissonGenerator;

  double wavelength;
  double pulseEnergy;
  double waistRadius;
  double directionX, directionY, directionZ;
  double waistX, waistY, waistZ;
  double sizeX, sizeY, sizeZ;
  
  double maxIonizationDensity = 0.;
  
  const double pi = atan(1)*4;
  const double ionizationRateConstant = 2948437307.082676;
  const double beamQuality = 1.5;

  double rayleighRange;
  
  int averagePrimaryIonization = -1;

  bool debugging = false;
};
