#include <iostream>
#include "IFdmDataflow.h"

FDM::IFdmDataflow::IFdmDataflow()
    : fFile(NULL)
{}

FDM::IFdmDataflow::IFdmDataflow(const std::string& filename)
    : fFile(NULL)
{
  fFile = new TFile(filename.c_str(), "RECREATE");
}

FDM::IFdmDataflow::~IFdmDataflow()
{
  fFile->Close();
}

void FDM::IFdmDataflow::SetFile(const std::string& name)
{
  if (!fFile)  fFile = new TFile(name.c_str(), "RECREATE");
}

void FDM::IFdmDataflow::SetSubFolder(const std::string& foldername)
{
  TDirectory* dir = fFile->mkdir(foldername.c_str());
  fDir.insert( std::map<std::string, TDirectory*>::value_type(foldername, dir) );
}

void FDM::IFdmDataflow::Fill(const std::string& sol, const std::string& name, FDM::IFdmThermalSolver* mag)
{
  fDir[sol]->cd();

  TTree* tree = new TTree(name.c_str(), Form("Quench time: %s", name.c_str()));
  
  IElectricContainer* evt = NULL;

  tree->Branch("data", "IElectricContainer", &evt, 128000, 0);

  for (int i=0; i<mag->GetMesh()[0]+2; i++) {
    for (int j=0; j<mag->GetMesh()[1]+2; j++) {
      for (int k=0; k<mag->GetMesh()[2]+2; k++) {
        evt = mag->GetContainer(i,j,k);
        tree->Fill();
      }
    }
  }
}

void FDM::IFdmDataflow::Close()
{
  fFile->Write();
}
