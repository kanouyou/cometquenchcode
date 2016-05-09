#ifndef IFdmThermalSolver_HH
#define IFdmThermalSolver_HH

#include <vector>
#include "IFdmParameters.h"
#include "IFdmAluminium.h"
#include "IFdmCopper.h"
#include "IFdmKapton.h"
#include "IFdmNbTi.h"
#include "IThermalContainer.h"

namespace FDM
{ class IFdmThermalSolver; }

class FDM::IFdmThermalSolver
{
  public:
    IFdmThermalSolver();

    IFdmThermalSolver(const int Mz, const int Mp, const int Mr);

    ~IFdmThermalSolver();

    void SetMesh (const int MZ, const int MP, const int MR);

    void SetField(double B);

    void SetRRR(double RRR);

    void SetLayer(const int layer) { fLayer = layer; }

    void SetTurn (const int turn)  { fTurn = turn; }

    void SetCurrent(const double I) { fCurrent = I; }

    double GetAvgkz(double l1, double k1, double l2, double k2);

    double GetAvgkp(double A1, double k1, double A2, double k2);

    std::vector<IElectricContainer*> GetContainerVector() { return fHC; }

    IElectricContainer* GetContainer(int i, int j, int k);

    void Fill(IElectricContainer* hc, double T0, double B0, double RRR);

    void Initialize();

    double GetResistance();

    void Upgrade(double I, double B);

    double Solve(double dt);

  private:
    static std::vector<IElectricContainer*> fHC;

  private:
    int* fMesh;
    int  fLayer;
    int  fTurn;
    double fCurrent;
    FDM::IFdmAluminium* fAl;
    FDM::IFdmCopper* fCu;
    FDM::IFdmKapton* fTape;
    FDM::IFdmNbTi*   fSC;
    double fRRR;
    double fField;
};

#endif
