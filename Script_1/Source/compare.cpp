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



void compare()
{
    //vector<int> run_list{1051, 1057, 1059, 1066};
    //vector<int> run_list{1079, 1083, 1085, 1089, 1092};
    //vector<int> run_list{1093, 1097, 1098, 1099};
    vector<int> run_list{1093};
    TTree *data = NULL;
    EvRec0* evReco = NULL;

    string plot_type = "S2";
    //string plot_type = "tdrift";

    double S1_max = 2000;
    double S2_max = /*25000*/ 45000;
    double S2_S1_max = 40;

    vector<TH1F*> h1_Tdrift_vec(run_list.size());
    vector<TH1F*> h1_S1_TBA_vec(run_list.size());
    vector<TH1F*> h1_S2_total_vec(run_list.size());

    for(int i = 0; i< run_list.size(); i++)
    {
        ostringstream h1_Tdrift_vec_name;
        h1_Tdrift_vec_name << "run_" << run_list[i] << "  ";

        if(run_list[i] == 1051)
            h1_Tdrift_vec_name << "Am GP 1.4W 3780/85/-815";

        if(run_list[i] == 1057)
            h1_Tdrift_vec_name << "Bkg GP 1.4W 3780/85/-815";

        if(run_list[i] == 1059)
            h1_Tdrift_vec_name << "Bkg GP 1.4W 5211/156/-744";

        if(run_list[i] == 1066)
            h1_Tdrift_vec_name << "Am GP 1.4W 5211/156/-744";

        if(run_list[i] == 1079)
            h1_Tdrift_vec_name << "Am GP 0.939W 5211/156/-744";

        if(run_list[i] == 1083)
            h1_Tdrift_vec_name << "Am GP 1.17W 5211/156/-744";

        if(run_list[i] == 1085)
            h1_Tdrift_vec_name << "Am GP 1.42W 5211/156/-744";

        if(run_list[i] == 1089)
            h1_Tdrift_vec_name << "Am GP 1.99W 5211/156/-744";

        if(run_list[i] == 1092)
            h1_Tdrift_vec_name << "Am GP 0.830W 5211/156/-744";

        if(run_list[i] == 1093)
            h1_Tdrift_vec_name << "Am GP 1.99W 5211/156/-744";

        if(run_list[i] == 1097)
            h1_Tdrift_vec_name << "Am GP 1.99W 5500/156/-744";

        if(run_list[i] == 1098)
            h1_Tdrift_vec_name << "Am GP 1.99W 6000/156/-744";

        if(run_list[i] == 1099)
            h1_Tdrift_vec_name << "Am GP 1.99W 6500/156/-744";

        h1_Tdrift_vec[i] = new TH1F( (h1_Tdrift_vec_name.str() + "Tdrift").c_str() , h1_Tdrift_vec_name.str().c_str(), 101, -1, 100);
        h1_S1_TBA_vec[i] =new TH1F( (h1_Tdrift_vec_name.str() + "TBA").c_str() , h1_Tdrift_vec_name.str().c_str(), 101, -1, 100);
        h1_S2_total_vec[i] =new TH1F( (h1_Tdrift_vec_name.str() + "S2_total").c_str() , h1_Tdrift_vec_name.str().c_str(), 400, -100, S2_max);
    }

    for(int i = 0; i < run_list.size(); i++)
    {
        ostringstream path_root_file;
        path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_list[i] << ".root";
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

        data = (TTree*)f->Get("reco");
        evReco = new EvRec0();
        data->SetBranchAddress("recoevent",&evReco);

        for (int ev = 0; ev < data->GetEntries(); ev++)
        {
            data->GetEntry(ev);
            vector<RDCluster*> clusters = evReco->GetClusters();


            if(clusters.size() == 2)
            {
                bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
                bool cut_S1_total = clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800;
                bool cut_S2_total = clusters.at(1)->charge < 30000;

                if (cut_f90 /*&& clusters.at(0)->rep == 1*/ && cut_S1_total /*&& cut_S2_total*/)
                {
                    double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;
                    h1_Tdrift_vec[i]->Fill(Tdrift);
                    h1_S2_total_vec[i]->Fill(clusters.at(1)->charge);
                }

            }
        }
    }

    gStyle->SetOptFit(1);
    double X1NDC = 0.7;
    double X2NDC = 0.99;
    double Y1NDC = 0.7;
    double Y2NDC = 0.99;
    int cd_i = 0;
    vector<int> hist_color = {kBlack, kRed, kBlue, kMagenta, kGreen};

    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(1,1,0.01,0.01);
    vector<TPaveStats*> st_h1_S1(6);

    cd_i = 0;
    c1->cd(cd_i + 1);
    auto legend = new TLegend(0.7,0.13,0.9,0.43);
    for(int i = 0; i < run_list.size(); i++)
    {
        if(i == 0)
        {
            if (plot_type == "tdrift")
            {
                h1_Tdrift_vec[i]->Draw("HIST");
                h1_Tdrift_vec[i]->SetStats(0);
            }

            if(plot_type == "S2")
            {
                h1_S2_total_vec[i]->Draw("HIST");
                h1_S2_total_vec[i]->SetStats(0);
            }
        }
        else
        {
           if (plot_type == "tdrift") h1_Tdrift_vec[i]->Draw("HIST SAME");
           if(plot_type == "S2") h1_S2_total_vec[i]->Draw("HIST SAME");
        }

        if (plot_type == "tdrift")
        {
            h1_Tdrift_vec[i]->SetLineColor(hist_color[i]);
            h1_Tdrift_vec[i]->Scale(1/h1_Tdrift_vec[i]->Integral());
            legend->AddEntry(h1_Tdrift_vec[i],h1_Tdrift_vec[i]->GetTitle(),"lp");
        }
        if(plot_type == "S2")
        {
            h1_S2_total_vec[i]->SetLineColor(hist_color[i]);
            h1_S2_total_vec[i]->Scale(1/h1_S2_total_vec[i]->Integral());
            legend->AddEntry(h1_S2_total_vec[i],h1_S2_total_vec[i]->GetTitle(),"lp");
        }

    }
    legend->Draw();

    if (plot_type == "tdrift")
    {
        h1_Tdrift_vec[0]->GetXaxis()->SetTitle("Tdrift [us]");
        h1_Tdrift_vec[0]->SetTitle("");
    }
    if(plot_type == "S2")
    {
        h1_S2_total_vec[0]->GetXaxis()->SetTitle("S2 [pe]");
        h1_S2_total_vec[0]->SetTitle("");

    }


}
