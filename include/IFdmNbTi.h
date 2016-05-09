#ifndef IFdmNbTi_HH
#define IFdmNbTi_HH

namespace FDM
{ class IFdmNbTi; }

class FDM::IFdmNbTi
{
  public:
    static IFdmNbTi* Instance();

    ~IFdmNbTi();

    void SetField(const double B) { fField = B; }

    double GetCapacity(double T);

    double GetDensity() const { return 6538.; }

    // calculate the critical current for NbTi
    double GetCriticalCurrent(double T);

    // calculate the critical temperature for NbTi
    double GetCriticalTemp(double B);

    // calculate current sharing temperture
    double GetCSTemp(double T, double B, double I);

  private:
    static IFdmNbTi* inst;

    IFdmNbTi();

  private:
    double fField;
};

#endif
