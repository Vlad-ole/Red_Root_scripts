//Qt
//#include <QCoreApplication>

//C++
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>

//root cern
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
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
#include "TRandom.h"
#include "TRandom3.h"

//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

TString cut_loop0_srt;
bool cut_loop0_bool = false;
#define REMEMBER_CUT_LOOP0(x) cut_loop0_srt = #x;  cut_loop0_bool = x;

TString cut_loop2_srt;
bool cut_loop2_bool = false;
#define REMEMBER_CUT_LOOP2(x) cut_loop2_srt = #x;  cut_loop2_bool = x;

using namespace std;
using namespace ROOT;

int run_number = 1079;

void color_map()
{
    //string run_text = ": Kr, heater 0.939W, A=+5211, R=+156, C=-744 ";
    string run_text = ": Am @ center, heater 0.939W, A=+5211, R=+156, C=-744 (before jump)";


    ostringstream hist_title;
    hist_title << "S2 relative charge. Run " << run_number << run_text;

    //": Kr, heater 1.4W, A=+5211, R=+156, C=-744"
    //": Kr, GP 0.935W, A=+3780, R=+85, C=-815"

    ostringstream path_in_file;
    path_in_file << "/home/vlad/Reports/XY/S2_uniformity/run_" << run_number << ".txt";
    //path_in_file << "/home/vlad/Reports/XY/xy_distr/run_" << run_number << ".txt";
    ifstream file_in(path_in_file.str().c_str());
    if(file_in.is_open())
    {
        cout << "File has been opened correctly: " << path_in_file.str().c_str() << endl;
    }
    else
    {
        cout << "Error: " << path_in_file.str().c_str() << endl;
        exit(1);
    }

    vector<double> x_centers = {0.625, 1.875, 3.125, 4.375};
    vector<double> y_centers = {0.416667, 1.25, 2.08333, 2.91667, 3.75, 4.58333};

    double x = 0;
    double y = 0;
    vector<double> xv;
    vector<double> yv;
    double ch_num = 0;
    while (file_in.good())
    {
         file_in >> y;
         ch_num++;
         xv.push_back(ch_num);
         yv.push_back(y);
    }

    TH2F* h2_S2_total_rel = new TH2F("h2_S2_total_rel","h2_S2_total_rel",4,0,5,6,0,5);
//    h2_S2_total_rel->SetBinContent(1,1,4);
//    cout << h2_S2_total_rel->GetBinContent(1,1) << endl;

    //----------------------------------------------------------------
    //palette settings - completely independent
    const Int_t NRGBs = 6;
    const Int_t NCont = 999;

    //v1
//    Double_t stops[NRGBs] = { 0.00, 0.1, 0.34, 0.61, 0.84, 1.00 };
//    Double_t red[NRGBs]   = { 0.99, 0.0, 0.00, 0.87, 1.00, 0.51 };
//    Double_t green[NRGBs] = { 0.00, 0.0, 0.81, 1.00, 0.20, 0.00 };
//    Double_t blue[NRGBs]  = { 0.99, 0.0, 1.00, 0.12, 0.00, 0.00 };

    //v2
//    Double_t stops[NRGBs] = { 0.00, 0.1, 0.34, 0.61, 0.84, 1.00 };
//    Double_t red[NRGBs]   = {0.00, 0.00, 0.00, 0.56, 0.99, 0.99};
//    Double_t green[NRGBs] = {0.83, 0.99, 0.99, 0.99, 0.73, 0.23};
//    Double_t blue[NRGBs]  = {0.99, 0.56, 0.00, 0.00, 0.00, 0};

    //v3
    Double_t stops[NRGBs] = { 0.00, 0.2, 0.4, 0.6, 0.8, 1.00 };
    Double_t red[NRGBs]   = { 0.99, 0.44, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.66, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.99, 0.72, 1.00, 0.12, 0.00, 0.00 };

    //0, 211, 253
    //0, 253, 143
    //0, 253, 0
    //143, 253, 0
    //253, 186, 0
    //253, 59, 0

//112, 169 183


    //0.99, 0, 0.99 = 252, 0, 252
    //
    //0, 0.81, 1 = 0, 207, 255

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    gStyle->SetOptStat(0);

    //here the actually interesting code starts
    const Double_t min = 0.65;
    const Double_t max = 1.3;

//    const Double_t min = 0;
//    const Double_t max = 600;

    const Int_t nLevels = 999;
    Double_t levels[nLevels];

    for(int i = 1; i < nLevels; i++) {
        levels[i] = min + (max - min) / (nLevels - 1) * (i);
    }
    levels[0] = 0.01;

    h2_S2_total_rel->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
    //----------------------------------------------------------------

    int n_events = 0;
    for(int i = 0; i < 24; i++)
    {
        int xi = i % 4 + 1;
        int yi = i/4 + 1;
        h2_S2_total_rel->SetBinContent(xi,yi,yv[i]);
        cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S2_total_rel->GetBinContent(xi,yi) <<  endl;
        n_events++;
    }

    h2_S2_total_rel->SetTitle(hist_title.str().c_str());
    h2_S2_total_rel->GetXaxis()->SetTitle("x [cm]");
    h2_S2_total_rel->GetYaxis()->SetTitle("y [cm]");
    gStyle->SetPaintTextFormat("2.3f");


    //----------------------------------------------------------------
    h2_S2_total_rel->DrawClone("col text");// draw "axes", "contents", "statistics box"

    h2_S2_total_rel->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

    h2_S2_total_rel->Draw("z same"); // draw the "color palette"
    h2_S2_total_rel->SetMarkerStyle(20);
    h2_S2_total_rel->SetMarkerSize(0);

    //----------------------------------------------------------------




}

