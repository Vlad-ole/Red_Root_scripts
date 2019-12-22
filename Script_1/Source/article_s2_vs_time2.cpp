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

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void article_s2_vs_time2()
{
    vector<double> x_0_bub{0.028, 0.823, 1.087, 1.387, 1.771};
    vector<double> x_1_bub{7.379, 8.049};

    vector<double> x_all_field{0.065, 0.089, 0.872, 1.137, 1.437, 1.820, 0.169, 0.187, 0.849, 1.625, 1.999, 3.991, 6.667, 6.716, 6.872, 7.692};
    vector<double> x_0_field{0.065, 0.089, 0.872, 1.137, 1.437, 1.820};
    vector<double> x_1_field{0.169, 0.187, 0.849, 1.625, 1.999, 3.991, 6.667, 6.716, 6.872, 7.692};

    vector<double> y_0_center{13138.6, 13236.7, 13425.8, 13348.9, 13203.3, 13284};
    vector<double> y_1_center{13376.2, 13419.7, 13281, 13346.2, 13299.4, 13225.4, 13206.6, 13221.6, 13141.1, 13286.8};

    vector<double> s1_0_center{642.126, 638.926, 638.59, 641.732, 639.091, 639.488};
    vector<double> s1_1_center{641.717, 643.471, 640.145, 642.774, 638.635, 639.902, 639.637, 637.587, 635.426, 640.41};

    vector<double> s1_0_border{647.791, 648.142, 648.571, 649.681, 648.265, 648.498};
    vector<double> s1_1_border{651.562, 651.741, 649.01, 650.573, 647.142, 647.187, 648.279, 647.544, 644.768, 648.457};

    vector<double> y_all_border{9406.21, 9267.79, 11534.9, 11766.6, 11935.3, 12116.3, 10176, 10327, 11545.5, 12080, 12195.6, 12408, 12768.7, 12728.1, 12701.1, 12813.5};
    vector<double> y_0_border{9406.21, 9267.79, 11534.9, 11766.6, 11935.3, 12116.3};
    vector<double> y_1_border{10176, 10327, 11545.5, 12080, 12195.6, 12408, 12768.7, 12728.1, 12701.1, 12813.5};

    vector<string> x_1_field_text{"1236", ".", "1238", "1239", "1240", "1242", "1246", "1247", "1248", "1249"};

    COUT(x_all_field.size());
    COUT(y_all_border.size());

    TMultiGraph *mg = new TMultiGraph();

    TGraph *gr = new TGraph(x_0_bub.size(), &x_0_bub[0], &y_0_center[0]);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(3.5);
    gr->SetMarkerColor(kBlue);

    TGraph *gr2 = new TGraph(x_0_bub.size(), &x_0_bub[0], &y_0_border[0]);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerSize(3.5);
    gr2->SetMarkerColor(kBlue);

    TGraph *gr3 = new TGraph(x_1_bub.size(), &x_1_bub[0], &y_1_center[0]);
    gr3->SetMarkerStyle(20);
    gr3->SetMarkerSize(3.5);
    gr3->SetMarkerColor(kGreen);

    TGraph *gr4 = new TGraph(x_1_bub.size(), &x_1_bub[0], &y_1_border[0]);
    gr4->SetMarkerStyle(21);
    gr4->SetMarkerSize(3.5);
    gr4->SetMarkerColor(kGreen);



    /////
    TGraph *gr5 = new TGraph(x_0_field.size(), &x_0_field[0], &y_0_center[0]);
    gr5->SetMarkerStyle(20);
    gr5->SetMarkerSize(3.5);
    gr5->SetMarkerColor(kBlue);

    TGraph *gr6 = new TGraph(x_0_field.size(), &x_0_field[0], &y_0_border[0]);
    gr6->SetMarkerStyle(21);
    gr6->SetMarkerSize(3.5);
    gr6->SetMarkerColor(kBlue);

    TGraph *gr7 = new TGraph(x_1_field.size(), &x_1_field[0], &y_1_center[0]);
    gr7->SetMarkerStyle(20);
    gr7->SetMarkerSize(3.5);
    gr7->SetMarkerColor(kGreen);

    TGraph *gr8 = new TGraph(x_1_field.size(), &x_1_field[0], &y_1_border[0]);
    gr8->SetMarkerStyle(21);
    gr8->SetMarkerSize(3.5);
    gr8->SetMarkerColor(kGreen);

    TGraph *gr9 = new TGraph(x_all_field.size(), &x_all_field[0], &y_all_border[0]);
    gr9->SetMarkerStyle(21);
    gr9->SetMarkerSize(0);
    gr9->SetMarkerColor(kMagenta);

    //v0
//    TF1 *f2 = new TF1("f2","[0]*(1 - exp(x*[1]) ) + [2]",0,15);
//    f2->SetParameter(0, 2000);
//    //f2->FixParameter(0, 13289.6);
//    f2->SetParameter(1, -1.0/3.5);
//    //f2->FixParameter(1, -1.0/3.5);
//    f2->SetParameter(2, 9500);
//    //f2->FixParameter(2, 11000);
//    gr9->Fit("f2","R","",0,15);
//    cout << "tau = " << -1.0/f2->GetParameter(1) << " [days]" << endl;

    //v1
    TF1 *f2 = new TF1("f2","(13236 - [0] - [1]) + [0]*( 1 - exp(x*[2])) + [1]*( 1 - exp(x*[3]))  ",0,15);
    f2->SetParameter(0, 4000);
    f2->SetParameter(1, 6000);
    f2->SetParameter(2, -1);
    f2->SetParLimits(2, -10, 0);
    f2->SetParameter(3, -0.2);
    f2->SetParLimits(3, -4, 0);
    gr9->Fit("f2","R","",0,15);
    cout << "tau1 = " << -1.0/f2->GetParameter(2) << " [days]" << endl;
    cout << "tau2 = " << -1.0/f2->GetParameter(3) << " [days]" << endl;

    //v2
//    TF1 *f2 = new TF1("f2","(13236 - [0]) + [0]*( 1 - exp(x*[1])) ",0,15);
//    f2->SetParameter(0, 4000);
//    f2->SetParameter(2, -1);
//    f2->SetParLimits(2, -10, 0);
//    gr9->Fit("f2","R","",0,15);
//    cout << "tau1 = " << -1.0/f2->GetParameter(1) << " [days]" << endl;




//    TF1 *f1 = new TF1("f1","[0]",0,15);
//    gr->Fit("f1","R","",0,15);

//    TGraph *gr2 = new TGraph(x.size(), &x[0], &y2[0]);
//    gr2->SetMarkerStyle(20);
//    gr2->SetMarkerSize(3.5);
//    gr2->SetMarkerColor(kBlue);

//    TF1 *f2 = new TF1("f2","[0]*(1 - exp(x*[1]) + [2] )",0,15);
//    //f2->SetParameter(0, 13289.6);
//    f2->FixParameter(0, 13289.6);
//    //f2->SetParameter(1, -1.0/3.5);
//    f2->FixParameter(1, -1.0/3.5);
//    //gr2->Fit("f2","R","",0,15);

//    TGraph *gr3 = new TGraph(x_fields1.size(), &x_fields1[0], &y1[0]);
//    gr3->SetMarkerStyle(20);
//    gr3->SetMarkerSize(3.5);
//    gr3->SetMarkerColor(kBlack);

//    TGraph *gr4 = new TGraph(x_fields1.size(), &x_fields1[0], &y2[0]);
//    gr4->SetMarkerStyle(20);
//    gr4->SetMarkerSize(3.5);
//    gr4->SetMarkerColor(kBlue);

//    TGraph *gr5 = new TGraph(x_fields1.size(), &x_fields1[0], &y2[0]);
//    gr5->SetMarkerStyle(20);
//    gr5->SetMarkerSize(3.5);
//    gr5->SetMarkerColor(kBlue);


//    mg->Add(gr);
//    mg->Add(gr2);
//    mg->Add(gr3);
//    mg->Add(gr4);
    mg->Add(gr5);
    mg->Add(gr6);
    mg->Add(gr7);
    mg->Add(gr8);
    mg->Add(gr9, "AP");


    TMultiGraph *mg2 = new TMultiGraph();

    TGraph *gr_s1_0_center = new TGraph(x_0_field.size(), &x_0_field[0], &s1_0_center[0]);
    gr_s1_0_center->SetMarkerStyle(20);
    gr_s1_0_center->SetMarkerSize(3.5);
    gr_s1_0_center->SetMarkerColor(kBlue);

    TGraph *gr_s1_0_border = new TGraph(x_0_field.size(), &x_0_field[0], &s1_0_border[0]);
    gr_s1_0_border->SetMarkerStyle(21);
    gr_s1_0_border->SetMarkerSize(3.5);
    gr_s1_0_border->SetMarkerColor(kBlue);

    TGraph *gr_s1_1_center = new TGraph(x_1_field.size(), &x_1_field[0], &s1_1_center[0]);
    gr_s1_1_center->SetMarkerStyle(20);
    gr_s1_1_center->SetMarkerSize(3.5);
    gr_s1_1_center->SetMarkerColor(kGreen);

    TGraph *gr_s1_1_border = new TGraph(x_1_field.size(), &x_1_field[0], &s1_1_border[0]);
    gr_s1_1_border->SetMarkerStyle(21);
    gr_s1_1_border->SetMarkerSize(3.5);
    gr_s1_1_border->SetMarkerColor(kGreen);

    mg2->Add(gr_s1_0_center);
    mg2->Add(gr_s1_0_border);
    mg2->Add(gr_s1_1_center);
    mg2->Add(gr_s1_1_border);




    TCanvas *c1 = new TCanvas("c1","c1");
    //mg->GetXaxis()->SetTitle("Time since V_bubbler > 0V [days]");
    mg->GetXaxis()->SetTitle("Time since fields were turned on [days]");
    mg->GetYaxis()->SetTitle("S2 [PE]");
    //mg->GetYaxis()->SetRangeUser(0, 14000);
    mg->Draw("AP");

//    // draw text
//    TLatex l;
//    l.SetTextSize(0.020);
//    l.SetTextAngle(30.);
//    for (int i=0; i < x_1_field.size(); i++)
//    {
//       l.DrawLatex(x_1_field[i],y_1_center[i], x_1_field_text[i].c_str());
//    }


    gPad->Update();
    //mg->GetXaxis()->SetRangeUser(0, 10);


//    TCanvas *c2 = new TCanvas("c2","c2");
//    mg2->Draw("AP");

////    // draw text
////    TLatex l2;
////    l2.SetTextSize(0.020);
////    l2.SetTextAngle(0.);
////    for (int i=0; i < x_1_field.size(); i++)
////    {
////       l2.DrawLatex(x_1_field[i],s1_1_center[i], x_1_field_text[i].c_str());
////    }


//    gPad->Update(); gPad->Modified();
//    mg2->GetXaxis()->SetTitle("Time since fields were turned on [days]");
//    mg2->GetYaxis()->SetTitle("S1 [PE]");

}
