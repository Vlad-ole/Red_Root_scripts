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
#include "TGaxis.h"
#include "TExec.h"

using namespace std;

void article_s2_rel_charge_rms()
{
    vector<double> x{0.302, 0.027, 0.215, 0.272, 0.381, 2.233, 2.261, 2.326, 3.206, 9.346};
    vector<double> y{0.152, 0.168, 0.161, 0.145, 0.147, 0.133, 0.119, 0.121, 0.117, 0.079};

    vector<double> x0{0.028, 0.823, 1.087, 1.387, 1.771};
    vector<double> y0{0.172, 0.138, 0.123, 0.116, 0.105};

    vector<double> x1{7.379, 8.049};
    vector<double> y1{0.15, 0.1306};

    vector<double> x2{0.028, 0.823, 1.087, 1.387, 1.771, 7.379, 8.049};
    vector<double> y2{0.172, 0.138, 0.123, 0.116, 0.105, 0.15, 0.1306};

    vector<double> x3{0.065, 0.089, 0.872, 1.137, 1.437, 1.820};//time since fields were turned on
    vector<double> y3{0.174458, 0.173855, 0.138, 0.123, 0.116, 0.105};

    vector<double> x4{0.169, 0.187, 0.849, 1.625, 1.999, 3.991, 6.667, 6.716, 6.872, 7.692, 7.773, 7.865, 7.888, 7.910};//time since fields were turned on
    vector<double> y4{0.158, 0.145, 0.1306, 0.118, 0.106, 0.0956, 0.0717, 0.0749, 0.0799, 0.0624, 0.0659, 0.0549, 0.0793, 0.0881};
    vector<string> text4{"1236", "1236", "1238", "1239", "1240", "1242", "1246", "1247", "1248", "1249, R=+86", "1251, R=+16", "1253, R=+16", "1254, R=+156", "1255, R=+226"};

    vector<double> x5{0.024};
    vector<double> y5{0.173};
    vector<string> text5{"1261"};

    TMultiGraph *mg = new TMultiGraph();
    TGraph *gr = new TGraph(x.size(), &x[0], &y[0]);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.5);
    //gr->Draw("AP");

//    TF1 *f1 = new TF1("f1","[0]*exp(x*[1])",0,15);
//    gr->Fit("f1","R","",0,15);

//    TGraph *gr0 = new TGraph(x0.size(), &x0[0], &y0[0]);
//    gr0->SetMarkerStyle(20);
//    gr0->SetMarkerSize(1.5);
//    gr0->SetMarkerColor(kBlue);
//    TF1 *f0 = new TF1("f0","[0]*exp(x*[1])",0,3);
//    gr0->Fit("f0","R","",0,6);

//    TGraph *gr1 = new TGraph(x1.size(), &x1[0], &y1[0]);
//    gr1->SetMarkerStyle(20);
//    gr1->SetMarkerSize(1.5);
//    gr1->SetMarkerColor(kGreen);
//    //TF1 *f2 = new TF1("f2","[0]*exp(x*[1])",0,3);
//    //gr2->Fit("f2","R","",0,6);

//    TGraph *gr2 = new TGraph(x2.size(), &x2[0], &y2[0]);
//    gr2->SetMarkerStyle(20);
//    gr2->SetMarkerSize(1.5);
//    gr2->SetMarkerColor(kBlue);
//    TF1 *f2 = new TF1("f2","[0]*exp(x*[1])",0,3);
//    gr2->Fit("f2","R","",0,6);

    TGraph *gr3 = new TGraph(x3.size(), &x3[0], &y3[0]);
    gr3->SetMarkerStyle(20);
    gr3->SetMarkerSize(1.5);
    gr3->SetMarkerColor(kBlue);
    TF1 *f3 = new TF1("f3","[0]*exp(x*[1])",0,3);
    //gr3->Fit("f3","R","",0,6);

    TGraph *gr4 = new TGraph(x4.size(), &x4[0], &y4[0]);
    gr4->SetMarkerStyle(20);
    gr4->SetMarkerSize(1.5);
    gr4->SetMarkerColor(kGreen);
    TF1 *f4 = new TF1("f4","[0]*exp(x*[1])",0,3);
    //gr4->Fit("f4","R","",0,6);

    TGraph *gr5 = new TGraph(x5.size(), &x5[0], &y5[0]);
    gr5->SetMarkerStyle(20);
    gr5->SetMarkerSize(1.5);
    gr5->SetMarkerColor(kMagenta);

//    TExec *ex = new TExec("ex","drawtext();");
//    gr4->GetListOfFunctions()->Add(ex);

    //mg->Add(gr0);
    //mg->Add(gr1);

    mg->Add(gr3);
    mg->Add(gr4);
    mg->Add(gr5);

    //mg->GetXaxis()->SetTitle("Time since V_bubbler > 12V [days]");
    mg->GetXaxis()->SetTitle("Time since fields were turned on [days]");
    mg->GetYaxis()->SetTitle("S2_rel_charge_RMS");
    mg->GetYaxis()->SetRangeUser(0, 0.2);
    mg->GetXaxis()->SetRangeUser(0, 15);
    mg->Draw("AP");

    // draw text
    TLatex l;
    l.SetTextSize(0.020);
    l.SetTextAngle(0.);
    for (int i=0; i < x4.size(); i++)
    {
       l.DrawLatex(x4[i],y4[i], text4[i].c_str());
    }

    TLatex l2;
    l2.SetTextSize(0.020);
    l2.SetTextAngle(0.);
    for (int i=0; i < x5.size(); i++)
    {
       l.DrawLatex(x5[i],y5[i], text5[i].c_str());
    }

//    double low = 0;
//    double high = 100;
//    TGaxis *axis = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(),gPad->GetUymax(),low,high,510,"+L");
//    axis->Draw();
}
