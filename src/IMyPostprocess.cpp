#include <iostream>
#include <TH2F.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include "IFdmParameters.h"
#include "IMyPostprocess.h"

using namespace FDM;

IMyPostprocess::IMyPostprocess()
    : IFdmPostprocess(), fOutput(NULL)
{}

IMyPostprocess::~IMyPostprocess()
{
  fOutput->Write();
  fOutput->Close();
}

void IMyPostprocess::SetFile(const std::string& filename)
{
  fOutput = new TFile(filename.c_str(), "RECREATE");
}

void IMyPostprocess::PlotTimeTemp()
{
  //TCanvas* c0 = new TCanvas("time_temp", "time_temp", 550, 420);
  //c0->SetTicks(1, 1);

  int* msh = GetMesh();
  int color[5] = {kBlack, kOrange+8, kAzure+3, kSpring+9, kRed};

  TGraph* gr[5];
  gr[0] = GetTempGraph(  msh[0],   msh[1],   msh[2]);
  gr[1] = GetTempGraph(msh[0]/2,   msh[1],   msh[2]);
  gr[2] = GetTempGraph(  msh[0], msh[1]/2,   msh[2]);
  gr[3] = GetTempGraph(  msh[0],   msh[1], msh[2]/2);
  gr[4] = GetTempGraph(msh[0]/2, msh[1]/2, msh[2]/2);

  std::cout << "postprocessing.." << std::endl;

  TMultiGraph* mg = new TMultiGraph();
  TLegend*     lg = new TLegend();

  for (int i=0; i<5; i++) {
    gr[i]->SetLineWidth(2);
    gr[i]->SetLineColor(color[i]);
    mg->Add(gr[i], "l");
    lg->AddEntry(gr[i], gr[i]->GetName(), "l");
  }

  lg->Draw();
  mg->Draw("a");

  fOutput->cd();
  mg->Write("time_vs_temp");
}

TH2F* IMyPostprocess::GetFieldHist(const std::string& name)
{
  int* msh = GetMesh();

  TH2F* hist = new TH2F(Form("fld/%s",name.c_str()), Form("fld/%s",name.c_str()),
                        msh[0], 0., (double)msh[0]+1,
                        msh[2], 0., (double)msh[2]+1);

  IElectricContainer* collect = new IElectricContainer();

  TTree* tree = dynamic_cast<TTree*>(GetFile()->GetDirectory(fDirName.c_str())->Get(name.c_str()));
  tree->SetBranchAddress("data", &collect);

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (collect->GetId()[1]==1) {
      if (collect->GetId()[0]!=0 && collect->GetId()[0]!=msh[0]+1 &&
          collect->GetId()[2]!=0 && collect->GetId()[2]!=msh[2]+1)
        hist->Fill((double)collect->GetId()[0], (double)collect->GetId()[2], collect->GetField());
    }
  }

  return hist;
}

TH2F* IMyPostprocess::GetTempHist(const std::string& name)
{
   int* msh = GetMesh();

  TH2F* hist = new TH2F(Form("temp/%s",name.c_str()), Form("temp/%s",name.c_str()),
                        msh[0], 0., (msh[0]+1)*LZCDT,
                        msh[2], 0., (msh[2]+1)*LRCDT);

  IElectricContainer* collect = new IElectricContainer();

  TTree* tree = dynamic_cast<TTree*>(GetFile()->GetDirectory(fDirName.c_str())->Get(name.c_str()));
  tree->SetBranchAddress("data", &collect);

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (collect->GetId()[1]==1) {
      if (collect->GetId()[0]!=0 && collect->GetId()[0]!=msh[0]+1 &&
          collect->GetId()[2]!=0 && collect->GetId()[2]!=msh[2]+1)
        hist->Fill(collect->GetId()[0]*LZCDT, collect->GetId()[2]*LRCDT, collect->GetTemp());
    }
  }

  return hist;
}

TH2F* IMyPostprocess::GetHist(const std::string& name)
{
   int* msh = GetMesh();

  TH2F* hist = new TH2F(Form("heat/%s",name.c_str()), Form("heat/%s",name.c_str()),
                        msh[0], 0., (msh[0]+1)*LZCDT,
                        msh[2], 0., (msh[2]+1)*LRCDT);

  IElectricContainer* collect = new IElectricContainer();

  TTree* tree = dynamic_cast<TTree*>(GetFile()->GetDirectory(fDirName.c_str())->Get(name.c_str()));
  tree->SetBranchAddress("data", &collect);

  for (int i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (collect->GetId()[1]==1) {
      if (collect->GetId()[0]!=0 && collect->GetId()[0]!=msh[0]+1 &&
          collect->GetId()[2]!=0 && collect->GetId()[2]!=msh[2]+1)
        hist->Fill(collect->GetId()[0]*LZCDT, collect->GetId()[2]*LRCDT, collect->GetSource());
    }
  }

  return hist;
}

void IMyPostprocess::PlotField2D()
{
  std::vector<std::string> name = GetTreeName();

  // z-r plot
  fOutput->cd();
  fOutput->mkdir("Field2D");
  fOutput->cd("Field2D");
  std::cout << "filling field plot ..." << std::endl;

  for (std::vector<int>::size_type i=0; i<name.size(); i++) 
    GetFieldHist(name[i])->Write( Form("fld.%s", name[i].c_str()) );
}

void IMyPostprocess::PlotTemp2D()
{
  std::vector<std::string> name = GetTreeName();

  fOutput->cd();
  fOutput->mkdir("Temp2D");
  fOutput->cd("Temp2D");
  std::cout << "filling field plot ..." << std::endl;

  for (std::vector<int>::size_type i=0; i<name.size(); i++)
    GetTempHist(name[i])->Write( Form("temp.%s", name[i].c_str()) );
}

void IMyPostprocess::PlotHeat2D()
{
  std::vector<std::string> name = GetTreeName();

  fOutput->cd();
  fOutput->mkdir("Heat2D");
  fOutput->cd("Heat2D");

  for (std::vector<int>::size_type i=0; i<name.size(); i++)
    GetHist(name[i])->Write( Form("heat.%s", name[i].c_str()) );
}
