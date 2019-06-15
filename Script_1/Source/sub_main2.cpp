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

void sub_main2()
{
    int run_number = 1033;

    //main code
    ostringstream path_root_file;
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

    TH1F *h1_S1_tot_1cl = new TH1F("h1_S1_tot_1cl", "h1_S1_tot_1cl", 400, -100, 3000);
    TH1F *h1_S1_f90_1cl = new TH1F("h1_S1_f90_1cl", "h1_S1_f90_1cl", 400, -0.2, 1);//f90
    TH1F *h1_S1_f90_2cl = new TH1F("h1_S1_f90_2cl", "h1_S1_f90_2cl", 400, -0.2, 1);//f90
    TH1F *h1_S2_f90_2cl = new TH1F("h1_S2_f90_2cl", "h1_S2_f90_2cl", 400, -0.2, 1);//f90
    TH2F *h2_S2_S1_tdrift = new TH2F("h2_S2_S1_tdrift", "h2_S2_S1_tdrift", 150, 0, 100, 200, 0, 10);

    vector<double> Tdrift_vec;
    vector<double> S2_S1_vec;
    vector<double> S1_f90_2cl_vec;
    vector<double> S2_f90_2cl_vec;
    double Tdrift = -1;

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        //cout << "line 106" << endl;
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        if(nc == 1)
        {
            h1_S1_f90_1cl->Fill(clusters.at(0)->f90);
            h1_S1_tot_1cl->Fill(clusters.at(0)->f90);
        }

        if(nc == 2 /*&& clusters.at(0)->f90 > 0.3*/ /*&& clusters.at(1)->f90 < 0.1*/ /* && clusters.at(0)->f90 > 0.3 && clusters.at(0)->f90 < 0.4*/)
        {
            h1_S1_f90_2cl->Fill(clusters.at(0)->f90);
            h1_S2_f90_2cl->Fill(clusters.at(1)->f90);
            S1_f90_2cl_vec.push_back(clusters.at(0)->f90);
            S2_f90_2cl_vec.push_back(clusters.at(1)->f90);

            Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;
            Tdrift_vec.push_back(Tdrift);
            S2_S1_vec.push_back(clusters.at(1)->charge/clusters.at(0)->charge);
            h2_S2_S1_tdrift->Fill(Tdrift, clusters.at(1)->charge/clusters.at(0)->charge);
        }

    }

    TCanvas *c1 = new TCanvas("c1","Analysis");
    c1->Divide(2,2,0.01,0.01);
    c1->cd(1);
    h1_S1_f90_1cl->Draw();
    h1_S1_f90_1cl->Scale(1/h1_S1_f90_1cl->Integral());
    h1_S1_f90_1cl->SetTitle("");

    h1_S1_f90_2cl->Draw("same");
    h1_S1_f90_2cl->Scale(1/h1_S1_f90_2cl->Integral());
    h1_S1_f90_2cl->SetLineColor(kRed);

    c1->cd(3);
    h1_S2_f90_2cl->Draw();
    h1_S2_f90_2cl->Scale(1/h1_S2_f90_2cl->Integral());

//    c1->cd(2);
//    h2_S2_S1_tdrift->Draw();
//    h2_S2_S1_tdrift->SetMarkerStyle(20);

    c1->cd(2);
    TGraph *gr_S2_f90_2cl__S2_f90_2cl = new TGraph(S1_f90_2cl_vec.size(), &S1_f90_2cl_vec[0], &S2_f90_2cl_vec[0]);
    gr_S2_f90_2cl__S2_f90_2cl->Draw("AP");
    gr_S2_f90_2cl__S2_f90_2cl->SetMarkerStyle(20);
    gr_S2_f90_2cl__S2_f90_2cl->GetXaxis()->SetTitle("S1_f90");
    gr_S2_f90_2cl__S2_f90_2cl->GetYaxis()->SetTitle("S2_f90");

    c1->cd(4);
    gPad->SetLogy();
    TGraph *gr_S2_S1_Tdrift = new TGraph(Tdrift_vec.size(), &Tdrift_vec[0], &S2_S1_vec[0]);
    gr_S2_S1_Tdrift->Draw("AP");
    gr_S2_S1_Tdrift->SetMarkerStyle(20);
    gr_S2_S1_Tdrift->GetXaxis()->SetTitle("TDrift [us]");


}
