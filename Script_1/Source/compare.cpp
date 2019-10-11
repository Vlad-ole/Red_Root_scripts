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
    //vector<int> run_list{1134, 1135, 1092, 1079, 1083, 1085, 1089};

    vector<int> run_list{1093, 1097, 1098, 1099};
    vector<int> lifetimes{770, 620, 700, 870};
    //vector<double> correstions{1.0, 1.0, 1.0, 1.0};
    //vector<double> corrections_S2{12150.0/12150.0, 12150.0/16500.0 * 1.01, 12150.0/21700.0 * 0.98, 12150.0/27300.0 * 0.97}; //S2 vs Tdrift
    //vector<double> corrections_S1{645.0/645.0, 645.0/654.0, 645.0/647.0, 645.0/642.0}; //S1 vs Tdrift
    //vector<double> correstions{1/645.0, 1/654.0, 1/647.0, 1/642.0}; //S1 vs Tdrift
    //vector<double> correstions{1/100.0, 1/100.0, 1/100.0, 1/100.0}; //S1 vs Tdrift

//    vector<int> run_list{1085, 1102, 1104};
//    vector<int> lifetimes{1770, 334, 467};

    //vector<int> lifetimes;

    //vector<int> run_list{1118,1110,1106,1107,1109};


    TTree *data = NULL;
    EvRec0* evReco = NULL;

    //string plot_type = "S1";
    //string plot_type = "S2";
    //string plot_type = "S2_over_S1";
    //string plot_type = "tdrift";
    //string plot_type = "S2_total_tdrift";
    //string plot_type = "S1_total_tdrift";
    string plot_type = "TBA";


    double S1_max = 2000;
    double S2_max = /*25000*/ 45000 /*12000*/;
    double S2_S1_max = 40;

    vector<TH1F*> h1_S1_vec(run_list.size());
    vector<TH1F*> h1_Tdrift_vec(run_list.size());
    vector<TH1F*> h1_TBA_vec(run_list.size());
    vector<TH1F*> h1_S2_total_vec(run_list.size());
    vector<TH1F*> h1_S2_over_S1_vec(run_list.size());
    vector<TH2F*> h2_S2_total_tdrift_vec(run_list.size());
    vector<TH2F*> h2_S1_total_tdrift_vec(run_list.size());
    vector<TProfile*> profile_S2_total_tdrift_vec(run_list.size());
    vector<TProfile*> profile_S1_total_tdrift_vec(run_list.size());


    for(int i = 0; i < run_list.size(); i++)
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
            h1_Tdrift_vec_name << "Am GP 16V 5211/156/-744";

        if(run_list[i] == 1083)
            h1_Tdrift_vec_name << "Am GP 18V 5211/156/-744";

        if(run_list[i] == 1085)
            h1_Tdrift_vec_name << "Am GP 20V 5211/156/-744";

        if(run_list[i] == 1089)
            h1_Tdrift_vec_name << "Am GP 24V 5211/156/-744";

        if(run_list[i] == 1092)
            h1_Tdrift_vec_name << "Am GP 15V 5211/156/-744";

        if(run_list[i] == 1093)
            h1_Tdrift_vec_name << "Am GP 1.99W 5211/156/-744";

        if(run_list[i] == 1097)
            h1_Tdrift_vec_name << "Am GP 1.99W 5500/156/-744";

        if(run_list[i] == 1098)
            h1_Tdrift_vec_name << "Am GP 1.99W 6000/156/-744";

        if(run_list[i] == 1099)
            h1_Tdrift_vec_name << "Am GP 1.99W 6500/156/-744";

        if(run_list[i] == 1102)
            h1_Tdrift_vec_name << "Am GP 20V 5211/16/-744";

        if(run_list[i] == 1104)
            h1_Tdrift_vec_name << "Am GP 20V 5211/86/-744";

        if(run_list[i] == 1106)
            h1_Tdrift_vec_name << "Kr GP 20V 5211/156/-744";

        if(run_list[i] == 1107)
            h1_Tdrift_vec_name << "Kr GP 20V 5211/226/-744";

        if(run_list[i] == 1109)
            h1_Tdrift_vec_name << "Kr GP 20V 5211/296/-744";

        if(run_list[i] == 1110)
            h1_Tdrift_vec_name << "Kr GP 20V 5211/16/-744";

        if(run_list[i] == 1118)
            h1_Tdrift_vec_name << "Kr GP 20V 5211/-66/-744";

        if(run_list[i] == 1134)
            h1_Tdrift_vec_name << "Am GP 13V 5211/+86/-744";
        //h1_Tdrift_vec_name << "Am GP 13V 5211/+86/-744 1081-1085mbar";

        if(run_list[i] == 1135)
            h1_Tdrift_vec_name << "Am GP 14.5V 5211/+86/-744";
        //h1_Tdrift_vec_name << "Am GP 14.5V 5211/+86/-744 1078-1087mbar";


        h1_S2_over_S1_vec[i] = new TH1F( (h1_Tdrift_vec_name.str() + "S2_over_S1").c_str() , h1_Tdrift_vec_name.str().c_str(), 100, 0, 30);
        h1_S1_vec[i] = new TH1F( (h1_Tdrift_vec_name.str() + "S1").c_str() , h1_Tdrift_vec_name.str().c_str(), 400, 0, 1000);
        h1_Tdrift_vec[i] = new TH1F( (h1_Tdrift_vec_name.str() + "Tdrift").c_str() , h1_Tdrift_vec_name.str().c_str(), 101*4, -1, 100);
        h1_TBA_vec[i] = new TH1F( (h1_Tdrift_vec_name.str() + "TBA").c_str() , h1_Tdrift_vec_name.str().c_str(), 100, -1, 1);
        h1_S2_total_vec[i] = new TH1F( (h1_Tdrift_vec_name.str() + "S2_total").c_str() , h1_Tdrift_vec_name.str().c_str(), /*400*/200, -100, S2_max);
        h2_S2_total_tdrift_vec[i] = new TH2F( (h1_Tdrift_vec_name.str() + "S2_total_tdrift").c_str(), h1_Tdrift_vec_name.str().c_str(), /*150*/50, 0, 100, 200, 0, S2_max );
        h2_S1_total_tdrift_vec[i] = new TH2F( (h1_Tdrift_vec_name.str() + "S1_total_tdrift").c_str(), h1_Tdrift_vec_name.str().c_str(), /*150*/50, 0, 100, 200, 0, S1_max );
    }


    for(int i = 0; i < run_list.size(); i++)
    {
        int n_events_after_cuts = 0;
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

        for (int ev  = 0; ev < data->GetEntries(); ev++)
        {
            data->GetEntry(ev);
            vector<RDCluster*> clusters = evReco->GetClusters();

            if(clusters.size() == 1)
            {

            }


            if(clusters.size() == 2)
            {
                bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
                bool cut_S1_total = clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800;
                bool cut_S2_total = clusters.at(1)->charge < 30000;
                bool cental_8_sipms_x = clusters.at(1)->pos_x > 1.3 && clusters.at(1)->pos_x < 3.7 &&
                        clusters.at(1)->pos_y > 0.9 && clusters.at(1)->pos_y < 4.2;

                double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;
                bool Tdrift_30_50us = Tdrift > 30 && Tdrift < 50;

                if (cut_f90 && clusters.at(0)->rep == 1 /*&& Tdrift_30_50us*/ && cut_S1_total /*&& cut_S2_total*/ /*&& !cental_8_sipms_x*/ )
                {
                    n_events_after_cuts++;

                    h1_Tdrift_vec[i]->Fill(Tdrift);

                    h1_S1_vec[i]->Fill(clusters.at(0)->charge);
                    h1_S2_over_S1_vec[i]->Fill(clusters.at(1)->charge/clusters.at(0)->charge);


                    h2_S2_total_tdrift_vec[i]->Fill(Tdrift, clusters.at(1)->charge * corrections_S2[i]);
                    h2_S1_total_tdrift_vec[i]->Fill(Tdrift, clusters.at(0)->charge * corrections_S1[i]);

                    if(lifetimes.size() == 0)
                        h1_S2_total_vec[i]->Fill(clusters.at(1)->charge);
                    else
                       h1_S2_total_vec[i]->Fill( clusters.at(1)->charge * exp(Tdrift/lifetimes[i]) );
                }

            }
        }
        cout << "n_events_after_cuts = " << n_events_after_cuts << endl;
    }

    gStyle->SetOptFit(1);
    double X1NDC = 0.7;
    double X2NDC = 0.99;
    double Y1NDC = 0.7;
    double Y2NDC = 0.99;
    int cd_i = 0;
    vector<int> hist_color = {kBlack, kRed, kBlue, kMagenta, kGreen, kYellow - 2, kAzure + 10};

    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(1,1,0.01,0.01);
    vector<TPaveStats*> st_h1_S1(6);

    cd_i = 0;
    c1->cd(cd_i + 1);
    auto legend = new TLegend(0.7,0.13,0.9,0.43);
    for(int i = 0; i < run_list.size(); i++)
    {
        profile_S1_total_tdrift_vec[i] = h2_S1_total_tdrift_vec[i]->ProfileX();
        profile_S1_total_tdrift_vec[i]->SetMarkerStyle(20);
        profile_S1_total_tdrift_vec[i]->SetMarkerColor(hist_color[i]);
        profile_S1_total_tdrift_vec[i]->SetLineColor(hist_color[i]);
        profile_S1_total_tdrift_vec[i]->SetLineWidth(3);

        profile_S2_total_tdrift_vec[i] = h2_S2_total_tdrift_vec[i]->ProfileX();
        profile_S2_total_tdrift_vec[i]->SetMarkerStyle(20);
        profile_S2_total_tdrift_vec[i]->SetMarkerColor(hist_color[i]);
        profile_S2_total_tdrift_vec[i]->SetLineColor(hist_color[i]);
        profile_S2_total_tdrift_vec[i]->SetLineWidth(3);


        if(i == 0)
        {
            if (plot_type == "tdrift")
            {
                h1_Tdrift_vec[i]->Draw("HIST");
                h1_Tdrift_vec[i]->SetStats(0);
            }

            if (plot_type == "S1")
            {
                h1_S1_vec[i]->Draw("HIST");
                h1_S1_vec[i]->SetStats(0);
            }


            if(plot_type == "S2")
            {
                h1_S2_total_vec[i]->Draw("HIST");
                h1_S2_total_vec[i]->SetStats(0);
            }

            if(plot_type == "S2_over_S1")
            {
                h1_S2_over_S1_vec[i]->Draw("HIST");
                h1_S2_over_S1_vec[i]->SetStats(0);
            }


            if(plot_type == "S2_total_tdrift")
            {
                profile_S2_total_tdrift_vec[i]->Draw("HIST pL");
            }
            if(plot_type == "S1_total_tdrift")
            {
                profile_S1_total_tdrift_vec[i]->Draw("HIST pL");
            }
        }
        else
        {
           if (plot_type == "tdrift") h1_Tdrift_vec[i]->Draw("HIST SAME");
           if(plot_type == "S2") h1_S2_total_vec[i]->Draw("HIST SAME");
           if(plot_type == "S2_total_tdrift") profile_S2_total_tdrift_vec[i]->Draw("same HIST pL");
           if(plot_type == "S1_total_tdrift") profile_S1_total_tdrift_vec[i]->Draw("same HIST pL");
           if(plot_type == "S1") h1_S1_vec[i]->Draw("HIST SAME");
           if(plot_type == "S2_over_S1") h1_S2_over_S1_vec[i]->Draw("HIST SAME");
        }



        if (plot_type == "tdrift")
        {
            h1_Tdrift_vec[i]->SetLineColor(hist_color[i]);
            h1_Tdrift_vec[i]->Scale(1/h1_Tdrift_vec[i]->Integral());
            legend->AddEntry(h1_Tdrift_vec[i],h1_Tdrift_vec[i]->GetTitle(),"lp");
        }

        if(plot_type == "S1")
        {
            h1_S1_vec[i]->SetLineColor(hist_color[i]);
            h1_S1_vec[i]->Scale(1/h1_S1_vec[i]->Integral());
            legend->AddEntry(h1_S1_vec[i],h1_S1_vec[i]->GetTitle(),"lp");
        }

        if(plot_type == "S2")
        {
            h1_S2_total_vec[i]->SetLineColor(hist_color[i]);
            h1_S2_total_vec[i]->Scale(1/h1_S2_total_vec[i]->Integral());
            legend->AddEntry(h1_S2_total_vec[i],h1_S2_total_vec[i]->GetTitle(),"lp");
        }

        if(plot_type == "S2_over_S1")
        {
            h1_S2_over_S1_vec[i]->SetLineColor(hist_color[i]);
            h1_S2_over_S1_vec[i]->Scale(1/h1_S2_over_S1_vec[i]->Integral());
            legend->AddEntry(h1_S2_over_S1_vec[i],h1_S2_over_S1_vec[i]->GetTitle(),"lp");
        }

        if(plot_type == "S2_total_tdrift")
        {
            legend->AddEntry(profile_S2_total_tdrift_vec[i],profile_S2_total_tdrift_vec[i]->GetTitle(),"lp");
        }
        if(plot_type == "S1_total_tdrift")
        {
            legend->AddEntry(profile_S1_total_tdrift_vec[i],profile_S1_total_tdrift_vec[i]->GetTitle(),"lp");
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
    if(plot_type == "S1")
    {
        h1_S1_vec[0]->GetXaxis()->SetTitle("S1 [pe]");
        h1_S1_vec[0]->SetTitle("");
    }

    if(plot_type == "S2_over_S1")
    {
        h1_S2_over_S1_vec[0]->GetXaxis()->SetTitle("S2/S1");
        h1_S2_over_S1_vec[0]->SetTitle("");
    }

    if(plot_type == "S2_total_tdrift")
    {
        profile_S2_total_tdrift_vec[0]->GetXaxis()->SetTitle("Tdrift [us]");
        //profile_S2_total_tdrift_vec[0]->GetYaxis()->SetTitle("S2_total [PE]");
        profile_S2_total_tdrift_vec[0]->GetYaxis()->SetTitle("S2_total [Arb. units]");
        profile_S2_total_tdrift_vec[0]->SetTitle("");
        gPad->Update();
        profile_S2_total_tdrift_vec[0]->SetStats(0);
        profile_S2_total_tdrift_vec[0]->GetXaxis()->SetRangeUser(15,60);
        profile_S2_total_tdrift_vec[0]->GetYaxis()->SetRangeUser(10000,12000);
        gPad->Update(); gPad->Modified();
    }
    if(plot_type == "S1_total_tdrift")
    {
        profile_S1_total_tdrift_vec[0]->GetXaxis()->SetTitle("Tdrift [us]");
        profile_S1_total_tdrift_vec[0]->GetYaxis()->SetTitle("S1_total [PE]");
        profile_S1_total_tdrift_vec[0]->SetTitle("");
        gPad->Update();
        profile_S1_total_tdrift_vec[0]->SetStats(0);
        gPad->Update(); gPad->Modified();
    }


}
