/*!
   @file   main.cpp
   @brief  Quench simulation code for COMET capture solenoid
   @author Y.YANG
   @date   07-05-2016
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <TApplication.h>
#include "IFdmFieldHandle.h"
#include "IFdmParameters.h"
#include "IFdmConstruction.h"
#include "IFdmException.h"
#include "IMyPostprocess.h"

using namespace std;

/*! usage */
void Usage()
{
  std::cout << " Quench Simulation of Capture Superconducting Solenoid for COMET Experiment\n" << std::endl;
  std::cout << " Usage: " << std::endl;
  std::cout << "         ./SimCOMETCaptureQuench.exe -[option] <string> \n" << std::endl;
  std::cout << " Option: " << std::endl;
  std::cout << "         -f or --field <magnet>          Calculate and plot field inside this magnet." << std::endl;
  std::cout << "         -p or --post  <file>            Make graph from output and save it into file." << std::endl;
  std::cout << "         -r or --run                     Run the quench simulation." << std::endl;
  std::cout << "         -t ot --time  <time>            Set simulated quenched time. " << std::endl;
}

/*! plot the magnetic field inside the magnet */
void PlotMagnetField(const string& name)
{
  FDM::IFdmFieldHandle* hand = new FDM::IFdmFieldHandle();
  hand->SetCurrent(2700.);
  hand->ConstructSolenoid("CS1", 672.*mm, 823.65*mm, -79.525*cm, 59.525*cm);
  hand->ConstructSolenoid("CS0", 672.*mm, 823.65*mm, -103.812*cm, -85.788*cm);
  hand->SetMesh("CS1", 9, 270);
  hand->SetMesh("CS0", 9, 35);
  hand->EvalField(name);
  hand->Plot(name);
}

/*! run quench simulation */
void Run()
{
  // set time step
  double t0 = 0.*sec;
  double tf = 200.*sec;
  double dt = 0.01*msec;

  try {
    FDM::IFdmConstruction* cons = new FDM::IFdmConstruction();
    cons->SetTime(t0, tf, dt);
    cons->SetThreshold(0.2);
    cons->Construct();
    cons->TimeLoop();
  }
  catch (FDM::IFdmException& ex) {
    std::cerr << "Error: "  << ex.what() << "\n"
              << " Class: " << ex.GetFileName() << "\n"
              << " Function: " << ex.GetFunction() << std::endl;
  }
}

/*! postprocess */
void Post()
{
  IMyPostprocess* post = new IMyPostprocess();
  post->SetDirectory("CS0");
  post->SetFile("cs0plot.root");
  post->PlotTimeTemp();
  post->PlotField2D();
  post->PlotTemp2D();
  post->PlotHeat2D();

  IMyPostprocess* cs1 = new IMyPostprocess();
  cs1->SetDirectory("CS1");
  cs1->SetFile("cs1plot.root");
  cs1->PlotTimeTemp();
  cs1->PlotField2D();
  cs1->PlotTemp2D();
  cs1->PlotHeat2D();
}

/*! main program */
int main(int argc, char** argv)
{
  //TApplication* app = new TApplication("app", &argc, argv);

  Run();
  Post();

/*
  FDM::IFdmField* fld = new FDM::IFdmField();
  fld->SetMapMesh(9, 270);
  //fld->SetMap(672.*mm, 823.65*mm, -103.812*cm, -85.788*cm);
  fld->SetMap(672.*mm, 823.65*mm, -79.525*cm, 59.525*cm);
  //fld->SetSolenoid(672.*mm, 823.65*mm, -103.812*cm, -85.788*cm);
  fld->SetSolenoid(672.*mm, 823.65*mm, -79.525*cm, 59.525*cm);
  fld->SetSolenoidMesh(9, 50, 270);
  fld->SetCurrent(2700.);
  //fld->SetCurrentDensity(33.75e+6);
  fld->EvalField();
*/
  //app->Run();
  return 0;
}
