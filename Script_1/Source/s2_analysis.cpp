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
#include "Math/Polynomial.h"
#include "Math/Interpolator.h"


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


//int run_number = 1099;
//double e_lifetime = 700;

int run_number = 1261;
double e_lifetime = 1E6;


void s2_analysis()
{
    //string draw_plots = "S1 S2_p1 S2_p2 S2_uniformity_gr S2_uniformity_h2 S2_uniformity_ch S2_max_ch_h2 more_plots To_print lifetime S2_bottom_vec";//all
    string draw_plots = "S1 S2_p1 S2_p2 S2_uniformity_gr S2_uniformity_h2 S2_uniformity_ch S2_max_ch_h2 more_plots lifetime S2_bottom_vec";//standart
    //string draw_plots = "S1 S2_p1 S2_uniformity_ch";
    //string draw_plots = "S1 S2_p1 S2_p2 more_plots S2_uniformity_ch lifetime";
    //string draw_plots = "S1 S2_p1 S2_p2 more_plots";

    string center_border = "all";
    //string center_border = "center";
    //string center_border = "border";

    string ev_part = "all";
    //string ev_part = "p1";
    //string ev_part = "p2";

    COUT(center_border);
    COUT(ev_part);

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

    ostringstream path_file_out_s2_uniformity;
    path_file_out_s2_uniformity << "/home/vlad/Reports/XY/S2_uniformity/run_" << run_number << ".txt";
    ofstream file_out_s2_uniformity(path_file_out_s2_uniformity.str().c_str());

    ostringstream path_file_out_xy_distr;
    path_file_out_xy_distr << "/home/vlad/Reports/XY/xy_distr/run_" << run_number << ".txt";
    ofstream file_out_xy_distr(path_file_out_xy_distr.str().c_str());


    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    //bkg
//    double S1_max = 20000;
//    double S2_max = 40000;
//    double S2_S1_max = 40;

    //Kr83
//    double S1_max = 800;
//    double S2_max = 35000;
//    double S2_S1_max = 40;
//    double S1_low_cut = 380;
//    double S1_high_cut = 500;

    //Am E_EL = 4.2kV
//    double S1_max = 1000;
//    double S2_max = 2500;
//    double S2_S1_max = 5;

    //Am E_EL = 5.0kV
//    double S1_max = 2000;
//    double S2_max = 20000;
//    double S2_S1_max = 40;

    //Am arb. 5211, 86, -744
    double S1_max = 2000;
    double S2_max = 50000 /*50000*/;
    double S2_S1_max = 80/*80*/;
    double S1_low_cut = 400/*400*/;
    double S1_high_cut = 800/*800*/;


    //Bkg 5211, 86, -744
//    double S1_max = 10000;
//    double S2_max = 35000;
//    double S2_S1_max = 80;
//    double S1_low_cut = 100;
//    double S1_high_cut = 30000;

    double S2_sum_of_means = 0;
    double S2_sum_of_means_center = 0;
    double S2_sum_of_means_border = 0;

    double S2_rel_charge_mean = 0;
    double S2_rel_charge_sigma = 0;

    double S2_rel_charge_center_mean = 0;
    double S2_rel_charge_center_sigma = 0;

    double S2_rel_charge_border_mean = 0;
    double S2_rel_charge_border_sigma = 0;

    //double range_scale = 1;
    vector<double> x_centers = {0.625, 1.875, 3.125, 4.375};
    vector<double> y_centers = {0.416667, 1.25, 2.08333, 2.91667, 3.75, 4.58333};
    int n_events_test = 0;
    double left_lim = 20 /*15*/;
    double right_lim = 50 /*55*/;
    int max_ev_number = data->GetEntries();
    cout << "max_ev_number = " << max_ev_number << endl;
    double Tdrift_max = 100;

    //hists
    TH1F *h1_S3_total = new TH1F("h1_S3_total", "h1_S3_total", 600, -2, 2500);

    //S1
    TH2F *h2_S1_total_event = new TH2F("h2_S1_total_event", "h2_S1_total_event", 110, 0, max_ev_number, 200, 0, S1_max);
    TH1F *h1_S1_top = new TH1F("h1_S1_top", "h1_S1_top", 200, 0, S1_max/2.0);
    TH1F *h1_S1_bot = new TH1F("h1_S1_bot", "h1_S1_bot", 200, 0, S1_max/2.0);
    TH1F *h1_S1_total = new TH1F("h1_S1_total", "h1_S1_total", 400, 0, S1_max);
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 400, -0.2, 1);
    TH1F *h1_S1_TBA = new TH1F("h1_S1_TBA", "h1_S1_TBA", 200, -1, 1);
    //TH2F *h2_S1_TBA = new TH2F("h2_S1_TBA", "h2_S1_TBA", 200, -0.5, 0.5, 200, 0, S1_max);
    TH2F *h2_S1_total_tdrift = new TH2F("h2_S1_total_tdrift", "h2_S1_total_tdrift", 150, 0, 100, 200, 0, S1_max);
    TH2F *h2_S1_TBA_tdrift = new TH2F("h2_S1_TBA_tdrift", "h2_S1_TBA_tdrift", (Tdrift_max + 1.0), -1, Tdrift_max, 200, -1, 1);
    TH2F *h2_S1_TBA = new TH2F("h2_S1_TBA", "h2_S1_TBA", 200, -1, 1, 400, 0, S1_max);
    TH2F *h2_S1_TBA_corr = new TH2F("h2_S1_TBA_corr", "h2_S1_TBA_corr", 200, -1, 1, 400, 0, S1_max);

    //S2
    TH2F *h2_S2_total_event = new TH2F("h2_S2_total_event", "h2_S2_total_event", 110, 0, max_ev_number, 150, 0, S2_max);
    TH1F *h1_S2_total = new TH1F("h1_S2_total", "h1_S2_total", 400, -100, S2_max);
    TH1F *h1_S2_top = new TH1F("h1_S2_top", "h1_S2_top", 200, -100, S2_max/2.0 );
    TH1F *h1_S2_bot = new TH1F("h1_S2_bot", "h1_S2_bot", 200, -100, S2_max/2.0 );
    TH1F *h1_S2_S1_ratio = new TH1F("h1_S2_S1_ratio", "h1_S2_S1_ratio", 300, -1, S2_S1_max);
    TH1F *h1_S2_f90 = new TH1F("h1_S2_f90", "h1_S2_f90", 400, -0.2, 1);
    TH1F *h1_S2_TBA = new TH1F("h1_S2_TBA", "h1_S2_TBA", 200, -1, 1);
    TH2F *h2_S2_total_tdrift = new TH2F("h2_S2_total_tdrift", "h2_S2_total_tdrift", 150, 0, 100, 200, 0, S2_max);
    TH2F *h2_S2_total_evcorr_tdrift = new TH2F("h2_S2_total_evcorr_tdrift", "h2_S2_total_evcorr_tdrift", 150, 0, 100, 200, 0, S2_max);
    //TH2F *h2_S2_TBA = new TH2F("h2_S2_TBA", "h2_S2_TBA", 200, -0.5, 0.5, 200, 0, S2_max);
    TH2F *h2_S2_TBA_ev = new TH2F("h2_S2_TBA_ev", "h2_S2_TBA_ev", 110, 0, max_ev_number, 200, -1, 1);
    TH2F *h2_S2_S1_ratio_tdrift = new TH2F("h2_S2_S1_ratio_tdrift", "h2_S2_S1_ratio_tdrift", 150, 0, 100, 200, 0, S2_S1_max);
    TH2F *h2_S2_S1_TBA_corr_ratio_tdrift = new TH2F("h2_S2_S1_TBA_corr_ratio_tdrift", "h2_S2_S1_TBA_corr_ratio_tdrift", 150, 0, 100, 200, 0, S2_S1_max);
    TH2F *h2_S2_bot_S2_top = new TH2F("h2_S2_bot_S2_top", "h2_S2_bot_S2_top", 200, 0, S2_max*0.5, 200, 0, S2_max*0.5);
    TH2F *h2_S2_TBA_tdrift = new TH2F("h2_S2_TBA_tdrift", "h2_S2_TBA_tdrift", (Tdrift_max + 1.0), -1, Tdrift_max, 200, -1, 1);

    //S1 uniformity
    TH2F* h2_S1_ch_avr = new TH2F("h2_S1_ch_avr","h2_S1_ch_avr",4,0,5,6,0,5);
    vector<TH1F*> h1_S1_ch_avr;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S1_ch_avr" << i;
        h1_S1_ch_avr.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 450, -50, 400) );
    }
    vector<double> S1_ch_avr_v;
    S1_ch_avr_v.resize(24);


    //S2 uniformity top
    TH2F* h2_S2_total_rel = new TH2F("h2_S2_total_rel","h2_S2_total_rel",4,0,5,6,0,5);
    TH2F* h2_S2_max_ch = new TH2F("h2_S2_max_ch","h2_S2_max_ch",4,0,5,6,0,5);
    vector<TH1F*> h1_S2_maxch_vec;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S2_maxch_vec" << i;
        h1_S2_maxch_vec.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 400, -100, S2_max) );
    }
    TGraph* gr_S2_maxch;
    vector<double> S2_mean_v;
    vector<double> ch_number_v;
    S2_mean_v.resize(24);
    ch_number_v.resize(24);
    vector<double> S2_mean_center_v(24);
    vector<double> S2_mean_border_v(24);

    //S2_bottom
    vector<TH1F*> h1_S2_bottom_vec;
    for(int i = 0; i < 4; i++)
    {
        ostringstream oss;
        oss << "h1_S2_bottom_vec" << i;
        h1_S2_bottom_vec.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 400, -100, 5000) );
    }

    //general
    TH1F *h1_Tdrift = new TH1F("h1 Tdrift", "h1 Tdrift", (Tdrift_max + 1.0), -1, Tdrift_max);
    TH1F *h1_nc = new TH1F("h1_nc", "h1_nc", 200, 0, 10);

    //Saturation
    //TH2F *h2_S2_amp_S2_charge = new TH2F("h2_S2_amp_S2_charge", "h2_S2_amp_S2_charge", 500, 0, S2_max*0.5, 500, 0, 50000);
    TGraph *gr_S2_amp_S2_charge;
    vector<double> S2_amp_v;
    vector<double> S2_charge_v;


    double S2_ev_par0 = 911.403;
    double S2_ev_par1 = 0;


    //prepare values for TBA corrections
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();


        if(clusters.size() == 2)
        {
            double TBA_S1 = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) / (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);
            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = clusters.at(0)->charge > S1_low_cut && clusters.at(0)->charge < S1_high_cut;
            bool cental_8_sipms_x = clusters.at(1)->pos_x > 1.3 && clusters.at(1)->pos_x < 3.7 &&
                    clusters.at(1)->pos_y > 0.9 && clusters.at(1)->pos_y < 4.2;

            bool is_center_border = false;
            if(center_border == "all")
                is_center_border = true;
            else if(center_border == "center")
                is_center_border = cental_8_sipms_x;
            else if (center_border == "border")
                is_center_border = !cental_8_sipms_x;

            bool is_p1 = ev < (max_ev_number/2) ? true : false;
            bool is_ev_part = false;
            if(ev_part == "all")
                is_ev_part = true;
            else if(ev_part == "p1")
                is_ev_part = is_p1;
            else if (ev_part == "p2")
                is_ev_part = !is_p1;

            if(/*ev>2000 &&*/ cut_f90 && cut_S1_total && clusters.at(0)->rep == 1 && is_center_border && is_ev_part)
            {

                h2_S1_TBA->Fill(TBA_S1, clusters.at(0)->charge);
                h1_S1_TBA->Fill(TBA_S1);
            }
        }

    }
    h2_S1_TBA->GetYaxis()->SetRangeUser(S1_low_cut, S1_high_cut);
    TProfile *prof_h2_S1_TBA = h2_S1_TBA->ProfileX();
    //COUT(prof_h2_S1_TBA->GetNbinsX());
    vector<double> prof_h2_S1_TBA_bin_content;
    vector<double> prof_h2_S1_TBA_bin_pos;
    for(int i = 0; i < prof_h2_S1_TBA->GetNbinsX(); i++)
    {
        prof_h2_S1_TBA_bin_pos.push_back(prof_h2_S1_TBA->GetBinCenter(i));

        if(prof_h2_S1_TBA->GetBinContent(i))
            prof_h2_S1_TBA_bin_content.push_back(prof_h2_S1_TBA->GetBinContent(i));
        else
            prof_h2_S1_TBA_bin_content.push_back(h2_S1_TBA->GetMean(2));

        //cout << i << "\t" << prof_h2_S1_TBA_bin_pos[i] << "\t" << prof_h2_S1_TBA_bin_content[i] << endl;
    }
    prof_h2_S1_TBA_bin_pos.push_back(1.1);
    prof_h2_S1_TBA_bin_content.push_back(h2_S1_TBA->GetMean(2));

    //prof_h2_S1_TBA->GetBinContent();
    ROOT::Math::Interpolator S1_TBA_inter(prof_h2_S1_TBA_bin_pos.size(), ROOT::Math::Interpolation::kLINEAR);
    S1_TBA_inter.SetData(prof_h2_S1_TBA_bin_pos.size(), &prof_h2_S1_TBA_bin_pos[0], &prof_h2_S1_TBA_bin_content[0]);

    double S1_TBA_mean = h1_S1_TBA->GetMean();
    (S1_TBA_mean);


    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        h1_nc->Fill(clusters.size());

        if(clusters.size() == 2 /*&& ev > 15000*/ /* && ev < 8000*/)
        {
            double TBA_S1 = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) / (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);
            double TBA_S2 = (clusters.at(1)->tot_charge_top - clusters.at(1)->tot_charge_bottom) / (clusters.at(1)->tot_charge_top + clusters.at(1)->tot_charge_bottom);
            double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = clusters.at(0)->charge > S1_low_cut && clusters.at(0)->charge < S1_high_cut;
            bool cut_S2_total = clusters.at(1)->charge > 500 && clusters.at(1)->charge < 1500;
            bool cental_8_sipms_x = clusters.at(1)->pos_x > 1.3 && clusters.at(1)->pos_x < 3.7 &&
                    clusters.at(1)->pos_y > 0.9 && clusters.at(1)->pos_y < 4.2;

            bool is_center_border = false;
            if(center_border == "all")
                is_center_border = true;
            else if(center_border == "center")
                is_center_border = cental_8_sipms_x;
            else if (center_border == "border")
                is_center_border = !cental_8_sipms_x;

            bool is_p1 = ev < (max_ev_number/2) ? true : false;
            bool is_ev_part = false;
            if(ev_part == "all")
                is_ev_part = true;
            else if(ev_part == "p1")
                is_ev_part = is_p1;
            else if (ev_part == "p2")
                is_ev_part = !is_p1;


            if(/*ev>2000 &&*/ cut_f90 && cut_S1_total && clusters.at(0)->rep == 1 && is_center_border && is_ev_part)
            {
                //general
                h1_Tdrift->Fill(Tdrift);

                //(ev);


                double S1_corr = clusters.at(0)->charge * S1_TBA_inter.Eval(S1_TBA_mean)/S1_TBA_inter.Eval(TBA_S1);


                //S1
                h2_S1_total_event->Fill(ev, clusters.at(0)->charge);
                h1_S1_total->Fill(clusters.at(0)->charge);
                h1_S1_f90->Fill(clusters.at(0)->f90);
                h1_S1_top->Fill(clusters.at(0)->tot_charge_top);
                h1_S1_bot->Fill(clusters.at(0)->tot_charge_bottom);
                //h1_S1_TBA->Fill(TBA_S1);
                h2_S1_TBA_tdrift->Fill(Tdrift,TBA_S1);
                //h2_S1_TBA->Fill(TBA_S1, clusters.at(0)->charge);
                h2_S1_total_tdrift->Fill(Tdrift, clusters.at(0)->charge);
                //h2_S1_TBA->Fill(TBA_S1, clusters.at(0)->charge);
                h2_S1_TBA_corr->Fill(TBA_S1, S1_corr);

                //S2
                h1_S2_S1_ratio->Fill(clusters.at(1)->charge/clusters.at(0)->charge * exp(Tdrift/e_lifetime) );
                //h1_S2_S1_ratio->Fill(clusters.at(1)->charge/S1_corr * exp(Tdrift/e_lifetime) );
                h1_S2_total->Fill(clusters.at(1)->charge);
                h1_S2_f90->Fill(clusters.at(1)->f90);
                h1_S2_top->Fill(clusters.at(1)->tot_charge_top);
                h1_S2_bot->Fill(clusters.at(1)->tot_charge_bottom);
                h1_S2_TBA->Fill(TBA_S2);
                h2_S2_total_tdrift->Fill(Tdrift, clusters.at(1)->charge);
                h2_S2_TBA_ev->Fill(ev, TBA_S2);
                h2_S2_TBA_tdrift->Fill(Tdrift,TBA_S2);
                h2_S2_total_event->Fill(ev, clusters.at(1)->charge);
                h2_S2_S1_ratio_tdrift->Fill(Tdrift, clusters.at(1)->charge/clusters.at(0)->charge);
                h2_S2_bot_S2_top->Fill(clusters.at(1)->tot_charge_top, clusters.at(1)->tot_charge_bottom);
                h2_S2_S1_TBA_corr_ratio_tdrift->Fill(Tdrift, clusters.at(1)->charge / S1_corr);

                for(int i=0; i < 4; i++)
                {
                    h1_S2_bottom_vec[i]->Fill( clusters.at(1)->charge_bottom[i] );
                }

                //saturation
                S2_amp_v.push_back( evReco->GetBaseMean().at(10) - evReco->GetYmin().at(10) );
                S2_charge_v.push_back( clusters.at(1)->charge );

                //S2 uniformity
                double epsilon = 0.01;
                for(int j = 0; j < 6; j++)
                {
                    int abs_index;
                    for(int k = 0; k < 4; k++)
                    {
                        //if(clusters.at(1)->pos_x == x_centers[k] && clusters.at(1)->pos_y == y_centers[j])
                        if( abs(clusters.at(1)->pos_x - x_centers[k]) < epsilon && abs(clusters.at(1)->pos_y - y_centers[j]) < epsilon)
                        {
                            abs_index = k + 4*j;
                            h1_S2_maxch_vec[abs_index]->Fill(clusters.at(1)->charge);
                            n_events_test++;
//                                if(ev == 5)
//                                {
//                                     << clusters.at(1)->pos_x << "\t" << clusters.at(1)->pos_y << "\t" << k << "\t" << j << "\t" << x_centers[k] << "\t" << y_centers[j] << endl;
//                                }
                        }
                    }
                }


                //S2 ev_corr
                double S2_ev_corr = clusters.at(1)->charge + S2_ev_par1*(max_ev_number/2.0 - ev);
                h2_S2_total_evcorr_tdrift->Fill(Tdrift, S2_ev_corr);
                //h2_S2_TBA->Fill(TBA_S1, clusters.at(1)->charge);
                // << "S2 = " << clusters.at(1)->charge << "; S2_ev_corr = " << S2_ev_corr << endl;

                //h1_S3_total->Fill(clusters.at(2)->charge);

            }
        }
    }






    //Draw

    gStyle->SetOptFit(1);
    double X1NDC = 0.7;
    double X2NDC = 0.99;
    double Y1NDC = 0.7;
    double Y2NDC = 0.99;
    int cd_i = 0;

    if(draw_plots.find("S1") != std::string::npos)
    {
        TCanvas *c1 = new TCanvas("S1_analysis","S1_analysis");
        c1->Divide(3,2,0.01,0.01);
        vector<TPaveStats*> st_h1_S1(6);

        COUT(h1_S1_total->GetMean());
        COUT(h1_S1_TBA->GetMean());

        cd_i = 0;
        c1->cd(cd_i + 1);
        h1_S1_total->Draw();
        h1_S1_total->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S1[cd_i] = (TPaveStats*)h1_S1_total->GetListOfFunctions()->FindObject("stats");
        st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
        st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 1;
        c1->cd(cd_i + 1);
        h1_S1_top->Draw();
        h1_S1_top->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S1[cd_i] = (TPaveStats*)h1_S1_top->GetListOfFunctions()->FindObject("stats");
        st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
        st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 2;
        c1->cd(cd_i + 1);
        h1_S1_bot->Draw();
        h1_S1_bot->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S1[cd_i] = (TPaveStats*)h1_S1_bot->GetListOfFunctions()->FindObject("stats");
        st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
        st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 3;
        c1->cd(cd_i + 1);
        h1_S1_f90->Draw();
        //h1_S1_f90->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S1[cd_i] = (TPaveStats*)h1_S1_f90->GetListOfFunctions()->FindObject("stats");
        st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
        st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();


        cd_i = 4;
        c1->cd(cd_i + 1);
        h1_S1_TBA->Draw();
        //h1_S1_f90->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S1[cd_i] = (TPaveStats*)h1_S1_TBA->GetListOfFunctions()->FindObject("stats");
        st_h1_S1[cd_i]->SetX1NDC(X1NDC); st_h1_S1[cd_i]->SetX2NDC(X2NDC);
        st_h1_S1[cd_i]->SetY1NDC(Y1NDC); st_h1_S1[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 5;
        c1->cd(cd_i + 1);
        h2_S1_total_event->GetXaxis()->SetTitle("event");
        h2_S1_total_event->GetYaxis()->SetTitle("S1_total");
        h2_S1_total_event->GetYaxis()->SetRangeUser(S1_low_cut, S1_high_cut);
        h2_S1_total_event->Draw("colz");
        TProfile *prof_h2_S1_total_event = h2_S1_total_event->ProfileX();
        prof_h2_S1_total_event->Draw("same");
        prof_h2_S1_total_event->SetMarkerStyle(20);
        prof_h2_S1_total_event->SetMarkerColor(kRed);
        gPad->Modified(); gPad->Update();
    }

    if(draw_plots.find("S2_p1") != std::string::npos)
    {
        TCanvas *c2 = new TCanvas("S2_analysis","S2_analysis (part 1)");
        c2->Divide(3,2,0.01,0.01);
        vector<TPaveStats*> st_h1_S2(6);
        gStyle->SetOptFit(1);

        cd_i = 0;
        c2->cd(cd_i + 1);
        h1_S2_total->Draw();
        h1_S2_total->GetXaxis()->SetTitle("[PE]");
        COUT(h1_S2_total->GetMean());
        COUT(h1_S2_total->GetRMS());
        gPad->Update();
        st_h1_S2[cd_i] = (TPaveStats*)h1_S2_total->GetListOfFunctions()->FindObject("stats");
        st_h1_S2[cd_i]->SetX1NDC(X1NDC); st_h1_S2[cd_i]->SetX2NDC(X2NDC);
        st_h1_S2[cd_i]->SetY1NDC(Y1NDC); st_h1_S2[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 1;
        c2->cd(cd_i + 1);
        h1_S2_top->Draw();
        h1_S2_top->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S2[cd_i] = (TPaveStats*)h1_S2_top->GetListOfFunctions()->FindObject("stats");
        st_h1_S2[cd_i]->SetX1NDC(X1NDC); st_h1_S2[cd_i]->SetX2NDC(X2NDC);
        st_h1_S2[cd_i]->SetY1NDC(Y1NDC); st_h1_S2[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 2;
        c2->cd(cd_i + 1);
        h1_S2_bot->Draw();
        h1_S2_bot->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S2[cd_i] = (TPaveStats*)h1_S2_bot->GetListOfFunctions()->FindObject("stats");
        st_h1_S2[cd_i]->SetX1NDC(X1NDC); st_h1_S2[cd_i]->SetX2NDC(X2NDC);
        st_h1_S2[cd_i]->SetY1NDC(Y1NDC); st_h1_S2[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 3;
        c2->cd(cd_i + 1);
        h1_S2_f90->Draw();
        //h1_S1_f90->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S2[cd_i] = (TPaveStats*)h1_S2_f90->GetListOfFunctions()->FindObject("stats");
        st_h1_S2[cd_i]->SetX1NDC(X1NDC); st_h1_S2[cd_i]->SetX2NDC(X2NDC);
        st_h1_S2[cd_i]->SetY1NDC(Y1NDC); st_h1_S2[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 4;
        c2->cd(cd_i + 1);
        h1_S2_TBA->Draw();
        //h1_S1_f90->GetXaxis()->SetTitle("[PE]");
        gPad->Update();
        st_h1_S2[cd_i] = (TPaveStats*)h1_S2_TBA->GetListOfFunctions()->FindObject("stats");
        st_h1_S2[cd_i]->SetX1NDC(X1NDC); st_h1_S2[cd_i]->SetX2NDC(X2NDC);
        st_h1_S2[cd_i]->SetY1NDC(Y1NDC); st_h1_S2[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

        cd_i = 5;
        c2->cd(cd_i + 1);
        h2_S2_total_event->GetXaxis()->SetTitle("event");
        h2_S2_total_event->GetYaxis()->SetTitle("S2_total [PE]");
        h2_S2_total_event->Draw("colz");
        gPad->Update();
        TProfile *prof_h2_S2_total_event = h2_S2_total_event->ProfileX();
        prof_h2_S2_total_event->Draw("same");
        prof_h2_S2_total_event->SetMarkerStyle(20);
        prof_h2_S2_total_event->SetMarkerColor(kBlack);
        TF1 *f1_S2_evcorr = new TF1("f1_S2_evcorr","[0] + x*[1]",0,max_ev_number);
        prof_h2_S2_total_event->Fit("f1_S2_evcorr","R","",0,data->GetEntries());
        cout << "f1_S2_evcorr: par0 = " << f1_S2_evcorr->GetParameter(0) << "; par1 = " << f1_S2_evcorr->GetParameter(1) << endl;
        gPad->Modified(); gPad->Update();

    }

    if(draw_plots.find("S2_p2") != std::string::npos)
    {
        TCanvas *c3 = new TCanvas("S2_analysis p2","S2_analysis (part 2)");
        c3->Divide(3,2,0.01,0.01);
        vector<TPaveStats*> st_h1_S2_p2(6);



        if(0)
        {
            cd_i = 1;
            c3->cd(cd_i + 1);
            h2_S2_total_evcorr_tdrift->Draw();
            h2_S2_total_evcorr_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
            h2_S2_total_evcorr_tdrift->GetYaxis()->SetTitle("S2 [PE]");
            h2_S2_total_evcorr_tdrift->Draw("colz");
            gPad->Update();

            TProfile *prof_h2_S2_total_evcorr_tdriftt = h2_S2_total_evcorr_tdrift->ProfileX();
            prof_h2_S2_total_evcorr_tdriftt->Draw("same");
            prof_h2_S2_total_evcorr_tdriftt->SetMarkerStyle(20);
            prof_h2_S2_total_evcorr_tdriftt->SetMarkerColor(kBlack);

            st_h1_S2_p2[cd_i] = (TPaveStats*)h2_S2_total_evcorr_tdrift->GetListOfFunctions()->FindObject("stats");
            st_h1_S2_p2[cd_i]->SetX1NDC(0.45); st_h1_S2_p2[cd_i]->SetX2NDC(0.89);
            st_h1_S2_p2[cd_i]->SetY1NDC(0.8); st_h1_S2_p2[cd_i]->SetY2NDC(0.89);

            TF1 *f1_exp_purity_2 = new TF1("f1_exp_purity_2","exp([0] + x*[1])",0,150);
            prof_h2_S2_total_evcorr_tdriftt->Fit("f1_exp_purity_2","R","",left_lim,right_lim);
            ostringstream TPaveStats_fit_info_l1_2;
            TPaveStats_fit_info_l1_2 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_exp_purity_2->GetParameter(1) << " +- " << f1_exp_purity_2->GetParError(1)/pow(f1_exp_purity_2->GetParameter(1), 2.0) << " us";

            st_h1_S2_p2[cd_i] = (TPaveStats*)gPad->GetPrimitive("stats");
            st_h1_S2_p2[cd_i]->SetName("mystats_c2_cd1");

            TList *listOfLines_2 = st_h1_S2_p2[cd_i]->GetListOfLines();

            TText *tconst_2 = st_h1_S2_p2[cd_i]->GetLineWith("Std");
            listOfLines_2->Remove(tconst_2);
            tconst_2 = st_h1_S2_p2[cd_i]->GetLineWith("Std");
            listOfLines_2->Remove(tconst_2);
            tconst_2 = st_h1_S2_p2[cd_i]->GetLineWith("Mean");
            listOfLines_2->Remove(tconst_2);
            tconst_2 = st_h1_S2_p2[cd_i]->GetLineWith("Mean");
            listOfLines_2->Remove(tconst_2);
            tconst_2 = st_h1_S2_p2[cd_i]->GetLineWith("h2");
            listOfLines_2->Remove(tconst_2);
            tconst_2 = st_h1_S2_p2[cd_i]->GetLineWith("Entries");
            listOfLines_2->Remove(tconst_2);

            TLatex *myt_2 = new TLatex(0,0,TPaveStats_fit_info_l1_2.str().c_str());
            myt_2 ->SetTextFont(42);
            myt_2 ->SetTextSize(0.04);
            myt_2 ->SetTextColor(kRed);
            listOfLines_2->Add(myt_2);
            h2_S2_total_evcorr_tdrift->SetStats(0);
            gPad->Modified();
        }

        cd_i = 1;
        c3->cd(cd_i + 1);
        h1_Tdrift->Draw();


        cd_i = 2;
        c3->cd(cd_i + 1);
        h2_S2_TBA_ev->Draw();
        h2_S2_TBA_ev->GetXaxis()->SetTitle("ev");
        h2_S2_TBA_ev->GetYaxis()->SetTitle("TBA_S2");
        h2_S2_TBA_ev->Draw("colz");
        h2_S2_TBA_ev->GetYaxis()->SetRangeUser(-0.4,0.4);
        gPad->Update();
        TProfile *prof_h2_S2_TBA_ev = h2_S2_TBA_ev->ProfileX();
        prof_h2_S2_TBA_ev->Draw("same");
        prof_h2_S2_TBA_ev->SetMarkerStyle(20);
        prof_h2_S2_TBA_ev->SetMarkerColor(kBlack);
        gPad->Modified(); gPad->Update();


        {
            cd_i = 4;
            c3->cd(cd_i + 1);
            h2_S1_total_tdrift->Draw();
            h2_S1_total_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
            h2_S1_total_tdrift->GetYaxis()->SetTitle("S1");
            h2_S1_total_tdrift->Draw("colz");
            h2_S1_total_tdrift->GetXaxis()->SetRangeUser(10,70);
            h2_S1_total_tdrift->GetYaxis()->SetRangeUser(S1_low_cut,S1_high_cut);
            gPad->Update();

            TProfile *prof_h2_S1_total_tdrift = h2_S1_total_tdrift->ProfileX();
            prof_h2_S1_total_tdrift->Draw("same");
            prof_h2_S1_total_tdrift->SetMarkerStyle(20);
            prof_h2_S1_total_tdrift->SetMarkerColor(kBlack);

        }

        cd_i = 5;
        c3->cd(cd_i + 1);
        h1_S2_S1_ratio->Draw();
        //h1_S2_S1_ratio->GetXaxis()->SetTitle("");
        gPad->Update();
        st_h1_S2_p2[cd_i] = (TPaveStats*)h1_S2_S1_ratio->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_p2[cd_i]->SetX1NDC(X1NDC); st_h1_S2_p2[cd_i]->SetX2NDC(X2NDC);
        st_h1_S2_p2[cd_i]->SetY1NDC(Y1NDC); st_h1_S2_p2[cd_i]->SetY2NDC(Y2NDC);
        gPad->Modified(); gPad->Update();

    }

    if(draw_plots.find("more_plots") != std::string::npos)
    {
        TCanvas *c4 = new TCanvas("More plots","More plots");
        c4->Divide(3,2,0.01,0.01);
        vector<TPaveStats*> st_general_analysis(4);

        c4->cd(1);
        h1_nc->Sumw2(kTRUE);
        h1_nc->GetXaxis()->SetTitle("nc");
        //h1_nc->Scale(1, "width");
        //h1_nc->SetBarWidth(0.4);
        h1_nc->SetFillColor(kMagenta);
        h1_nc->Scale(1/h1_nc->Integral());
        h1_nc->Draw("HIST b");
        h1_nc->SetBarWidth(10);
        h1_nc->SetBarOffset(0);
        gPad->Modified(); gPad->Update();

        c4->cd(2);
        h2_S2_bot_S2_top->Draw("colz");
        h2_S2_bot_S2_top->GetXaxis()->SetTitle("S2_top [pe]");
        h2_S2_bot_S2_top->GetYaxis()->SetTitle("S2_bottom [pe]");
        gPad->Modified(); gPad->Update();

        c4->cd(4);
        h2_S1_TBA_tdrift->Draw("colz");
        h2_S1_TBA_tdrift->GetXaxis()->SetTitle("Drift time [us]");
        h2_S1_TBA_tdrift->GetYaxis()->SetTitle("S1 TBA");
        gPad->Modified(); gPad->Update();

        c4->cd(5);
        h2_S2_TBA_tdrift->Draw("colz");
        h2_S2_TBA_tdrift->GetXaxis()->SetTitle("Drift time [us]");
        h2_S2_TBA_tdrift->GetYaxis()->SetTitle("S2 TBA");
        gPad->Modified(); gPad->Update();

        c4->cd(3);
        gr_S2_amp_S2_charge = new TGraph(S2_amp_v.size(), &S2_charge_v[0], &S2_amp_v[0]);
        gr_S2_amp_S2_charge->Draw("AP");
        gr_S2_amp_S2_charge->GetXaxis()->SetRangeUser(0, 50000);
        gr_S2_amp_S2_charge->GetXaxis()->SetTitle("clusters.at(1)->charge");
        gr_S2_amp_S2_charge->GetYaxis()->SetTitle("evReco->GetBaseMean().at(10) - evReco->GetYmin().at(10)");
        gr_S2_amp_S2_charge->SetMarkerSize(1.2);

    }

    if(draw_plots.find("S2_uniformity_ch") != std::string::npos)
    {
        TCanvas *c5 = new TCanvas("S2_uniformity_ch","S2_uniformity_ch");
        c5->Divide(4,6,0.01,0.01);
        vector<TPaveStats*> st_general_analysis(6);
        //double S2_sum_of_means = 0;
//        for(int i = 0; i < 24; i++)
//            S2_sum_of_means += h1_S2_maxch_vec[i]->GetMean();

        for(int j = 0; j < 6; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                int abs_index = k + 4*j;
                c5->cd(abs_index+1);
                h1_S2_maxch_vec[abs_index]->Draw();
            }
        }
    }

    {

        S2_sum_of_means = 0;
        int n_ch_not_null =0;
        int n_ch_not_null_center = 0;
        int n_ch_not_null_border = 0;
        for(int i = 0; i < 24; i++)
        {
            S2_sum_of_means += h1_S2_maxch_vec[i]->GetMean();
            if( h1_S2_maxch_vec[i]->GetMean() )
                n_ch_not_null++;

            bool cental_ch = i==5 || i==6 || i==9 || i==10 || i==13 || i==14 || i==17 || i==18;
            if(cental_ch)
            {
                S2_sum_of_means_center += h1_S2_maxch_vec[i]->GetMean();
                n_ch_not_null_center++;
            }

            if(!cental_ch)
            {
                S2_sum_of_means_border += h1_S2_maxch_vec[i]->GetMean();
                n_ch_not_null_border++;
            }
        }
//        COUT(n_ch_not_null);
//        COUT(n_ch_not_null_center);
//        COUT(n_ch_not_null_border);

        S2_sum_of_means /= n_ch_not_null;
        S2_sum_of_means_center /= n_ch_not_null_center;
        S2_sum_of_means_border /= n_ch_not_null_border;

//        COUT(S2_sum_of_means);
//        COUT(S2_sum_of_means_center);
//        COUT(S2_sum_of_means_border);


        for(int i = 0; i < 24; i++)
        {
            //COUT(h1_S2_maxch_vec[i]->GetEntries());
            S2_mean_v[i] = h1_S2_maxch_vec[i]->GetMean()/S2_sum_of_means;
            ch_number_v[i] = i+1;
            //file_out_s2_uniformity << S2_mean_v[i] << endl;
            //file_out_xy_distr << h1_S2_maxch_vec[i]->GetEntries() << endl;
            S2_rel_charge_mean += S2_mean_v[i];

            bool cental_ch = i==5 || i==6 || i==9 || i==10 || i==13 || i==14 || i==17 || i==18;
            if(cental_ch)
            {
                S2_mean_center_v[i] = h1_S2_maxch_vec[i]->GetMean()/S2_sum_of_means_center;
                S2_rel_charge_center_mean += S2_mean_center_v[i];
            }

            if(!cental_ch)
            {
                S2_mean_border_v[i] = h1_S2_maxch_vec[i]->GetMean()/S2_sum_of_means_border;
                S2_rel_charge_border_mean += S2_mean_border_v[i];
            }

        }
        S2_rel_charge_mean /= n_ch_not_null;
        S2_rel_charge_center_mean /= n_ch_not_null_center;
        S2_rel_charge_border_mean /= n_ch_not_null_border;

        for(int i = 0; i < 24; i++)
        {

            bool cental_ch = i==5 || i==6 || i==9 || i==10 || i==13 || i==14 || i==17 || i==18;

            if(S2_mean_v[i] != 0)
            {
                S2_rel_charge_sigma += pow(S2_mean_v[i] - S2_rel_charge_mean, 2.0);
//                cout << i << "\t" << S2_mean_v[i] << "\t" << S2_rel_charge_mean << "\t"
//                     << (S2_mean_v[i] - S2_rel_charge_mean)
//                     << "\t" << pow(S2_mean_v[i] - S2_rel_charge_mean, 2.0) << endl;

//                cout << i << "\t" << h1_S2_maxch_vec[i]->GetMean() << "\t" <<
//                        S2_mean_v[i] << "\t" << S2_rel_charge_center_mean << "\t"
//                     << (S2_mean_v[i] - S2_rel_charge_center_mean)
//                     << "\t" << pow(S2_mean_v[i] - S2_rel_charge_center_mean, 2.0) << endl;

                if(cental_ch)
                {
                    //cout << endl;
                    S2_rel_charge_center_sigma += pow(S2_mean_center_v[i] - S2_rel_charge_center_mean, 2.0);
//                    cout << i << "\t" << h1_S2_maxch_vec[i]->GetMean() << "\t" <<
//                            S2_mean_center_v[i] << "\t" << S2_rel_charge_center_mean << "\t"
//                         << (S2_mean_center_v[i] - S2_rel_charge_center_mean)
//                         << "\t" << pow(S2_mean_center_v[i] - S2_rel_charge_center_mean, 2.0) << endl;

                }

                if(!cental_ch)
                    S2_rel_charge_border_sigma += pow(S2_mean_border_v[i] - S2_rel_charge_border_mean, 2.0);

            }
        }
        S2_rel_charge_sigma = sqrt(S2_rel_charge_sigma/(n_ch_not_null - 1));
        S2_rel_charge_center_sigma = sqrt(S2_rel_charge_center_sigma/(n_ch_not_null_center - 1));
        S2_rel_charge_border_sigma = sqrt(S2_rel_charge_border_sigma/(n_ch_not_null_border - 1));

        COUT(S2_rel_charge_sigma);
//        COUT(S2_rel_charge_center_sigma);
//        COUT(S2_rel_charge_border_sigma);


    }

    if(draw_plots.find("S2_uniformity_gr") != std::string::npos)
    {
        TCanvas *c6 = new TCanvas("S2_uniformity (rel. charge)","S2_uniformity (rel. charge)");
        c6->Divide(1,1,0.01,0.01);

        cd_i = 0;
        c6->cd(cd_i + 1);
        gr_S2_maxch = new TGraph(ch_number_v.size(), &ch_number_v[0], &S2_mean_v[0]);
        gr_S2_maxch->Draw("AP");
        gr_S2_maxch->SetMarkerStyle(20);
        gr_S2_maxch->GetXaxis()->SetTitle("Relative ch number");
        gr_S2_maxch->GetYaxis()->SetTitle("S2_total_relative");
        gPad->SetGrid();
        //COUT(n_events_test);
    }

    if(draw_plots.find("S2_uniformity_h2") != std::string::npos)
    {
        TCanvas *c7 = new TCanvas("S2_uniformity_h2","S2_uniformity_h2");
        //c6->Divide(1,1,0.01,0.01);

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
            //int yi = i/4 + 1;
            int yi = 6 - i/4;
            h2_S2_total_rel->SetBinContent(xi,yi,S2_mean_v[i]);
            //cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S2_total_rel->GetBinContent(xi,yi) <<  endl;
            n_events++;
        }

        ostringstream h2_S2_total_rel_name;
        h2_S2_total_rel_name << "S2 relative charge: run " << run_number;
        h2_S2_total_rel->SetTitle(h2_S2_total_rel_name.str().c_str());
        h2_S2_total_rel->GetXaxis()->SetTitle("x [cm]");
        h2_S2_total_rel->GetYaxis()->SetTitle("y [cm]");
        gStyle->SetPaintTextFormat("2.2f");
        //gStyle->SetPaintTextFormat("2d");
        h2_S2_total_rel->SetMarkerSize(2);


        //----------------------------------------------------------------

        //h2_S2_total_rel->DrawClone("col text");// draw "axes", "contents", "statistics box"
        h2_S2_total_rel->Draw("col text");
        gPad->Update();
        h2_S2_total_rel->SetStats(0);

        h2_S2_total_rel->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

        h2_S2_total_rel->Draw("z same"); // draw the "color palette"
        //gPad->Update();
        //
    }

    if(draw_plots.find("S2_max_ch_h2") != std::string::npos)
    {
        TCanvas *c8 = new TCanvas("S2_max_ch_h2","S2_max_ch_h2");
        //c6->Divide(1,1,0.01,0.01);

        //gStyle->SetOptStat(0);

        Double_t min_ = h1_S2_maxch_vec[0]->GetEntries();
        Double_t max_ = h1_S2_maxch_vec[0]->GetEntries();
        for(int i = 0; i < 24; i++)
        {
            double tmp = h1_S2_maxch_vec[i]->GetEntries();
            if(tmp < min_) min_ = tmp;
            if(tmp > max_) max_ = tmp;
        }

        cout << "min = " << min_ << "; max = " << max_ << endl;


        const Double_t min = min_;//warning
        const Double_t max = max_;//warning

        const Int_t nLevels = 999;
        Double_t levels[nLevels];

        for(int i = 1; i < nLevels; i++)
        {
            levels[i] = min + (max - min) / (nLevels - 1) * (i);
        }
        levels[0] = 0.01;


        h2_S2_max_ch->SetContour((sizeof(levels)/sizeof(Double_t)), levels);


        int n_events = 0;
        for(int i = 0; i < 24; i++)
        {
            int xi = i % 4 + 1;
            //int yi = i/4 + 1;
            int yi = 6 - i/4;
            h2_S2_max_ch->SetBinContent(xi,yi,h1_S2_maxch_vec[i]->GetEntries());
            //cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S2_max_ch->GetBinContent(xi,yi) <<  endl;
            n_events++;
        }

        ostringstream h2_S2_max_ch_name;
        h2_S2_max_ch_name << "S2_max_ch: run " << run_number;
        h2_S2_max_ch->SetTitle(h2_S2_max_ch_name.str().c_str());
        h2_S2_max_ch->GetXaxis()->SetTitle("x [cm]");
        h2_S2_max_ch->GetYaxis()->SetTitle("y [cm]");
        //gStyle->SetPaintTextFormat("3.3f");
        //gStyle->SetPaintTextFormat("3.0f");
        h2_S2_max_ch->SetMarkerSize(2);


        //----------------------------------------------------------------

        //h2_S2_total_rel->DrawClone("col text");// draw "axes", "contents", "statistics box"
        //h2_S2_max_ch->Draw("col text");
        h2_S2_max_ch->Draw("col text scat=0.0");
        gPad->Update();
        h2_S2_max_ch->SetStats(0);

        h2_S2_max_ch->GetZaxis()->SetRangeUser(min, max); // ... set the range ...
        h2_S2_max_ch->Draw("z same scat=0.0"); // draw the "color palette"

        //gPad->Update();
        //
    }

    if(draw_plots.find("To_print") != std::string::npos)
    {
        TCanvas *c9 = new TCanvas("To_print","To_print");
        //c9->Divide(1,1,0.01,0.01);
        vector<TPaveStats*> st_h1_to_print(6);

        //S2 vs Td
//        h2_S2_total_tdrift->Draw();
//        h2_S2_total_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
//        h2_S2_total_tdrift->GetYaxis()->SetTitle("S2 [PE]");
//        h2_S2_total_tdrift->Draw("colz");
//        h2_S2_total_tdrift->GetXaxis()->SetRangeUser(0, 70);
//        ostringstream run_number_string;
//        run_number_string << "run " << run_number;
//        h2_S2_total_tdrift->SetTitle( run_number_string.str().c_str() );
//        gPad->Update();

//        TProfile *prof_h2_S2_total_tdrift = h2_S2_total_tdrift->ProfileX();
//        prof_h2_S2_total_tdrift->Draw("same");
//        prof_h2_S2_total_tdrift->SetMarkerStyle(20);
//        prof_h2_S2_total_tdrift->SetMarkerColor(kBlack);

//        TF1 *f1_exp_purity_3 = new TF1("f1_exp_purity_3","exp([0] + x*[1])",0,150);
//        prof_h2_S2_total_tdrift->Fit("f1_exp_purity_3","R","",left_lim,right_lim);
//        ostringstream TPaveStats_fit_info_l1;
//        TPaveStats_fit_info_l1 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_exp_purity_3->GetParameter(1) << " +- " << f1_exp_purity_3->GetParError(1)/pow(f1_exp_purity_3->GetParameter(1), 2.0) << " us";
//        gPad->Modified();

        //S1 vs Td
        //h2_S1_total_tdrift->Draw();
        h2_S1_total_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S1_total_tdrift->GetYaxis()->SetTitle("S1_total[PE]");
        h2_S1_total_tdrift->Draw("colz");
        h2_S1_total_tdrift->GetXaxis()->SetRangeUser(10,70);
        h2_S1_total_tdrift->GetYaxis()->SetRangeUser(S1_low_cut,S1_high_cut);
        ostringstream run_number_string;
        run_number_string << "run " << run_number;
        h2_S1_total_tdrift->SetTitle(run_number_string.str().c_str());
        gPad->Update();

        TProfile *prof_h2_S1_total_tdrift = h2_S1_total_tdrift->ProfileX();
        prof_h2_S1_total_tdrift->Draw("same");
        prof_h2_S1_total_tdrift->SetMarkerStyle(20);
        prof_h2_S1_total_tdrift->SetMarkerColor(kBlack);
        h2_S1_total_tdrift->SetStats(0);
    }


    if(draw_plots.find("lifetime") != std::string::npos)
    {
        TCanvas *c10 = new TCanvas("lifetime","lifetime");
        c10->Divide(3,2,0.01,0.01);
        vector<TPaveStats*> st_h1_S2_c10(6);


        cd_i = 0;
        c10->cd(cd_i + 1);
        h2_S2_total_tdrift->Draw();
        h2_S2_total_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_total_tdrift->GetYaxis()->SetTitle("S2 [PE]");
        h2_S2_total_tdrift->Draw("colz");
        h2_S2_total_tdrift->GetXaxis()->SetRangeUser(10, 65);
        gPad->Update();

        TProfile *prof_h2_S2_total_tdrift = h2_S2_total_tdrift->ProfileX();
        prof_h2_S2_total_tdrift->Draw("same");
        prof_h2_S2_total_tdrift->SetMarkerStyle(20);
        prof_h2_S2_total_tdrift->SetMarkerColor(kBlack);

        st_h1_S2_c10[cd_i] = (TPaveStats*)h2_S2_total_tdrift->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_c10[cd_i]->SetX1NDC(0.45); st_h1_S2_c10[cd_i]->SetX2NDC(0.89);
        st_h1_S2_c10[cd_i]->SetY1NDC(0.8); st_h1_S2_c10[cd_i]->SetY2NDC(0.89);

        TF1 *f1_exp_purity = new TF1("f1_exp_purity","exp([0] + x*[1])",0,150);
        prof_h2_S2_total_tdrift->Fit("f1_exp_purity","R","",left_lim,right_lim);
        ostringstream TPaveStats_fit_info_l1;
        TPaveStats_fit_info_l1 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_exp_purity->GetParameter(1) << " +- " << f1_exp_purity->GetParError(1)/pow(f1_exp_purity->GetParameter(1), 2.0) << " us";

        st_h1_S2_c10[cd_i] = (TPaveStats*)gPad->GetPrimitive("stats");
        st_h1_S2_c10[cd_i]->SetName("mystats_c2_cd1");

        TList *listOfLines = st_h1_S2_c10[cd_i]->GetListOfLines();

        TText *tconst = st_h1_S2_c10[cd_i]->GetLineWith("Std");
        listOfLines->Remove(tconst);
        tconst = st_h1_S2_c10[cd_i]->GetLineWith("Std");
        listOfLines->Remove(tconst);
        tconst = st_h1_S2_c10[cd_i]->GetLineWith("Mean");
        listOfLines->Remove(tconst);
        tconst = st_h1_S2_c10[cd_i]->GetLineWith("Mean");
        listOfLines->Remove(tconst);
        tconst = st_h1_S2_c10[cd_i]->GetLineWith("h2");
        listOfLines->Remove(tconst);
        tconst = st_h1_S2_c10[cd_i]->GetLineWith("Entries");
        listOfLines->Remove(tconst);

        TLatex *myt = new TLatex(0,0,TPaveStats_fit_info_l1.str().c_str());
        myt ->SetTextFont(42);
        myt ->SetTextSize(0.04);
        myt ->SetTextColor(kRed);
        listOfLines->Add(myt);
        h2_S2_total_tdrift->SetStats(0);
        gPad->Modified();


        cd_i = 2;
        c10->cd(cd_i+1);
        h2_S2_S1_TBA_corr_ratio_tdrift->Draw("colz");
        h2_S2_S1_TBA_corr_ratio_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_S1_TBA_corr_ratio_tdrift->GetYaxis()->SetTitle("S2/S1(TBA corrected)");
        h2_S2_S1_TBA_corr_ratio_tdrift->GetXaxis()->SetRangeUser(10, 65);
        gPad->Update();

        TProfile *prof_h2_S2_S1_TBA_corr_ratio_tdrift = h2_S2_S1_TBA_corr_ratio_tdrift->ProfileX();
        prof_h2_S2_S1_TBA_corr_ratio_tdrift->Draw("same");
        prof_h2_S2_S1_TBA_corr_ratio_tdrift->SetMarkerStyle(20);
        prof_h2_S2_S1_TBA_corr_ratio_tdrift->SetMarkerColor(kBlack);
        //h2_S1_total_tdrift->SetStats(0);

        st_h1_S2_c10[cd_i] = (TPaveStats*)h2_S2_S1_TBA_corr_ratio_tdrift->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_c10[cd_i]->SetX1NDC(0.45); st_h1_S2_c10[cd_i]->SetX2NDC(0.89);
        st_h1_S2_c10[cd_i]->SetY1NDC(0.8); st_h1_S2_c10[cd_i]->SetY2NDC(0.89);

        TF1 *f1_c10_cd1 = new TF1("f1_c10_cd1","exp([0] + x*[1])",0,150);
        prof_h2_S2_S1_TBA_corr_ratio_tdrift->Fit("f1_c10_cd1","R","",left_lim,right_lim);
        ostringstream TPaveStats_fit_c10_cd1;
        TPaveStats_fit_c10_cd1 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_c10_cd1->GetParameter(1) << " +- " << f1_c10_cd1->GetParError(1)/pow(f1_c10_cd1->GetParameter(1), 2.0) << " us";

        st_h1_S2_c10[cd_i] = (TPaveStats*)gPad->GetPrimitive("stats");
        st_h1_S2_c10[cd_i]->SetName("mystats_c10_cd1");

        TList *listOfLines_c10_cd1 = st_h1_S2_c10[cd_i]->GetListOfLines();
        //listOfLines_c10_cd1->Dump();

        TText *tconst_c10_cd1 = st_h1_S2_c10[cd_i]->GetLineWith("Std");
        listOfLines_c10_cd1->Remove(tconst_c10_cd1);
        tconst_c10_cd1 = st_h1_S2_c10[cd_i]->GetLineWith("Std");
        listOfLines_c10_cd1->Remove(tconst_c10_cd1);
        tconst_c10_cd1 = st_h1_S2_c10[cd_i]->GetLineWith("Mean");
        listOfLines_c10_cd1->Remove(tconst_c10_cd1);
        tconst_c10_cd1 = st_h1_S2_c10[cd_i]->GetLineWith("Mean");
        listOfLines_c10_cd1->Remove(tconst_c10_cd1);
        tconst_c10_cd1 = st_h1_S2_c10[cd_i]->GetLineWith("h2");
        listOfLines_c10_cd1->Remove(tconst_c10_cd1);
        tconst_c10_cd1 = st_h1_S2_c10[cd_i]->GetLineWith("Entries");
        listOfLines_c10_cd1->Remove(tconst_c10_cd1);

        TLatex *myt_c10_cd1 = new TLatex(0,0,TPaveStats_fit_c10_cd1.str().c_str());
        myt_c10_cd1 ->SetTextFont(42);
        myt_c10_cd1 ->SetTextSize(0.04);
        myt_c10_cd1 ->SetTextColor(kRed);
        listOfLines_c10_cd1->Add(myt_c10_cd1);
        h2_S2_S1_TBA_corr_ratio_tdrift->SetStats(0);
        gPad->Modified();


        cd_i = 1;
        c10->cd(cd_i + 1);
        h2_S2_S1_ratio_tdrift->Draw();
        h2_S2_S1_ratio_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_S1_ratio_tdrift->GetYaxis()->SetTitle("S2/S1");
        h2_S2_S1_ratio_tdrift->Draw("colz");
        h2_S2_S1_ratio_tdrift->GetXaxis()->SetRangeUser(10, 65);
        gPad->Update();

        TProfile *prof_h2_S2_S1_ratio_tdrift = h2_S2_S1_ratio_tdrift->ProfileX();
        prof_h2_S2_S1_ratio_tdrift->Draw("same");
        prof_h2_S2_S1_ratio_tdrift->SetMarkerStyle(20);
        prof_h2_S2_S1_ratio_tdrift->SetMarkerColor(kBlack);

        st_h1_S2_c10[cd_i] = (TPaveStats*)h2_S2_S1_ratio_tdrift->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_c10[cd_i]->SetX1NDC(0.45); st_h1_S2_c10[cd_i]->SetX2NDC(0.89);
        st_h1_S2_c10[cd_i]->SetY1NDC(0.8); st_h1_S2_c10[cd_i]->SetY2NDC(0.89);

        TF1 *f1_exp_purity_3 = new TF1("f1_exp_purity_3","exp([0] + x*[1])",0,150);
        prof_h2_S2_S1_ratio_tdrift->Fit("f1_exp_purity_3","R","",left_lim,right_lim);
        ostringstream TPaveStats_fit_info_l1_3;
        TPaveStats_fit_info_l1_3 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_exp_purity_3->GetParameter(1) << " +- " << f1_exp_purity_3->GetParError(1)/pow(f1_exp_purity_3->GetParameter(1), 2.0) << " us";

        st_h1_S2_c10[cd_i] = (TPaveStats*)gPad->GetPrimitive("stats");
        st_h1_S2_c10[cd_i]->SetName("mystats_c2_cd1");

        TList *listOfLines_3 = st_h1_S2_c10[cd_i]->GetListOfLines();

        TText *tconst_3 = st_h1_S2_c10[cd_i]->GetLineWith("Std");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_c10[cd_i]->GetLineWith("Std");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_c10[cd_i]->GetLineWith("Mean");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_c10[cd_i]->GetLineWith("Mean");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_c10[cd_i]->GetLineWith("h2");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_c10[cd_i]->GetLineWith("Entries");
        listOfLines_3->Remove(tconst_3);

        TLatex *myt_3 = new TLatex(0,0,TPaveStats_fit_info_l1_3.str().c_str());
        myt_3 ->SetTextFont(42);
        myt_3 ->SetTextSize(0.04);
        myt_3 ->SetTextColor(kRed);
        listOfLines_3->Add(myt_3);
        h2_S2_S1_ratio_tdrift->SetStats(0);
        gPad->Modified();


        cd_i = 3;
        c10->cd(cd_i + 1);
        auto legend = new TLegend(0.4, 0.75, 0.99, 1.0);
        TH1F *prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp = (TH1F*) prof_h2_S2_S1_TBA_corr_ratio_tdrift->Clone();
        prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp->Draw("HIST E P");
        prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp->SetTitle("");
        prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp->SetMarkerColor(kRed);
        prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp->SetStats(0);
        prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp->GetXaxis()->SetRangeUser(10, 65);
        prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp->GetYaxis()->SetRangeUser(14, 21.5);
        legend->AddEntry(prof_h2_S2_S1_TBA_corr_ratio_tdrift_cp, "S2/S1(TBA corrected) vs. Tdrift", "lp");

        TH1F *prof_h2_S2_S1_ratio_tdrift_cp = (TH1F*) prof_h2_S2_S1_ratio_tdrift->Clone();
        prof_h2_S2_S1_ratio_tdrift_cp->Draw("same HIST E P");
        prof_h2_S2_S1_ratio_tdrift_cp->SetMarkerColor(kBlue);
        prof_h2_S2_S1_ratio_tdrift_cp->SetStats(0);
        legend->AddEntry(prof_h2_S2_S1_ratio_tdrift_cp, "S2/S1 vs. Tdrift", "lp");

        TH1F *prof_h2_S2_total_tdrift_cp = (TH1F*) prof_h2_S2_total_tdrift->Clone();
        prof_h2_S2_total_tdrift_cp->Scale(1.0/h1_S1_total->GetMean());
        prof_h2_S2_total_tdrift_cp->Draw("same HIST E P");
        prof_h2_S2_total_tdrift_cp->SetMarkerColor(kGreen);
        prof_h2_S2_total_tdrift_cp->SetStats(0);
        legend->AddEntry(prof_h2_S2_total_tdrift_cp, "S2/S1_mean vs. Tdrift", "lp");
        legend->Draw();


        c10->cd(5);
        h2_S1_TBA->Draw("colz");
        h2_S1_TBA->GetXaxis()->SetTitle("S1 TBA");
        h2_S1_TBA->GetYaxis()->SetTitle("S1 [PE]");
        h2_S1_TBA->SetStats(0);
        h2_S1_TBA->GetXaxis()->SetRangeUser(-0.6, 0.2);

        prof_h2_S1_TBA->Draw("same");
        prof_h2_S1_TBA->SetMarkerStyle(20);
        prof_h2_S1_TBA->SetMarkerColor(kBlack);

        //test interpolator
        int n = 100;
        vector<double> x;
        vector<double> y;
        for (int i = 0; i < n; i++)
        {
            double x_tmp = 2.0/(n-1)*i - 1;
            x.push_back(x_tmp);
            y.push_back(S1_TBA_inter.Eval(x_tmp));
            //cout << i << "\t" << x_tmp << "\t" << S1_TBA_inter.Eval(x_tmp) << endl;
        }
        TGraph *gr = new TGraph(n, &x[0], &y[0]);
        gr->SetLineColor(kRed);
        gr->Draw("same");
        gPad->Modified(); gPad->Update();

        c10->cd(6);
        h2_S1_TBA_corr->Draw("colz");
        h2_S1_TBA_corr->GetXaxis()->SetTitle("S1 TBA");
        h2_S1_TBA_corr->GetYaxis()->SetTitle("S1 [PE]");
        h2_S1_TBA_corr->GetYaxis()->SetRangeUser(S1_low_cut, S1_high_cut);
        h2_S1_TBA_corr->GetXaxis()->SetRangeUser(-0.6, 0.2);
        h2_S1_TBA_corr->SetStats(0);

        TProfile *prof_h2_S1_TBA_corr = h2_S1_TBA_corr->ProfileX();
        prof_h2_S1_TBA_corr->Draw("same");
        prof_h2_S1_TBA_corr->SetMarkerStyle(20);
        prof_h2_S1_TBA_corr->SetMarkerColor(kBlack);
        gPad->Modified(); gPad->Update();
    }


    if(draw_plots.find("S2_bottom_vec") != std::string::npos)
    {
        TCanvas *c11 = new TCanvas("S2_bottom_uniformity_ch","S2_bottom_uniformity_ch");
        c11->Divide(2,2,0.01,0.01);
        vector<TPaveStats*> st_general_analysis(6);
        //double S2_sum_of_means = 0;
//        for(int i = 0; i < 24; i++)
//            S2_sum_of_means += h1_S2_maxch_vec[i]->GetMean();

        for(int i = 0; i < 4; i++)
        {
            int abs_index = i;
            c11->cd(abs_index+1);
            h1_S2_bottom_vec[abs_index]->Draw();
            h1_S2_bottom_vec[abs_index]->GetXaxis()->SetTitle("[PE]");
        }
    }

}
