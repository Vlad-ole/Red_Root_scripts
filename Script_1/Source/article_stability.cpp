//C++
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>

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

using namespace std;

void article_stability()
{
    vector<int> run_list{1042, 1071, 1137, 1195};
    vector<double> Time{0.99, 17.92, 38.90, 102.11};
    vector<double> S1_err{2, 2, 2, 2};
    vector<double> Time_err{0, 0, 0, 0};
    vector<double> S1_total{793.25, 798.54, 771.00, 779.00};
    vector<double> S1_top{381.25, 388.54, 317.00, 334.00};
    vector<double> S1_bottom{412, 410, 454, 445};
    vector<double> Bottom_avr_calib{3390.33, 3394.8325, 3389.405, 3369.3425};
    vector<double> Top_avr_calib{2453.016087, 2442.214348, 2433.512917, 2435.259167};
    vector<double> S1_total_yield_net{9.95, 10.02, 9.67, 9.77};
    vector<double> S1_total_yield_net_err{0.03, 0.03, 0.03, 0.03};

    TCanvas *c1 = new TCanvas("S1_analysis","S1_analysis");
    c1->Divide(3,2,0.01,0.01);

    c1->cd(1);
    TGraphErrors *gr1 = new TGraphErrors(Time.size(), &Time[0], &S1_total[0], &Time_err[0], &S1_err[0]);
    gr1->Draw("AP");
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerSize(1.5);
    gr1->SetTitle("");
    gr1->GetYaxis()->SetTitle("S1 total [PE]");
    gr1->GetXaxis()->SetTitle("Time since condensation [days]");

    c1->cd(2);
    TGraphErrors *gr2 = new TGraphErrors(Time.size(), &Time[0], &S1_top[0], &Time_err[0], &S1_err[0]);
    gr2->Draw("AP");
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerSize(1.5);
    gr2->SetTitle("");
    gr2->GetYaxis()->SetTitle("S1 top [PE]");
    gr2->GetXaxis()->SetTitle("Time since condensation [days]");

    c1->cd(3);
    TGraphErrors *gr3 = new TGraphErrors(Time.size(), &Time[0], &S1_bottom[0], &Time_err[0], &S1_err[0]);
    gr3->Draw("AP");
    gr3->SetMarkerStyle(20);
    gr3->SetMarkerSize(1.5);
    gr3->SetTitle("");
    gr3->GetYaxis()->SetTitle("S1 bottom [PE]");
    gr3->GetXaxis()->SetTitle("Time since condensation [days]");

    c1->cd(5);
    TGraphErrors *gr4 = new TGraphErrors(Time.size(), &Time[0], &Top_avr_calib[0], &Time_err[0], &Time_err[0]);
    gr4->Draw("AP");
    gr4->SetMarkerStyle(20);
    gr4->SetMarkerSize(1.5);
    gr4->SetTitle("");
    gr4->GetYaxis()->SetTitle("Top_avr_calib [ADC*Sample]");
    gr4->GetXaxis()->SetTitle("Time since condensation [days]");

    c1->cd(6);
    TGraphErrors *gr5 = new TGraphErrors(Time.size(), &Time[0], &Bottom_avr_calib[0], &Time_err[0], &Time_err[0]);
    gr5->Draw("AP");
    gr5->SetMarkerStyle(20);
    gr5->SetMarkerSize(1.5);
    gr5->SetTitle("");
    gr5->GetYaxis()->SetTitle("Bottom_avr_calib [ADC*Sample]");
    gr5->GetXaxis()->SetTitle("Time since condensation [days]");

    TCanvas *c2 = new TCanvas("c2","c2");
    TGraphErrors *gr7 = new TGraphErrors(Time.size(), &Time[0], &S1_total_yield_net[0], &Time_err[0], &S1_total_yield_net_err[0]);
    gr7->Draw("AP");
    gr7->SetMarkerStyle(20);
    gr7->SetMarkerSize(1.5);
    gr7->SetTitle("");
    gr7->GetYaxis()->SetTitle("S1 light yield [PE/keV]");
    gr7->GetXaxis()->SetTitle("Time since filling the TPC [days]");

}
