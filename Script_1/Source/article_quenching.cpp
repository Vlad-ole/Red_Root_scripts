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

    //ReD
    double A = 0.001 * 1E3;// cm/keV
    double C = 0.79;
    double D = 0.00172;

    //Aris
//    double A = 0.0025;
//    double C = 0.77;
//    double D = 0.0035;


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
    gr_qench->Draw("AL");
    gr_qench->GetXaxis()->SetTitle("Drift field [V/cm]");
    gr_qench->GetYaxis()->SetTitle("S1/S1_{0}");
    gr_qench->SetTitle("");
}
