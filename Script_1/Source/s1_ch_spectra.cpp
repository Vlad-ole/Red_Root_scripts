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


int run_number = 1042;

void s1_ch_spectra()
{
    ostringstream path_root_file;
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";
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


    //hists
    TH1F *h1_S1_total = new TH1F("h1_S1_total", "h1_S1_total", 120, -2, 100);
    vector<TH1F*> h1_S1_top_vec(24);
    vector<TH1F*> h1_S1_bottom_vec(4);
    for(int ch = 0; ch < 24; ch++)
    {
        ostringstream h1_S1_top_name;
        h1_S1_top_name << "h1_S1_top_ch" << ch;
        h1_S1_top_vec[ch] = new TH1F(h1_S1_top_name.str().c_str(), h1_S1_top_name.str().c_str(), 120, -2, 100);
        h1_S1_top_vec[ch]->GetXaxis()->SetTitle("S1 for top ch [PE]");
    }
    for(int ch = 0; ch < 4; ch++)
    {
        ostringstream h1_S1_bottom_name;
        h1_S1_bottom_name << "h1_S1_bottom_ch" << ch;
        h1_S1_bottom_vec[ch] = new TH1F(h1_S1_bottom_name.str().c_str(), h1_S1_bottom_name.str().c_str(), 120, -2, 250);
        h1_S1_bottom_vec[ch]->GetXaxis()->SetTitle("S1 for bottom ch [PE]");
    }





    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        //size_t nc = clusters.size();

        if(clusters.size() == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.3)
        {
            //cout << h1_S1_top_vec.size() << endl;

            for(int ch = 0; ch < 24; ch++)
            {
                h1_S1_top_vec[ch]->Fill(clusters.at(0)->charge_top[ch]);
            }
            for(int ch = 0; ch < 4; ch++)
            {
                h1_S1_bottom_vec[ch]->Fill(clusters.at(0)->charge_bottom[ch]);
            }
        }
    }

    //map<string,int>

    TCanvas *c1 = new TCanvas("S1_top","clusters.at(0)->charge_top[ch]");
    c1->Divide(4,3,0.01,0.01);
    gStyle->SetOptFit(1);
    double shift = 0.55;
    double shift_x1 = -0.1;
    double shift_y1 = -0.1;
    int ch;
    int cd_i = 0;
    vector<TPaveStats*> st_h1_S1_top(24);

    for (int row_i = 0; row_i < 6; row_i++)
    {
        for (int col_i = 0; col_i < 4; col_i++)
        {
            cd_i++;
            c1->cd(cd_i);
            h1_S1_top_vec[cd_i-1]->Draw();
            gPad->Update();
            st_h1_S1_top[cd_i-1] = (TPaveStats*)h1_S1_top_vec[cd_i-1]->GetListOfFunctions()->FindObject("stats");
            st_h1_S1_top[cd_i-1]->SetX1NDC(0.12+shift+shift_x1); st_h1_S1_top[cd_i-1]->SetX2NDC(0.35+shift);
            st_h1_S1_top[cd_i-1]->SetY1NDC(0.72+shift_y1); st_h1_S1_top[cd_i-1]->SetY2NDC(0.89);
            gPad->Modified(); gPad->Update();
        }
    }


    TCanvas *c2 = new TCanvas("S1_bottom","clusters.at(0)->charge_bottom[ch]");
    c2->Divide(2,2,0.01,0.01);
    cd_i = 0;
    vector<TPaveStats*> st_h1_S1_bottom(4);
    for (int col_i = 0; col_i < 4; col_i++)
    {
        cd_i++;
        c2->cd(cd_i);
        h1_S1_bottom_vec[cd_i-1]->Draw();
        gPad->Update();
        st_h1_S1_bottom[cd_i-1] = (TPaveStats*)h1_S1_bottom_vec[cd_i-1]->GetListOfFunctions()->FindObject("stats");
        st_h1_S1_bottom[cd_i-1]->SetX1NDC(0.12+shift+shift_x1); st_h1_S1_bottom[cd_i-1]->SetX2NDC(0.35+shift);
        st_h1_S1_bottom[cd_i-1]->SetY1NDC(0.72+shift_y1); st_h1_S1_bottom[cd_i-1]->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();
    }




}
