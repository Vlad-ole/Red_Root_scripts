//C++
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>
#include <algorithm>

//root cern
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TStyle.h"
#include "TTree.h"
#include "TROOT.h"
#include "TString.h"
#include "TPaveStats.h"
#include "TBox.h"
#include "TArrow.h"
#include "TList.h"
#include "TLatex.h"
#include "TF1.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TMultiGraph.h"
#include "TLatex.h"

using namespace std;
using namespace TMath;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

TGraph *gloss;

double QuenchingFunc(double *x, double*p)
{
  //consts
  const double energy = 59.5;//keV
  const double alpha = 0.21;
  double dEdx = gloss->Eval(energy);

  //varibles
  double field = x[0];

  //parameters
  double A = p[0];
  double C = p[1];
  double D = p[2];

  double C_field = C * Exp(-D*field);
  double B_field = A/(1-C_field);

  double C_zero_field = C;
  double B_zero_field = A/(1-C_zero_field);

  double reco_field = A*dEdx / (1 + B_field*dEdx) + C_field;
  double reco_zero_field = A*dEdx/ (1 + B_zero_field*dEdx) + C_zero_field;

  //return (alpha + reco_field) / (alpha + reco_zero_field);
  return (alpha + reco_field) / (alpha + 1);//wrong
}

void article_quenching4()
{
    TFile *fin = TFile::Open("eloss.root");
    gloss = (TGraph*) fin->Get("gr_eloss");
    COUT( gloss->Eval(59.5) );

    double x_tpm[1] = {500};
    double y_tpm[3] = {2.5,  0.77, 3.5E-3};
    COUT( QuenchingFunc(x_tpm, y_tpm) );

    //Aris data
    TFile *faris = TFile::Open("dataER.root");
    TGraphErrors *grAm;
    TGraphErrors *grAmP = new TGraphErrors(4);
    int E[4]={50,100,200,500};
    Double_t x;
    Double_t y;
    Double_t ex,ey;
    for (int i=0;i<4;i++)
    {
       if (i==0) grAm = (TGraphErrors*) faris->Get("gam50");
       if (i==1) grAm = (TGraphErrors*) faris->Get("gam100");
       if (i==2) grAm = (TGraphErrors*) faris->Get("gam200");
       if (i==3) grAm = (TGraphErrors*) faris->Get("gam500");

       grAm->GetPoint(0,x,y);ey = grAm->GetErrorY(0);
       //printf ("%d %f %f %f\n",E[i],x,y, ey);
       grAmP->SetPoint(i,(Double_t) E[i],y);  grAmP->SetPointError(i,0.1,ey);
    }

    //Aris
    grAmP->SetMarkerStyle(23);
    grAmP->SetMarkerColor(3);
    grAmP->SetName("Aris");
    grAmP->SetTitle("");
    grAmP->Draw("APE");
    grAmP->GetXaxis()->SetTitle("E_{d} (V/cm)");
    grAmP->GetYaxis()->SetTitle("S1/S1_{0}");
    grAmP->GetYaxis()->SetRangeUser(0.7, 1.0);
    grAmP->GetXaxis()->SetRangeUser(0, 600);

    TF1 *func = new TF1("fit",QuenchingFunc,0,1000,3);
    func->SetParNames("A [cm/keV]", "C", "D [cm/V]");
    //func->SetParameters(2.5E-3, 0.77, 3.5E-3);
    func->FixParameter(0, 2.5);
    func->FixParameter(1, 0.77);
    func->FixParameter(2, 3.5E-3);
    func->SetLineColor(kBlue);
    grAmP->Fit("fit");
    func->Draw("same");
}

