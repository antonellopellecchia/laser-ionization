#include <math.h>
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>

using namespace std;

class Function;

class Function {
public:
  Function(int dimension);
  virtual ~Function();

  void SetExpression(std::function<double(double,double,double)>);
  void SetDomain(double, double, double, double, double, double);

  double Evaluate(double, double, double);
  void SamplePoint(double, double, double);
  double IntegrateMonteCarlo(int);
    
private:
  int dimension;
  double x1, y1, z1, x2, y2, z2;
  std::function<double(double,double,double)> functionLambda;
};
