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
#include "TGaxis.h"


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;


using namespace std;


int run_number = 744;
bool is_A3_works = true;

void s1_analysis()
{
    vector<int> top_ch_map = {0, 1, 2, 3, 6, 4, 5, 10, 7, 11, 8, 9, 15,
                              14, 12, 13, 22, 19, 18, 16, 17, 20, 21, 23};


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

    double S1_max = 2000;

    TH1F *h1_S1_total = new TH1F("h1_S1_total", "h1_S1_total", 400, 0, S1_max);
    TH1F *h1_S1_top = new TH1F("h1_S1_top", "h1_S1_top", 200, 0, S1_max/2.0);
    TH1F *h1_S1_bot = new TH1F("h1_S1_bot", "h1_S1_bot", 200, 0, S1_max/2.0);
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 400, -0.2, 1);
    TH1F *h1_S1_TBA = new TH1F("h1_S1_TBA", "h1_S1_TBA", 200, -1, 1);

    //S1_top
    vector<TH1F*> h1_S1_top_vec;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S1_top_vec" << i;
        h1_S1_top_vec.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 400, -20, 100) );
    }


    //S1_bottom
    vector<TH1F*> h1_S1_bottom_vec;
    for(int i = 0; i < 4; i++)
    {
        ostringstream oss;
        oss << "h1_S1_bottom_vec" << i;
        h1_S1_bottom_vec.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 400, -100, 600) );
    }

    TH2F* h2_S1_mean_ch = new TH2F("h2_S1_mean_ch","h2_S1_mean_ch",4,0,5,6,0,5);

    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        if(clusters.size() == 1)
        {
            double TBA_S1 = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) / (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);

            bool cut_S1_total = clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800;

            if(clusters.at(0)->f90 > 0.2 /*&& cut_S1_total*/)
            {
                h1_S1_total->Fill(clusters.at(0)->charge);
                h1_S1_top->Fill(clusters.at(0)->tot_charge_top);
                h1_S1_bot->Fill(clusters.at(0)->tot_charge_bottom);
                h1_S1_f90->Fill(clusters.at(0)->f90);
                h1_S1_TBA->Fill(TBA_S1);

                for(int i=0; i < 4; i++)
                {
                    h1_S1_bottom_vec[i]->Fill( clusters.at(0)->charge_bottom[i] );
                }

                for(int i=0; i < 24; i++)
                {
                    h1_S1_top_vec[i]->Fill( clusters.at(0)->charge_top[i] );
                }
            }
        }
    }

    TCanvas *c1 = new TCanvas("c1", "c1");
    c1->Divide(3,2);
    gStyle->SetOptFit(1);

    c1->cd(1);
    h1_S1_total->Draw();
    h1_S1_total->GetXaxis()->SetTitle("[PE]");

    c1->cd(2);
    h1_S1_top->Draw();
    h1_S1_top->GetXaxis()->SetTitle("[PE]");

    c1->cd(3);
    h1_S1_bot->Draw();
    h1_S1_bot->GetXaxis()->SetTitle("[PE]");

    c1->cd(4);
    h1_S1_f90->Draw();

    c1->cd(5);
    h1_S1_TBA->Draw();


    TCanvas *c2 = new TCanvas("S1_bottom_uniformity_ch","S1_bottom_uniformity_ch");
    c2->Divide(2,2,0.01,0.01);
    for(int i = 0; i < 4; i++)
    {
        int abs_index = i;
        c2->cd(abs_index+1);
        h1_S1_bottom_vec[abs_index]->Draw();
        h1_S1_bottom_vec[abs_index]->GetXaxis()->SetTitle("[PE]");
    }

    TCanvas *c3 = new TCanvas("S1_top_uniformity_ch","S1_top_uniformity_ch");
    c3->Divide(4,6,0.01,0.01);
    for(int i = 0; i < 24; i++)
    {
        int abs_index = i;
        c3->cd(abs_index+1);
        h1_S1_top_vec[abs_index]->Draw();
        h1_S1_top_vec[abs_index]->GetXaxis()->SetTitle("[PE]");
    }


    TCanvas *c8 = new TCanvas("S1_mean_ch_h2","S1_mean_ch_h2");

    const Int_t NRGBs = 6;
    const Int_t NCont = 999;

    Double_t stops[NRGBs] = { 0.00, 0.2, 0.4, 0.6, 0.8, 1.00 };
    Double_t red[NRGBs]   = { 0.99, 0.44, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.66, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.99, 0.72, 1.00, 0.12, 0.00, 0.00 };

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    Double_t min_ = h1_S1_top_vec[0]->GetMean();
    Double_t max_ = h1_S1_top_vec[0]->GetMean();
    for(int i = 0; i < 24; i++)
    {
        if (!is_A3_works && i == 2) continue;
        double tmp = h1_S1_top_vec[i]->GetMean();
        if(tmp < min_) min_ = tmp;
        if(tmp > max_) max_ = tmp;
    }

    cout << "min = " << min_ << "; max = " << max_ << endl;
    const Double_t min = min_ - min_*0.01;//warning
    const Double_t max = max_ + max_*0.01;//warning

    const Int_t nLevels = 999;
    Double_t levels[nLevels];

    for(int i = 1; i < nLevels; i++)
    {
        levels[i] = min + (max - min) / (nLevels - 1) * (i);
    }
    levels[0] = 0.01;


    h2_S1_mean_ch->SetContour((sizeof(levels)/sizeof(Double_t)), levels);


    for(int i = 0; i < 24; i++)
    {
        if (!is_A3_works && i == 2) continue;
        int xi = i % 4 + 1;
        //int yi = i/4 + 1;
        int yi = 6 - i/4;
        h2_S1_mean_ch->SetBinContent(xi,yi,h1_S1_top_vec[top_ch_map[i]]->GetMean());
        //cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S2_max_ch->GetBinContent(xi,yi) <<  endl;
    }

    ostringstream h2_S1_mean_ch_name;
    h2_S1_mean_ch_name << "S1_mean_ch: run " << run_number;
    h2_S1_mean_ch->SetTitle(h2_S1_mean_ch_name.str().c_str());
    h2_S1_mean_ch->GetXaxis()->SetTitle("x [cm]");
    h2_S1_mean_ch->GetYaxis()->SetTitle("y [cm]");
    gStyle->SetPaintTextFormat("2.2f");
    //gStyle->SetPaintTextFormat("3.0f");
    h2_S1_mean_ch->SetMarkerSize(2);


    h2_S1_mean_ch->Draw("col text scat=0.0");
    gPad->Update();
    h2_S1_mean_ch->SetStats(0);

    h2_S1_mean_ch->GetZaxis()->SetRangeUser(min, max); // ... set the range ...
    h2_S1_mean_ch->Draw("z same scat=0.0"); // draw the "color palette"


    //    double low = 0;
    //    double high = 100;
    gPad->Update();
    //TGaxis *axis = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(),gPad->GetUymax(),low,high,510,"-L");
    //TGaxis *axis = new TGaxis(0,175,2000,175,low,high,510,"");
    //TGaxis *axis = new TGaxis(h1_S1_total->GetXaxis()->GetXmin(),h1_S1_total->GetYaxis()->GetXmin(),h1_S1_total->GetXaxis()->GetXmax(),h1_S1_total->GetYaxis()->GetXmin(),low,high,510,"");
    //axis->Draw();

}

