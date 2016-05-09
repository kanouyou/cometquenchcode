#ifndef IFdmConstruction_HH
#define IFdmConstruction_HH

#include "IFdmThermalSolver.h"

namespace FDM
{ class IFdmConstruction; }


class FDM::IFdmConstruction
{
  public:
    IFdmConstruction();

    ~IFdmConstruction();

    void SetTime(double, double, double);

    void SetCurrent(double I) { fCurrent = I; }

    void SetInductance(double L) { fIndct = L; }

    void SetDumpResistor(double R) { fDump = R; }

    void SetThreshold(double V) { fThV = V; }

    void Construct();

    void TimeLoop();

  private:
    double* fTime;
    double  fDump;
    double  fIndct;
    double  fCurrent;
    double  fThV;
    FDM::IFdmThermalSolver* fCS0;
    FDM::IFdmThermalSolver* fCS1;
};

#endif
