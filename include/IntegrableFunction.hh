#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>

using namespace std;

class IntegrableFunction;

class IntegrableFunction {
public:
  IntegrableFunction();
  virtual ~IntegrableFunction();

  void SetExpression(std::function<double(double,double,double)>);
  void SetDomain(double, double, double, double, double, double);
  void SetCodomain(double, double);

  double Evaluate(double, double, double);
  void SamplePoint(double &, double &, double &);
  double IntegrateMonteCarlo(int);
    
private:
  double x1, y1, z1, x2, y2, z2;
  double ymin, ymax;
  std::function<double(double,double,double)> functionLambda;

  uniform_real_distribution<double> uniformDistribution;
  default_random_engine randomGenerator;
};
