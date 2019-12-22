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

void article_quenching2()
{
    TFile *fin = TFile::Open("eloss.root");
    gloss = (TGraph*) fin->Get("gr_eloss");

    double x_tpm[1] = {0};
    double y_tpm[3] = {2.5 * 1E-3,  0.77, 3.5E-3};

    COUT( gloss->Eval(59.5) );
    COUT( QuenchingFunc(x_tpm, y_tpm) );

    bool is_draw_ReD_DP = true;

    //ReD SP data
    vector<double> field_red_v{50, 100, 200, 300, 400, 500, 700, 1000};
    vector<double> field_err_red_v{0, 0, 0, 0, 0, 0, 0, 0};
    vector<double> quench_red_v{0.947, 0.914, 0.855, 0.810, 0.772, 0.740, 0.686, 0.612};
    vector<double> quench_err_red_v{0.002, 0.002, 0.002, 0.002, 0.002,0.002, 0.002, 0.002};

    //ReD DP data
    vector<string> text_red_dp{"6500/86/-744", "5211/86/-744", "3780/86/-744", "0/86/-744"};
    vector<double> field_red_dp{200, 200, 200, 200};
    vector<double> field_err_red_dp{0.1, 0.1, 0.1, 0.1};
    vector<double> quench_red_dp{0.818, 0.847, 0.872, 0.892};
    vector<double> quench_err_red_dp{0.002, 0.002, 0.002, 0.002};
    TGraphErrors *gr_red_dp = new TGraphErrors(field_red_dp.size(), &field_red_dp[0], &quench_red_dp[0],
            &field_err_red_dp[0], &quench_err_red_dp[0]);


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

    //(ReD SP + Aris)/2.0
    vector<double> fields_all{50, 100, 200, 300, 400, 500, 700, 1000};
    vector<double> quecnch_avr{ (0.947+0.9419)/2, (0.914+0.9072)/2, (0.855+0.86088)/2.0,
                                0.81, 0.772, (0.74+0.736676)/2, 0.686, 0.612};
    TGraph *gr_q_f_avr = new TGraph(fields_all.size(), &fields_all[0], &quecnch_avr[0]);
    TGraph *gr_f_q_avr = new TGraph(fields_all.size(), &quecnch_avr[0], &fields_all[0]);

    //ReD SP + Aris
    vector<double> field_ReDSP_Aris;
    vector<double> field_err_ReDSP_Aris;
    vector<double> quench_ReDSP_Aris;
    vector<double> quench_err_ReDSP_Aris;
    for(int i = 0 ; i < field_red_v.size(); i++)
    {
        field_ReDSP_Aris.push_back(field_red_v[i]);
        field_err_ReDSP_Aris.push_back(field_err_red_v[i]);
        quench_ReDSP_Aris.push_back(quench_red_v[i]);
        quench_err_ReDSP_Aris.push_back(quench_err_red_v[i]);
    }
    for(int i = 0 ; i < grAmP->GetN(); i++)
    {
        double x,y;
        grAmP->GetPoint(i, x, y);
        field_ReDSP_Aris.push_back(x);
        quench_ReDSP_Aris.push_back(y);
        field_err_ReDSP_Aris.push_back(grAmP->GetErrorX(i));
        quench_err_ReDSP_Aris.push_back(grAmP->GetErrorY(i));
        //cout << x << "\t" << y << "\t" << grAmP->GetErrorX(i) << "\t" << grAmP->GetErrorY(i) << endl;
    }

    for(int i = 0; i < field_ReDSP_Aris.size(); i++)
    {
        cout << field_ReDSP_Aris[i] << "\t" << quench_ReDSP_Aris[i] << "\t" <<
                field_err_ReDSP_Aris[i] << "\t" << quench_err_ReDSP_Aris[i] << endl;
    }

    TGraphErrors *gr_ReDSP_Aris = new TGraphErrors(field_ReDSP_Aris.size(), &field_ReDSP_Aris[0], &quench_ReDSP_Aris[0],
            &field_err_ReDSP_Aris[0], &quench_err_ReDSP_Aris[0]);


    TCanvas *c1 = new TCanvas("c1", "c1");

    //ReD SP
    TGraphErrors *gr_qench_red = new TGraphErrors(field_red_v.size(), &field_red_v[0], &quench_red_v[0],
            &field_err_red_v[0], &quench_err_red_v[0]);
    gr_qench_red->SetTitle("");
    gr_qench_red->SetMarkerStyle(20);
    gr_qench_red->SetMarkerColor(kRed);
    gr_qench_red->Draw("APE");
    gr_qench_red->SetName("gr_qench_red");
    gr_qench_red->GetYaxis()->SetRangeUser(0.5, 1);
    gr_qench_red->GetXaxis()->SetRangeUser(0, 1200);
    gr_qench_red->GetXaxis()->SetTitle("E_{d} (V/cm)");
    gr_qench_red->GetYaxis()->SetTitle("S1/S1_{0}");

    //Aris
    grAmP->SetMarkerStyle(23);
    grAmP->SetMarkerColor(3);
    grAmP->SetName("Aris");
    grAmP->Draw("PE same");

    //ReD SP + Aris
    gr_ReDSP_Aris->SetMarkerColor(kBlue);
    gr_ReDSP_Aris->SetMarkerStyle(20);
    gr_ReDSP_Aris->SetMarkerSize(1);
    TF1 *func = new TF1("fit",QuenchingFunc,0,1000,3);
    func->SetParNames("A [cm/keV]", "C", "D [cm/V]");
    func->SetParameters(2.5 * 1E-6, 0.77, 0.0035);
    func->SetLineColor(kBlue);
    gr_ReDSP_Aris->Fit("fit");
    func->Draw("same");
    //gStyle->SetOptFit(1111);

    //spline
    gr_q_f_avr->Draw("same C");
    gr_q_f_avr->SetLineColor(kMagenta);
    gr_q_f_avr->SetLineWidth(2);
    gr_q_f_avr->SetName("gr_q_f_avr");

    //ReD DP
    if(is_draw_ReD_DP)
    {
        gr_red_dp->SetMarkerStyle(20);
        gr_red_dp->SetMarkerColor(kOrange + 4);
        gr_red_dp->Draw("same P");
        gr_red_dp->SetName("gr_red_dp");
    }



    cout << "Expected quenching at 200V/cm (from fit) = " << func->Eval(200) << endl;
    cout << "Expected quenching at 200V/cm (from spline) = " << gr_q_f_avr->Eval(200) << endl;
    COUT(gr_f_q_avr->Eval(0.723));
    COUT(gr_f_q_avr->Eval(0.775));
    COUT(gr_f_q_avr->Eval(0.887));
//    COUT(gr_f_q_avr->Eval(0.818));
//    COUT(gr_f_q_avr->Eval(0.847));
//    COUT(gr_f_q_avr->Eval(0.872));
//    COUT(gr_f_q_avr->Eval(0.892));
//    COUT(gr_f_q_avr->Eval(0.8544395924));
//    COUT(gr_f_q_avr->Eval(0.8427947598));
//    COUT(gr_f_q_avr->Eval(0.8602620087));


    TLegend * l= new TLegend(0.4,0.5,0.9,0.8);
    l->AddEntry("gr_qench_red","ReD Single Phase ^{241}Am data TBA corr.","P");
    l->AddEntry("Aris","Aris ","P");
    l->AddEntry("fit","Fit of ReD SP and Aris data","L");
    l->AddEntry("gr_q_f_avr","Cubic interpolation of ReD SP and Aris data","L");
    if(is_draw_ReD_DP) l->AddEntry("gr_red_dp","ReD Double Phase ^{241}Am data","P");
    //l->SetBorderSize(0);
    l->Draw();

    if(is_draw_ReD_DP)
    {
        TLatex lat;
        lat.SetTextSize(0.020);
        lat.SetTextAngle(0.);
        for (int i=0; i < field_red_dp.size(); i++)
        {
            lat.DrawLatex(field_red_dp[i],quench_red_dp[i], text_red_dp[i].c_str());
        }
    }
}
