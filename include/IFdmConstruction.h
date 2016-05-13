#ifndef IFdmConstruction_HH
#define IFdmConstruction_HH

#include <vector>
#include "IFdmField.h"
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

    void SetDiode(double V) { fVd = V; }

    void SetThreshold(double V) { fThV = V; }

    /*! calculate current decay and return the current at this time */
    double CurrentDecay(double R, double t);

    /*! calculate field decay */
    void FieldDecay(FDM::IFdmThermalSolver* mag, double factor);

    /*! calculate initialized field and store into field container */
    void ConstructField(const std::string& name, FDM::IFdmThermalSolver* mag);

    void Construct();

    void TimeLoop();

  private:
    double* fTime;
    double  fDump;
    double  fIndct;
    double  fCurrent;
    double  fThV;
    double  fVd;
    FDM::IFdmThermalSolver* fCS0;
    FDM::IFdmThermalSolver* fCS1;
};

#endif
