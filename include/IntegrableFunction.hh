#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>

#include "Point3D.hh"

#ifndef INTEGRABLE_FUNCTION_DEFINITION
#define INTEGRABLE_FUNCTION_DEFINITION

template<class T> class IntegrableFunction {
public:
  IntegrableFunction(bool verbose=false);

  void SetExpression(T*, std::function<double(T&,Point3D)>);
  void SetDomain(Point3D, Point3D);
  void SetCodomain(double, double);

  double GetDomainVolume();
  double Evaluate(Point3D);
  Point3D SamplePoint();
  double IntegrateMonteCarlo(int);
    
private:
  bool verbose;

  T *parameterObject; // holds function parameters
  Point3D domain1, domain2;
  double ymin=0, ymax=0;
  std::function<double(T&,Point3D)> functionExpression;

  std::uniform_real_distribution<double> uniformDistribution;
  std::default_random_engine randomGenerator;
};

#endif