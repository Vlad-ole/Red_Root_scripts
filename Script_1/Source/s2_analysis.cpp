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


int run_number = 1051;
//int run_number = 976;

void s2_analysis()
{
    ostringstream path_root_file;
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    TString filename = path_root_file.str().c_str();
    TFile *f = new TFile(filename, "read");
    if (!(f->IsOpen()))
    {
        cout << "could not open file: " << filename << endl;
        exit(1);
    }
    else
    {
        cout << "File has been opened correctly: " << filename << endl;
    }

    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    double S1_max = 1000;
    double S2_max = 2500;
    double S2_S1_max = 4;
    //double range_scale = 1;
    double left_lim = 15 /*15*/;
    double right_lim = 55 /*55*/;
    int max_ev_number = 110000;

    //hists
    TH1F *h1_S3_total = new TH1F("h1_S3_total", "h1_S3_total", 600, -2, 2500);

    //S1
    TH2F *h2_S1_total_event = new TH2F("h2_S1_event", "h2_S1_event", 110, 0, max_ev_number, 200, 0, S1_max);
    TH1F *h1_S1_top = new TH1F("h1_S1_top", "h1_S1_top", 200, 0, S1_max);
    TH1F *h1_S1_bot = new TH1F("h1_S1_bot", "h1_S1_bot", 200, 0, S1_max);
    TH1F *h1_S1_total = new TH1F("h1_S1_total", "h1_S1_total", 400, 0, S1_max);
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 400, -0.2, 1);
    TH1F *h1_S1_TBA = new TH1F("h1_S1_TBA", "h1_S1_TBA", 200, -1, 1);
    TH2F *h2_S1_TBA = new TH2F("h2_S1_TBA", "h2_S1_TBA", 200, -0.5, 0.5, 200, 0, S1_max);

    //S2
    TH2F *h2_S2_event = new TH2F("h2_S2_event", "h2_S2_event", 110, 0, max_ev_number, 150, 0, 1E4);
    TH1F *h1_S2 = new TH1F("h1 S2", "h1 title", 400, -100, S2_max);
    TH1F *h1_S2_top = new TH1F("h1 S2 top", "h1 title", 200, -100, S2_max/2.0 );
    TH1F *h1_S2_bot = new TH1F("h1 S2 bot", "h1 title", 200, -100, S2_max/2.0 );
    TH1F *h1_S2_S1_ratio = new TH1F("h1 S2/S1", "h1 title", 300, -1, 30);
    TH1F *h1_S2_f90 = new TH1F("h1_S2_f90", "h1_S2_f90", 400, -0.2, 1);

    TH2F *h2_S2_TBA = new TH2F("h2_S2_TBA", "h2_S2_TBA", 200, -0.5, 0.5, 200, 0, S2_max);





    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        if(clusters.size() == 2)
        {
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = true /*clusters.at(0)->charge > 300*/;
            bool cut_S2_totoal = true /*clusters.at(0)->charge > 300*/;
            if(cut_f90 && cut_S1_total && cut_S2_totoal)
            {
                double TBA_S1 = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) / (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);


                //S1
                h2_S1_total_event->Fill(ev, clusters.at(0)->charge);
                h1_S1_total->Fill(clusters.at(0)->charge);
                h1_S1_f90->Fill(clusters.at(0)->f90);
                h1_S1_top->Fill(clusters.at(0)->tot_charge_top);
                h1_S1_bot->Fill(clusters.at(0)->tot_charge_bottom);
                h1_S1_TBA->Fill(TBA_S1);
                h2_S1_TBA->Fill(TBA_S1, clusters.at(0)->charge);

                //S2
                h2_S2_event->Fill(ev, clusters.at(1)->charge);


                //h1_S3_total->Fill(clusters.at(2)->charge);

            }
        }
    }






    //Draw

    gStyle->SetOptFit(1);
    double X1NDC = 0.7;
    double X2NDC = 0.99;
    double Y1NDC = 0.7;
    double Y2NDC = 0.99;
    int cd_i = 0;

    TCanvas *c1 = new TCanvas("S1_analysis","S1_analysis");
    c1->Divide(3,2,0.01,0.01);
    vector<TPaveStats*> st_h1_S1(4);

    cd_i = 0;
    c1->cd(cd_i + 1);
    h1_S1_total->Draw();
    h1_S1_total->GetXaxis()->SetTitle("[PE]");
    gPad->Update();
    st_h1_S1[cd_i] = (TPaveStats*)h1_S1_total->GetListOfFunctions()->FindObject("stats");
    st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
    st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
    gPad->Modified(); gPad->Update();

    cd_i = 1;
    c1->cd(cd_i + 1);
    h1_S1_top->Draw();
    h1_S1_top->GetXaxis()->SetTitle("[PE]");
    gPad->Update();
    st_h1_S1[cd_i] = (TPaveStats*)h1_S1_top->GetListOfFunctions()->FindObject("stats");
    st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
    st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
    gPad->Modified(); gPad->Update();

    cd_i = 2;
    c1->cd(cd_i + 1);
    h1_S1_bot->Draw();
    h1_S1_bot->GetXaxis()->SetTitle("[PE]");
    gPad->Update();
    st_h1_S1[cd_i] = (TPaveStats*)h1_S1_bot->GetListOfFunctions()->FindObject("stats");
    st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
    st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
    gPad->Modified(); gPad->Update();

    cd_i = 3;
    c1->cd(cd_i + 1);
    h1_S1_f90->Draw();
    //h1_S1_f90->GetXaxis()->SetTitle("[PE]");
    gPad->Update();
    st_h1_S1[cd_i] = (TPaveStats*)h1_S1_f90->GetListOfFunctions()->FindObject("stats");
    st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
    st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
    gPad->Modified(); gPad->Update();


    cd_i = 4;
    c1->cd(cd_i + 1);
    h1_S1_TBA->Draw();
    //h1_S1_f90->GetXaxis()->SetTitle("[PE]");
    gPad->Update();
    st_h1_S1[cd_i] = (TPaveStats*)h1_S1_TBA->GetListOfFunctions()->FindObject("stats");
    st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
    st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
    gPad->Modified(); gPad->Update();

    cd_i = 5;
    c1->cd(cd_i + 1);
    h2_S1_total_event->GetXaxis()->SetTitle("event");
    h2_S1_total_event->GetYaxis()->SetTitle("S1_total");
    h2_S1_total_event->Draw("colz");
    TProfile *prof_h2_S1_total_event = h2_S1_total_event->ProfileX();
    prof_h2_S1_total_event->Draw("same");
    prof_h2_S1_total_event->SetMarkerStyle(20);
    prof_h2_S1_total_event->SetMarkerColor(kRed);



}
