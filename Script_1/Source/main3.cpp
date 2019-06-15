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
//bool total_cut = false;
//bool tmp_cut = false;
TString cut_loop0_srt;
TString cut_loop1_srt;
TString cut_loop2_srt;
bool cut_loop0_bool = false;
bool cut_loop1_bool = false;
bool cut_loop2_bool = false;
#define REMEMBER_CUT_LOOP0(x) cut_loop0_srt = #x;  cut_loop0_bool = x;
#define REMEMBER_CUT_LOOP1(x) cut_loop1_srt = #x;  cut_loop1_bool = x;
#define REMEMBER_CUT_LOOP2(x) cut_loop2_srt = #x;  cut_loop2_bool = x;

//double h1_fill_value;
//TString h1_title;
//#define REMEMBER_h1_FILL_VALUE(x) h1_title = #x; h1_fill_value = x;

using namespace std;


//some predefined vars
//int run_number = 844; //for xy reco
int run_number = 1051;
double e_lifetime_us = 150.8;

//var1: 1.5 2.1 2.9 3.5
//var2:1.5 2.3 2.7 3.5
double area_cut_x1 = 1.5;//cm
double area_cut_x2 = 2.1;//cm
double area_cut_x3 = 2.9;//cm
double area_cut_x4 = 3.5;//cm
double area_cut_y1 = area_cut_x1;
double area_cut_y2 = area_cut_x2;
double area_cut_y3 = area_cut_x3;
double area_cut_y4 = area_cut_x4;


class BoolCut
{

private:


public:
    //void set_values (int,int);
    //int area() {return width*height;}

    BoolCut (vector<RDCluster*> clusters, int nc_i);

    size_t nc;

    //cuts1
    bool cls0_is_S1;
    bool cls0_is_full;
    bool S1_Am_peak_r537;
    bool S1_Am_peak_r542;
    bool S1_Am_peak_r544;
    bool S1_Am_peak_r554;
    bool is_S1_S2;
    bool is_S1_only;
    bool is_good_r537_v1;
    bool is_good_r550_v1;

    //cuts2
    bool cls0;//cluster 0
    bool cls1;//cluster 1
    bool is_S1;
    bool is_S2;
    bool is_S2_v2;


    bool cent_spot;
    bool edges;
    bool corners;
    bool region_of_S2_uniformity;
    bool region_of_S2_uniformity_run550;

    bool edge_left;
    bool edge_right;
    bool edge_bot;
    bool edge_top;

    bool corner_left_bot;
    bool corner_right_bot;
    bool corner_left_top;
    bool corner_right_top;

};

BoolCut::BoolCut (vector<RDCluster*> clusters, int nc_i)
{
    nc = clusters.size();

    //cuts1
    cls0_is_S1 = clusters.at(0)->f90 > 0.2;
    cls0_is_full = clusters.at(0)->rep == 1;
    S1_Am_peak_r537 = (clusters.at(0)->charge > 440) && (clusters.at(0)->charge < 630); // mean +- 1.5sigma using run 537
    S1_Am_peak_r542 = (clusters.at(0)->charge > 419) && (clusters.at(0)->charge < 587); // mean +- 1.5sigma using run 542
    S1_Am_peak_r544 = (clusters.at(0)->charge > 342) && (clusters.at(0)->charge < 482); // mean +- 1.5sigma using run 544
    S1_Am_peak_r554 = (clusters.at(0)->charge > 252) && (clusters.at(0)->charge < 366); // mean +- 1.5sigma using run 554
    is_S1_S2 = nc == 2 && cls0_is_full && cls0_is_S1;
    is_S1_only = nc == 1 && cls0_is_full && cls0_is_S1;


    //cuts2
    cls0 = nc_i == 0;//cluster 0
    cls1 = nc_i == 1;//cluster 1
    is_S1 =  nc == 2 && cls0 && cls0_is_full && cls0_is_S1;
    is_S2 =  nc == 2 && cls1 && cls0_is_full && cls0_is_S1;

    is_S2_v2 = false;
    if(nc == 2)
        is_S2_v2 = is_S2 && clusters.at(1)->f90 < 0.2;

    cent_spot = (clusters.at(nc_i)->bar_x > area_cut_x2) && (clusters.at(nc_i)->bar_x < area_cut_x3) &&
            (clusters.at(nc_i)->bar_y > area_cut_y2) && (clusters.at(nc_i)->bar_y < area_cut_y3);

    //edges
    edge_left = (clusters.at(nc_i)->bar_x > area_cut_x1) && (clusters.at(nc_i)->bar_x < area_cut_x2) &&
            (clusters.at(nc_i)->bar_y > area_cut_y2) && (clusters.at(nc_i)->bar_y < area_cut_y3);

    edge_right = (clusters.at(nc_i)->bar_x > area_cut_x3) && (clusters.at(nc_i)->bar_x < area_cut_x4) &&
            (clusters.at(nc_i)->bar_y > area_cut_y2) && (clusters.at(nc_i)->bar_y < area_cut_y3);

    edge_bot = (clusters.at(nc_i)->bar_x > area_cut_x2) && (clusters.at(nc_i)->bar_x < area_cut_x3) &&
            (clusters.at(nc_i)->bar_y > area_cut_y1) && (clusters.at(nc_i)->bar_y < area_cut_y2);

    edge_top = (clusters.at(nc_i)->bar_x > area_cut_x2) && (clusters.at(nc_i)->bar_x < area_cut_x3) &&
            (clusters.at(nc_i)->bar_y > area_cut_y3) && (clusters.at(nc_i)->bar_y < area_cut_y4);

    edges = edge_left || edge_right || edge_bot || edge_top;

    //corners
    corner_left_bot = (clusters.at(nc_i)->bar_x > area_cut_x1) && (clusters.at(nc_i)->bar_x < area_cut_x2) &&
            (clusters.at(nc_i)->bar_y > area_cut_y1) && (clusters.at(nc_i)->bar_y < area_cut_y2);

    corner_right_bot = (clusters.at(nc_i)->bar_x > area_cut_x3) && (clusters.at(nc_i)->bar_x < area_cut_x4) &&
            (clusters.at(nc_i)->bar_y > area_cut_y1) && (clusters.at(nc_i)->bar_y < area_cut_y2);

    corner_left_top = (clusters.at(nc_i)->bar_x > area_cut_x1) && (clusters.at(nc_i)->bar_x < area_cut_x2) &&
            (clusters.at(nc_i)->bar_y > area_cut_y3) && (clusters.at(nc_i)->bar_y < area_cut_y4);

    corner_right_top = (clusters.at(nc_i)->bar_x > area_cut_x3) && (clusters.at(nc_i)->bar_x < area_cut_x4) &&
            (clusters.at(nc_i)->bar_y > area_cut_y3) && (clusters.at(nc_i)->bar_y < area_cut_y4);





    corners = corner_left_bot || corner_right_bot || corner_left_top || corner_right_top;

    if(nc == 2)
    {
        region_of_S2_uniformity = (clusters.at(1)->bar_x > 2.2) && (clusters.at(1)->bar_x < 2.8) &&
                (clusters.at(1)->bar_y > 2) && (clusters.at(1)->bar_y < 3);

        region_of_S2_uniformity_run550 = (clusters.at(1)->bar_x > 2.1) && (clusters.at(1)->bar_x < 2.9) &&
                (clusters.at(1)->bar_y > 2.3) && (clusters.at(1)->bar_y < 3);
    }
    else
    {
       region_of_S2_uniformity = false;
       region_of_S2_uniformity_run550 = false;
    }




    //complex cuts;
    is_good_r537_v1 = is_S1_S2 && region_of_S2_uniformity && clusters.at(0)->charge > 300 && clusters.at(0)->charge < 700;
    is_good_r550_v1 = is_S1_S2 && region_of_S2_uniformity_run550 && clusters.at(0)->charge > 200 && clusters.at(0)->charge < 450;
}



vector<double> Get_Cov_r(vector<double> xv, vector<double> yv)
{
    vector<double> result;

    double mean_x = TMath::Mean(xv.begin(), xv.end());
    double mean_y = TMath::Mean(yv.begin(), yv.end());
    double rms_x = TMath::RMS(xv.begin(), xv.end());
    double rms_y = TMath::RMS(yv.begin(), yv.end());

    vector<double> tmp;
    for(int i = 0; i < xv.size(); i++)
    {
        tmp.push_back( (xv[i] - mean_x) * (yv[i] - mean_y) );
    }

    double cov = TMath::Mean(tmp.begin(), tmp.end());
    double r = cov / (rms_x * rms_y);

    result.push_back( cov );
    result.push_back( r );

    return result;
}

// define a gauss function with 3 + 1 parameters
Double_t gauss_3p_1p(Double_t *x,Double_t *par)
{
    Double_t arg = 0;
    if (par[2]!=0)
    {
        arg = (x[0] - par[1])/par[2];
    }
//    if (par[1]!=0)
//    {
//        par[3] = par[2]/par[1];
//    }

    Double_t fitval = par[0]*TMath::Exp(-0.5*arg*arg);
    return fitval;
}




int main3(/*int argc, char *argv[]*/)
{
    //start code
    std::cout << "You are in program." << std::endl;

    //
    TF1 *func_gauss_3p_1p = new TF1("gauss_3p_1p",gauss_3p_1p,-1000,10000,3);
    //func_gauss_3p_1p->SetParNames ("Constant","Mean","Sigma","Sigma/Mean");
    func_gauss_3p_1p->SetParNames ("Constant","Mean","Sigma");

    //main code
    ostringstream path_root_file;
//    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_V/v4/" << "run_" << run_number << ".root";
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";
    TString filename = path_root_file.str().c_str();

    ostringstream path_file_out_s1_tdrift;
    path_file_out_s1_tdrift << "/home/vlad/Reports/S1_LY/s1_tdrift/run_" << run_number << ".txt";
    ofstream file_out_s1_tdrift(path_file_out_s1_tdrift.str().c_str());

    ostringstream path_file_out_s1_bottom_tdrift;
    path_file_out_s1_bottom_tdrift << "/home/vlad/Reports/S1_LY/s1_bottom_tdrift/run_" << run_number << ".txt";
    ofstream file_out_s1_bottom_tdrift(path_file_out_s1_bottom_tdrift.str().c_str());

    ostringstream path_file_out_s1_top_tdrift;
    path_file_out_s1_top_tdrift << "/home/vlad/Reports/S1_LY/s1_top_tdrift/run_" << run_number << ".txt";
    ofstream file_out_s1_top_tdrift(path_file_out_s1_top_tdrift.str().c_str());

    ostringstream path_file_out_s2_uniformity;
    path_file_out_s2_uniformity << "/home/vlad/Reports/XY/S2_uniformity/run_" << run_number << ".txt";
    //ofstream file_out_s2_uniformity(path_file_out_s2_uniformity.str().c_str());

    bool read_corrections = false;
    ofstream file_out_x_corrections;
    ostringstream path_file_out_x_corrections;
    path_file_out_x_corrections << "/home/vlad/Reports/XY/xy_corrections/run_" << run_number << "/x_corr" <<".txt";
    if(!read_corrections)
    {
        file_out_x_corrections.open(path_file_out_x_corrections.str().c_str());
    }

    ifstream file_in_x_corrections;
    if(read_corrections)
    {
        file_in_x_corrections.open(path_file_out_x_corrections.str().c_str());
        if ( !file_in_x_corrections.is_open() )
        {
            cout << "could not open file: " << path_file_out_x_corrections.str().c_str() << endl;
            return 1;
        }
        else
        {
            cout << "File has been opened correctly: " << path_file_out_x_corrections.str().c_str() << endl;
        }
    }


    cout << path_file_out_x_corrections.str() << endl;

    TFile *f = new TFile(filename, "read");
    if (!(f->IsOpen()))
    {
        cout << "could not open file: " << filename << endl;
        return 1;
    }
    else
    {
        cout << "File has been opened correctly: " << filename << endl;
    }

    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    double S1_max = 2500;
    double S2_max = 3500;
    double S2_S1_max = 4;
    //double range_scale = 1;
    double left_lim = 15 /*15*/;
    double right_lim = 55 /*55*/;


    TH1F *h1_nc = new TH1F("h1_nc", "h1_nc", 200, 0, 10);

    //time stability
    TH2F *h2_S2_event = new TH2F("h2_S2_event", "h2_S2_event", 110, 0, 110000, 150, 0, 1E4);

    //S1_ij
    TH2F *h2_S1_ij = new TH2F("h2 S1_i vs S1_j", "h2 S1_i vs S1_j", 150, 0, S1_max, 150, 0, S1_max);//S1_i vs S1_j
    TH2F *h2 = new TH2F("h2 XY S2", "h2 title", 200, 0, 5, 200, 0, 5);//XY S2
    TH2F *h2_S1 = new TH2F("h2 XY S1", "h2 title", 200, 0, 5, 200, 0, 5);//XY S1
    vector<double> S1_i_v;
    vector<double> S1_j_v;

    //S2_ij
    TH2F *h2_S2_ij = new TH2F("h2_S2_ij", "h2_S2_ij", 150, 0, S2_max, 150, 0, S2_max);
    vector<double> S2_i_v;
    vector<double> S2_j_v;

    TH2F *h2_S1_S2 = new TH2F("h2_S1_S2", "h2_S1_S2", 150, 0, S1_max, 150, 0, S2_max);

    TH1F *h1;

    //TBA
    TH2F *h2_S1_TBA = new TH2F("h2_S1_TBA", "h2_S1_TBA", 200, -0.5, 0.5, 200, 0, S1_max);
    TH2F *h2_Tdrift_TBA = new TH2F("h2_Tdrift_TBA", "h2_Tdrift_TBA", 200, -0.5, 0.5, 100, 0, 100);
    vector<double> v_S1;
    vector<double> v_TBA;


    TH1F *h1_S1_top = new TH1F("h1_S1_top", "h1_S1_top", 200, 0, S1_max*0.6);//Am S1
    TH1F *h1_S1_bot = new TH1F("h1_S1_bot", "h1_S1_bot", 200, 0, S1_max*0.6);//Am S1
    TH1F *h1_S1 = new TH1F("h1 S1", "h1 title", 400, 0, S1_max);//Am S1 charge
    //TH1F *h1_S1 = new TH1F("h1 S1", "h1 title", 400, -100, 2000*range_scale);//Am S1 charge

    TH1F *h1_S2 = new TH1F("h1 S2", "h1 title", 400, -100, S2_max);//Am S2 charge
    TH1F *h1_S2_top = new TH1F("h1 S2 top", "h1 title", 200, -100, S2_max/2.0 );
    TH1F *h1_S2_bot = new TH1F("h1 S2 bot", "h1 title", 200, -100, S2_max/2.0 );
    TH1F *h1_S2_S1_ratio = new TH1F("h1 S2/S1", "h1 title", 300, -1, 30);//Am S2_charge/S1_charge
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 8000);//Am S2 tot_charge_top or tot_charge_bottom
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 1000);//Am S1 tot_charge_top or tot_charge_bottom
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 400, -0.2, 1);//f90
    TH1F *h1_S2_f90 = new TH1F("h1_S2_f90", "h1_S2_f90", 400, -0.2, 1);//f90
    //TH1F *h1 = new TH1F("h1 x or y", "h1 title", 400, -1, 5);//x ix or iy
    TH1F *h1_Tdrift = new TH1F("h1 Tdrift", "h1 Tdrift", 71, -1, 70);//
    //h1 = h1_Tdrift;
    TH1F *h1_S2_tot_purity_corr = new TH1F("h1_S2_tot_purity_corr", "h1_S2_tot_purity_corr", 400, -100, S2_max*1.5);
    TH1F *h1_S2_top_purity_corr = new TH1F("h1_S2_top_purity_corr", "h1_S2_top_purity_corr", 200, -100, S2_max*1.5/2.0);
    TH1F *h1_S2_bottom_purity_corr = new TH1F("h1_S2_bottom_purity_corr", "h1_S2_bottom_purity_corr", 200, -100, S2_max*1.5/2.0);
    vector<TH1F*> h1_S2_maxch_vec;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S2_maxch_vec" << i;
        h1_S2_maxch_vec.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 400, -100, S2_max) );
    }
    TGraph* gr_S2_maxch;


    TH2F *h2_S2_tdrift = new TH2F("h2_S2_tdrift", "h2_S2_tdrift", 150, 0, 100, 200, 0, S2_max);
    TH2F *h2_S2_top_tdrift = new TH2F("h2_S2_top_tdrift", "h2_S2_top_tdrift", 150, 0, 100, 200, 0, S2_max/1.5);
    TH2F *h2_S2_bottom_tdrift = new TH2F("h2_S2_bottom_tdrift", "h2_S2_bottom_tdrift", 150, 0, 100, 200, 0, S2_max/1.5);
    TH2F *h2_S2_S1_tdrift = new TH2F("h2_S2_S1_tdrift", "h2_S2_S1_tdrift", 150, 0, 100, 200, 0, S2_S1_max);
    TH2F *h2_S1_tdrift = new TH2F("h2_S1_tdrift", "h2_S1_tdrift", 150, 0, 100, 150, 0, S1_max);
    TH2F *h2_S1_bot_tdrift = new TH2F("h2_S1_bot_tdrift", "h2_S1_bot_tdrift", 150, 0, 100, 150, 0, S1_max/2.0);
    TH2F *h2_S1_top_tdrift = new TH2F("h2_S1_top_tdrift", "h2_S1_top_tdrift", 150, 0, 100, 150, 0, S1_max/2.0);

    //xy
    vector<TH1F*> h1_x_bar_v;
    h1_x_bar_v.push_back(new TH1F("h1_x_bar_v_#0", "h1_x_bar_v_#0", 250, -1, 6));
    h1_x_bar_v.push_back(new TH1F("h1_x_bar_v_#1", "h1_x_bar_v_#1", 250, -1, 6));
    h1_x_bar_v.push_back(new TH1F("h1_x_bar_v_#2", "h1_x_bar_v_#2", 250, -1, 6));
    h1_x_bar_v.push_back(new TH1F("h1_x_bar_v_#3", "h1_x_bar_v_#4", 250, -1, 6));
    TH2F *h2_xmaxch_xbar = new TH2F("h2_xmaxch_xbar", "h2_xmaxch_xbar", 50, -1, 6, 50, -1, 6);
    TH2F *h2_xbar_xmaxch = new TH2F("h2_xbar_xmaxch", "h2_xbar_xmaxch", 50, -1, 6, 50, -1, 6);
    TH3F *h3_xmaxch_xbar_ybar = new TH3F("h3_xmaxch_xbar_ybar", "h3_xmaxch_xbar_ybar", 50, -1, 6, 50, -1, 6, 50, -1, 6);
    vector<TH1F*> h1_LRF_v;
    for(int ih1 = 0; ih1 < 24; ih1++)
    {
        ostringstream h1_name;
        h1_name << "h1_LRF_" << ih1;
        h1_LRF_v.push_back( new TH1F(h1_name.str().c_str(), h1_name.str().c_str(), 100, 0, 0.3) );
    }
    vector<double> x_centers = {0.625, 1.875, 3.125, 4.375};
    vector<double> y_centers = {0.416667, 1.25, 2.08333, 2.91667, 3.75, 4.58333};
    int n_events_test = 0;

    vector<double> S2_v;
    vector<double> event_v;

    int n_bins_h1_S1_bottom = 200;
    TH1F *h1_S1_bottom_chF2 = new TH1F("h1_S1_bottom_chF2", "h1 title", n_bins_h1_S1_bottom, -30, 200);//Am S1 charge_bottom
    TH1F *h1_S1_bottom_chF3 = new TH1F("h1_S1_bottom_chF3", "h1 title", n_bins_h1_S1_bottom, -30, 200);//Am S1 charge_bottom
    TH1F *h1_S1_bottom_chF4 = new TH1F("h1_S1_bottom_chF4", "h1 title", n_bins_h1_S1_bottom, -30, 200);//Am S1 charge_bottom
    TH1F *h1_S1_bottom_chF5 = new TH1F("h1_S1_bottom_chF5", "h1 title", n_bins_h1_S1_bottom, -30, 200);//Am S1 charge_bottom

    int n_bins_h1_S1_top = 200;
    double xmin_h1_S1_top = -30;
    double xmax_h1_S1_top = 200;
    TH1F *h1_S1_top_A1_A2_B1_B2_C1_C2 = new TH1F("h1_S1_top_A1_A2_B1_B2_C1_C2", "h1 title", n_bins_h1_S1_top, xmin_h1_S1_top, xmax_h1_S1_top);//Am S1 charge_top
    TH1F *h1_S1_top_C5_D1_D5_E2_E3_E4 = new TH1F("h1_S1_top_C5_D1_D5_E2_E3_E4", "h1 title", n_bins_h1_S1_top, xmin_h1_S1_top, xmax_h1_S1_top);//Am S1 charge_top
    TH1F *h1_S1_top_A3_A4_A5_B4_B3_B5 = new TH1F("h1_S1_top_A3_A4_A5_B4_B3_B5", "h1 title", n_bins_h1_S1_top, xmin_h1_S1_top, xmax_h1_S1_top);//Am S1 charge_top
    TH1F *h1_S1_top_C4_C3_D4_D3_D2_E5 = new TH1F("h1_S1_top_C4_C3_D4_D3_D2_E5", "h1 title", n_bins_h1_S1_top, xmin_h1_S1_top, xmax_h1_S1_top);//Am S1 charge_top

    TH1F *h1_S1_top_half_A1_C5 =new TH1F("h1_S1_top_half_A1_C5", "h1 title", 200, xmin_h1_S1_top, 300);
    TH1F *h1_S1_top_half_A3_C4 =new TH1F("h1_S1_top_half_A3_C4", "h1 title", 200, xmin_h1_S1_top, 300);

    TH1F *h1_S1_top_C5_D5_E3 = new TH1F("h1_S1_top_C5_D5_E3", "h1 title", 100, xmin_h1_S1_top, 80);
    TH1F *h1_S1_top_D1_E2_E4 = new TH1F("h1_S1_top_D1_E2_E4", "h1 title", 100, xmin_h1_S1_top, 80);

    TH1F *h1_S1_top_C5_D5 = new TH1F("h1_S1_top_C5_D5", "h1 title", 100, -20, 80);


    vector<TH1F *> h1_S1_top_vec;
    for(int ih1 = 0; ih1 < 23; ih1++)
    {
        ostringstream h1_S1_top_name;
        h1_S1_top_name << "h1_S1_top_ch" << ih1;
        h1_S1_top_vec.push_back( new TH1F(h1_S1_top_name.str().c_str(), h1_S1_top_name.str().c_str(), 120, -10, 40) );
    }


    vector<bool> is_in_cut(data->GetEntries(), false);



    //baseline
    vector<TH1F*> beseline_vec;

    //first event loop
    TString total_cut_srt_loop1_0;
    TString total_cut_srt_loop1;
    //TString total_cut_srt_loop2;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        //cout << "line 106" << endl;
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();
        h1_nc->Fill(nc);
        //cout << "Found n. " << nc << " clusters" << endl;
        double Tdrift = -1;

        if(nc)
        {

            BoolCut C0(clusters, 0);
            total_cut_srt_loop1_0 = "C0.nc == 2";

            if(C0.nc == 2)
            {
                //if(clusters.at(1)->f90 < 0.2)
                {
                    Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;
                    //h1_Tdrift->Fill(Tdrift);
                }
            }


            for(int nc_i = 0; nc_i < nc; nc_i++)
            {
                double radius = sqrt( pow(clusters.at(nc_i)->bar_x - 2.62, 2.0) + pow(clusters.at(nc_i)->bar_y - 2.54, 2.0) );

                //total_cut = cuts(clusters, nc_i);
                BoolCut C1(clusters, nc_i);


                //C1.is_S2_v2 && clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800
                //REMEMBER_CUT_LOOP1(C1.is_S2_v2 && clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800);
                //C1.nc == 2 && C1.cls1 && C1.cls0_is_full && clusters.at(0)->f90 > 0.3 && clusters.at(0)->f90 < 0.35
                REMEMBER_CUT_LOOP1(C1.nc == 2 && C1.cls1);

                if ( cut_loop1_bool ) //cuts
                {
                    is_in_cut[ev] = true;

                    S2_v.push_back(clusters.at(nc_i)->charge);

                    h1_Tdrift->Fill(Tdrift);
                    h2->Fill(clusters.at(nc_i)->bar_x, clusters.at(nc_i)->bar_y);
                    h1_S2->Fill(clusters.at(nc_i)->charge);
                    h1_S2_top->Fill(clusters.at(nc_i)->tot_charge_top);
                    h1_S2_bot->Fill(clusters.at(nc_i)->tot_charge_bottom);
                    h1_S2_f90->Fill(clusters.at(nc_i)->f90);
                    h2_S2_ij->Fill(clusters.at(nc_i)->tot_charge_top, clusters.at(nc_i)->tot_charge_bottom);
                    S2_i_v.push_back(clusters.at(nc_i)->tot_charge_top);
                    S2_j_v.push_back(clusters.at(nc_i)->tot_charge_bottom);
                    h2_S1_S2->Fill(clusters.at(0)->charge, clusters.at(1)->charge);
                    h2_S2_tdrift->Fill(Tdrift, clusters.at(1)->charge);
                    h2_S2_top_tdrift->Fill(Tdrift, clusters.at(1)->tot_charge_top);
                    h2_S2_bottom_tdrift->Fill(Tdrift, clusters.at(1)->tot_charge_bottom);

                    h2_S2_S1_tdrift->Fill(Tdrift, clusters.at(1)->charge/clusters.at(0)->charge);
                    h2_S1_tdrift->Fill(Tdrift, clusters.at(0)->charge);
                    //file_out_s1_tdrift << Tdrift << "\t" << clusters.at(0)->charge << endl;
                    h2_S1_top_tdrift->Fill(Tdrift, clusters.at(0)->tot_charge_top);
                    h2_S1_bot_tdrift->Fill(Tdrift, clusters.at(0)->tot_charge_bottom);

                    double purity_corr = exp(Tdrift/e_lifetime_us);
                    h1_S2_tot_purity_corr->Fill(clusters.at(nc_i)->charge * purity_corr);
                    h1_S2_top_purity_corr->Fill(clusters.at(nc_i)->tot_charge_top * purity_corr);
                    h1_S2_bottom_purity_corr->Fill(clusters.at(nc_i)->tot_charge_bottom * purity_corr);


                    h2_xmaxch_xbar->Fill(clusters.at(1)->bar_x, clusters.at(1)->pos_x);
                    h2_xbar_xmaxch->Fill(clusters.at(1)->pos_x, clusters.at(1)->bar_x);
                    h3_xmaxch_xbar_ybar->Fill(clusters.at(1)->bar_x, clusters.at(1)->bar_y, clusters.at(1)->pos_x);


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
                                h1_S2_maxch_vec[abs_index]->Fill(clusters.at(1)->charge * purity_corr);
                                n_events_test++;
//                                if(ev == 5)
//                                {
//                                    cout << clusters.at(1)->pos_x << "\t" << clusters.at(1)->pos_y << "\t" << k << "\t" << j << "\t" << x_centers[k] << "\t" << y_centers[j] << endl;
//                                }
                            }
                        }
                    }


//                    if(ev < 100)
//                    {
//                        cout << clusters.at(1)->pos_y << endl;
//                    }



                    if(clusters.at(1)->pos_x == 0.625)
                        h1_x_bar_v[0]->Fill(clusters.at(1)->bar_x);

                    if(clusters.at(1)->pos_x == 1.875)
                        h1_x_bar_v[1]->Fill(clusters.at(1)->bar_x);

                    if(clusters.at(1)->pos_x == 3.125)
                        h1_x_bar_v[2]->Fill(clusters.at(1)->bar_x);

                    if(clusters.at(1)->pos_x == 4.375)
                        h1_x_bar_v[3]->Fill(clusters.at(1)->bar_x);

                    for(int ih1 = 0; ih1 < 24; ih1++)
                    {
                        h1_LRF_v[ih1]->Fill(clusters.at(nc_i)->charge_top[ih1]/clusters.at(nc_i)->tot_charge_top);
                        h1_LRF_v[ih1]->SetTitle(cut_loop2_srt);
                    }


                }


            }   // end nc_i loop

            if(is_in_cut[ev])
            {
                event_v.push_back(ev);
                h2_S2_event->Fill(ev, clusters.at(1)->charge);
            }


        }// end if(nc)

        if(ev%5000==0) cout << "Event " << ev << " processed" << endl;
    }


    //second event loop
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        if(is_in_cut[ev])
        {
            double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

            for(int nc_i = 0; nc_i < nc; nc_i++)
            {

                BoolCut C2(clusters, nc_i);
                REMEMBER_CUT_LOOP2(C2.cls0);
                if(cut_loop2_bool)
                {
                    h1_S1_f90->Fill(clusters.at(nc_i)->f90);

                    h2_S1->Fill(clusters.at(nc_i)->bar_x, clusters.at(nc_i)->bar_y);
                    h1_S1->Fill(clusters.at(nc_i)->charge);

                    {
                        h1_S1_bottom_chF2->Fill(clusters.at(nc_i)->charge_bottom[0]);
                        h1_S1_bottom_chF3->Fill(clusters.at(nc_i)->charge_bottom[1]);
                        h1_S1_bottom_chF4->Fill(clusters.at(nc_i)->charge_bottom[2]);
                        h1_S1_bottom_chF5->Fill(clusters.at(nc_i)->charge_bottom[3]);


                        double S1_top_A1_A2_B1_B2_C1_C2 = clusters.at(nc_i)->charge_top[0] + clusters.at(nc_i)->charge_top[1] +
                                0 + clusters.at(nc_i)->charge_top[5] +
                                clusters.at(nc_i)->charge_top[9] + clusters.at(nc_i)->charge_top[10];

                        double S1_top_A3_A4_A5_B4_B3_B5 = /*clusters.at(nc_i)->charge_top[2]*/ + clusters.at(nc_i)->charge_top[3] +
                                clusters.at(nc_i)->charge_top[4] + clusters.at(nc_i)->charge_top[7] +
                                clusters.at(nc_i)->charge_top[6] + clusters.at(nc_i)->charge_top[8];

                        double S1_top_C5_D1_D5_E2_E3_E4 = clusters.at(nc_i)->charge_top[13] + clusters.at(nc_i)->charge_top[14] +
                                clusters.at(nc_i)->charge_top[18] + clusters.at(nc_i)->charge_top[19] +
                                clusters.at(nc_i)->charge_top[20] + clusters.at(nc_i)->charge_top[21];


                        double S1_top_C4_C3_D4_D3_D2_E5 = /*clusters.at(nc_i)->charge_top[12]*/ + clusters.at(nc_i)->charge_top[11] +
                                clusters.at(nc_i)->charge_top[17] + clusters.at(nc_i)->charge_top[16] +
                                clusters.at(nc_i)->charge_top[15] + clusters.at(nc_i)->charge_top[22];


                        double S1_top_D1_E2_E4 = clusters.at(nc_i)->charge_top[14] + clusters.at(nc_i)->charge_top[19] +
                                clusters.at(nc_i)->charge_top[21];

                        double S1_top_C5_D5_E3 = clusters.at(nc_i)->charge_top[13] + clusters.at(nc_i)->charge_top[18] +
                                clusters.at(nc_i)->charge_top[20];


                        h1_S1_top_D1_E2_E4->Fill(S1_top_D1_E2_E4);
                        h1_S1_top_C5_D5_E3->Fill(S1_top_C5_D5_E3);

                        h1_S1_top_C5_D5->Fill(clusters.at(nc_i)->charge_top[13] + clusters.at(nc_i)->charge_top[18]);


                        h1_S1_top_A1_A2_B1_B2_C1_C2->Fill(S1_top_A1_A2_B1_B2_C1_C2);

                        h1_S1_top_A3_A4_A5_B4_B3_B5->Fill(S1_top_A3_A4_A5_B4_B3_B5);

                        h1_S1_top_C5_D1_D5_E2_E3_E4->Fill(S1_top_C5_D1_D5_E2_E3_E4);

                        h1_S1_top_C4_C3_D4_D3_D2_E5->Fill(S1_top_C4_C3_D4_D3_D2_E5);

                        //h1_S1_top_half_A1_C5->Fill();

                        h1_S1_top_half_A1_C5->Fill(S1_top_A1_A2_B1_B2_C1_C2 + S1_top_C5_D1_D5_E2_E3_E4);
                        h1_S1_top_half_A3_C4->Fill(S1_top_A3_A4_A5_B4_B3_B5 + S1_top_C4_C3_D4_D3_D2_E5);

                        //TBA
                        double TBA = (clusters.at(nc_i)->tot_charge_top - clusters.at(nc_i)->tot_charge_bottom) / (clusters.at(nc_i)->tot_charge_top + clusters.at(nc_i)->tot_charge_bottom);
                        h2_S1_TBA->Fill(TBA, clusters.at(0)->charge);
                        h2_Tdrift_TBA->Fill(TBA, Tdrift);
                        v_S1.push_back(clusters.at(0)->charge);
                        v_TBA.push_back(TBA);

                        //choose i and j channels
                        double S1_i = clusters.at(nc_i)->tot_charge_top;
                        double S1_j = clusters.at(nc_i)->tot_charge_bottom;
                        S1_i_v.push_back(S1_i);
                        S1_j_v.push_back(S1_j);
                        h2_S1_ij->Fill(S1_i, S1_j);

                        //
                        for(int ih1 = 0; ih1 < 23; ih1++)
                        {
                            h1_S1_top_vec[ih1]->Fill(clusters.at(nc_i)->charge_top[ih1]);
                            h1_S1_top_vec[ih1]->SetTitle(cut_loop2_srt);
                        }



                        h1_S1_top->Fill(clusters.at(nc_i)->tot_charge_top);
                        h1_S1_bot->Fill(clusters.at(nc_i)->tot_charge_bottom);



                    }

                }//if(C2.is_S1)

            }//for nc_i

            if(nc > 1)
                h1_S2_S1_ratio->Fill(clusters.at(1)->charge / clusters.at(0)->charge);

        }//if(is_in_cut[ev])
    }


    //Draw options

    bool is_draw_var6 = 1;


    if(is_draw_var6)
    {
        TCanvas *c1 = new TCanvas("c1","c1");

        //v4
        c1->Divide(4,4,0.01,0.01);


        c1->cd(1);
        h2->SetTitle(cut_loop1_srt);
        h2->GetXaxis()->SetTitle("x [cm]");
        h2->GetYaxis()->SetTitle("y [cm]");
        h2->GetXaxis()->SetRangeUser(0, 5);
        h2->GetYaxis()->SetRangeUser(0, 5);
        h2->Draw("colz");
        //h2->SetStats(0); //delete statbox
        gPad->Update();
        TPaveStats *st = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
        st->SetX1NDC(0.12); st->SetX2NDC(0.35);
        st->SetY1NDC(0.72); st->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();


        c1->cd(2);
//        TGraph *gr_S2_event = new TGraph(event_v.size(), &event_v[0], &S2_v[0]);
//        gr_S2_event->Draw("AP");
//        gPad->SetLogy();
//        gr_S2_event->SetMarkerStyle(20);
//        gr_S2_event->SetMarkerSize(0.5);
//        gr_S2_event->GetXaxis()->SetTitle("event number");
//        gr_S2_event->GetYaxis()->SetTitle("clusters.at(nc_i)->charge [PE]");
//        gr_S2_event->SetTitle(cut_loop1_srt);
//        gr_S2_event->GetYaxis()->SetRangeUser(10, 2E5);
        h2_S2_event->SetTitle(cut_loop1_srt);
        h2_S2_event->GetXaxis()->SetTitle("event");
        h2_S2_event->GetYaxis()->SetTitle("S2");
        //h2_S2_event->GetXaxis()->SetRangeUser(1.5, 3.5);
        //h2_S2_event->GetYaxis()->SetRangeUser(2, 1E5);
        //h2_S2_event->Draw("colz");
        TProfile *prof_h2_S2_event = h2_S2_event->ProfileX();
        prof_h2_S2_event->Draw();
        //TF1 *f1_exp_purity = new TF1("f1_exp_purity","exp([0] + x*[1])",0,150);
        //prof->Fit("f1_exp_purity","R","",left_lim,right_lim);
        //cout << "e- lifetime (S1 vs T_drift) [us] = " << -1/f1_exp_purity->GetParameter(1) << " +- " << f1_exp_purity->GetParError(1)/pow(f1_exp_purity->GetParameter(1), 2.0) << endl;
        //cout <<  << endl;
        gPad->Modified(); gPad->Update();


        c1->cd(5);
        gStyle->SetOptFit(1);
        //gStyle->SetOptStat(11);
        h1_Tdrift->SetTitle(cut_loop1_srt);
        h1_Tdrift->GetXaxis()->SetTitle("(clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000 [us]");
        h1_Tdrift->Draw();
        gPad->Update();
        h1_Tdrift->GetYaxis()->SetRangeUser(0., h1_Tdrift->GetMaximum() * 1.5 );
        TPaveStats *st6  = (TPaveStats*)h1_Tdrift->GetListOfFunctions()->FindObject("stats");
        st6->SetX1NDC(0.40); st6->SetX2NDC(0.98);
        st6->SetY1NDC(0.65); st6->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();


        c1->cd(3);
        gStyle->SetOptFit(1);
        h1_S2_bot->SetTitle(cut_loop1_srt);
        h1_S2_bot->GetXaxis()->SetTitle("clusters.at(nc_i)->tot_charge_bottom [PE]");
        h1_S2_bot->Draw();
        gPad->Update();
        TPaveStats *st3  = (TPaveStats*)h1_S2_bot->GetListOfFunctions()->FindObject("stats");
        st3->SetX1NDC(0.60); st3->SetX2NDC(0.98);
        st3->SetY1NDC(0.50); st3->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();


        c1->cd(4);
        gStyle->SetOptFit(1);
        h1_S2_top->SetTitle(cut_loop1_srt);
        h1_S2_top->GetXaxis()->SetTitle("clusters.at(nc_i)->tot_charge_top [PE]");
        h1_S2_top->Draw();
        gPad->Update();
        TPaveStats *st7  = (TPaveStats*)h1_S2_top->GetListOfFunctions()->FindObject("stats");
        st7->SetX1NDC(0.60); st7->SetX2NDC(0.98);
        st7->SetY1NDC(0.50); st7->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c1->cd(7);
        h1_S2_f90->SetTitle(cut_loop2_srt);
        h1_S2_f90->GetXaxis()->SetTitle("S2");
        h1_S2_f90->Draw();
        gPad->Update();
        TPaveStats *st4  = (TPaveStats*)h1_S2_f90->GetListOfFunctions()->FindObject("stats");
        st4->SetX1NDC(0.60); st4->SetX2NDC(0.98);
        st4->SetY1NDC(0.50); st4->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c1->cd(9);
        //TH1F *h1_S1_ij = h1_S1;
        TString str_S1_i = "h1_S1_top [PE]";
        TString str_S1_j = "h1_S1_bot [PE]";
        TString str_S1_ij = "h1_S1 [PE]";
        h2_S1_ij->SetTitle(cut_loop2_srt);
        h2_S1_ij->GetXaxis()->SetTitle(str_S1_i);
        h2_S1_ij->GetYaxis()->SetTitle(str_S1_j);
        h2_S1_ij->GetXaxis()->SetRangeUser(0, 1000);
        h2_S1_ij->GetYaxis()->SetRangeUser(0, 1000);
        h2_S1_ij->Draw("colz");
        //h2->SetStats(0); //delete statbox
        gPad->Update();
        TPaveStats *st_h2_S1_ij = (TPaveStats*)h2_S1_ij->GetListOfFunctions()->FindObject("stats");
        st_h2_S1_ij->SetX1NDC(0.12); st_h2_S1_ij->SetX2NDC(0.35);
        st_h2_S1_ij->SetY1NDC(0.72); st_h2_S1_ij->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        double mean_i = TMath::Mean(S1_i_v.begin(), S1_i_v.end());
        double mean_j = TMath::Mean(S1_j_v.begin(), S1_j_v.end());
        double rms_i = TMath::RMS(S1_i_v.begin(), S1_i_v.end());
        double rms_j = TMath::RMS(S1_j_v.begin(), S1_j_v.end());
        //cout << ;
        vector<double> cov_r = Get_Cov_r(S1_i_v, S1_j_v);
        TPaveText *t_h2_S1_ij = new TPaveText(0.7,0.13,0.9,0.43,"brNDC");
        t_h2_S1_ij->AddText(Form("cov = %g", cov_r[0] ));
        t_h2_S1_ij->AddText(Form("r = %g", cov_r[1] ));
        t_h2_S1_ij->Draw();


        c1->cd(10);
        h2_S2_ij->SetTitle(cut_loop1_srt);
        h2_S2_ij->GetXaxis()->SetTitle("S2_top [PE]");
        h2_S2_ij->GetYaxis()->SetTitle("S2_bot [PE]");
        //h2_S2_ij->GetXaxis()->SetRangeUser(1.5, 3.5);
        //h2_S2_ij->GetYaxis()->SetRangeUser(1.5, 3.5);
        h2_S2_ij->Draw("colz");
        //h2->SetStats(0); //delete statbox
        gPad->Update();
        TPaveStats *st_h2_S2_ij = (TPaveStats*)h2_S2_ij->GetListOfFunctions()->FindObject("stats");
        st_h2_S2_ij->SetX1NDC(0.12); st_h2_S2_ij->SetX2NDC(0.35);
        st_h2_S2_ij->SetY1NDC(0.72); st_h2_S2_ij->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c1->cd(11);
        h2_S1_S2->SetTitle(cut_loop1_srt);
        h2_S1_S2->GetXaxis()->SetTitle("S1 [PE]");
        h2_S1_S2->GetYaxis()->SetTitle("S2 [PE]");
        //h2_S2_ij->GetXaxis()->SetRangeUser(1.5, 3.5);
        //h2_S2_ij->GetYaxis()->SetRangeUser(1.5, 3.5);
        h2_S1_S2->Draw("colz");
        //h2->SetStats(0); //delete statbox
        gPad->Update();
        TPaveStats *st_h2_S1_S2 = (TPaveStats*)h2_S1_S2->GetListOfFunctions()->FindObject("stats");
        st_h2_S1_S2->SetX1NDC(0.12); st_h2_S1_S2->SetX2NDC(0.35);
        st_h2_S1_S2->SetY1NDC(0.72); st_h2_S1_S2->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c1->cd(6);
        h1_S1_f90->SetTitle(cut_loop1_srt);
        h1_S1_f90->GetXaxis()->SetTitle("S1_f90");
        h1_S1_f90->Draw();
        gPad->Update();
        TPaveStats *st_h1_S1_f90  = (TPaveStats*)h1_S1_f90->GetListOfFunctions()->FindObject("stats");
        st_h1_S1_f90->SetX1NDC(0.60); st_h1_S1_f90->SetX2NDC(0.98);
        st_h1_S1_f90->SetY1NDC(0.50); st_h1_S1_f90->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c1->cd(12);
        h1_nc->Sumw2(kTRUE);
        h1_nc->GetXaxis()->SetTitle("nc");
        h1_nc->GetYaxis()->SetTitle("Part of events");
        //h1_nc->Scale(1, "width");
        //h1_nc->SetBarWidth(0.4);
        h1_nc->SetFillColor(kMagenta);
        h1_nc->Scale(1/h1_nc->Integral());
        h1_nc->Draw("HIST b");
        h1_nc->SetBarWidth(10);
        h1_nc->SetBarOffset(0);
        gPad->Modified(); gPad->Update();



//        c1->cd(12);
//        h2_S2_tdrift->SetTitle(cut_loop1_srt);
//        h2_S2_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
//        h2_S2_tdrift->GetYaxis()->SetTitle("S2 [PE]");
//        h2_S2_tdrift->Draw("colz");
//        c1->cd(13);
//        TProfile *prof = h2_S2_tdrift->ProfileX();
//        TF1 *f1_exp_purity = new TF1("f1_exp_purity","exp([0] + x*[1])",0,150);
//        prof->Fit("f1_exp_purity","R","",left_lim,right_lim);
//        cout << "e- lifetime (S2 vs T_drift) [us] = " << -1/f1_exp_purity->GetParameter(1) << " +- " << f1_exp_purity->GetParError(1)/pow(f1_exp_purity->GetParameter(1), 2.0) << endl;
//        //cout <<  << endl;
//        gPad->Modified(); gPad->Update();

//        c1->cd(14);
//        h2_S2_S1_tdrift->SetTitle(cut_loop1_srt);
//        h2_S2_S1_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
//        h2_S2_S1_tdrift->GetYaxis()->SetTitle("S2/S1");
//        h2_S2_S1_tdrift->Draw("colz");
//        c1->cd(15);
//        TProfile *prof_S2_S1_TDrift = h2_S2_S1_tdrift->ProfileX();
//        TF1 *f1_exp_purity_S2_S1 = new TF1("f1_exp_purity_S2_S1","exp([0] + x*[1])",0,150);
//        prof_S2_S1_TDrift->Fit("f1_exp_purity_S2_S1","R","",left_lim,right_lim);
//        cout << "e- lifetime (S2/S1 vs T_drift) [us] = " << -1/f1_exp_purity_S2_S1->GetParameter(1) << " +- " << f1_exp_purity_S2_S1->GetParError(1)/pow(f1_exp_purity_S2_S1->GetParameter(1), 2.0) << endl;
//        gPad->Modified(); gPad->Update();

        c1->cd(8);
        h1_S2->SetTitle(cut_loop1_srt);
        h1_S2->GetXaxis()->SetTitle("S2 [PE]");        
        h1_S2->Draw();
        gPad->Update();
        TPaveStats *st_h1_S2  = (TPaveStats*)h1_S2->GetListOfFunctions()->FindObject("stats");
        st_h1_S2->SetX1NDC(0.60); st_h1_S2->SetX2NDC(0.98);
        st_h1_S2->SetY1NDC(0.50); st_h1_S2->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

//        c1->cd(16);
//        h2_S1_tdrift->SetTitle(cut_loop1_srt);
//        h2_S1_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
//        h2_S1_tdrift->GetYaxis()->SetTitle("S1");
//        h2_S1_tdrift->Draw("colz");


        //////////////////////////////////////////////////////////////////////////////////////////
        TCanvas *c2 = new TCanvas("c2","c2");
        c2->Divide(3,3,0.01,0.01);
//        c2->cd(1);
//        h2_S2_tdrift->SetTitle(cut_loop1_srt);
//        h2_S2_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
//        h2_S2_tdrift->GetYaxis()->SetTitle("S2 [PE]");
//        h2_S2_tdrift->Draw("colz");
//        c2->cd(4);
//        TProfile *prof = h2_S2_tdrift->ProfileX();
//        prof->GetYaxis()->SetTitle("S2 [PE]");
//        TF1 *f1_exp_purity = new TF1("f1_exp_purity","exp([0] + x*[1])",0,150);
//        prof->Fit("f1_exp_purity","R","",left_lim,right_lim);
//        cout << "e- lifetime (S2 vs T_drift) [us] = " << -1/f1_exp_purity->GetParameter(1) << " +- " << f1_exp_purity->GetParError(1)/pow(f1_exp_purity->GetParameter(1), 2.0) << endl;
//        //cout <<  << endl;
//        gPad->Modified(); gPad->Update();

        c2->cd(2);
        h2_S1_tdrift->SetTitle(cut_loop1_srt);
        h2_S1_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S1_tdrift->GetYaxis()->SetTitle("S1");
        h2_S1_tdrift->Draw("colz");
        c2->cd(5);
        TProfile *prof_h2_S1_tdrift = h2_S1_tdrift->ProfileX();
        prof_h2_S1_tdrift->Draw();
        prof_h2_S1_tdrift->GetYaxis()->SetTitle("S1");
        //TF1 *f1_exp_purity = new TF1("f1_exp_purity","exp([0] + x*[1])",0,150);
        //prof->Fit("f1_exp_purity","R","",left_lim,right_lim);
        //cout << "e- lifetime (S2 vs T_drift) [us] = " << -1/f1_exp_purity->GetParameter(1) << " +- " << f1_exp_purity->GetParError(1)/pow(f1_exp_purity->GetParameter(1), 2.0) << endl;
        //cout <<  << endl;
        gPad->Modified(); gPad->Update();

        c2->cd(3);
        h2_S2_S1_tdrift->SetTitle(cut_loop1_srt);
        h2_S2_S1_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_S1_tdrift->GetYaxis()->SetTitle("S2/S1");
        h2_S2_S1_tdrift->Draw("colz");
        c2->cd(6);
        TProfile *prof_S2_S1_TDrift = h2_S2_S1_tdrift->ProfileX();
        prof_S2_S1_TDrift->GetYaxis()->SetTitle("S2/S1");
        TF1 *f1_exp_purity_S2_S1 = new TF1("f1_exp_purity_S2_S1","exp([0] + x*[1])",0,150);
        prof_S2_S1_TDrift->Fit("f1_exp_purity_S2_S1","R","",left_lim,right_lim);
        cout << "e- lifetime (S2/S1 vs T_drift) [us] = " << -1/f1_exp_purity_S2_S1->GetParameter(1) << " +- " << f1_exp_purity_S2_S1->GetParError(1)/pow(f1_exp_purity_S2_S1->GetParameter(1), 2.0) << endl;
        gPad->Modified(); gPad->Update();

        c2->cd(7);
        h2_S1_TBA->SetTitle(cut_loop2_srt);
        h2_S1_TBA->GetXaxis()->SetTitle("TBA");
        h2_S1_TBA->GetYaxis()->SetTitle("S1");
        h2_S1_TBA->Draw("colz");
        c2->cd(8);
        TProfile *prof_h2_S1_TBA = h2_S1_TBA->ProfileX();
        prof_h2_S1_TBA->GetYaxis()->SetTitle("S1");
        prof_h2_S1_TBA->Draw();
        //TF1 *f1_exp_purity_S2_S1 = new TF1("f1_exp_purity_S2_S1","exp([0] + x*[1])",0,150);
        //prof_S2_S1_TDrift->Fit("f1_exp_purity_S2_S1","R","",left_lim,right_lim);
        //cout << "e- lifetime (S2/S1 vs T_drift) [us] = " << -1/f1_exp_purity_S2_S1->GetParameter(1) << " +- " << f1_exp_purity_S2_S1->GetParError(1)/pow(f1_exp_purity_S2_S1->GetParameter(1), 2.0) << endl;
        gPad->Modified(); gPad->Update();

        c2->cd(9);
        h2_Tdrift_TBA->SetTitle(cut_loop2_srt);
        h2_Tdrift_TBA->GetXaxis()->SetTitle("TBA");
        h2_Tdrift_TBA->GetYaxis()->SetTitle("Tdrift [us]");
        h2_Tdrift_TBA->Draw("colz");

        ////////////////////////////////////////////////////////////////////////////////////////////
        TCanvas *c3 = new TCanvas("c3","S1 analysis");
        c3->Divide(3,3,0.01,0.01);

        c3->cd(1);
        h2_S1_top_tdrift->SetTitle(cut_loop1_srt);
        h2_S1_top_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S1_top_tdrift->GetYaxis()->SetTitle("S1_top [pe]");
        h2_S1_top_tdrift->Draw("colz");
        //h2_S1_top_tdrift->GetXaxis()->SetRangeUser(h2_S1_top_tdrift->GetMinimum(), h2_S1_top_tdrift->GetMaximum());
        //h2_S1_top_tdrift->GetYaxis()->SetRangeUser(50, 300);
        h2_S1_top_tdrift->SetStats(0); //delete statbox
        TProfile *prof_h2_S1_top_tdrift = h2_S1_top_tdrift->ProfileX();
        prof_h2_S1_top_tdrift->Draw("same PC");
        prof_h2_S1_top_tdrift->SetMarkerStyle(20);
        prof_h2_S1_top_tdrift->SetStats(0); //delete statbox
        gPad->Modified(); gPad->Update();
        for(int i= 0; i < prof_h2_S1_top_tdrift->GetNbinsX(); i++)
        {
           file_out_s1_top_tdrift << prof_h2_S1_top_tdrift->GetBinCenter(i) << "\t" << prof_h2_S1_top_tdrift->GetBinContent(i) << endl;
        }

        c3->cd(2);
        h2_S1_bot_tdrift->SetTitle(cut_loop1_srt);
        h2_S1_bot_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S1_bot_tdrift->GetYaxis()->SetTitle("S1_bottom [pe]");
        h2_S1_bot_tdrift->Draw("colz");
        h2_S1_bot_tdrift->SetStats(0); //delete statbox
        TProfile *prof_h2_S1_bot_tdrift = h2_S1_bot_tdrift->ProfileX();
        prof_h2_S1_bot_tdrift->Draw("same PC");
        prof_h2_S1_bot_tdrift->SetMarkerStyle(20);
        prof_h2_S1_bot_tdrift->SetStats(0); //delete statbox
        gPad->Modified(); gPad->Update();
        for(int i= 0; i < prof_h2_S1_bot_tdrift->GetNbinsX(); i++)
        {
           file_out_s1_bottom_tdrift << prof_h2_S1_bot_tdrift->GetBinCenter(i) << "\t" << prof_h2_S1_bot_tdrift->GetBinContent(i) << endl;
        }


        c3->cd(3);
        h2_S1_tdrift->Draw("colz");
        h2_S1_tdrift->SetStats(0); //delete statbox
        prof_h2_S1_tdrift->Draw("same PC");
        prof_h2_S1_tdrift->SetMarkerStyle(20);
        prof_h2_S1_tdrift->SetStats(0); //delete statbox
        gPad->Modified(); gPad->Update();

        COUT(prof_h2_S1_tdrift->GetNbinsX());

        for(int i= 0; i < prof_h2_S1_tdrift->GetNbinsX(); i++)
        {
           file_out_s1_tdrift << prof_h2_S1_tdrift->GetBinCenter(i) << "\t" << prof_h2_S1_tdrift->GetBinContent(i) << endl;
        }

        c3->cd(4);
        gStyle->SetOptFit(1);
        double shift = 0.55;
        double shift_x1 = -0.2;
        double shift_y1 = -0.3;
        double mean = 0;
        double sigma = 0;
        h1_S1_top->SetTitle("");
        h1_S1_top->GetXaxis()->SetTitle("S1_top [PE]");
        h1_S1_top->Draw();
        h1_S1_top->Fit("gaus");
        TF1 *myfunc_h1_S1_top = h1_S1_top->GetFunction("gaus");
        mean = myfunc_h1_S1_top->GetParameter(1);
        sigma = myfunc_h1_S1_top->GetParameter(2);
        h1_S1_top->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
        gPad->Update();
        TPaveStats *st_h1_S1_top = (TPaveStats*)h1_S1_top->GetListOfFunctions()->FindObject("stats");
        st_h1_S1_top->SetX1NDC(0.12+shift+shift_x1); st_h1_S1_top->SetX2NDC(0.35+shift);
        st_h1_S1_top->SetY1NDC(0.72+shift_y1); st_h1_S1_top->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c3->cd(5);
        h1_S1_bot->SetTitle("");
        h1_S1_bot->GetXaxis()->SetTitle("S1_bot [PE]");
        h1_S1_bot->Draw();
        h1_S1_bot->Fit("gaus");
        TF1 *myfunc_S1_bot = h1_S1_bot->GetFunction("gaus");
        mean = myfunc_S1_bot->GetParameter(1);
        sigma = myfunc_S1_bot->GetParameter(2);
        h1_S1_bot->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
        gPad->Update();
        TPaveStats *st_h1_S1_bot = (TPaveStats*)h1_S1_bot->GetListOfFunctions()->FindObject("stats");
        st_h1_S1_bot->SetX1NDC(0.12+shift+shift_x1); st_h1_S1_bot->SetX2NDC(0.35+shift);
        st_h1_S1_bot->SetY1NDC(0.72+shift_y1); st_h1_S1_bot->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c3->cd(6);
        h1_S1->SetTitle("");
        h1_S1->GetXaxis()->SetTitle("S1_total [PE]");
        h1_S1->Draw();
        h1_S1->Fit("gaus");
        TF1 *myfunc = h1_S1->GetFunction("gaus");
        mean = myfunc->GetParameter(1);
        sigma = myfunc->GetParameter(2);
        h1_S1->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
        gPad->Update();
        TPaveStats *st_h1_S1 = (TPaveStats*)h1_S1->GetListOfFunctions()->FindObject("stats");
        st_h1_S1->SetX1NDC(0.12+shift+shift_x1); st_h1_S1->SetX2NDC(0.35+shift);
        st_h1_S1->SetY1NDC(0.72+shift_y1); st_h1_S1->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();





        ////////////////////////////////////////////////////////////////////////////////////////////
        TCanvas *c5 = new TCanvas("c5","S2 analysis");
        c5->Divide(3,3,0.01,0.01);

        c5->cd(1);
        h2_S2_top_tdrift->SetTitle(cut_loop1_srt);
        h2_S2_top_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_top_tdrift->GetYaxis()->SetTitle("S2_top [pe]");
        h2_S2_top_tdrift->Draw("colz");
        //h2_S1_top_tdrift->GetXaxis()->SetRangeUser(h2_S1_top_tdrift->GetMinimum(), h2_S1_top_tdrift->GetMaximum());
        //h2_S2_top_tdrift->GetYaxis()->SetRangeUser(50, 300);
        h2_S2_top_tdrift->SetStats(0); //delete statbox
        TProfile *prof_h2_S2_top_tdrift = h2_S2_top_tdrift->ProfileX();
        prof_h2_S2_top_tdrift->Draw("same PC");
        prof_h2_S2_top_tdrift->SetMarkerStyle(20);
        prof_h2_S2_top_tdrift->SetStats(0); //delete statbox
        gPad->Modified(); gPad->Update();

        c5->cd(2);
        h2_S2_bottom_tdrift->SetTitle(cut_loop1_srt);
        h2_S2_bottom_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_bottom_tdrift->GetYaxis()->SetTitle("S2_bottom [pe]");
        h2_S2_bottom_tdrift->Draw("colz");
        h2_S2_bottom_tdrift->SetStats(0); //delete statbox
        TProfile *prof_h2_S2_bot_tdrift = h2_S2_bottom_tdrift->ProfileX();
        prof_h2_S2_bot_tdrift->Draw("same PC");
        prof_h2_S2_bot_tdrift->SetMarkerStyle(20);
        prof_h2_S2_bot_tdrift->SetStats(0); //delete statbox
        gPad->Modified(); gPad->Update();

        c5->cd(3);
        h2_S2_tdrift->SetTitle(cut_loop1_srt);
        h2_S2_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_tdrift->GetYaxis()->SetTitle("S2_top [pe]");
        h2_S2_tdrift->Draw("colz");
        h2_S2_tdrift->SetStats(0); //delete statbox
        TProfile *prof_h2_S2_tdrift = h2_S2_tdrift->ProfileX();
        prof_h2_S2_tdrift->Draw("same PC");
        prof_h2_S2_tdrift->SetMarkerStyle(20);
        prof_h2_S2_tdrift->SetStats(0); //delete statbox
        gPad->Modified(); gPad->Update();


        c5->cd(4);
        gStyle->SetOptFit(1);
        h1_S2_top_purity_corr->SetTitle("");
        h1_S2_top_purity_corr->GetXaxis()->SetTitle("S2_top_purity_corr [PE]");
        h1_S2_top_purity_corr->Draw();
//        h1_S2_top_purity_corr->Fit("gaus");
//        TF1 *myfunc_h1_S2_top_purity_corr = h1_S2_top_purity_corr->GetFunction("gaus");
//        mean = myfunc_h1_S2_top_purity_corr->GetParameter(1);
//        sigma = myfunc_h1_S2_top_purity_corr->GetParameter(2);
//        h1_S2_top_purity_corr->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
        gPad->Update();
        TPaveStats *st_h1_S2_top_purity_corr = (TPaveStats*)h1_S2_top_purity_corr->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_top_purity_corr->SetX1NDC(0.12+shift+shift_x1); st_h1_S2_top_purity_corr->SetX2NDC(0.35+shift);
        st_h1_S2_top_purity_corr->SetY1NDC(0.72+shift_y1); st_h1_S2_top_purity_corr->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c5->cd(5);
        gStyle->SetOptFit(1);
        h1_S2_bottom_purity_corr->SetTitle("");
        h1_S2_bottom_purity_corr->GetXaxis()->SetTitle("S2_bottom_purity_corr [PE]");
        h1_S2_bottom_purity_corr->Draw();
//        h1_S2_bottom_purity_corr->Fit("gaus");
//        TF1 *myfunc_h1_S2_bottom_purity_corr = h1_S2_bottom_purity_corr->GetFunction("gaus");
//        mean = myfunc_h1_S2_bottom_purity_corr->GetParameter(1);
//        sigma = myfunc_h1_S2_bottom_purity_corr->GetParameter(2);
//        h1_S2_bottom_purity_corr->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
        gPad->Update();
        TPaveStats *st_h1_S2_bottom_purity_corr = (TPaveStats*)h1_S2_bottom_purity_corr->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_bottom_purity_corr->SetX1NDC(0.12+shift+shift_x1); st_h1_S2_bottom_purity_corr->SetX2NDC(0.35+shift);
        st_h1_S2_bottom_purity_corr->SetY1NDC(0.72+shift_y1); st_h1_S2_bottom_purity_corr->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();


        c5->cd(6);
        gStyle->SetOptFit(1);
        h1_S2_tot_purity_corr->SetTitle("");
        h1_S2_tot_purity_corr->GetXaxis()->SetTitle("S2_tot_purity_corr [PE]");
        h1_S2_tot_purity_corr->Draw();
//        h1_S2_tot_purity_corr->Fit("gaus");
//        TF1 *myfunc_h1_S2_tot_purity_corr = h1_S2_tot_purity_corr->GetFunction("gaus");
//        mean = myfunc_h1_S2_tot_purity_corr->GetParameter(1);
//        sigma = myfunc_h1_S2_tot_purity_corr->GetParameter(2);
//        h1_S2_tot_purity_corr->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
        gPad->Update();
        TPaveStats *st_S2_tot_purity_corr = (TPaveStats*)h1_S2_tot_purity_corr->GetListOfFunctions()->FindObject("stats");
        st_S2_tot_purity_corr->SetX1NDC(0.12+shift+shift_x1); st_S2_tot_purity_corr->SetX2NDC(0.35+shift);
        st_S2_tot_purity_corr->SetY1NDC(0.72+shift_y1); st_S2_tot_purity_corr->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        c5->cd(9);
        vector<double> S2_mean_v;
        vector<double> ch_number_v;
        S2_mean_v.resize(24);
        ch_number_v.resize(24);
        double S2_sum_of_means = 0;
        for(int i = 0; i < 24; i++)
            S2_sum_of_means += h1_S2_maxch_vec[i]->GetMean();

        S2_sum_of_means /= 24;
        for(int i = 0; i < 24; i++)
        {
            COUT(h1_S2_maxch_vec[i]->GetEntries());
            S2_mean_v[i] = h1_S2_maxch_vec[i]->GetMean()/S2_sum_of_means;
            ch_number_v[i] = i+1;
            //file_out_s2_uniformity << S2_mean_v[i] << endl;
        }
        gr_S2_maxch = new TGraph(ch_number_v.size(), &ch_number_v[0], &S2_mean_v[0]);
        gr_S2_maxch->Draw("AP");
        gr_S2_maxch->SetMarkerStyle(20);
        gr_S2_maxch->GetXaxis()->SetTitle("Relative ch number");
        gr_S2_maxch->GetYaxis()->SetTitle("S2 mean using S2_max_ch cut / S2 mean all coordinates");
        c5->SetGrid();
        COUT(n_events_test);



//        c3->cd(4);
//        h2_xmaxch_xbar->Draw("colz");
//        TProfile *prof_h2_xmaxch_xbar = h2_xmaxch_xbar->ProfileY();
//        prof_h2_xmaxch_xbar->Draw("same PC");
//        prof_h2_xmaxch_xbar->SetMarkerStyle(20);
//        prof_h2_xmaxch_xbar->SetMarkerColor(kRed);
//        prof_h2_xmaxch_xbar->SetStats(0); //delete statbox
//        gPad->Modified(); gPad->Update();

//        c3->cd(5);
//        h1_LRF_v[0]->Draw();

//        c3->cd(6);
//        h2_xbar_xmaxch->Draw("colz");
//        //c3->cd(9);
//        TProfile *prof_h2_xbar_xmaxch = h2_xbar_xmaxch->ProfileX();
//        prof_h2_xbar_xmaxch->Draw("same PC");
//        prof_h2_xbar_xmaxch->SetMarkerStyle(20);
//        prof_h2_xbar_xmaxch->SetStats(0); //delete statbox
//        gPad->Modified(); gPad->Update();

//        TCanvas *c4 = new TCanvas("c4","c4");
//        c4->cd(1);
//        auto legend = new TLegend(0.7,0.13,0.9,0.43);
//        ostringstream h1_name_oss_0;
//        h1_name_oss_0 << "pos_x = 0.625; <bar_x> = " << h1_x_bar_v[0]->GetMean();
//        ostringstream h1_name_oss_1;
//        h1_name_oss_1 << "pos_x = 1.875; <bar_x> = " << h1_x_bar_v[1]->GetMean();
//        ostringstream h1_name_oss_2;
//        h1_name_oss_2 << "pos_x = 3.125; <bar_x> = " << h1_x_bar_v[2]->GetMean();
//        ostringstream h1_name_oss_3;
//        h1_name_oss_3 << "pos_x = 4.375; <bar_x> = " << h1_x_bar_v[3]->GetMean();

//        legend->AddEntry(h1_x_bar_v[0], h1_name_oss_0.str().c_str(),"lep");
//        legend->AddEntry(h1_x_bar_v[1], h1_name_oss_1.str().c_str(),"lep");
//        legend->AddEntry(h1_x_bar_v[2], h1_name_oss_2.str().c_str(),"lep");
//        legend->AddEntry(h1_x_bar_v[3], h1_name_oss_3.str().c_str(),"lep");
//        h1_x_bar_v[0]->Scale(1/h1_x_bar_v[0]->Integral());
//        h1_x_bar_v[1]->Scale(1/h1_x_bar_v[1]->Integral());
//        h1_x_bar_v[2]->Scale(1/h1_x_bar_v[2]->Integral());
//        h1_x_bar_v[3]->Scale(1/h1_x_bar_v[3]->Integral());

//        h1_x_bar_v[0]->SetTitle("");
//        h1_x_bar_v[0]->SetStats(0); //delete statbox
//        h1_x_bar_v[0]->Draw();
//        h1_x_bar_v[1]->Draw("same");
//        h1_x_bar_v[2]->Draw("same");
//        h1_x_bar_v[3]->Draw("same");
//        legend->Draw();
//        h1_x_bar_v[0]->GetXaxis()->SetTitle("bar_x [cm]");
//        h1_x_bar_v[0]->SetLineWidth(2);
//        h1_x_bar_v[1]->SetLineWidth(2);
//        h1_x_bar_v[2]->SetLineWidth(2);
//        h1_x_bar_v[3]->SetLineWidth(2);
//        h1_x_bar_v[1]->SetLineColor(kRed);
//        h1_x_bar_v[2]->SetLineColor(kBlack);
//        h1_x_bar_v[3]->SetLineColor(kGreen);

//        TLine *line_x0 = new TLine(0.625, 0, 0.625, h1_x_bar_v[0]->GetMaximum());
//        line_x0->Draw("same");
//        line_x0->SetLineWidth(3);
//        line_x0->SetLineColor(kMagenta);
//        TLine *line_x1 = new TLine(1.875, 0, 1.875, h1_x_bar_v[0]->GetMaximum());
//        line_x1->Draw("same");
//        line_x1->SetLineWidth(3);
//        line_x1->SetLineColor(kMagenta);
//        TLine *line_x2 = new TLine(3.125, 0, 3.125, h1_x_bar_v[0]->GetMaximum());
//        line_x2->Draw("same");
//        line_x2->SetLineWidth(3);
//        line_x2->SetLineColor(kMagenta);
//        TLine *line_x3 = new TLine(4.375, 0, 4.375, h1_x_bar_v[0]->GetMaximum());
//        line_x3->Draw("same");
//        line_x3->SetLineWidth(3);
//        line_x3->SetLineColor(kMagenta);

//        file_out_x_corrections << (x_centers[0] - h1_x_bar_v[0]->GetMean()) << endl;
//        file_out_x_corrections << (x_centers[1] - h1_x_bar_v[1]->GetMean()) << endl;
//        file_out_x_corrections << (x_centers[2] - h1_x_bar_v[2]->GetMean()) << endl;
//        file_out_x_corrections << (x_centers[3] - h1_x_bar_v[3]->GetMean()) << endl;
//        //h3_xmaxch_xbar_ybar->Draw("iso");
////        h3_xmaxch_xbar_ybar->Draw("box");
////        h3_xmaxch_xbar_ybar->GetXaxis()->SetTitle("X_barycenter [cm]");
////        h3_xmaxch_xbar_ybar->GetYaxis()->SetTitle("Y_barycenter [cm]");
////        h3_xmaxch_xbar_ybar->GetZaxis()->SetTitle("X_maxch [cm]");



    }

    //finalization code
    cout << "all is ok" << endl;

    //Cleanup
    //delete app;

    return 0;
}
