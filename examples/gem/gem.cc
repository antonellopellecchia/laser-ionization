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

using namespace Garfield;
using namespace std;

int main(int argc, char * argv[]) {

  /* initialize the electric field calculated with Ansys */
  ComponentAnsys123* fieldMap = new ComponentAnsys123();
  const string ansys_path = string("field");
  fieldMap->Initialise(ansys_path+"/ELIST.lis", ansys_path+"/NLIST.lis", ansys_path+"/MPLIST.lis", ansys_path+"/PRNSOL.lis", "mm");
  fieldMap->EnableMirrorPeriodicityX();
  fieldMap->EnableMirrorPeriodicityY();
  fieldMap->PrintRange();
  
  
  /* initialize the gas parameters to run Magboltz */
  MediumMagboltz* gas = new MediumMagboltz();
  gas->SetComposition("ar", 70., "co2", 30.);
  gas->SetTemperature(293.15);
  gas->SetPressure(760.);
  gas->Initialise(true);
  const double rPenning = 0.57;
  const double lambdaPenning = 0.;
  gas->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  fieldMap->SetMedium(1, gas);


  /* plot electrostatic field in gas */ 
  const double pitch = 140e-4; // GEM hole pitch
  const double zbot = -0.03;
  const double ztop = +0.03;
  ViewField *fieldView = new ViewField();
  fieldView->SetComponent(fieldMap);
  fieldView->SetPlane(0, -1, 0, 0, 0, 0);
  fieldView->SetArea(-pitch, zbot, pitch, ztop);
  fieldView->SetVoltageRange(-160., 160.);
  TCanvas *fieldCanvas = new TCanvas("field", "", 800, 600);
  fieldView->SetCanvas(fieldCanvas);
  fieldView->PlotContour();
  mkdir("plots/", 0700);
  fieldCanvas->SaveAs("plots/potential.eps");


  /* laser setup */
  double laserWavelength = 266e-9;
  double laserWaistRadius = 25e-6;
  Laser *laser = new Laser(laserWavelength, laserWaistRadius);
  laser->SetBeamDirection(1., 0., 0.);
  laser->SetWaistPosition(0., 0., 200e-6);
  const double sizeXY = 400e-6, sizeZ = 200e-6, centerZ = 200e-6; // restrict to 200x200x200 um^3 gas volume
  laser->SetGasVolume(-0.5*sizeXY, -0.5*sizeXY, centerZ-0.5*sizeZ, +0.5*sizeXY, +0.5*sizeXY, centerZ+0.5*sizeZ);
  laser->SetPulseEnergy(20e-7);
  laser->SetDebugging(true);
  laser->Initialize();

  /* inizialize sensor object, useful for drift lines visualization */
  Sensor *sensor = new Sensor();
  sensor->AddComponent(fieldMap);
  sensor->SetArea(-5*pitch, -5*pitch, -0.02, 5*pitch, 5*pitch, 0.03);

  /* create primary electrons by laser ionization and calculate drift lines */
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval->SetSensor(sensor);
  ViewDrift *driftView = new ViewDrift();
  aval->EnablePlotting(driftView);

  TH1F *primaryHistoX = new TH1F("PrimaryX", ";x position [cm];", 50, -0.03, 0.03);
  TH1F *primaryHistoZ = new TH1F("PrimaryZ", ";z position [cm];", 60, 0.01, 0.04);

  int nprimaries = laser->Pulse();
  double x0 = 0., y0 = 0., z0 = 0.;
  double dx0 = 0., dy0 = 0., dz0 = 0., e0=0.1, t0=0.;
  int nTotalElectrons = 0;
  cout << endl;
  for (int nelectron=0; nelectron<nprimaries; nelectron++) {
    laser->GetPrimaryElectron(x0, y0, z0); // retrieve position of nth primary electron
    x0*=1e2, y0*=1e2, z0 *= 1e2; // Garfield uses cm as unit
    primaryHistoX->Fill(x0); primaryHistoZ->Fill(z0); // fill x and z histograms
    aval->AvalancheElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0); // calculate drift and avalanche for primary electron
    unsigned int nAvalancheElectrons = aval->GetNumberOfElectronEndpoints(); // calculate number of electrons after avalanche
    nTotalElectrons += nAvalancheElectrons;
    cout << "\rElectron " << nelectron << "/" << nprimaries << ", (" << x0 << "," << y0 << "," << z0 << ")           ";
  }
  cout << endl;
  cout << nprimaries << " primary electrons " << endl;
  cout << nTotalElectrons << " total electrons " << endl;


  /* plot x and y distributions of primary electrons */
  TCanvas *primaryPositionCanvas = new TCanvas("PrimaryPositionCanvas", "", 1800, 800);
  primaryPositionCanvas->Divide(2, 1);
  primaryPositionCanvas->cd(1);
  primaryHistoX->Draw();
  primaryPositionCanvas->cd(2);
  primaryHistoZ->Draw();
  primaryPositionCanvas->SaveAs("plots/primaries.eps");


  /* plot drift lines */
  ViewFEMesh* meshView = new ViewFEMesh();
  meshView->SetArea(-2*pitch, -2*pitch, -0.02, 2*pitch, 2*pitch, 0.03);
  TCanvas *driftCanvas = new TCanvas("drift", "", 800, 600);
  meshView->SetCanvas(driftCanvas);
  meshView->SetComponent(fieldMap);
  meshView->SetPlane(0, -1, 0, 0, 0, 0);
  meshView->SetFillMesh(true); 
  meshView->SetColor(0, kYellow + 3);
  meshView->SetColor(1, kWhite);
  meshView->SetColor(2, kYellow + 2);
  meshView->EnableAxes();
  meshView->SetXaxisTitle("x [cm]");
  meshView->SetYaxisTitle("z [cm]");
  meshView->SetViewDrift(driftView);
  meshView->Plot();
  driftCanvas->SaveAs("plots/drift.eps");
  
  return 0;
}
