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

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void ser()
{
    vector<double> time{0.92,
                        1.01,
                        1.07,
                        1.23,
                        5.94,
                        6.10,
                        6.19,
                        15.32,
                        17.89,
                        18.89,
                        /*18.96,*/
                        19.24,
                        20.94,
                        20.97,
                        22.14,
                        23.12,
                        26.15,
                        26.89,
                        27.88,
                        29.04,
                        32.11,
                        33.91,
                        35.14,
                        36.28,
                        36.94,
                        38.88,
                        38.95,
                        39.11,
                        39.29,
                        40.13,
                        40.88,
                        56.33,
                        61.20,
                        62.02,
                        63.20,
                        64.03,
                        102.09,
                        118.25,
                        118.92,
                        /*152.23,*/
                        153.92};

    vector<double> ser_top{2453.02,
                           2447.91,
                           2447.58,
                           2441.91,
                           2443.84,
                           2441.78,
                           2442.53,
                           2444.20,
                           2442.21,
                           2444.22,
                           /*2387.22,*/
                           2432.02,
                           2436.77,
                           2439.07,
                           2419.35,
                           2413.07,
                           2418.69,
                           2424.14,
                           2421.36,
                           2423.19,
                           2425.70,
                           2428.37,
                           2424.43,
                           2430.92,
                           2419.09,
                           2433.51,
                           2431.70,
                           2431.09,
                           2432.82,
                           2424.56,
                           2427.56,
                           2432.80,
                           2427.80,
                           2425.93,
                           2434.06,
                           2424.42,
                           2435.26,
                           2437.79,
                           2437.58,
                           /*2362.27,*/
                           2431.52};


    vector<double> ser_bottom{3390.33,
                              3399.62,
                              3396.21,
                              3379.24,
                              3390.88,
                              3390.79,
                              3403.47,
                              3409.18,
                              3394.83,
                              3377.40,
                              /*3343.68,*/
                              3396.62,
                              3378.32,
                              3378.49,
                              3396.53,
                              3378.77,
                              3397.65,
                              3407.31,
                              3394.00,
                              3389.57,
                              3400.74,
                              3380.88,
                              3404.25,
                              3403.53,
                              3384.45,
                              3389.41,
                              3410.82,
                              3414.90,
                              3377.43,
                              3381.07,
                              3368.41,
                              3401.06,
                              3395.19,
                              3360.08,
                              3397.39,
                              3381.66,
                              3369.34,
                              3398.30,
                              3394.13,
                              /*3318.72,*/
                              3376.53};

    vector<int> run_int{1040,
                        1043,
                        1047,
                        1052,
                        1053,
                        1055,
                        1058,
                        1069,
                        1070,
                        1078,
                        /*1080,*/
                        1084,
                        1086,
                        1087,
                        1094,
                        1100,
                        1101,
                        1105,
                        1111,
                        1116,
                        1122,
                        1124,
                        1127,
                        1132,
                        1133,
                        1136,
                        1140,
                        1147,
                        1158,
                        1162,
                        1166,
                        1174,
                        1176,
                        1178,
                        1185,
                        1190,
                        1194,
                        1222,
                        1224,
                        /*1265,*/
                        1270};

    vector<string> run_srt;
    for (int i = 0; i < run_int.size(); i++)
    {
        run_srt.push_back(to_string(run_int[i]));
    }

    TCanvas *c1 = new TCanvas("c1", "c1");
    TMultiGraph *mg = new TMultiGraph();

    TGraph *gr = new TGraph(time.size(), &time[0], &ser_top[0]);
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kRed);
    //gr->Draw("AP");
    //gr->GetXaxis()->SetTitle("Days after June 6th, 2019");
    //gr->GetYaxis()->SetTitle("Charge [ADC*Sample]");
    gr->SetTitle("Top SiPM matrix");
    //gr->SetTitle("Bottom SiPM matrix");
    //gr->SetTitle("");

    TGraph *gr2 = new TGraph(time.size(), &time[0], &ser_bottom[0]);
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerColor(kBlue);
    gr2->SetTitle("Bottom SiPM matrix");

    mg->Add(gr);
    mg->Add(gr2);
    mg->Draw("AP");
    mg->SetTitle(";Days after June 6th, 2019; Charge [ADC*Sample]");

    gPad->BuildLegend();

//    TLatex l;
//    l.SetTextSize(0.020);
//    l.SetTextAngle(20.);
//    for (int i=0; i < time.size(); i++)
//    {
//       l.DrawLatex(time[i],ser_top[i], run_srt[i].c_str());
//    }

    TCanvas *c2 = new TCanvas("c2", "c2");
    TGraph *gr3 = new TGraph();

}
