#ifndef IThermalContainer_HH
#define IThermalContainer_HH

#include <TObject.h>

class IThermalContainer : public TObject
{
  public:
    IThermalContainer();

    ~IThermalContainer();

    void SetId(int z, int p, int r);

    void SetDensity(double rho) { fDensity = rho; }

    void SetCapacity(double C) { fCapacity = C; }

    void SetRRR(double rrr) { fRRR = rrr; }

    void SetField(double B) { fField = B; }

    void SetConductivity(double kz, double kp, double kr);

    void SetPreTemp(double T) { fPreT = T; }    

    void SetTemp(double T) { fTemp = T; }

    void SetSource(double Q) { fSource = Q; }

    // Get Method
    int*   GetId() { return fId; }

    double GetDensity() { return fDensity; }

    double GetCapacity() { return  fCapacity; } 

    double GetRRR() { return fRRR; }

    double GetField() { return fField; }

    double* GetConductivity() { return fk; }

    double GetPreTemp() { return fPreT; }

    double GetTemp() { return fTemp; }

    double GetSource() { return fSource; }

  protected:
    int*    fId;
    double  fDensity;
    double  fCapacity;
    double  fRRR;
    double  fField;
    double* fk;
    double  fPreT;
    double  fTemp;
    double  fSource;
};


class IElectricContainer : public IThermalContainer
{
  public:
    IElectricContainer();

    ~IElectricContainer() {}

    void SetCurrent(double I) { fCurrent = I; }

    void SetResistance(double R) { fResist = R; }

    void SetVoltage(double V) { fVoltage = V; }

    void SetQuench(bool qch) { fQchON = qch; }

    // Get Info
    double GetCurrent() { return fCurrent; }

    double GetResistance() { return fResist; }

    double GetVoltage() { return fVoltage; }

    double GetQuench() { return fQchON; }

    ClassDef(IElectricContainer, 1);

  private:
    double fCurrent;
    double fResist;
    double fVoltage;
    bool   fQchON;
};

#endif
