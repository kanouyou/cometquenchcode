/*!
   @file
   @brief  Quench simulation code for COMET capture solenoid
   @author Y.YANG
   @date   07-05-2016
*/

#include <iostream>
#include <iomanip>
#include <TApplication.h>
#include "IFdmFieldHandle.h"
#include "IFdmParameters.h"
#include "IFdmConstruction.h"
#include "IFdmException.h"

using namespace std;

int main(int argc, char** argv)
{
  // set time step
  double t0 = 0.*sec;
  double tf = 200*sec;
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
  //TApplication* app = new TApplication("app", &argc, argv);
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
/*
  FDM::IFdmFieldHandle* hand = new FDM::IFdmFieldHandle();
  hand->SetCurrent(2700.);
  hand->ConstructSolenoid("CS1", 672.*mm, 823.65*mm, -79.525*cm, 59.525*cm);
  hand->ConstructSolenoid("CS0", 672.*mm, 823.65*mm, -103.812*cm, -85.788*cm);
  hand->SetMesh("CS1", 9, 270);
  hand->SetMesh("CS0", 9, 35);
  hand->EvalField("CS1");
  hand->Plot("CS1");

  app->Run();
*/
  return 0;
}
