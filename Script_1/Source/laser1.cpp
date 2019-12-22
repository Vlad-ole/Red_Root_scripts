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
#include "TLatex.h"

//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

using namespace std;
using namespace TMath;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void laser1()
{
    int run_number = 1094;
    ostringstream path_root_file;
    if(run_number >= 743 && run_number <= 1034)
        path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    else if(run_number >= 1040)
        path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";


    TFile *f = new TFile(path_root_file.str().c_str(), "read"); // change this path in order to make LNS SERs
    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);


    TH1F *h1_top = new TH1F("h1_top", "h1_top", 1000, -5000, 100000);
    TH1F *h1_bottom = new TH1F("h1_bottom", "h1_bottom", 1000, -5000, 100000);

    for (int ev = 0; ev < data->GetEntries(); ev++)
        {
        data->GetEntry(ev);
        //vector<RDCluster*> clusters = evReco->GetClusters();

        double charge_bottom = 0;
        for (int i = 0; i < 4; i++)
        {
            charge_bottom += evReco->GetCharge()[i];
        }
        h1_bottom->Fill(charge_bottom);

        double charge_top = 0;
        for (int i = 4; i < 28; i++)
        {
            charge_top += evReco->GetCharge()[i];
        }
        h1_top->Fill(charge_top);

    }


    //
    run_number = 1100;
    ostringstream path_root_file2;
    if(run_number >= 743 && run_number <= 1034)
        path_root_file2 << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    else if(run_number >= 1040)
        path_root_file2 << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";

   f = new TFile(path_root_file2.str().c_str(), "read"); // change this path in order to make LNS SERs
   data = (TTree*)f->Get("reco");
   data->SetBranchAddress("recoevent",&evReco);

   TH1F *h1_top_2 = new TH1F("h1_top_2", "h1_top_2", 1000, -5000, 100000);
   TH1F *h1_bottom_2 = new TH1F("h1_bottom_2", "h1_bottom_2", 1000, -5000, 100000);

   for (int ev = 0; ev < data->GetEntries(); ev++)
   {
       data->GetEntry(ev);

       double charge_bottom = 0;
       for (int i = 0; i < 4; i++)
       {
           charge_bottom += evReco->GetCharge()[i];
       }
       h1_bottom_2->Fill(charge_bottom);

       double charge_top = 0;
       for (int i = 4; i < 28; i++)
       {
           charge_top += evReco->GetCharge()[i];
       }
       h1_top_2->Fill(charge_top);
   }

   h1_top->Scale(1/h1_top->Integral());
   h1_top_2->Scale(1/h1_top_2->Integral());
   h1_bottom->Scale(1/h1_bottom->Integral());
   h1_bottom_2->Scale(1/h1_bottom_2->Integral());

    TCanvas *c1 = new TCanvas("c1", "c1");
    c1->Divide(2,2);

    c1->cd(1);
    h1_top->Draw();
    h1_top->SetStats(0);
    h1_top->SetName("h1_top");
    h1_top->GetXaxis()->SetTitle("Charge (ADC*Sample)");
    h1_top_2->Draw("same");
    h1_top_2->SetLineColor(kRed);
    h1_top_2->SetName("h1_top_2");

    TLegend *lcd1= new TLegend(0.4,0.5,0.9,0.8);
    lcd1->AddEntry("h1_top","Laser run 1094: 5211/156/-744","l");
    lcd1->AddEntry("h1_top_2","Laser run 1100: 6500/156/-744","l");
    lcd1->Draw();

    c1->cd(2);
    h1_bottom->Draw();
    h1_bottom->SetStats(0);
    h1_bottom->SetName("h1_bottom");
    h1_bottom->GetXaxis()->SetTitle("Charge (ADC*Sample)");
    h1_bottom_2->Draw("same");
    h1_bottom_2->SetName("h1_bottom_2");
    h1_bottom_2->SetLineColor(kRed);

    TLegend *lcd2= new TLegend(0.4,0.5,0.9,0.8);
    lcd2->AddEntry("h1_bottom","Laser run 1094: 5211/156/-744","l");
    lcd2->AddEntry("h1_bottom_2","Laser run 1100: 6500/156/-744","l");
    lcd2->Draw();

    COUT(h1_top->GetMean());
    COUT(h1_top_2->GetMean());
    COUT(h1_bottom->GetMean());
    COUT(h1_bottom_2->GetMean());

    COUT(h1_top->GetMean()/h1_top_2->GetMean());
    COUT(h1_bottom->GetMean()/h1_bottom_2->GetMean());
}
