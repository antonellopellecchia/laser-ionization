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

#include "Laser.cc"

using namespace Garfield;
using namespace std;

int main(int argc, char * argv[]) {
  //plottingEngine.SetDefaultStyle();
  
  // initialize the electric field and potential maps
  ComponentAnsys123* fm = new ComponentAnsys123();
  const string ansys_path = string("field");
  fm->Initialise(ansys_path+"/ELIST.lis", ansys_path+"/NLIST.lis", ansys_path+"/MPLIST.lis", ansys_path+"/PRNSOL.lis", "mm");
  //fm->SetWeightingField(ansys_path+"/WEIGHTSOL.lis", "ground");
  fm->EnableDeleteBackgroundElements(false);
  fm->PrintMaterials();
  fm->EnableMirrorPeriodicityX();
  fm->EnableMirrorPeriodicityY();
  fm->PrintRange();
  fm->NotDriftMedium(1);

  // sizes in cm
  
  // initialize the gas parameters
  MediumMagboltz* gas = new MediumMagboltz();
  gas->SetComposition("ar", 70., "co2", 30.);
  gas->SetTemperature(293.15);
  gas->SetPressure(760.);
  gas->SetMaxElectronEnergy(200.);
  gas->LoadGasFile("ar_70_co2_30.gas");
  gas->Initialise(true);
  const double rPenning = 0.57;
  const double lambdaPenning = 0.;
  gas->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  fm->SetMedium(1, gas);

  // ion mobility parameters
  const std::string path = getenv("GARFIELD_HOME");
  gas->LoadIonMobility(path + "/Data/IonMobility_Ar+_Ar.txt");

  const double pitch = 140e-4;
  const double zbot = -0.03;
  const double ztop = +0.03;
  ViewField *fieldView = new ViewField();
  fieldView->SetComponent(fm);
  fieldView->SetPlane(0, -1, 0, 0, 0, 0);
  fieldView->SetArea(-pitch, zbot, pitch, ztop);
  fieldView->SetVoltageRange(-1000., 0.);
  fieldView->SetNumberOfContours(50);
  //fieldView->SetNumberOfSamples2d(30, 30);

  TCanvas *fieldCanvas = new TCanvas("field", "", 800, 600);
  fieldView->SetCanvas(fieldCanvas);
  fieldView->Plot("v", "CONT1;COLZ");
  fieldCanvas->SaveAs("plots/potential.eps");

  Laser *laser = new Laser();
  laser->SetWavelength(266e-9);
  laser->SetWaistRadius(1500e-6);
  laser->SetDirection(1., 0., 0.);
  laser->SetWaistPosition(0., 0., 0.02);
  laser->SetBoundaries(5e-2, 5e-2, 4e-2); // units in m
  laser->SetPulseEnergy(51e-6);
  laser->SetDebugging(true);

  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  int nprimaries = laser->Pulse();

  double x0 = 0., y0 = 0., z0 = 0.;
  double dx0 = 0., dy0 = 0., dz0 = 0., e0=0., t0=0.;
  int nTotalElectrons = 0;
  for (int nelectron=0; nelectron<nprimaries; nelectron++) {
      laser->GetPrimaryElectron(nelectron, x0, y0, z0);
      aval->AvalancheElectron(x0, y0, z0, t0, e0, dx0, dy0, dz0);
      unsigned int nAvalancheElectrons = aval->GetNumberOfElectronEndpoints();
      nTotalElectrons += nAvalancheElectrons;
    }
    cout << nprimaries << " primary electrons, " << endl;
    cout << nTotalElectrons << " total electrons, " << endl;
  }
  
  return 0;
}
