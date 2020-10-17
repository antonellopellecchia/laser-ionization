#include "Laser.cc"

using namespace std;

int main(int argc, char * argv[]) {
  Laser *laser = new Laser();
  laser->SetWavelength(266e-9);
  laser->SetWaistRadius(1500e-6);
  laser->SetDirection(1., 0., 0.);
  laser->SetWaistPosition(0., 0., 0.02);
  laser->SetBoundaries(5e-2, 5e-2, 4e-2); // units in m
  laser->SetPulseEnergy(51e-6);
  laser->SetDebugging(true);

  int nprimaries = laser->Pulse();

  cout << nprimaries << endl;
}
