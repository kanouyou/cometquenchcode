#include <iostream>
#include <sstream>
#include <algorithm>
#include <TStyle.h>
#include "IFdmPostprocess.h"
#include "IFdmException.h"

FDM::IFdmPostprocess::IFdmPostprocess(const std::string& name)
    : fFile(NULL)
{
  fFile = new TFile(name.c_str());
}

FDM::IFdmPostprocess::~IFdmPostprocess()
{}

void FDM::IFdmPostprocess::SetDirectory(const std::string& name)
{
  fDirName = name;
  double time;
  std::string strname;

  fFile->GetDirectory(name.c_str())->cd();
  fFile->GetDirectory(name.c_str())->pwd();

  for (int i=0; i<fFile->GetDirectory(name.c_str())->GetListOfKeys()->GetEntries(); i++) {
    strname = std::string(fFile->GetDirectory(name.c_str())->GetListOfKeys()->At(i)->GetName());
    strname.erase(4);
    std::istringstream name(strname); 
    name >> time;
    fTime.push_back(time);
  }
}

std::vector<std::string> FDM::IFdmPostprocess::GetTreeName()
{
  std::string name;
  std::vector<std::string> treename;

  for (int i=0; i<fFile->GetDirectory(fDirName.c_str())->GetListOfKeys()->GetEntries(); i++) {
    name = std::string(fFile->GetDirectory(fDirName.c_str())->GetListOfKeys()->At(i)->GetName());
    treename.push_back(name);
  }

  return treename;
}

int* FDM::IFdmPostprocess::GetMesh()
{
  int* msh = new int[3];

  msh[0] = 0; msh[1] = 0; msh[2] = 0;

  std::string name = std::string(fFile->GetDirectory(fDirName.c_str())->GetListOfKeys()->At(0)->GetName());
  IElectricContainer* collect = new IElectricContainer();

  TTree* tree = dynamic_cast<TTree*>(fFile->GetDirectory(fDirName.c_str())->Get(name.c_str()));
  tree->SetBranchAddress("data", &collect);

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (collect->GetId()[0] > msh[0])  msh[0] = collect->GetId()[0];
    if (collect->GetId()[1] > msh[1])  msh[1] = collect->GetId()[1];
    if (collect->GetId()[2] > msh[2])  msh[2] = collect->GetId()[2];
  }

  msh[0] -= 1;
  msh[1] -= 1;
  msh[2] -= 1;

  return msh;
}

double FDM::IFdmPostprocess::GetTemperature(const std::string& name, const int z, const int p, const int r)
{
  double temp = 0.;
  IElectricContainer* collect = new IElectricContainer();

  TTree* tree = dynamic_cast<TTree*>(fFile->GetDirectory(fDirName.c_str())->Get(name.c_str()));
  tree->SetBranchAddress("data", &collect);

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (collect->GetId()[0]==z && collect->GetId()[1]==p && collect->GetId()[2]==r) {
      temp = collect->GetTemp();
      break;
    }
  }

  return temp;
}

TGraph* FDM::IFdmPostprocess::GetTempGraph(const int z, const int p, const int r)
{
  FDM::IFdmException except("", "IFdmPostprocess", "PlotTemperature");
  
  if (fTime.size()==0) {
    except.SetMessage("Subdirectory is not set!");
    throw except;
  }

  std::vector<std::string> name = GetTreeName();
  if (fTime.size()!=name.size()) {
    except.SetMessage("Time size is not equal to name size!");
    std::cout << "fTime: " << fTime.size() << " ; name: " << name.size() << std::endl;
    throw except;
  }

  TGraph* gr = new TGraph();

  double temp = 0.;

  for (std::vector<int>::size_type i=0; i<name.size(); i++) {
    temp = GetTemperature(name[i], z, p, r);
    gr->SetPoint(i, fTime[i], temp);
  }

  gr->SetTitle(Form("(Z,#phi,R)=(%i,%i,%i); Time [sec]; Temperature [K]",z,p,r));
  gr->SetName(Form("(Z,#phi,R)=(%i,%i,%i)", z,p,r));

  return gr;
}
