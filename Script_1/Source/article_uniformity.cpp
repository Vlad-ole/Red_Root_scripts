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

int run_number_kr = 966 /*1061*/;
int run_number_bkg = 963 /*1059*/;

void CheckFile(TString filename, TFile *f)
{
    if (!(f->IsOpen()))
    {
        cout << "could not open file: " << f << endl;
        exit(1);
    }
    else
    {
        cout << "File has been opened correctly: " << filename << endl;
    }
}

void article_uniformity()
{
    vector<double> x_centers = {0.625, 1.875, 3.125, 4.375};
    vector<double> y_centers = {0.416667, 1.25, 2.08333, 2.91667, 3.75, 4.58333};

    int S2_n_bins = 35;//PE
    double S2_max = 15000 /*17000*/;
    double y_range_min = -50;
    double y_range_max = 120 /*25*/;

    //S1 cut for Kr
    double S1_left_cut_sig = 350/*400*/;
    double S1_right_cut_sig = 550/*600*/;

    //settings for Kr/Bkg normalisation
    double S2_max_right_tail = 30000;
    double S1_max_right_tail = 10000;
    double S1_left_cut = 4000 /*2000*/;
    double S1_right_cut = 10000;
    double S2_left_cut = 10000;
    double S2_right_cut = 30000;

    TH2F* h2_S2_total_rel = new TH2F("h2_S2_total_rel","h2_S2_total_rel",4,0,5,6,0,5);
    vector<TH1F*> h1_S2_maxch_vec_kr_bkg;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S2_maxch_vec_kr_bkg " << i;
        h1_S2_maxch_vec_kr_bkg.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), S2_n_bins, -100, S2_max) );
    }


    //Kr
    ostringstream path_root_file_kr;
    if(run_number_kr >= 1040 )
        path_root_file_kr << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number_kr << ".root";
    else
        path_root_file_kr << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number_kr << ".root";

    TString filename_kr = path_root_file_kr.str().c_str();
    TFile *f_kr = new TFile(filename_kr, "read");
    CheckFile(filename_kr, f_kr);
    int n_events_test_kr = 0;

    TH1F *h1_S1_total_kr = new TH1F("h1_S1_total_kr", "h1_S1_total_kr", 200, -100, S1_max_right_tail);
    TH1F *h1_S2_total_kr = new TH1F("h1_S2_total_kr", "h1_S2_total_kr", 200, -100, S2_max_right_tail);
    TH1F *h1_S1_total_right_tail_kr = new TH1F("h1_S1_total_right_tail_kr", "h1_S1_total_right_tail_kr", 200, -100, S1_max_right_tail);
    TH1F *h1_S2_total_right_tail_kr = new TH1F("h1_S2_total_right_tail_kr", "h1_S2_total_right_tail_kr", 200, -100, S2_max_right_tail);
    vector<TH1F*> h1_S2_maxch_vec_kr;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S2_maxch_vec_kr " << i;
        h1_S2_maxch_vec_kr.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), S2_n_bins, -100, S2_max) );
    }


    TTree *data = (TTree*)f_kr->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        if(clusters.size() == 2)
        {
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = clusters.at(0)->charge > S1_left_cut_sig && clusters.at(0)->charge < S1_right_cut_sig;

            if(cut_f90 && cut_S1_total && clusters.at(0)->rep == 1)
            {
                //fill hists using events located near (x_centers, y_centers)
                double epsilon = 0.01;
                for(int j = 0; j < 6; j++)
                {
                    int abs_index;
                    for(int k = 0; k < 4; k++)
                    {
                        if( abs(clusters.at(1)->pos_x - x_centers[k]) < epsilon && abs(clusters.at(1)->pos_y - y_centers[j]) < epsilon)
                        {
                            abs_index = k + 4*j;
                            h1_S2_maxch_vec_kr[abs_index]->Fill(clusters.at(1)->charge);
                            h1_S2_maxch_vec_kr_bkg[abs_index]->Fill(clusters.at(1)->charge);
                            n_events_test_kr++;
                        }
                    }
                }

            }

            if(cut_f90 && clusters.at(0)->rep == 1)
            {
                h1_S2_total_kr->Fill(clusters.at(1)->charge);
                h1_S1_total_kr->Fill(clusters.at(0)->charge);
            }

            if( cut_f90 && clusters.at(0)->rep == 1 &&
                    (clusters.at(1)->charge > S2_left_cut) && (clusters.at(1)->charge < S2_right_cut) )
            {
                h1_S2_total_right_tail_kr->Fill(clusters.at(1)->charge);
            }

            if( cut_f90 && clusters.at(0)->rep == 1 &&
                    (clusters.at(0)->charge > S1_left_cut) && (clusters.at(0)->charge < S1_right_cut) )
            {
                h1_S1_total_right_tail_kr->Fill(clusters.at(0)->charge);
            }

        }

    }


    //Bkg
    ostringstream path_root_file_bkg;
    if( run_number_bkg >= 1040 )
        path_root_file_bkg << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number_bkg << ".root";
    else
        path_root_file_bkg << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number_bkg << ".root";

    TString filename_bkg = path_root_file_bkg.str().c_str();
    TFile *f_bkg = new TFile(filename_bkg, "read");
    CheckFile(filename_bkg, f_bkg);
    int n_events_test_bkg = 0;

    TH1F *h1_S2_total_bkg = new TH1F("h1_S2_total_bkg", "h1_S2_total_bkg", 200, -100, S2_max_right_tail);
    TH1F *h1_S1_total_bkg = new TH1F("h1_S1_total_bkg", "h1_S1_total_bkg", 200, -100, S1_max_right_tail);
    TH1F *h1_S1_total_right_tail_bkg = new TH1F("h1_S1_total_right_tail_bkg", "h1_S1_total_right_tail_bkg", 200, -100, S1_max_right_tail);
    TH1F *h1_S2_total_right_tail_bkg = new TH1F("h1_S2_total_right_tail_bkg", "h1_S2_total_right_tail_bkg", 200, -100, S2_max_right_tail);
    TH1F *h1_S2_total_bkg_corr = new TH1F("h1_S2_total_bkg_corr", "h1_S2_total_bkg_corr", 200, -100, S2_max_right_tail);
    TH1F *h1_S1_total_bkg_corr = new TH1F("h1_S1_total_bkg_corr", "h1_S1_total_bkg_corr", 200, -100, S1_max_right_tail);

    vector<TH1F*> h1_S2_maxch_vec_bkg;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S2_maxch_vec_bkg" << i;
        h1_S2_maxch_vec_bkg.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), S2_n_bins, -100, S2_max) );
    }


    data = (TTree*)f_bkg->Get("reco");
    evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        if(clusters.size() == 2)
        {
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = clusters.at(0)->charge > S1_left_cut_sig && clusters.at(0)->charge < S1_right_cut_sig;

            if(cut_f90 && cut_S1_total && clusters.at(0)->rep == 1)
            {
                //fill hists using events located near (x_centers, y_centers)
                double epsilon = 0.01;
                for(int j = 0; j < 6; j++)
                {
                    int abs_index;
                    for(int k = 0; k < 4; k++)
                    {
                        if( abs(clusters.at(1)->pos_x - x_centers[k]) < epsilon && abs(clusters.at(1)->pos_y - y_centers[j]) < epsilon)
                        {
                            abs_index = k + 4*j;
                            h1_S2_maxch_vec_bkg[abs_index]->Fill(clusters.at(1)->charge);
                            n_events_test_bkg++;
                        }
                    }
                }

            }

            if(cut_f90 && clusters.at(0)->rep == 1)
            {
                h1_S2_total_bkg->Fill(clusters.at(1)->charge);
                h1_S1_total_bkg->Fill(clusters.at(0)->charge);
            }

            if( cut_f90 && clusters.at(0)->rep == 1 &&
                    (clusters.at(1)->charge > S2_left_cut) && (clusters.at(1)->charge < S2_right_cut) )
            {
                h1_S2_total_right_tail_bkg->Fill(clusters.at(1)->charge);
            }

            if( cut_f90 && clusters.at(0)->rep == 1 &&
                    (clusters.at(0)->charge > S1_left_cut) && (clusters.at(0)->charge < S1_right_cut) )
            {
                h1_S1_total_right_tail_bkg->Fill(clusters.at(0)->charge);
            }

        }

    }

    double Corr_factor_S1 = 7.0;//test
    //double Corr_factor_S2 = Corr_factor_S1;

    //double Corr_factor_S1 = h1_S1_total_right_tail_kr->GetEntries() / h1_S1_total_right_tail_bkg->GetEntries();
    double Corr_factor_S2 = h1_S2_total_right_tail_kr->GetEntries() / h1_S2_total_right_tail_bkg->GetEntries();
    cout << "Correstion factor form S1 = " << Corr_factor_S1 << endl;
    cout << "Correstion factor form S2 = " << Corr_factor_S2 << endl;


    COUT(h1_S1_total_kr->GetEntries());
    COUT(h1_S1_total_bkg->GetEntries());

    //draw full S2 distributions to choose Kr/bkg normalization
    TCanvas *c4 = new TCanvas("c4","c4");
    c4->Divide(2,2,0.01,0.01);

    c4->cd(1);
    h1_S1_total_kr->Draw();
    h1_S1_total_bkg->Draw("same");
    h1_S1_total_kr->SetTitle("Kr vs. bkg before correction.");
    h1_S1_total_kr->GetXaxis()->SetTitle("S1 [PE]");
    h1_S1_total_bkg->SetFillColor(kRed);
    h1_S1_total_bkg->SetFillStyle(3002);


    c4->cd(2);
//    h1_S2_total_right_tail_kr->Draw();
//    h1_S2_total_right_tail_kr->GetXaxis()->SetTitle("S2 [PE]");

    h1_S2_total_kr->Draw();
    h1_S2_total_bkg->Draw("same");
    h1_S2_total_kr->SetTitle("Kr vs. bkg before correction.");
    h1_S2_total_kr->GetXaxis()->SetTitle("S2 [PE]");
    h1_S2_total_bkg->SetFillColor(kRed);
    h1_S2_total_bkg->SetFillStyle(3002);

    c4->cd(3);
    h1_S1_total_bkg_corr->Add(h1_S1_total_bkg, Corr_factor_S1);
    h1_S1_total_bkg_corr->Draw();
    h1_S1_total_bkg_corr->GetXaxis()->SetTitle("S1 [PE]");
    h1_S1_total_bkg_corr->SetTitle("Kr vs. bkg after correction.");
    h1_S1_total_bkg_corr->SetFillColor(kRed);
    h1_S1_total_bkg_corr->SetFillStyle(3002);
    h1_S1_total_kr->Draw("same");


    c4->cd(4);
    //h1_S2_total_right_tail_bkg->Draw();

    h1_S2_total_bkg_corr->Add(h1_S2_total_bkg, Corr_factor_S2);
    h1_S2_total_bkg_corr->Draw();
    h1_S2_total_bkg_corr->GetXaxis()->SetTitle("S2 [PE]");
    h1_S2_total_bkg_corr->SetTitle("Kr vs. bkg after correction.");
    h1_S2_total_bkg_corr->SetFillColor(kRed);
    h1_S2_total_bkg_corr->SetFillStyle(3002);
    h1_S2_total_kr->Draw("same");



    //draw individual distributions
    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(4,3,0.01,0.01);
    for(int i = 0; i < 12; i++)
    {
        c1->cd(i+1);
        h1_S2_maxch_vec_kr[i]->Draw();
        h1_S2_maxch_vec_bkg[i]->Scale(Corr_factor_S1);
        h1_S2_maxch_vec_bkg[i]->Draw("hist same");
        h1_S2_maxch_vec_bkg[i]->SetFillColor(kRed);
        h1_S2_maxch_vec_bkg[i]->SetFillStyle(3002);
        h1_S2_maxch_vec_kr[i]->GetYaxis()->SetRangeUser(y_range_min, y_range_max);

        h1_S2_maxch_vec_kr_bkg[i]->Add(h1_S2_maxch_vec_bkg[i], -1);
        h1_S2_maxch_vec_kr_bkg[i]->Draw("hist same");
        h1_S2_maxch_vec_kr_bkg[i]->SetFillColor(kGreen);
        h1_S2_maxch_vec_kr_bkg[i]->SetFillStyle(3002);
        //gPad->SetLogy();
    }
    TCanvas *c2 = new TCanvas("c2","c2");
    c2->Divide(4,3,0.01,0.01);
    for(int i = 0; i < 12; i++)
    {
        c2->cd(i+1);
        h1_S2_maxch_vec_kr[i+12]->Draw();
        h1_S2_maxch_vec_bkg[i+12]->Scale(Corr_factor_S1);
        h1_S2_maxch_vec_bkg[i+12]->Draw("hist same");
        h1_S2_maxch_vec_bkg[i+12]->SetFillColor(kRed);
        h1_S2_maxch_vec_bkg[i+12]->SetFillStyle(3002);
        h1_S2_maxch_vec_kr[i+12]->GetYaxis()->SetRangeUser(y_range_min, y_range_max);

        h1_S2_maxch_vec_kr_bkg[i+12]->Add(h1_S2_maxch_vec_bkg[i+12], -1);
        h1_S2_maxch_vec_kr_bkg[i+12]->Draw("hist same");
        h1_S2_maxch_vec_kr_bkg[i+12]->SetFillColor(kGreen);
        h1_S2_maxch_vec_kr_bkg[i+12]->SetFillStyle(3002);

        //gPad->SetLogy();
    }



    vector<double> S2_rel_charge_v(24);
    double S2_sum_of_means = 0;
    int n_ch_not_null =0;
    for(int i = 0; i < 24; i++)
    {

        if( h1_S2_maxch_vec_kr_bkg[i]->GetMean() > 0 )
        {
            n_ch_not_null++;
            S2_sum_of_means += h1_S2_maxch_vec_kr_bkg[i]->GetMean();
        }

    }
    COUT(n_ch_not_null);
    S2_sum_of_means /= n_ch_not_null;
    COUT(S2_sum_of_means);


    for(int i = 0; i < 24; i++)
    {
        //COUT(h1_S2_maxch_vec_kr_bkg[i]->GetEntries());
        S2_rel_charge_v[i] = h1_S2_maxch_vec_kr_bkg[i]->GetMean()/S2_sum_of_means;
        cout << i << "\t" << h1_S2_maxch_vec_kr_bkg[i]->GetMean() << "\t" << S2_rel_charge_v[i] << endl;
    }


    //draw S2 relative
    TCanvas *c3 = new TCanvas("c3","c3");

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

    const Double_t min = 0.65;
    const Double_t max = 1.3;

    const Int_t nLevels = 999;
    Double_t levels[nLevels];

    for(int i = 1; i < nLevels; i++)
    {
        levels[i] = min + (max - min) / (nLevels - 1) * (i);
    }
    levels[0] = 0.01;


    h2_S2_total_rel->SetContour((sizeof(levels)/sizeof(Double_t)), levels);


    int n_events = 0;
    for(int i = 0; i < 24; i++)
    {
        int xi = i % 4 + 1;
        int yi = 6 - i/4;

        if(S2_rel_charge_v[i] > 0)
            h2_S2_total_rel->SetBinContent(xi,yi,S2_rel_charge_v[i]);

        //cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S2_total_rel->GetBinContent(xi,yi) <<  endl;
        n_events++;
    }

    ostringstream h2_S2_total_rel_name;
    h2_S2_total_rel_name << "S2 relative charge. Kr run: " << run_number_kr << ". Bkg run: " << run_number_bkg;
    h2_S2_total_rel->SetTitle(h2_S2_total_rel_name.str().c_str());
    h2_S2_total_rel->GetXaxis()->SetTitle("x [cm]");
    h2_S2_total_rel->GetYaxis()->SetTitle("y [cm]");
    gStyle->SetPaintTextFormat("2.2f");
    h2_S2_total_rel->SetMarkerSize(2);

    h2_S2_total_rel->Draw("col text");
    gPad->Update();
    h2_S2_total_rel->SetStats(0);

    h2_S2_total_rel->GetZaxis()->SetRangeUser(min, max);
    h2_S2_total_rel->Draw("z same");

}
