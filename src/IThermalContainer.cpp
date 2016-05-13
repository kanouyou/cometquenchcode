#include <iostream>
#include "IThermalContainer.h"

ClassImp(IElectricContainer);

IThermalContainer::IThermalContainer()
    : fId(NULL), fDensity(0.), fCapacity(0.),
      fRRR(0.), fField(0.),
      fk(NULL), fPreT(0.),
      fTemp(0.), fSource(0.)
{}

IThermalContainer::~IThermalContainer()
{
  if (fk)  delete[] fk;
  if (fId) delete[] fId;
}

void IThermalContainer::SetId(int z, int p, int r)
{
  if (!fId) fId = new int[3];

  fId[0] = z;
  fId[1] = p;
  fId[2] = r;
}

void IThermalContainer::SetConductivity(double kz, double kp, double kr)
{
  if (!fk)  fk = new double[3];

  fk[0] = kz;
  fk[1] = kp;
  fk[2] = kr;
}


// **********************

IElectricContainer::IElectricContainer()
    : IThermalContainer(),
      fCurrent(0.), fResist(0.), fVoltage(0.),
      fQchON(false)
{}
