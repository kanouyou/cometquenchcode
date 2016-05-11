#include <iostream>
#include <cmath>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH2F.h>
#include "IFdmField.h"

FDM::IFdmField::IFdmField()
    : fMapMesh(NULL),
      fMap(NULL)
{}

FDM::IFdmField::~IFdmField()
{
  if (fMapMesh)  delete[] fMapMesh;
  if (fMap)      delete[] fMap;
}

void FDM::IFdmField::SetMapMesh(const int mr, const int mz)
{
  if (!fMapMesh)  fMapMesh = new int[2];

  fMapMesh[0] = mr;
  fMapMesh[1] = mz;
}

void FDM::IFdmField::SetMap(double r0, double r1, double z0, double z1)
{
  if (!fMap)  fMap = new double[4];

  fMap[0] = r0;
  fMap[1] = r1;
  fMap[2] = z0;
  fMap[3] = z1;
}

void FDM::IFdmField::EvalField()
{
  const double dr = (fMap[1] - fMap[0]) / fMapMesh[0];
  const double dz = (fMap[3] - fMap[2]) / fMapMesh[1];

  double R, Z;
  double Br, Bz, B;
  double A[fMapMesh[0]+1][fMapMesh[1]+1];
  Field::IFieldContainer* fld = NULL;

  fHC.clear();

  for (int i=0; i<fMapMesh[0]+1; i++) {
    for (int j=0; j<fMapMesh[1]+1; j++) {
      R = fMap[0] + (i-1) * dr + dr / 2;
      Z = fMap[2] + (j-1) * dz + dz / 2;
      A[i][j] = EvalPotential(R, Z);

      if (i>0 && j>0) {
        Br = - (A[i][j] - A[i][j-1]) / dz;
        Bz = A[i][j] / R + (A[i][j] - A[i-1][j]) / dr;
        B  = sqrt(pow(Bz,2) + pow(Br,2));
        fld = new Field::IFieldContainer();
        fld->SetId(i, j);
        fld->SetPosition(R,Z);
        fld->SetField(Br, Bz);
        fld->SetPotential(A[i][j]);
        fHC.push_back(fld);
      }
      else {
        Br = - (A[i][j] - 0.) / dz;
        Bz = A[i][j] / R + (A[i][j] - 0.) / dr;
        B  = sqrt(pow(Bz,2) + pow(Br,2));
      }

      //std::cout << R << "    " << Z << "    " << Br << "    " << Bz << "     " << B << std::endl;
    }
  }
}

double FDM::IFdmField::GetSelfInductance()
{
  double I   = GetCurrent();
  double phi = 0.;
  int    mp  = GetSolenoidMesh()[1];
  double R;

  if (fHC.size()==0) {
    SetMap(GetSolenoid()[0], GetSolenoid()[1], GetSolenoid()[2], GetSolenoid()[3]);
    EvalField();
  }

  const double dr = (fMap[1] - fMap[0]) / fMapMesh[0];
//  const double dz = (fMap[3] - fMap[2]) / fMapMesh[1];
  const double dp = (2*M_PI - 0.) / mp;

/*
  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) 
    phi += fHC[i]->GetField()[0]*dp*dz + fHC[i]->GetField()[1]*dp*dr;
*/

  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
    R = fMap[0] + (fHC[i]->GetId()[0]-1) * dr + dr / 2;
    phi += fHC[i]->GetPotential() * R * dp;
  }

  // self inductance of dp
  double L = phi / I;
  // must times mesh phi to integral phi direction
  L *= mp;

  return L;
}

void FDM::IFdmField::Plot()
{
  TCanvas* c0 = new TCanvas("field", "field", 750, 420);
  c0->SetTicks(1,1);
  c0->SetRightMargin(0.13);

  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);

  TH2F* hist = new TH2F("field", "field", fMapMesh[1], fMap[2]*1000., fMap[3]*1000.,
                                          fMapMesh[0], fMap[0]*1000., fMap[1]*1000.);

  double Bz, Br, B;
  double Bmax = 0.;

  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
    Bz = fHC[i]->GetField()[1];
    Br = fHC[i]->GetField()[0];
    B  = sqrt(pow(Bz,2) + pow(Br,2));
    hist->Fill(fHC[i]->GetPosition()[1]*1000., fHC[i]->GetPosition()[0]*1000., B);
    if (B > Bmax)  Bmax = B;
  } 

  std::cout << "Bmax: " << Bmax  << " [Tesla]" << std::endl;

  hist->SetTitle("; Z [mm]; R [mm]; Magnetic Field [Tesla]");
  hist->Draw("colz");

  c0->Print("magfld.pdf");
}
