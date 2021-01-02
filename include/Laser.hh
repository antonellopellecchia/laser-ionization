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
  void SetBeamDirection(double dx, double dy, double dz);
  void SetGasVolume(double x1, double y1, double z1, double x2, double y2, double z2);

  // getter methods
  double GetWavelength() {return this->wavelength;}
  double GetPulseEnergy() {return this->pulseEnergy;}
  double GetWaistRadius() {return this->waistRadius;}
  double GetRayleighRange();

  void Initialize();
  int Pulse();
  void GetPrimaryElectron(double &x, double &y, double &z);

  void SetDebugging(bool debugging) {this->debugging=debugging;}
    
private:
  poisson_distribution<int> poissonDistribution;
  default_random_engine randomGenerator;

  double wavelength;
  double pulseEnergy;
  double waistRadius;
  double directionX, directionY, directionZ;
  double waistX, waistY, waistZ;
  double gasX1, gasY1, gasZ1;
  double gasX2, gasY2, gasZ2;

  double maxIonizationDensity = 0.;
  
  const double pi = atan(1)*4;
  const double ionizationRateConstant = 2948437307.082676;
  const double beamQuality = 1.5;

  double rayleighRange;
  
  int averagePrimaryIonization = -1;

  bool debugging = false;
};
