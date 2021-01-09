#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

#include "IntegrableFunction.hh"


IntegrableFunction::IntegrableFunction() {
  this->randomGenerator = default_random_engine();
  this->uniformDistribution = uniform_real_distribution<double>(0., 1.);
}

IntegrableFunction::~IntegrableFunction() {}

void IntegrableFunction::SetExpression(std::function<double(double,double,double)> function) {
  this->functionLambda = function;
}

double IntegrableFunction::Evaluate(double x, double y, double z) {
  return this->functionLambda(x, y, z);
}

void IntegrableFunction::SetDomain(double x1, double y1, double z1, double x2, double y2, double z2) {
  this->x1 = x1;
  this->x2 = x2;
  this->y1 = y1;
  this->y2 = y2;
  this->z1 = z1;
  this->z2 = z2;
}

void IntegrableFunction::SetCodomain(double min, double max) {
  // needed for rejection sampling
  this->ymin = min;
  this->ymax = max;
}

void IntegrableFunction::SamplePoint(double &x, double &y, double &z) {
  // sample point according to distribution
  // by rejection sampling
  double sampledY = 0., calculatedY = 0.;
  do {
    x = this->x1 + this->uniformDistribution(randomGenerator) * (this->x2-this->x1);
    y = this->y1 + this->uniformDistribution(randomGenerator) * (this->y2-this->y1);
    z = this->z1 + this->uniformDistribution(randomGenerator) * (this->z2-this->z1);
    sampledY = this->ymin + this->uniformDistribution(randomGenerator)*this->ymax;
    calculatedY = this->Evaluate(x, y, z);
  } while (calculatedY<sampledY);
}

double IntegrableFunction::IntegrateMonteCarlo(int nsamples) {
  double x, y, z;
  double averagePrimaryIonization = 0.;
  cout << "Calculating primary ionization...";
  for (int i=0; i<nsamples; i++) {
    x = this->x1 + ((double)rand())/RAND_MAX * (this->x2 - this->x1);
    y = this->y1 + ((double)rand())/RAND_MAX * (this->y2 - this->y1);
    z = this->z1 + ((double)rand())/RAND_MAX * (this->z2 - this->z1);
    averagePrimaryIonization += this->Evaluate(x, y, z);
    cout << "\rCalculating primary ionization... " << (float)i/nsamples*100 << "%";
  }
  cout << "\rCalculating primary ionization....... Done" << endl;

  averagePrimaryIonization /= nsamples;
  averagePrimaryIonization *= (this->x2-this->x1)*(this->y2-this->y1)*(this->z2-this->z1);
  return averagePrimaryIonization;
}