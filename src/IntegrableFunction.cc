#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>

#include "IntegrableFunction.hh"
#include "Laser.hh"

template<class T> IntegrableFunction<T>::IntegrableFunction(bool verbose) {
  this->verbose = verbose;
  this->randomGenerator = std::default_random_engine();
  this->uniformDistribution = std::uniform_real_distribution<double>(0., 1.);
}

template<class T> void IntegrableFunction<T>::SetExpression(T *parameterObject, std::function<double(T&,Point3D)> function) {
  this->parameterObject = parameterObject;
  this->functionExpression = function;
}

template<class T> double IntegrableFunction<T>::Evaluate(Point3D position) {
  return this->functionExpression(*parameterObject, position);
}

template<class T> void IntegrableFunction<T>::SetDomain(Point3D domain1, Point3D domain2) {
  this->domain1 = domain1;
  this->domain2 = domain2;
}

template<class T> void IntegrableFunction<T>::SetCodomain(double min, double max) {
  // needed for rejection sampling
  this->ymin = min;
  this->ymax = max;
}

template<class T> double IntegrableFunction<T>::GetDomainVolume() {
  return (this->domain2.x-this->domain1.x)*(this->domain2.y-this->domain1.y)*(this->domain2.z-this->domain1.z);
}

template<class T> Point3D IntegrableFunction<T>::SamplePoint() {
  // sample point according to distribution
  // by rejection sampling
  double sampledY = 0., calculatedY = 0.;

  Point3D position;
  Point3D generatedPoint; // temporary point with coordinates from 0 to 1
  do {
    Point3D generatedPoint{uniformDistribution(randomGenerator), uniformDistribution(randomGenerator), uniformDistribution(randomGenerator)};
    position = this->domain1 + generatedPoint*(this->domain2-this->domain1); // rescale temp point to function domain
    sampledY = this->ymin + this->uniformDistribution(randomGenerator)*this->ymax;
    calculatedY = this->Evaluate(position);
  } while (calculatedY<sampledY);

  return position;
}

template<class T> double IntegrableFunction<T>::IntegrateMonteCarlo(int nsamples) {
  double averagePrimaryIonization = 0.;
  if (verbose) std::cout << "Calculating primary ionization...";

  Point3D position;
  Point3D generatedPoint;
  for (int i=0; i<nsamples; i++) {
    Point3D generatedPoint{((double)rand())/RAND_MAX, ((double)rand())/RAND_MAX, ((double)rand())/RAND_MAX};
    position = this->domain1 + generatedPoint*(this->domain2-this->domain1);
    averagePrimaryIonization += this->Evaluate(position);
    if (verbose) std::cout << "\rCalculating primary ionization... " << (float)i/nsamples*100 << "%";
  }
  if (verbose) std::cout << "\rCalculating primary ionization....... Done" << std::endl;

  averagePrimaryIonization /= nsamples;
  averagePrimaryIonization *= this->GetDomainVolume();

  return averagePrimaryIonization;
}

template class IntegrableFunction<Laser>;