#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/stat.h>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>

#include "Garfield/ComponentAnsys123.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewFEMesh.hh"
#include "Garfield/ViewSignal.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/DriftLineRKF.hh"
#include "Garfield/Random.hh"
#include "Garfield/Plotting.hh"

#include "Laser.hh"
#include "Point3D.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  /* initialize the electric field calculated with Ansys */
  ComponentAnsys123 fieldMap;
  const std::string ansys_path = std::string("field");
  fieldMap.Initialise(ansys_path+"/ELIST.lis", ansys_path+"/NLIST.lis", ansys_path+"/MPLIST.lis", ansys_path+"/PRNSOL.lis", "mm");
  fieldMap.EnableMirrorPeriodicityX();
  fieldMap.EnableMirrorPeriodicityY();
  fieldMap.PrintRange();
  
  
  /* initialize the gas parameters to run Magboltz */
  MediumMagboltz gas;
  gas.SetComposition("ar", 70., "co2", 30.);
  gas.SetTemperature(293.15);
  gas.SetPressure(760.);
  gas.Initialise(true);
  const double rPenning = 0.57;
  const double lambdaPenning = 0.;
  gas.EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  fieldMap.SetMedium(1, &gas);


  /* plot electrostatic field in gas */ 
  const double pitch = 140e-4; // GEM hole pitch
  const double zbot = -0.03;
  const double ztop = +0.03;
  ViewField fieldView;
  fieldView.SetComponent(&fieldMap);
  fieldView.SetPlane(0, -1, 0, 0, 0, 0);
  fieldView.SetArea(-pitch, zbot, pitch, ztop);
  fieldView.SetVoltageRange(-160., 160.);
  TCanvas fieldCanvas{"field", "", 800, 600};
  fieldView.SetCanvas(&fieldCanvas);
  fieldView.PlotContour();
  mkdir("plots/", 0700);
  fieldCanvas.SaveAs("plots/potential.eps");


  /* laser setup */
  double laserWavelength = 266e-9;
  double laserPulseEnergy = 1e-6;
  double laserWaistRadius = 25e-6;
  Laser laser{laserWavelength, laserPulseEnergy, laserWaistRadius, Point3D(0., 0., 200e-6)};
  const double sizeXY = 400e-6, sizeZ = 200e-6, centerZ = 200e-6; // restrict to 200x200x200 um^3 gas volume
  Point3D gasVolume1{-0.5*sizeXY, -0.5*sizeXY, centerZ-0.5*sizeZ}, gasVolume2{+0.5*sizeXY, +0.5*sizeXY, centerZ+0.5*sizeZ};
  laser.SetGasVolume(gasVolume1, gasVolume2);

  /* inizialize sensor object, useful for drift lines visualization */
  Sensor sensor;
  sensor.AddComponent(&fieldMap);
  sensor.SetArea(-5*pitch, -5*pitch, -0.02, 5*pitch, 5*pitch, 0.03);

  /* create primary electrons by laser ionization and calculate drift lines */
  AvalancheMicroscopic aval;
  aval.SetSensor(&sensor);
  ViewDrift driftView;
  aval.EnablePlotting(&driftView);

  TH1F primaryHistoX{"PrimaryX", ";x position [cm];", 50, -0.03, 0.03};
  TH1F primaryHistoZ{"PrimaryZ", ";z position [cm];", 60, 0.01, 0.04};

  int nprimaries = laser.Pulse();
  //double x0 = 0., y0 = 0., z0 = 0.;
  double dx0 = 0., dy0 = 0., dz0 = 0., e0=0.1, t0=0.;
  int nTotalElectrons = 0;
  std::cout << std::endl;
  Point3D electronPosition;
  for (int nelectron=0; nelectron<nprimaries; nelectron++) {
    electronPosition = laser.GetPrimaryElectron(); // retrieve position of nth primary electron
    //electronPosition.x*=1e2, electronPosition.y*=1e2, electronPosition.z*=1e2; // Garfield uses cm as unit
    electronPosition*=1e2; // Garfield uses cm as unit
    primaryHistoX.Fill(electronPosition.x); primaryHistoZ.Fill(electronPosition.z); // fill x and z histograms
    aval.AvalancheElectron(electronPosition.x, electronPosition.y, electronPosition.z, t0, e0, dx0, dy0, dz0); // calculate drift and avalanche for primary electron
    unsigned int nAvalancheElectrons = aval.GetNumberOfElectronEndpoints(); // calculate number of electrons after avalanche
    nTotalElectrons += nAvalancheElectrons;
    std::cout << "\rElectron " << nelectron << "/" << nprimaries << ", (";
    std::cout << electronPosition.x << "," << electronPosition.y << "," << electronPosition.z;
    std::cout << ")           ";
  }
  std::cout << std::endl;
  std::cout << nprimaries << " primary electrons " << std::endl;
  std::cout << nTotalElectrons << " total electrons " << std::endl;


  /* plot x and y distributions of primary electrons */
  TCanvas primaryPositionCanvas{"PrimaryPositionCanvas", "", 1800, 800};
  primaryPositionCanvas.Divide(2, 1);
  primaryPositionCanvas.cd(1);
  primaryHistoX.Draw();
  primaryPositionCanvas.cd(2);
  primaryHistoZ.Draw();
  primaryPositionCanvas.SaveAs("plots/primaries.eps");


  /* plot drift lines */
  ViewFEMesh meshView;
  meshView.SetArea(-2*pitch, -2*pitch, -0.02, 2*pitch, 2*pitch, 0.03);
  TCanvas driftCanvas{"drift", "", 800, 600};
  meshView.SetCanvas(&driftCanvas);
  meshView.SetComponent(&fieldMap);
  meshView.SetPlane(0, -1, 0, 0, 0, 0);
  meshView.SetFillMesh(true); 
  meshView.SetColor(0, kYellow + 3);
  meshView.SetColor(1, kWhite);
  meshView.SetColor(2, kYellow + 2);
  meshView.EnableAxes();
  meshView.SetXaxisTitle("x [cm]");
  meshView.SetYaxisTitle("z [cm]");
  meshView.SetViewDrift(&driftView);
  meshView.Plot();
  driftCanvas.SaveAs("plots/drift.eps");
  
  return 0;
}
