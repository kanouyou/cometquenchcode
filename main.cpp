/*!
   @file
   @brief  Quench simulation code for COMET capture solenoid
   @author Y.YANG
   @date   07-05-2016
*/

#include <iostream>
#include <iomanip>
#include "IFdmParameters.h"
#include "IFdmConstruction.h"
#include "IFdmException.h"

using namespace std;

int main(int argc, char** argv)
{

  // set time step
  double t0 = 0.*sec;
  double tf = 120*sec;
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

  return 0;
}
