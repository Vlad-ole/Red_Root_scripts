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

void fixedrc1()
{
    vector<double> Va = {5211, 5500, 6000, 6500};
    vector<double> Va_err = {1, 1, 1, 1};

    vector<double> S2_top = {7460, 9450, 12600, 15850};
    vector<double> S2_top_err = {20, 50, 50, 100};

    vector<double> S2_bottom = {5550, 7000, 9500, 12250};
    vector<double> S2_bottom_err = {20, 50, 50, 50};

    vector<double> S1_top = {262, 259, 251, 243};
    vector<double> S1_top_err = {1, 1, 1, 1};

    vector<double> S1_bottom = {381, 382, 381, 386};
    vector<double> S1_bottom_err = {1, 1, 1, 1};

    vector<double> S1_top_bottom;
    vector<double> S1_top_bottom_err;

    vector<double> S2_top_bottom;
    vector<double> S2_top_bottom_err;

    vector<double> S2_tb_S1_tb;
    vector<double> S2_tb_S1_tb_err;

    vector<double> S1_top_bottom_norm(Va.size());
    vector<double> S1_top_bottom_norm_err(Va.size());

    vector<double> S2_top_bottom_norm(Va.size());
    vector<double> S2_top_bottom_norm_err(Va.size());

    for (int i = 0; i < Va.size(); i++)
    {
        double S2_top_bottom_tmp = S2_top[i]/S2_bottom[i];
        double S2_top_rel_err = S2_top_err[i]/S2_top[i];
        double S2_bottom_rel_err = S2_bottom_err[i]/S2_bottom[i];
        double S2_top_bottom_err_tmp = S2_top_bottom_tmp * sqrt(pow(S2_top_rel_err, 2) + pow(S2_bottom_rel_err, 2));

        S2_top_bottom.push_back( S2_top_bottom_tmp );
        S2_top_bottom_err.push_back( S2_top_bottom_err_tmp );


        double S1_top_bottom_tmp = S1_top[i]/S1_bottom[i];
        double S1_top_rel_err = S1_top_err[i]/S1_top[i];
        double S1_bottom_rel_err = S1_bottom_err[i]/S1_bottom[i];
        double S1_top_bottom_err_tmp = S1_top_bottom_tmp * sqrt(pow(S1_top_rel_err, 2) + pow(S1_bottom_rel_err, 2));

        S1_top_bottom.push_back( S1_top_bottom_tmp );
        S1_top_bottom_err.push_back( S1_top_bottom_err_tmp );

        double S2_tb_S1_tb_tmp = S2_top_bottom_tmp/S1_top_bottom_tmp;
        double S2_tb_S1_tb_err_tmp = S2_tb_S1_tb_tmp *
                sqrt(pow(S2_top_bottom_err_tmp/S2_top_bottom_tmp, 2.0) + pow(S1_top_bottom_err_tmp/S1_top_bottom_tmp, 2.0));

        S2_tb_S1_tb.push_back(S2_tb_S1_tb_tmp);
        S2_tb_S1_tb_err.push_back(S2_tb_S1_tb_err_tmp);

        S1_top_bottom_norm[i] = S1_top_bottom[i] / S1_top_bottom[0];
        S2_top_bottom_norm[i] = S2_top_bottom[i] / S2_top_bottom[0];

        S1_top_bottom_norm_err[i] = S1_top_bottom_err[i] / S1_top_bottom[0];
        S2_top_bottom_norm_err[i] = S2_top_bottom_err[i] / S2_top_bottom[0];

        //cout << S1_top_bottom_norm_err[i] << "\t" << S2_top_bottom_norm_err[i] << endl;
    }






    TCanvas *c1 = new TCanvas("c1", "c1");
    c1->Divide(3,2);
    //gStyle->SetOptFit(1);

    c1->cd(1);

    TGraphErrors *gr_top = new TGraphErrors(Va.size(), &Va[0], &S2_top[0], &Va_err[0], &S2_top_err[0]);
    gr_top->Draw("APE");
    gr_top->SetMarkerSize(1.3);
    gr_top->SetMarkerStyle(20);
    gr_top->SetMarkerColor(kBlack);
    gr_top->GetXaxis()->SetTitle("Anode voltage [V]");
    gr_top->GetYaxis()->SetTitle("S2 top or bottom [PE]");
    gr_top->SetTitle("");
    gr_top->SetName("gr_top");
    gr_top->GetYaxis()->SetRangeUser(5000, 17000);
    TF1 *f1 = new TF1("f1","[0] + x*[1]",5000,10000);
    gr_top->Fit("f1");

    TGraphErrors *gr_bottom = new TGraphErrors(Va.size(), &Va[0], &S2_bottom[0], &Va_err[0], &S2_bottom_err[0]);
    gr_bottom->Draw("same PE");
    gr_bottom->SetMarkerSize(1.3);
    gr_bottom->SetMarkerStyle(20);
    gr_bottom->SetMarkerColor(kBlue);
    gr_bottom->SetName("gr_bottom");
    TF1 *f2 = new TF1("f2","[0] + x*[1]",5000,10000);
    gr_bottom->Fit("f2");

    TLegend * l= new TLegend(0.4,0.5,0.9,0.8);
    l->AddEntry("gr_top","S2 top","P");
    l->AddEntry("gr_bottom","S2 bottom ","P");
    //l->SetBorderSize(0);
    l->Draw();

    c1->cd(2);

//    TF1 *f1_f2 = new TF1 ("f1_f2", "([0] + x*[1])/([2] + x*[3])", 5000, 7000);
//    f1_f2->FixParameter(0, f1->GetParameter(0) );
//    f1_f2->FixParameter(1, f1->GetParameter(1) );
//    f1_f2->FixParameter(2, f2->GetParameter(0) );
//    f1_f2->FixParameter(3, f2->GetParameter(1) );
//    f1_f2->Draw();
//    f1_f2->GetXaxis()->SetTitle("Anode voltage [V]");
//    f1_f2->GetYaxis()->SetTitle("S2_top / S2_bottom ");
//    f1_f2->SetTitle("");

    TGraphErrors *gr_top_bottom = new TGraphErrors(Va.size(), &Va[0], &S2_top_bottom[0], &Va_err[0], &S2_top_bottom_err[0]);
    gr_top_bottom->Draw("APE");
    gr_top_bottom->SetMarkerSize(1.3);
    gr_top_bottom->SetMarkerStyle(20);
    gr_top_bottom->SetMarkerColor(kBlue);
    gr_top_bottom->GetXaxis()->SetTitle("Anode voltage [V]");
    gr_top_bottom->GetYaxis()->SetTitle("S2_top / S2_bottom");
    gr_top_bottom->SetTitle("");

    c1->cd(4);
    TGraphErrors *gr_S1_top = new TGraphErrors(Va.size(), &Va[0], &S1_top[0], &Va_err[0], &S1_top_err[0]);
    gr_S1_top->Draw("APE");
    gr_S1_top->SetMarkerSize(1.3);
    gr_S1_top->SetMarkerStyle(20);
    gr_S1_top->SetMarkerColor(kBlack);
    gr_S1_top->GetXaxis()->SetTitle("Anode voltage [V]");
    gr_S1_top->GetYaxis()->SetTitle("S1_top [PE]");
    gr_S1_top->SetTitle("");
    gr_S1_top->SetName("gr_S1_top");
    //gr_S1_top->GetYaxis()->SetRangeUser(0, 1000);
    TF1 *f_S1_top = new TF1("f_S1_top","[0] + x*[1]",5000,10000);
    gr_S1_top->Fit("f_S1_top");

    c1->cd(5);
    TGraphErrors *gr_S1_bottom = new TGraphErrors(Va.size(), &Va[0], &S1_bottom[0], &Va_err[0], &S1_bottom_err[0]);
    gr_S1_bottom->Draw("APE");
    gr_S1_bottom->SetMarkerSize(1.3);
    gr_S1_bottom->SetMarkerStyle(20);
    gr_S1_bottom->SetMarkerColor(kBlack);
    gr_S1_bottom->GetXaxis()->SetTitle("Anode voltage [V]");
    gr_S1_bottom->GetYaxis()->SetTitle("S1_bottom [PE]");
    gr_S1_bottom->SetTitle("");
    gr_S1_bottom->SetName("gr_S1_bottom");
    //gr_S1_top->GetYaxis()->SetRangeUser(0, 1000);
    //TF1 *f_S1_bottom = new TF1("f_S1_bottom","[0] + x*[1]",5000,10000);
    //gr_S1_bottom->Fit("f_S1_bottom");

    c1->cd(3);
    TGraphErrors *gr_S1_top_bottom = new TGraphErrors(Va.size(), &Va[0], &S1_top_bottom[0], &Va_err[0], &S1_top_bottom_err[0]);
    gr_S1_top_bottom->Draw("APE");
    gr_S1_top_bottom->SetMarkerSize(1.3);
    gr_S1_top_bottom->SetMarkerStyle(20);
    gr_S1_top_bottom->SetMarkerColor(kBlue);
    gr_S1_top_bottom->GetXaxis()->SetTitle("Anode voltage [V]");
    gr_S1_top_bottom->GetYaxis()->SetTitle("S1_top / S1_bottom");
    gr_S1_top_bottom->SetTitle("");

    c1->cd(6);
//    TGraphErrors *gr_S2_tb_S1_tb = new TGraphErrors(Va.size(), &Va[0], &S2_tb_S1_tb[0], &Va_err[0], &S2_tb_S1_tb_err[0]);
//    gr_S2_tb_S1_tb->Draw("APE");
//    gr_S2_tb_S1_tb->SetMarkerSize(1.3);
//    gr_S2_tb_S1_tb->SetMarkerStyle(20);
//    gr_S2_tb_S1_tb->SetMarkerColor(kBlue);
//    gr_S2_tb_S1_tb->GetXaxis()->SetTitle("Anode voltage [V]");
//    gr_S2_tb_S1_tb->GetYaxis()->SetTitle(" (S2_top/S2_bottom) / (S1_top/S1_bottom)");
//    gr_S2_tb_S1_tb->SetTitle("");

    TGraphErrors *gr_S2_tb_norm = new TGraphErrors(Va.size(), &Va[0], &S2_top_bottom_norm[0], &Va_err[0], &S2_top_bottom_norm_err[0]);
    gr_S2_tb_norm->Draw("APE");
    gr_S2_tb_norm->SetMarkerSize(1.3);
    gr_S2_tb_norm->SetMarkerStyle(20);
    gr_S2_tb_norm->SetMarkerColor(kGreen);
    gr_S2_tb_norm->GetXaxis()->SetTitle("Anode voltage [V]");
    gr_S2_tb_norm->GetYaxis()->SetTitle("Normalized S2_top/S2_bottom or S1_top/S1_bottom");
    gr_S2_tb_norm->SetTitle("");
    gr_S2_tb_norm->SetName("gr_S2_tb_norm");
    //gPad->Update();
    gr_S2_tb_norm->GetYaxis()->SetRangeUser(0.9, 1.02);

    TGraphErrors *gr_S1_tb_norm = new TGraphErrors(Va.size(), &Va[0], &S1_top_bottom_norm[0], &Va_err[0], &S1_top_bottom_norm_err[0]);
    gr_S1_tb_norm->Draw("same PE");
    gr_S1_tb_norm->SetMarkerSize(1.3);
    gr_S1_tb_norm->SetMarkerStyle(20);
    gr_S1_tb_norm->SetMarkerColor(kMagenta);
    gr_S1_tb_norm->SetName("gr_S1_tb_norm");

    TLegend *lcd6= new TLegend(0.4,0.5,0.9,0.8);
    lcd6->AddEntry("gr_S2_tb_norm","S2_top/S2_bottom norm at V_{a} = 5211","P");
    lcd6->AddEntry("gr_S1_tb_norm","S1_top/S1_bottom norm at V_{a} = 5211","P");
    //l->SetBorderSize(0);
    lcd6->Draw();


}
