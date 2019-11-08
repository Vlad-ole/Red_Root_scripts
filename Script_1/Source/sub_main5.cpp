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

using namespace std;

void sub_main5()
{
    int run_number = 1104;

    ostringstream run_number_osst;
    run_number_osst << "run " << run_number;
    string run_number_str = run_number_osst.str();

    //Am arb. 5211, 86, -744
    double S1_max = 2000;
    double S2_max = 30000;
    double S2_S1_max = 40;
    double S1_low_cut = 400;
    double S1_high_cut = 800;
    double Tdrift_max = 100;
    double tdrift_coeff = 4;

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

    TH1F *h1_Tdrift = new TH1F("h1 Tdrift", "h1 Tdrift", (Tdrift_max + 1.0)*tdrift_coeff, -1, Tdrift_max);
    TH1F *h1_Tdrift_border = new TH1F("h1_Tdrift_border", "h1_Tdrift_border", (Tdrift_max + 1.0)*tdrift_coeff, -1, Tdrift_max);

    double Tdrift = -1;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        if(clusters.size() == 2)
        {
            double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

            bool cut_S1_total = clusters.at(0)->charge > S1_low_cut && clusters.at(0)->charge < S1_high_cut;
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cental_8_sipms_x = clusters.at(1)->pos_x > 1.3 && clusters.at(1)->pos_x < 3.7 &&
                    clusters.at(1)->pos_y > 0.9 && clusters.at(1)->pos_y < 4.2;

            if(cut_f90 && cut_S1_total /*&& clusters.at(0)->rep == 1*/ && cental_8_sipms_x)
            {
                h1_Tdrift->Fill(Tdrift);
            }

            if(cut_f90 && cut_S1_total /*&& clusters.at(0)->rep == 1*/ && !cental_8_sipms_x)
            {
                h1_Tdrift_border->Fill(Tdrift);
            }

        }

    }

    cout << "Bin width [us] = " << h1_Tdrift->GetBinCenter(1) - h1_Tdrift->GetBinCenter(0) << endl;

    h1_Tdrift->Scale(1/h1_Tdrift->Integral());
    h1_Tdrift_border->Scale(1/h1_Tdrift_border->Integral());

    h1_Tdrift->Draw("hist");
    h1_Tdrift->GetXaxis()->SetTitle("Drift time [us]");
    h1_Tdrift->GetYaxis()->SetTitle("Counts / (0.25 us)");
    h1_Tdrift->GetXaxis()->SetRangeUser(0, 70);
    h1_Tdrift->SetLineColor(kBlue);
    h1_Tdrift->SetStats(0);

    h1_Tdrift_border->Draw("same hist");
    h1_Tdrift_border->SetLineColor(kRed);
    h1_Tdrift_border->SetStats(0);

}



