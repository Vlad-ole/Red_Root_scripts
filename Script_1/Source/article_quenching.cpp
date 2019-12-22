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

using namespace std;
using namespace TMath;

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

  return (alpha + reco_field) / (alpha + reco_zero_field);
  //return (alpha + reco_field) / (alpha + 1);
}

double GerR(double A, double C, double D, double field_V_cm, double energy)
{
    double dEdx = gloss->Eval(energy);
    C *= Exp(-D*field_V_cm);
    double B = A/(1-C);
    double reco = A*dEdx/ (1 + B*dEdx) + C;
    return reco;
}

double GetQuenching(double A, double C, double D, double field_V_cm, double energy)
{
    double alpha = 0.21;

    return ( alpha + GerR(A, C, D, field_V_cm, energy) ) / ( alpha + GerR(A, C, D, 0, energy) );
    //return ( alpha + GerR(A, C, D, field_V_cm, energy) ) / ( alpha + 1 );
}

void article_quenching()
{
    TFile *fin = TFile::Open("eloss.root");
    gloss = (TGraph*) fin->Get("gr_eloss");

    double energy = 59.5;//keV

    double A = 0.001 * 1E3;// cm/keV
    double C = 0.79;
    double D = 0.00172;// cm/V

    //Aris
//    double A = 0.0025 * 1E3;// cm/keV
//    double C = 0.77;
//    double D = 0.0035;

    vector<double> field_red_v{50, 100, 200, 300, 400, 500, 700, 1000};
    vector<double> quench_red_v{0.947, 0.914, 0.855, 0.810, 0.772, 0.740, 0.686, 0.612};

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
       printf ("%d %f %f %f\n",E[i],x,y, ey);
       grAmP->SetPoint(i,(Double_t) E[i],y);  grAmP->SetPointError(i,0.1,ey);

    }


   //    double field = 0;// V/cm
//    cout << "field = " << field << "\t; R = " << GerR(0.001, 0.79, 0.00172, field) << endl;

    vector<double> field_v;
    vector<double> reco_v;
    vector<double> quench_v;
    for (double field_V_cm = 0; field_V_cm < 1000; field_V_cm += 5)
    {
        field_v.push_back( field_V_cm );
        reco_v.push_back( GerR(A, C, D, field_V_cm, energy) );
        quench_v.push_back( GetQuenching(A, C, D, field_V_cm, energy) );
    }

    TCanvas *c1 = new TCanvas("c1", "c1");
    TGraph *gr_r = new TGraph(field_v.size(), &field_v[0], &reco_v[0]);
    gr_r->Draw("AL");
    gr_r->GetXaxis()->SetTitle("Drift field [V/cm]");
    gr_r->GetYaxis()->SetTitle("Recombination probability");
    gr_r->SetTitle("");

    TCanvas *c2 = new TCanvas("c2", "c2");
    TGraph *gr_qench = new TGraph(field_v.size(), &field_v[0], &quench_v[0]);
    //gr_qench->Draw("AL");
    gr_qench->GetXaxis()->SetTitle("Drift field [V/cm]");
    gr_qench->GetYaxis()->SetTitle("S1/S1_{0}");
    gr_qench->SetTitle("ReD SP data Fit to Doke-Birks");

    TGraph *gr_qench_red = new TGraph(field_red_v.size(), &field_red_v[0], &quench_red_v[0]);
    gr_qench_red->SetTitle("ReD Single Phase");
    gr_qench_red->SetMarkerStyle(20);
    gr_qench_red->SetMarkerColor(kRed);

    // Aris
    grAmP->SetMarkerStyle(23);
    grAmP->SetMarkerColor(3);
    grAmP->SetTitle("Aris");

    //fit
    TF1 *func = new TF1("fit",QuenchingFunc,0,1000,3);
    //func->SetParameters(0.001 * 1E3, 0.79, 0.00172);
    func->SetParameters(0.0025 * 1E3, 0.77, 0.0035);

//    func->FixParameter(0, 0.0025 * 1E3);
//    func->FixParameter(1, 0.77);
//    func->FixParameter(2, 0.0035);

    grAmP->Fit("fit");

    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gr_qench, "L");
    mg->Add(gr_qench_red, "P");
    mg->Add(grAmP, "EP");
    mg->Draw("A");
    mg->SetTitle(";Drift field [V/cm]; S1/S1_{0}");
    gPad->BuildLegend();
}
