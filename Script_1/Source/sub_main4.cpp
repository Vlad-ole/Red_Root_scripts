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


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void sub_main4()
{
    int run_number = 1099;
    double lifetime = 870;

    //Am
    double S1_max = 2000;
    double S2_max = /*25000*/ 45000;
    double S2_S1_max = /*40*/ 40;
    double S1_low_cut = 400;
    double S1_high_cut = 800;

    //main code
    ostringstream path_root_file;
    if(run_number >= 743 && run_number <= 1034)
        path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    else if(run_number >= 1040)
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

    TH1F *h1_S2_total = new TH1F("h1_S2_total", "h1_S2_total", 400, -100, S2_max);
    TH1F *h1_S2_total_lifetime_corr = new TH1F("h1_S2_total_corr", "h1_S2_total", 400, -100, S2_max);

    double Tdrift = -1;

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        //cout << "line 106" << endl;
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        if(nc == 1)
        {

        }

        if(nc == 2)
        {
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = clusters.at(0)->charge > S1_low_cut && clusters.at(0)->charge < S1_high_cut;
            Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

            if(cut_f90 && cut_S1_total && clusters.at(0)->rep == 1)
            {
                h1_S2_total->Fill(clusters.at(1)->charge);
                h1_S2_total_lifetime_corr->Fill(clusters.at(1)->charge * exp(Tdrift/lifetime));
            }
        }

    }

    TCanvas *c1 = new TCanvas("c1","Analysis");

    //v1
//    h1_S2_total->Draw();
//    h1_S2_total->GetXaxis()->SetTitle("[PE]");
//    h1_S2_total_lifetime_corr->Draw("same");
//    h1_S2_total_lifetime_corr->SetLineColor(kBlue);

    //v2
    h1_S2_total_lifetime_corr->Draw();
    h1_S2_total_lifetime_corr->SetTitle("run 1099");
    h1_S2_total_lifetime_corr->GetXaxis()->SetTitle("S2 lifetime corrected [PE]");
    h1_S2_total_lifetime_corr->SetLineColor(kBlue);

    gPad->Update(); gPad->Modified();
    gStyle->SetOptFit(1);



}
