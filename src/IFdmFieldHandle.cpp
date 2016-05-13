#include <iostream>
#include <TCanvas.h>
#include <TH2F.h>
#include <TStyle.h>
#include "IFdmFieldHandle.h"

using std::string;
using std::map;

FDM::IFdmFieldHandle::IFdmFieldHandle()
    : fCount(0), fCurrent(2700.)
{}

FDM::IFdmFieldHandle::~IFdmFieldHandle()
{}

void FDM::IFdmFieldHandle::ConstructSolenoid(const string& name, double r0, double r1,
                                                                 double z0, double z1)
{
  Magnet::IMagnetContainer* mag = new Magnet::IMagnetContainer();
  mag->SetName(name);
  mag->SetDimension(r0, r1, z0, z1);

  fHC.insert( map<int, Magnet::IMagnetContainer*>::value_type(fCount, mag) );

  fCount ++;
}

void FDM::IFdmFieldHandle::SetMesh(const string& name, const int mr, const int mz)
{
  for(map<int, Magnet::IMagnetContainer*>::iterator it=fHC.begin(); it!=fHC.end(); it++) {
    if (it->second->GetName() == name)
      it->second->SetMesh(mr, mz);
  }
}

void FDM::IFdmFieldHandle::EvalField(const string& name)
{
  FDM::IFdmField* fld = new FDM::IFdmField();

  // set target solenoid mesh and dimension
  Magnet::IMagnetContainer* mag = NULL;

  for (map<int, Magnet::IMagnetContainer*>::iterator it=fHC.begin(); it!=fHC.end(); it++) {
    mag = it->second;
    if (mag->GetName() == name) {
      fld->SetMap( mag->GetDimension()[0], mag->GetDimension()[1],
                   mag->GetDimension()[2], mag->GetDimension()[3]);
      fld->SetMapMesh( mag->GetMesh()[0], mag->GetMesh()[1]);
      break;
    }
  }

  // loop to calculate the self field and mutal field
  double Bz, Br;

  for (map<int, Magnet::IMagnetContainer*>::iterator it=fHC.begin(); it!=fHC.end(); it++) {
    fld->SetSolenoid( it->second->GetDimension()[0], it->second->GetDimension()[1],
                      it->second->GetDimension()[2], it->second->GetDimension()[3]);
    fld->SetSolenoidMesh( it->second->GetMesh()[0], 50, it->second->GetMesh()[1]);
    fld->SetCurrent(fCurrent);
    fld->EvalField();
    if (it->first==0)
      fFieldCollect = fld->GetCollection();
    else {
      for (std::vector<int>::size_type i=0; i<fFieldCollect.size(); i++) {
        Br = fFieldCollect[i]->GetField()[0] + fld->GetCollection()[i]->GetField()[0];
        Bz = fFieldCollect[i]->GetField()[1] + fld->GetCollection()[i]->GetField()[1];
        fFieldCollect[i]->SetField( Br, Bz);
      }
    }
  }
}

Field::IFieldContainer* FDM::IFdmFieldHandle::GetContainer(const string& name, const int i, const int j)
{
  // get target dimension 
  int    mr = fHC[0]->GetMesh()[0];
  int    mz = fHC[0]->GetMesh()[1];

  for (map<int, Magnet::IMagnetContainer*>::iterator it=fHC.begin(); it!=fHC.end(); it++) {
    if (it->second->GetName() == name) {
      mr = it->second->GetMesh()[0];
      mz = it->second->GetMesh()[1];
      break;
    }
  }

  return fFieldCollect[i*mz + j];
}

void FDM::IFdmFieldHandle::Plot(const string& name)
{
  // get target dimension 
  double r0 = fHC[0]->GetDimension()[0];
  double r1 = fHC[0]->GetDimension()[1];
  double z0 = fHC[0]->GetDimension()[2];
  double z1 = fHC[0]->GetDimension()[3];
  int    mr = fHC[0]->GetMesh()[0];
  int    mz = fHC[0]->GetMesh()[1];

  for (map<int, Magnet::IMagnetContainer*>::iterator it=fHC.begin(); it!=fHC.end(); it++) {
    if (it->second->GetName() == name) {
      r0 = it->second->GetDimension()[0];
      r1 = it->second->GetDimension()[1];
      z0 = it->second->GetDimension()[2];
      z1 = it->second->GetDimension()[3];
      mr = it->second->GetMesh()[0];
      mz = it->second->GetMesh()[1];
      break;
    }
  }

  double width  = 350.;
  double length = width * (z1 - z0) / (r1 -r0);

  TCanvas* c0 = new TCanvas("field", "field", length, width);
  c0->SetTicks(1,1);
  c0->SetRightMargin(0.13);

  gStyle->SetOptStat(0);
  gStyle->SetNumberContours(99);

  TH2F* hist = new TH2F("field", "field", mz, z0*1000., z1*1000.,
                                          mr, r0*1000., r1*1000.);

  double Bz, Br, B;
  double Bmax = 0.; 

  for (std::vector<int>::size_type i=0; i<fFieldCollect.size(); i++) {
    Bz = fFieldCollect[i]->GetField()[1];
    Br = fFieldCollect[i]->GetField()[0];
    B  = sqrt(pow(Bz,2) + pow(Br,2));
    hist->Fill(fFieldCollect[i]->GetPosition()[1]*1000., fFieldCollect[i]->GetPosition()[0]*1000., B); 
    if (B > Bmax)  Bmax = B;
  }   

  std::cout << "Bmax: " << Bmax  << " [Tesla]" << std::endl;

  hist->SetTitle("; Z [mm]; R [mm]; Magnetic Field [Tesla]");
  hist->Draw("colz");
}
