#include <iostream>
#include <cmath>
#include "IFdmThermalSolver.h"

using std::vector;

/*! data container for thermal calculation. */
//vector<IElectricContainer*> FDM::IFdmThermalSolver::fHC;

FDM::IFdmThermalSolver::IFdmThermalSolver()
    : fMesh(NULL), fLayer(9),
      fTurn(270),
      fAl(NULL), fCu(NULL), fTape(NULL),
      fSC(NULL), fRRR(0.), fField(0.)
{
  fAl   = FDM::IFdmAluminium::Instance();
  fCu   = FDM::IFdmCopper::Instance();
  fTape = FDM::IFdmKapton::Instance();
  fSC   = FDM::IFdmNbTi::Instance();
}

FDM::IFdmThermalSolver::IFdmThermalSolver(const int Mz, const int Mp, const int Mr)
    : fMesh(new int[3]), fLayer(9),
      fTurn(270),
      fAl(NULL), fCu(NULL), fTape(NULL),
      fSC(NULL), fRRR(0.), fField(0.)
{
  fMesh[0] = Mz;
  fMesh[1] = Mp;
  fMesh[2] = Mr;

  fAl   = FDM::IFdmAluminium::Instance();
  fCu   = FDM::IFdmCopper::Instance();
  fTape = FDM::IFdmKapton::Instance();
  fSC   = FDM::IFdmNbTi::Instance();
}

FDM::IFdmThermalSolver::~IFdmThermalSolver()
{
  if (fMesh) delete[] fMesh;
}

void FDM::IFdmThermalSolver::SetMesh(const int MZ, const int MP, const int MR)
{
  if (!fMesh)  fMesh = new int[3];

  fMesh[0] = MZ;
  fMesh[1] = MP;
  fMesh[2] = MR;
}

void FDM::IFdmThermalSolver::SetField(double B)
{
  fField = B;
}

void FDM::IFdmThermalSolver::SetRRR(double RRR)
{
  fRRR = RRR;
}

double FDM::IFdmThermalSolver::GetAvgkz(double l1, double k1, double l2, double k2)
{
  double k = (l1 + l2) / (l1/k1 + l2/k2);
  return k;
}

double FDM::IFdmThermalSolver::GetAvgkp(double A1, double k1, double A2, double k2)
{
  double k = (A1*k1 + A2*k2) / (A1 + A2);
  return k;
}

IElectricContainer* FDM::IFdmThermalSolver::GetContainer(int i, int j, int k)
{
  return fHC[i*(fMesh[1]+2)*(fMesh[2]+2) + j*(fMesh[2]+2) + k];
}

void FDM::IFdmThermalSolver::Fill(IElectricContainer* hc, double T0, double B0, double RRR)
{
  double dlPhi  = LPHI / fMesh[1];
  double NumCdt = fTurn / fMesh[0];

  double kz, kp, kr, kCdt, kIns;
  double CAl, CCu, CSC, CAvg;
  double RAl, RCu, RAvg;

  // set property for material
  fAl->SetRRR(RRR);
  fAl->SetField(B0);
  fCu->SetRRR(RRR);
  fCu->SetField(B0);
  fSC->SetField(B0);

  // approach conductor capacity
  CAl  = RAL * fAl->GetDensity() * fAl->GetCapacity(T0) / RHOCDT;
  CCu  = RCU * fCu->GetDensity() * fCu->GetCapacity(T0) / RHOCDT;
  CSC  = RSC * fSC->GetDensity() * fSC->GetCapacity(T0) / RHOCDT;
  CAvg = CAl + CCu + CSC; 

  // thermal conductivity
  kCdt = fAl->GetThermalConductivity(T0);
  kIns = fTape->GetThermalConductivity(T0);
  kz = GetAvgkz(LZCDT, kCdt, WTAPE, kIns);
  //kp = GetAvgkp( ACDT, kCdt, ATAPE, kIns);
  kp = kCdt;
  kr = GetAvgkz(LRCDT, kCdt, WTAPE, kIns);

  // electrical resistivity
  RAl  = fAl->GetResistivity(T0) * dlPhi / AAL;
  RCu  = fCu->GetResistivity(T0) * dlPhi / ACU;
  RAvg = pow((1./RAl + 1./RCu), -1);

  RAvg /= NumCdt;

  hc->SetTemp(T0);
  hc->SetPreTemp(T0);
  hc->SetCurrent(fCurrent);
  if (hc->GetQuench()!=false) {
    hc->SetResistance(RAvg);
    hc->SetVoltage(RAvg * fCurrent);
  }
  hc->SetRRR(RRR);
  hc->SetField(B0);
  hc->SetDensity(RHOCDT);
  hc->SetCapacity(CAvg);
  hc->SetConductivity(kz, kp, kr);
}


void FDM::IFdmThermalSolver::Initialize()
{
  double T0  = 4.5;
  IElectricContainer* collect = NULL;

  for (int i=0; i<fMesh[0]+2; i++) {
    for (int j=0; j<fMesh[1]+2; j++) {
      for (int k=0; k<fMesh[2]+2; k++) {
        collect = new IElectricContainer();
        collect->SetId(i, j, k);
        Fill(collect, T0, fField, fRRR);
        fHC.push_back(collect);
      }
    }
  }  
}

double FDM::IFdmThermalSolver::GetResistance()
{
  double R = 0.;

  // calculate the total resistance of this magnet
  for (int i=1; i<fMesh[0]+1; i++) {
    for (int j=1; j<fMesh[1]+1; j++) {
      for (int k=1; k<fMesh[2]+1; k++) {
        R += GetContainer(i,j,k)->GetResistance();
      }
    }
  }

  return R;
}

void FDM::IFdmThermalSolver::Upgrade(double I)
{
  double dlPhi  = LPHI / fMesh[1];
  double NumCdt = fTurn / fMesh[0];
  I *= NumCdt;

  double kz, kp, kr, kCdt, kIns;
  double CAl, CCu, CSC, CAvg;
  double RAl, RCu, RAvg;
  double Temp, RRR, Tcs, Tc, Rcs;
  double vol, B;

  for (int i=0; i<fMesh[0]+2; i++) {
    for (int j=0; j<fMesh[1]+2; j++) {
      for (int k=0; k<fMesh[2]+2; k++) {
        Temp = GetContainer(i,j,k)->GetTemp();
        RRR  = GetContainer(i,j,k)->GetRRR();
        B    = GetContainer(i,j,k)->GetField();
        if (B<=0.) B = 0.;
        Tcs  = fSC->GetCSTemp(Temp, B, I);
        Tc   = fSC->GetCriticalTemp(B);

        // set property for material
        fAl->SetRRR(RRR);
        fAl->SetField(B);
        fCu->SetRRR(RRR);
        fCu->SetField(B);
        fSC->SetField(B);

        // approach conductor capacity
        CAl  = RAL * fAl->GetDensity() * fAl->GetCapacity(Temp) / RHOCDT;
        CCu  = RCU * fCu->GetDensity() * fCu->GetCapacity(Temp) / RHOCDT;
        CSC  = RSC * fSC->GetDensity() * fSC->GetCapacity(Temp) / RHOCDT;
        CAvg = CAl + CCu + CSC; 

        // thermal conductivity
        kCdt = fAl->GetThermalConductivity(Temp);
        kIns = fTape->GetThermalConductivity(Temp);
        kz = GetAvgkz(LZCDT, kCdt, WTAPE, kIns);
        //kp = GetAvgkp( ACDT, kCdt, ATAPE, kIns);
        kp = kCdt;
        kr = GetAvgkz(LRCDT, kCdt, WTAPE, kIns);

        // electrical resistivity
        RAl  = fAl->GetResistivity(Temp) * dlPhi / AAL;
        RCu  = fCu->GetResistivity(Temp) * dlPhi / ACU;
        RAvg = pow((1./RAl + 1./RCu), -1);

        RAvg /= NumCdt;

        // calculate volume of conductor
        vol  = NumCdt * (LZCDT + WTAPE) * (LRCDT + WTAPE) * dlPhi;

        // fill data into data class
        GetContainer(i,j,k)->SetPreTemp(Temp);
        GetContainer(i,j,k)->SetCurrent(I);
        GetContainer(i,j,k)->SetRRR(RRR);
        GetContainer(i,j,k)->SetField(B);
        GetContainer(i,j,k)->SetDensity(RHOCDT);
        GetContainer(i,j,k)->SetCapacity(CAvg);
        GetContainer(i,j,k)->SetConductivity(kz, kp, kr);

        // check whether T is exceeded over Tc or not and calculate heat generation
        if (Temp < Tcs) {
          GetContainer(i,j,k)->SetResistance(0.);
          GetContainer(i,j,k)->SetVoltage(0.);
          GetContainer(i,j,k)->SetSource(0.);
        }
        else if (Temp >= Tcs && Temp < Tc) {
          Rcs = RAvg * (Temp-Tcs) / (Tc - Tcs); 
          GetContainer(i,j,k)->SetResistance(Rcs);
          GetContainer(i,j,k)->SetVoltage(Rcs * I);
          GetContainer(i,j,k)->SetSource(I * I * Rcs / vol); 
        }
        else if (Temp >= Tc) {
          GetContainer(i,j,k)->SetResistance(RAvg);
          GetContainer(i,j,k)->SetVoltage(RAvg * I);
          GetContainer(i,j,k)->SetSource(I * I * RAvg / vol);
        }

      }
    }
  }

}

double FDM::IFdmThermalSolver::Solve(double dt)
{
  double Qzz, Qpp, Qrr, gQ;
  double Q, T, rho, C;
  double limit;
  double step = 1.;

  double NumCdt = fTurn / fMesh[0];

  for (int i=1; i<fMesh[0]+1; i++) {
    for (int j=1; j<fMesh[1]+1; j++) {
      for (int k=1; k<fMesh[2]+1; k++) {
        // applied heat transfer equation
        Qzz = GetContainer(i,j,k)->GetConductivity()[0] * (GetContainer(i+1,j,k)->GetPreTemp() - 2*GetContainer(i,j,k)->GetPreTemp()
                                                          + GetContainer(i-1,j,k)->GetPreTemp()) / pow(NumCdt*(LZCDT + WTAPE),2);
        Qpp = GetContainer(i,j,k)->GetConductivity()[1] * (GetContainer(i,j+1,k)->GetPreTemp() - 2*GetContainer(i,j,k)->GetPreTemp()
                                                          + GetContainer(i,j-1,k)->GetPreTemp()) / pow(LPHI/fMesh[1],2);
        Qrr = GetContainer(i,j,k)->GetConductivity()[2] * (GetContainer(i,j,k+1)->GetPreTemp() - 2*GetContainer(i,j,k)->GetPreTemp()
                                                          + GetContainer(i,j,k-1)->GetPreTemp()) / pow(LRCDT + WTAPE,2);

        gQ  = GetContainer(i,j,k)->GetSource();
        rho = GetContainer(i,j,k)->GetDensity();
        C   = GetContainer(i,j,k)->GetCapacity();

        Q = (Qzz + Qpp + Qrr + gQ) / rho / C;
        T = GetContainer(i,j,k)->GetPreTemp() + dt * Q;

        // calculate the maximum time step
        limit = rho * C * pow(GetContainer(i,j,k)->GetConductivity()[0] / pow(NumCdt*(LZCDT + WTAPE), 2) +
                              GetContainer(i,j,k)->GetConductivity()[1] / pow(LPHI/fMesh[1], 2) +
                              GetContainer(i,j,k)->GetConductivity()[2] / pow(LRCDT + WTAPE, 2), -1) / 2;

        if (limit < step)  step = limit;

        // update temperature
        GetContainer(i,j,k)->SetTemp(T);

        // set quench spot
        if (GetContainer(i,j,k)->GetQuench()==true && T<9.4) 
          GetContainer(i,j,k)->SetTemp(9.4);
      }
    }
  }

  // ************************************************ //
  // BOUNDARY CONDITION
  // ************************************************ //
  // z direction
  for (int j=0; j<fMesh[1]+2; j++) {
    for (int k=0; k<fMesh[2]+2; k++) {
      GetContainer(0,j,k)->SetTemp( GetContainer(1,j,k)->GetTemp() );
      GetContainer(fMesh[0]+1,j,k)->SetTemp( GetContainer(fMesh[0],j,k)->GetTemp() );
    }
  }
/*
  // phi direction
  for (int i=0; i<fMesh[0]+2; i++) {
    for (int k=0; k<fMesh[2]+2; k++) {
      GetContainer(i,0,k)->SetTemp( GetContainer(i,fMesh[1],k)->GetTemp() );
      GetContainer(i,fMesh[1]+1,k)->SetTemp( GetContainer(i,1,k)->GetTemp() );
    }
  }
*/

  // conductor axis
  for (int i=0; i<fMesh[0]+2; i++) {
    for (int k=0; k<fMesh[2]+2; k++) {
      GetContainer(i,0,k)->SetTemp( GetContainer(i,fMesh[1],k)->GetTemp() );
      GetContainer(i,fMesh[1]+1,k)->SetTemp( GetContainer(i,1,k)->GetTemp() );
      // connect this turn conductor to the next turn conductor
      if (i>1 && i<fMesh[0]) {
        GetContainer(i,0,k)->SetTemp( GetContainer(i-1,fMesh[1],k)->GetTemp() );
        GetContainer(i,fMesh[1]+1,k)->SetTemp( GetContainer(i+1,1,k)->GetTemp() );
      }
    }
  }

  // r direction
  for (int i=0; i<fMesh[0]+2; i++) {
    for (int j=0; j<fMesh[1]+2; j++) {
      GetContainer(i,j,0)->SetTemp( GetContainer(i,j,1)->GetTemp() );
      GetContainer(i,j,fMesh[2]+1)->SetTemp( GetContainer(i,j,fMesh[2])->GetTemp() );
    }
  }

  return step;
}

