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
    int run_number = 1195;
    double lifetime = 870;
    //vector<int> top_ch_list_v0{25, 26, 20, 27, 23, 24, 22, 21, 18, 19, 17, 16, 14, 15, 11, 13, 9, 10, 8, 12, 4, 5, 6, 7};
    vector<int> top_ch_list_v0{4, 5, 6, 7, 9, 10, 8, 12, 14, 15, 11, 13,
                               18, 19, 17, 16, 23, 24, 22, 21, 25, 26, 20, 27};
    vector<string> top_ch_list_str{"A1", "A2", "A3", "A4", "B1", "B2", "A5", "B4", "C1", "C2", "B3", "B5",
                                   "C5", "D1", "C4", "C3", "D5", "E2", "D4", "D3", "E3", "E4", "D2", "E5"};
    vector<int> top_ch_list(24);
    for(int i = 0; i < 24; i++)
    {
        top_ch_list[i] = top_ch_list_v0[i] - 4;
        //cout << i << "\t" << top_ch_list_str[i] << "\t" << top_ch_list[i] << endl;
    }

    vector<int> bottom_ch_list{0, 1, 3, 2};
    vector<string> bottom_ch_list_str{"F2","F3","F5", "F4"};



    cout <<  "top_ch_list.size() = " << top_ch_list.size() << endl;

    ostringstream run_number_osst;
    run_number_osst << "run " << run_number;
    string run_number_str = run_number_osst.str();

    //Am
    double S1_max = 2000;
    double S2_max = /*25000*/ 45000;
    double S2_S1_max = /*40*/ 40;
    double S1_low_cut = /*400*/ 400;
    double S1_high_cut = /*800*/ 1000;

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

    //s1
    TH1F *h1_S1_top_1cl = new TH1F("h1_S1_top_1cl", "h1_S1_top_1cl", 500, -50, 2000);
    TH1F *h1_S1_bottom_1cl = new TH1F("h1_S1_bottom_1cl", "h1_S1_bottom_1cl", 500, -50, 2000);
    TH1F *h1_S1_tot_1cl = new TH1F("h1_S1_tot_1cl", "h1_S1_tot_1cl", 800, -100, 3000);
    TH1F *h1_S1_f90_1cl = new TH1F("h1_S1_f90_1cl", "h1_S1_f90_1cl", 400, -0.2, 1);

    //S1 uniformity
    TH2F* h2_S1_top_ch_avr = new TH2F("h2_S1_top_ch_avr","h2_S1_top_ch_avr",4,0,5,6,0,5);
    vector<TH1F*> h1_S1_top_ch;
    for(int ch = 0; ch < 24; ch++)
    {
        ostringstream oss;
        oss << "h1_S1_top_ch" << ch << " " << top_ch_list_str[ch];
        h1_S1_top_ch.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 450, -50, 400) );
    }
    vector<double> S1_top_ch_avr_v;
    S1_top_ch_avr_v.resize(24);

    TH2F* h2_S1_bottom_ch_avr = new TH2F("h2_S1_bottom_ch_avr","h2_S1_bottom_ch_avr",2,0,5,2,0,5);
    vector<TH1F*> h1_S1_bottom_ch;
    for(int ch = 0; ch < 4; ch++)
    {
        ostringstream oss;
        oss << "h1_S1_bottom_ch" << ch << " " << bottom_ch_list_str[ch];
        h1_S1_bottom_ch.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 450, -50, 400) );
    }
    vector<double> S1_bottom_ch_avr_v;
    S1_bottom_ch_avr_v.resize(24);


    //s2
    TH1F *h1_S2_total = new TH1F("h1_S2_total", "h1_S2_total", 400, -100, S2_max);
    TH1F *h1_S2_total_lifetime_corr = new TH1F("h1_S2_total_corr", "h1_S2_total", 400, -100, S2_max);

    double Tdrift = -1;

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        //cout << "line 106" << endl;
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        bool cut_S1_total = false;
        if(nc > 0)
        cut_S1_total = clusters.at(0)->charge > S1_low_cut && clusters.at(0)->charge < S1_high_cut;

//        if(ev == 1 && nc == 1)
//        {
//            COUT(clusters.at(0)->charge_top.size());
//            for(int j = 0; j < 24; j++)
//                cout << j << "\t" << clusters.at(0)->charge_top[j] << endl;
//        }

        if(nc == 1)
        {
            if(true /*&& cut_S1_total*/ && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35)
            {
                h1_S1_tot_1cl->Fill(clusters.at(0)->charge);
                h1_S1_top_1cl->Fill(clusters.at(0)->tot_charge_top);
                h1_S1_bottom_1cl->Fill(clusters.at(0)->tot_charge_bottom);
                h1_S1_f90_1cl->Fill(clusters.at(0)->f90);

                //COUT(clusters.at(0)->charge_top.size());

                for(int ch = 0; ch < 24; ch++)
                {
                   h1_S1_top_ch[top_ch_list[ch]]->Fill(clusters.at(0)->charge_top[top_ch_list[ch]]);
                   // h1_S1_top_ch[ch]->Fill(12);
                }

                for(int ch = 0; ch < 4; ch++)
                {
                   h1_S1_bottom_ch[bottom_ch_list[ch]]->Fill(clusters.at(0)->charge_bottom[bottom_ch_list[ch]]);
                   // h1_S1_top_ch[ch]->Fill(12);
                }


            }
        }

        if(nc == 2)
        {
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;

            Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

            if(cut_f90 && cut_S1_total && clusters.at(0)->rep == 1)
            {
                h1_S2_total->Fill(clusters.at(1)->charge);
                h1_S2_total_lifetime_corr->Fill(clusters.at(1)->charge * exp(Tdrift/lifetime));
            }
        }

    }

    TCanvas *c1 = new TCanvas("c1","Analysis");
    c1->Divide(2,2,0.01,0.01);
    gStyle->SetOptFit(1);


    //v1
//    h1_S2_total->Draw();
//    h1_S2_total->GetXaxis()->SetTitle("[PE]");
//    h1_S2_total_lifetime_corr->Draw("same");
//    h1_S2_total_lifetime_corr->SetLineColor(kBlue);

    //v2
//    h1_S2_total_lifetime_corr->Draw();
//    h1_S2_total_lifetime_corr->SetTitle("run 1099");
//    h1_S2_total_lifetime_corr->GetXaxis()->SetTitle("S2 lifetime corrected [PE]");
//    h1_S2_total_lifetime_corr->SetLineColor(kBlue);

    //v3
    c1->cd(1);
    h1_S1_tot_1cl->Draw();
    h1_S1_tot_1cl->SetTitle(run_number_str.c_str());
    h1_S1_tot_1cl->GetXaxis()->SetTitle("S1 [PE]");
    h1_S1_tot_1cl->SetLineColor(kBlack);

    c1->cd(2);
    h1_S1_f90_1cl->Draw();
    h1_S1_f90_1cl->SetTitle(run_number_str.c_str());
    h1_S1_f90_1cl->GetXaxis()->SetTitle("f90(S1)");
    h1_S1_f90_1cl->SetLineColor(kBlack);

    c1->cd(3);
    h1_S1_top_1cl->Draw();
    h1_S1_top_1cl->SetTitle(run_number_str.c_str());
    h1_S1_top_1cl->GetXaxis()->SetTitle("S1 top [PE]");
    h1_S1_top_1cl->SetLineColor(kBlack);

    c1->cd(4);
    h1_S1_bottom_1cl->Draw();
    h1_S1_bottom_1cl->SetTitle(run_number_str.c_str());
    h1_S1_bottom_1cl->GetXaxis()->SetTitle("S1 bottom [PE]");
    h1_S1_bottom_1cl->SetLineColor(kBlack);

    TCanvas *c2 = new TCanvas("c2","p2");
    c2->Divide(4,3,0.01,0.01);
    for(int i = 0; i < 12; i++)
    {
        c2->cd(i+1);
        h1_S1_top_ch[i]->Draw();
        gPad->SetLogy();
    }

    TCanvas *c3 = new TCanvas("c3","p3");
    c3->Divide(4,3,0.01,0.01);
    for(int i = 0; i < 12; i++)
    {
        c3->cd(i+1);
        h1_S1_top_ch[i+12]->Draw();
        gPad->SetLogy();
    }

//    for(int i = 0; i < 24; i++)
//    {
//        S1_top_ch_avr_v[i] = h1_S1_top_ch[i]->GetMean();
//    }

    TCanvas *c4 = new TCanvas("c4","p4");
    double S1_mean = 0;
    int n_ch = 0;

    const Int_t NRGBs = 6;
    const Int_t NCont = 999;

    Double_t stops[NRGBs] = { 0.00, 0.2, 0.4, 0.6, 0.8, 1.00 };
    Double_t red[NRGBs]   = { 0.99, 0.44, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.66, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.99, 0.72, 1.00, 0.12, 0.00, 0.00 };

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    //gStyle->SetOptStat(0);

   // double S1_min = ;
   // S1_max;

    const Double_t min = 8;
    const Double_t max = 22;

    const Int_t nLevels = 999;
    Double_t levels[nLevels];

    for(int i = 1; i < nLevels; i++)
    {
        levels[i] = min + (max - min) / (nLevels - 1) * (i);
    }
    levels[0] = 0.01;


    h2_S1_top_ch_avr->SetContour((sizeof(levels)/sizeof(Double_t)), levels);


    int n_events = 0;
    for(int i = 0; i < 24; i++)
    {
        if( (run_number == 1042 || run_number == 1071) && i == 2)
        {
           continue;
        }
        int xi = i % 4 + 1;
        int yi = 6 - i/4;
        h2_S1_top_ch_avr->SetBinContent(xi,yi,h1_S1_top_ch[i]->GetMean());
        S1_mean += h1_S1_top_ch[i]->GetMean();
        n_ch++;
        //cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S1_top_ch_avr->GetBinContent(xi,yi) <<  endl;
        cout << i << "\t" << top_ch_list_str[i] << "\t" << h2_S1_top_ch_avr->GetBinContent(xi,yi) <<  endl;
        n_events++;
    }
    S1_mean /= n_ch;
    COUT(S1_mean);

    ostringstream h2_S1_top_ch_avr_name;
    h2_S1_top_ch_avr_name << "S1 average [PE]: run " << run_number;
    h2_S1_top_ch_avr->SetTitle(h2_S1_top_ch_avr_name.str().c_str());
    h2_S1_top_ch_avr->GetXaxis()->SetTitle("x [cm]");
    h2_S1_top_ch_avr->GetYaxis()->SetTitle("y [cm]");
    gStyle->SetPaintTextFormat("2.2f");
    //gStyle->SetPaintTextFormat("2d");
    h2_S1_top_ch_avr->SetMarkerSize(2);



    //h2_S1_top_ch_avr->DrawClone("col text scat=0.0");// draw "axes", "contents", "statistics box"
    h2_S1_top_ch_avr->Draw("col text scat=0.0");
    gPad->Update();
    h2_S1_top_ch_avr->SetStats(0);

    h2_S1_top_ch_avr->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

    h2_S1_top_ch_avr->Draw("z same scat=0.0"); // draw the "color palette"
    //gPad->Update();
    //


    TCanvas *c5 = new TCanvas("c5","p5");
    c5->Divide(2,2,0.01,0.01);
    for(int i = 0; i < 4; i++)
    {
        c5->cd(i+1);
        h1_S1_bottom_ch[i]->Draw();
        gPad->SetLogy();
    }


    TCanvas *c6 = new TCanvas("c6","c6");
    double S1_mean_bottom = 0;
    int n_ch_bottom = 0;

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    //gStyle->SetOptStat(0);

    const Double_t min_bottom  = 85;
    const Double_t max_bottom  = 115;

    Double_t levels_bottom[nLevels];
    for(int i = 1; i < nLevels; i++)
    {
        levels_bottom[i] = min_bottom + (max_bottom - min_bottom) / (nLevels - 1) * (i);
    }
    levels_bottom[0] = 0.01;


    h2_S1_bottom_ch_avr->SetContour((sizeof(levels_bottom)/sizeof(Double_t)), levels_bottom);


    for(int i = 0; i < 4; i++)
    {
        int xi = i % 2 + 1;
        int yi = 2 - i/2;
        h2_S1_bottom_ch_avr->SetBinContent(xi,yi,h1_S1_bottom_ch[i]->GetMean());
        S1_mean_bottom += h1_S1_bottom_ch[i]->GetMean();
        n_ch_bottom++;
        //cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S1_top_ch_avr->GetBinContent(xi,yi) <<  endl;
        cout << i << "\t" << bottom_ch_list_str[i] << "\t" << h2_S1_bottom_ch_avr->GetBinContent(xi,yi) <<  endl;
    }
    S1_mean_bottom /= n_ch_bottom;
    COUT(S1_mean_bottom);

    ostringstream h2_S1_bottom_ch_avr_name;
    h2_S1_bottom_ch_avr_name << "S1 average bottom [PE]: run " << run_number;
    h2_S1_bottom_ch_avr->SetTitle(h2_S1_top_ch_avr_name.str().c_str());
    h2_S1_bottom_ch_avr->GetXaxis()->SetTitle("x [cm]");
    h2_S1_bottom_ch_avr->GetYaxis()->SetTitle("y [cm]");
    gStyle->SetPaintTextFormat("2.2f");
    h2_S1_bottom_ch_avr->SetMarkerSize(2);



    //h2_S1_top_ch_avr->DrawClone("col text scat=0.0");// draw "axes", "contents", "statistics box"
    h2_S1_bottom_ch_avr->Draw("col text scat=0.0");
    gPad->Update();
    h2_S1_bottom_ch_avr->SetStats(0);

    h2_S1_bottom_ch_avr->GetZaxis()->SetRangeUser(min_bottom, max_bottom); // ... set the range ...

    h2_S1_bottom_ch_avr->Draw("z same scat=0.0"); // draw the "color palette"


    gPad->Update(); gPad->Modified();
}
