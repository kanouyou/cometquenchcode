#ifndef IFdmBiotSavart_HH
#define IFdmBiotSavart_HH

namespace FDM
{ class IFdmBiotSavart; }

class FDM::IFdmBiotSavart
{
  public:
    /*! constructor */
    IFdmBiotSavart();

    /*! deconstructor */
    ~IFdmBiotSavart();

    /*! set conductor dimension */
    void SetConductor(double l, double w) { fACdt = l * w; }

    /*! Setup the solenoid structure parameters */
    void SetSolenoid(double r0, double r1, double z1, double z2);

    /*! Setup solenoid mesh */
    void SetSolenoidMesh(const int mr, const int mp, const int mz);

    /*! Setup flowed current along phi direction */
    void SetCurrent(double I) { fCurrent = I / fACdt; }

    /*! Setup flowed current density along phi direction */
    void SetCurrentDensity(double J) { fCurrent = J; }

    /*! Calculate magnetic potential at (r,z) */
    double EvalPotential(double pr, double pz);

    /*! return the current density */
    double GetCurrentDensity() { return fCurrent; }

    /*! return the current */
    double GetCurrent() { return fCurrent * fACdt; }

    /*! return the information of solenoid */
    double* GetSolenoid() { return fSolenoid; }

    /*! return the mesh cutting of solenoid */
    int* GetSolenoidMesh() { return fSolMesh; }

  private:
    double* fSolenoid;
    int*    fSolMesh;
    double  fCurrent;
    double  fACdt;
};

#endif
