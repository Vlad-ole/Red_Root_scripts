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
#include "TMultiGraph.h"

using namespace std;

void article_s2_rms()
{
    vector<double> x1{0.065, 0.089, 0.872, 1.137, 1.437, 1.820};
    vector<double> x2{0.169, 0.187, 0.849, 1.625, 1.999, 3.991, 6.667, 6.716, 6.872, 7.692,  7.773, 7.865, 7.888, 7.910};
    vector<double> y1{4774.76, 4556.42, 3131.11, 3238.48, 3168.81, 3016.34};
    vector<double> y2{3936.8, 3920.42, 3229.65, 3025.92, 3063.78, 2938.99, 2840.41, 3053.05, 2894.67, 2888.66, 3281.19, 2648.06, 4335.7, 4766.72};
    vector<string> text2{"1236", ".", "1238", "1239", "1240", "1242", "1246", "1247", "1248", "1249, R=+86", "1251, R=+16", "1253, R=+16", "1254, R=+156", "1255, R=+226"};

    vector<double> x_all{0.077, 0.872, 1.137, 1.437, 1.820, 0.178, 0.849, 1.625, 1.999};
    vector<double> y_all{4667.15, 3131.11, 3238.48, 3168.81, 3016.34, 3929.16, 3229.65, 3025.92, 3063.78};

    TGraph *gr1 = new TGraph(x1.size(), &x1[0], &y1[0]);
    gr1->SetMarkerStyle(21);
    gr1->SetMarkerSize(3.5);
    gr1->SetMarkerColor(kBlue);
    //TF1 *f3 = new TF1("f3","[0]*exp(x*[1])",0,3);
    //gr1->Fit("f3","R","",0,6);

    TGraph *gr2 = new TGraph(x2.size(), &x2[0], &y2[0]);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerSize(3.5);
    gr2->SetMarkerColor(kGreen);
    //TF1 *f2 = new TF1("f4","[0]*exp(x*[1])",0,3);
    //gr2->Fit("f4","R","",0,6);

    TGraph *gr3 = new TGraph(x_all.size(), &x_all[0], &y_all[0]);
    gr3->SetMarkerStyle(21);
    gr3->SetMarkerSize(0);
    gr3->SetMarkerColor(kMagenta);
//    TF1 *f3 = new TF1("f3","[0]*exp(x*[1])",0,3);
//    gr3->Fit("f3","R","",0,6);



    TMultiGraph *mg = new TMultiGraph();

    mg->Add(gr1);
    mg->Add(gr2);
    //mg->Add(gr3);

    mg->GetXaxis()->SetTitle("Time since fields were turned on [days]");
    mg->GetYaxis()->SetTitle("S2 RMS [PE]");
    mg->GetYaxis()->SetRangeUser(0, 15000);
    mg->Draw("AP");

    // draw text
    TLatex l;
    l.SetTextSize(0.020);
    l.SetTextAngle(0.);
    for (int i=0; i < x2.size(); i++)
    {
       l.DrawLatex(x2[i],y2[i], text2[i].c_str());
       //l.DrawLatex(x2[i],y2[i],Form("(%g,%g)",x2[i],y2[i]));
    }
}
