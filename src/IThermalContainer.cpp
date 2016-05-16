#include <iostream>
#include "IThermalContainer.h"

ClassImp(IThermalContainer);

IThermalContainer::IThermalContainer()
    : fDensity(0.), fCapacity(0.),
      fRRR(0.), fField(0.),
      fPreT(0.),
      fTemp(0.), fSource(0.)//, fName("")
{}

IThermalContainer::~IThermalContainer()
{
  //fk.clear();
  //fId.clear();
}

void IThermalContainer::SetId(int z, int p, int r)
{
  fId.push_back(z);
  fId.push_back(p);
  fId.push_back(r);
}

void IThermalContainer::SetConductivity(double kz, double kp, double kr)
{
  fk.push_back(kz);
  fk.push_back(kp);
  fk.push_back(kr);
}


// **********************
ClassImp(IElectricContainer);

IElectricContainer::IElectricContainer()
    : IThermalContainer(),
      fCurrent(0.), fResist(0.), fVoltage(0.),
      fQchON(false)
{}
