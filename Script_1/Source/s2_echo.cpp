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


//int run_number = 1051;
int run_number = 976;

void s2_echo()
{
    ostringstream path_root_file;
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
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
    TH1F *h1_S3_total = new TH1F("h1_S3_total", "h1_S3_total", 600, -2, 2500);


    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

         //bool cuts2 = clusters.at(0)->charge > 300 && clusters.at(1)->charge > 300;
        //clusters.size() == 2 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.3
        if(clusters.size() == 3)
        {
            bool cut_f90 = /*clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2 &&*/ clusters.at(2)->f90 < 0.2;
            if(cut_f90 && clusters.at(0)->charge > 300 && clusters.at(1)->charge > 10000 && clusters.at(1)->charge < 20000)
            {
                h1_S3_total->Fill(clusters.at(2)->charge);
            }
        }
    }


    TCanvas *c1 = new TCanvas("S3","S3 analysis");
    c1->Divide(2,2,0.01,0.01);
    gStyle->SetOptFit(1);
    double shift = 0.55;
    double shift_x1 = -0.1;
    double shift_y1 = -0.1;
    vector<TPaveStats*> st_h1_S3(4);
    int cd_i = 0;

    c1->cd(1);
    cd_i = 0;
    h1_S3_total->Draw();
    gPad->Update();
    st_h1_S3[cd_i] = (TPaveStats*)h1_S3_total->GetListOfFunctions()->FindObject("stats");
    st_h1_S3[cd_i]->SetX1NDC(0.12+shift+shift_x1); st_h1_S3[cd_i]->SetX2NDC(0.35+shift);
    st_h1_S3[cd_i]->SetY1NDC(0.72+shift_y1); st_h1_S3[cd_i]->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();



}
