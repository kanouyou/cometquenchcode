#include <iostream>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>
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
  TCanvas* c0 = new TCanvas("time_temp", "time_temp", 550, 420);
  c0->SetTicks(1, 1);

  int* msh = GetMesh();
  int color[5] = {kBlack, kOrange+8, kAzure+3, kSpring+9, kRed};

  TGraph* gr[5];
  gr[0] = GetTempGraph(  msh[0],   msh[1],   msh[2]);
  gr[1] = GetTempGraph(msh[0]/2,   msh[1],   msh[2]);
  gr[2] = GetTempGraph(  msh[0], msh[1]/2,   msh[2]);
  gr[3] = GetTempGraph(  msh[0],   msh[1], msh[2]/2);
  gr[4] = GetTempGraph(msh[0]/2, msh[1]/2, msh[2]/2);

  TMultiGraph* mg = new TMultiGraph();
  TLegend*     lg = new TLegend();

  for (int i=0; i<5; i++) {
    gr[i]->SetLineWidth(2);
    gr[i]->SetLineColor(color[i]);
    mg->Add(gr[i], "l");
    lg->AddEntry(gr[i], gr[i]->GetName(), "l");
  }

  lg->Draw();

  fOutput->cd();
  mg->Write("time_vs_temp");
}
