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


//int run_number = 1025;
//int run_number = 1031;
//int run_number = 1033;
int run_number = 1051;

double area_cut_x1 = 1.5;//cm
double area_cut_x2 = 2.1;//cm
double area_cut_x3 = 2.9;//cm
double area_cut_x4 = 3.5;//cm
double area_cut_y1 = area_cut_x1;
double area_cut_y2 = area_cut_x2;
double area_cut_y3 = area_cut_x3;
double area_cut_y4 = area_cut_x4;


Double_t gauss(Double_t *x,Double_t *par)
{
    Double_t arg = 0;
    if (par[2] != 0)
    {
        arg = (x[0] - par[1])/par[2];
    }

    Double_t fitval = par[0]*TMath::Exp(-0.5*arg*arg);
    return fitval;
}

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

    if(nc)
    {
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
}


void main2()
{
    //start code
    std::cout << "You are in program." << std::endl;

    //main code
    ostringstream path_root_file;
    //path_root_file << "/media/vlad/Data/DS-data/reco/v1/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v1/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/CVII_v2/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_V/v4/" << "run_" << run_number << ".root";
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";
    TString filename = path_root_file.str().c_str();

    ostringstream path_file_out_common;
    path_file_out_common << "/home/vlad/Reports/run_properties/run_" << run_number ;

    ostringstream path_file_out_S1_f90;
    path_file_out_S1_f90 << path_file_out_common.str() << "_S1_f90.txt";
    ofstream file_out_S1_f90(path_file_out_S1_f90.str().c_str());

    ostringstream path_file_out_TBA;
    path_file_out_TBA << path_file_out_common.str() << "_TBA.txt";
    ofstream file_out_TBA(path_file_out_TBA.str().c_str());

    ostringstream path_file_out_S1_tot;
    path_file_out_S1_tot << path_file_out_common.str() << "_S1_tot.txt";
    ofstream file_out_S1_tot(path_file_out_S1_tot.str().c_str());

    ostringstream path_file_out_S1_top;
    path_file_out_S1_top << path_file_out_common.str() << "_S1_top.txt";
    ofstream file_out_S1_top(path_file_out_S1_top.str().c_str());

    ostringstream path_file_out_S1_bot;
    path_file_out_S1_bot << path_file_out_common.str() << "_S1_bot.txt";
    ofstream file_out_S1_bot(path_file_out_S1_bot.str().c_str());

    ostringstream path_file_out_baseline_mean_ch_0_1_2_3;
    path_file_out_baseline_mean_ch_0_1_2_3 << path_file_out_common.str() << "_baseline_mean_ch_0_1_2_3.txt";
    ofstream file_out_baseline_mean_ch_0_1_2_3(path_file_out_baseline_mean_ch_0_1_2_3.str().c_str());



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

    vector<bool> is_in_cut(data->GetEntries(), false);

    //double S1_max = 1600; //standart
    double S1_max = 3500; //V=35, V=70
    //double S1_max = 10000;//bkg
    double S2_max = 5000;

    //TH1::SetDefaultSumw2(kTRUE);
    TH1F *h1_nc = new TH1F("h1_nc", "h1_nc", 200, 0, 10);
    TH1F *h1_Tdrift = new TH1F("h1_Tdrift", "h1_Tdrift", 100, 0, 100);
    TH1F *h1_S1_top = new TH1F("h1_S1_top", "h1_S1_top", 200, 0, S1_max*0.6);
    TH1F *h1_S1_bot = new TH1F("h1_S1_bot", "h1_S1_bot", 200, 0, S1_max*0.6);
    TH1F *h1_S1_total = new TH1F("h1 S1", "h1_S1_total", 500, 0, S1_max);
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 200, 0, 1);
    TH1F *h1_TBA = new TH1F("h1_TBA", "h1_TBA", 200, -1, 1);

    TH2F *h2_f90_S1_total = new TH2F("h2_f90_S1_total", "h2_f90_S1_total", 200, 0, 1, 200, 0, S1_max);
    TH2F *h2_S1_bot_S1_top = new TH2F("h2_S1_bot_S1_top", "h2_S1_bot_S1_top", 200, 0, S1_max/2.0, 200, 0, S1_max/2.0);
    TH2F *h2_S1_total_TBA = new TH2F("h2_S1_total_TBA", "h2_S1_total_TBA", 200, 0, S1_max, 200, -1, 1);
    TH2F *h2_S1_TBA = new TH2F("h2_S1_TBA", "h2_S1_TBA", 200, -0.5, 0.5, 200, 0, S1_max);

    TH2F *h2_f90_S2_total = new TH2F("h2_f90_S2_total", "h2_f90_S2_total", 200, 0, 1, 200, 0, S2_max);
    TH1F *h1_S2_total = new TH1F("h1 S2", "h1_S2_total", 400, 0, S2_max);
    TH1F *h1_S2_rms_time = new TH1F("h1_S2_rms_time", "h1_S2_rms_time", 400, -1000000, 10000000);

    int n_ev_number_of_clusters_more_0 = 0;
    int n_ev_number_of_clusters_more_1 = 0;

    //zero event loop
    TString total_cut_srt_loop0;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        if(nc > 0)
            n_ev_number_of_clusters_more_0++;
        if(nc > 1)
            n_ev_number_of_clusters_more_1++;

        vector<double> BaseMean = evReco->GetBaseMean();

        file_out_baseline_mean_ch_0_1_2_3 << BaseMean[0] << "\t" << BaseMean[1] << "\t" << BaseMean[2] << "\t" << BaseMean[3] << endl;


        BoolCut C_S1(clusters, 0);
        //C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35
        //744//C_S1.nc == 1 && C_S1.cls0 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35 && clusters.at(0)->charge > 350 && clusters.at(0)->charge < 850
        //832//
        //930//C_S1.nc == 1 && C_S1.cls0 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35 && clusters.at(0)->charge > 400 && clusters.at(0)->charge < 900
        //951//
        REMEMBER_CUT_LOOP0(C_S1.nc == 1 && C_S1.cls0);
        //REMEMBER_CUT_LOOP0(C_S1.nc == 1 && C_S1.cls0 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35);
        //REMEMBER_CUT_LOOP0(C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35);
        //REMEMBER_CUT_LOOP0(C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35 && C_S1.cls0_is_full);
        //REMEMBER_CUT_LOOP0(C_S1.nc == 2 && C_S1.cls1 clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35 && C_S1.cls0_is_full);


        h1_nc->Fill(nc);
        //if(nc == 2)
         //   h1_Tdrift->Fill( (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000 );

        if(cut_loop0_bool)
        {
           is_in_cut[ev] = true;

           h1_S1_total->Fill(clusters.at(0)->charge);
           h1_S1_bot->Fill(clusters.at(0)->tot_charge_bottom);
           h1_S1_top->Fill(clusters.at(0)->tot_charge_top);
           h1_S1_f90->Fill(clusters.at(0)->f90);
           double TBA = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) /
                   (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);
           h1_TBA->Fill(TBA);

           file_out_S1_f90 << clusters.at(0)->f90 << endl;
           file_out_TBA << TBA << endl;
           file_out_S1_tot << clusters.at(0)->charge << endl;
           file_out_S1_top << clusters.at(0)->tot_charge_top << endl;
           file_out_S1_bot << clusters.at(0)->tot_charge_bottom << endl;

           h2_S1_TBA->Fill(TBA, clusters.at(0)->charge);

           h2_f90_S1_total->Fill(clusters.at(0)->f90, clusters.at(0)->charge);
           h2_S1_bot_S1_top->Fill(clusters.at(0)->tot_charge_bottom, clusters.at(0)->tot_charge_top);
           h2_S1_total_TBA->Fill(clusters.at(0)->charge, TBA);


        }

    }

    //second event loop
    TString total_cut_srt_loop2;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        if(is_in_cut[ev])
        {
            for(int nc_i = 0; nc_i < nc; nc_i++)
            {
                BoolCut C_S2(clusters, nc_i);
                //C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35
                REMEMBER_CUT_LOOP2(C_S2.nc == 2 && C_S2.cls1);

                if(cut_loop2_bool)
                {
                    h2_f90_S2_total->Fill(clusters.at(1)->f90, clusters.at(1)->charge);
                    h1_S2_total->Fill(clusters.at(1)->charge);
                    h1_S2_rms_time->Fill(clusters.at(1)->rms_time);


                    if(nc == 2)
                       h1_Tdrift->Fill( (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000 );
                }
            }
        }

    }





    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(3,3,0.01,0.01);
    gStyle->SetOptFit(1);
    double shift = 0.55;
    double shift_x1 = -0.2;
    double shift_y1 = -0.3;

    TF1 *f_gauss = new TF1("f_gauss",gauss,0,5000,3);

    c1->cd(1);
    h1_S1_total->SetTitle(cut_loop0_srt);
    h1_S1_total->GetXaxis()->SetTitle("S1_total [PE]");
    h1_S1_total->Draw();
    h1_S1_total->Fit("gaus");
    TF1 *myfunc = h1_S1_total->GetFunction("gaus");
    double mean = myfunc->GetParameter(1);
    double sigma = myfunc->GetParameter(2);
    h1_S1_total->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
    gPad->Update();
    TPaveStats *st_h1_S1_total = (TPaveStats*)h1_S1_total->GetListOfFunctions()->FindObject("stats");
    st_h1_S1_total->SetX1NDC(0.12+shift+shift_x1); st_h1_S1_total->SetX2NDC(0.35+shift);
    st_h1_S1_total->SetY1NDC(0.72+shift_y1); st_h1_S1_total->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c1->cd(2);
    h1_S1_bot->SetTitle(cut_loop0_srt);
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

    c1->cd(3);
    h1_S1_top->SetTitle(cut_loop0_srt);
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

    c1->cd(4);
    h1_S1_f90->SetTitle(cut_loop0_srt);
    h1_S1_f90->GetXaxis()->SetTitle("S1_f90");
    h1_S1_f90->Draw();
    h1_S1_f90->Fit("gaus","","",0.19,0.32);
    TF1 *myfunc3 = h1_S1_f90->GetFunction("gaus");
    double mean_3 = myfunc3->GetParameter(1);
    double sigma_3 = myfunc3->GetParameter(2);
    h1_S1_f90->Fit("gaus","","",mean_3-1.5*sigma_3,mean_3+1.5*sigma_3);
    gPad->Update();
    TPaveStats *st_h1_S1_f90 = (TPaveStats*)h1_S1_f90->GetListOfFunctions()->FindObject("stats");
    st_h1_S1_f90->SetX1NDC(0.12+shift+shift_x1); st_h1_S1_f90->SetX2NDC(0.35+shift);
    st_h1_S1_f90->SetY1NDC(0.72+shift_y1); st_h1_S1_f90->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c1->cd(5);
    h2_f90_S1_total->SetTitle(cut_loop0_srt);
    h2_f90_S1_total->GetXaxis()->SetTitle("S1_f90");
    h2_f90_S1_total->GetYaxis()->SetTitle("S1_total [PE]");
    h2_f90_S1_total->Draw("colz");
    gPad->Update();
    TPaveStats *st_h2_f90_S1_total = (TPaveStats*)h2_f90_S1_total->GetListOfFunctions()->FindObject("stats");
    st_h2_f90_S1_total->SetX1NDC(0.12+shift); st_h2_f90_S1_total->SetX2NDC(0.35+shift);
    st_h2_f90_S1_total->SetY1NDC(0.72); st_h2_f90_S1_total->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c1->cd(6);
    h2_S1_bot_S1_top->SetTitle(cut_loop0_srt);
    h2_S1_bot_S1_top->GetXaxis()->SetTitle("S1_bot [PE]");
    h2_S1_bot_S1_top->GetYaxis()->SetTitle("S1_top [PE]");
    h2_S1_bot_S1_top->Draw("colz");
    gPad->Update();
    TPaveStats *st_h2_S1_bot_S1_top = (TPaveStats*)h2_S1_bot_S1_top->GetListOfFunctions()->FindObject("stats");
    st_h2_S1_bot_S1_top->SetX1NDC(0.12); st_h2_S1_bot_S1_top->SetX2NDC(0.35);
    st_h2_S1_bot_S1_top->SetY1NDC(0.72); st_h2_S1_bot_S1_top->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();


    c1->cd(7);
    h1_TBA->SetTitle(cut_loop0_srt);
    h1_TBA->GetXaxis()->SetTitle("h1_TBA");
    h1_TBA->Draw();
    gPad->Update();
    TPaveStats *st_h1_TBA = (TPaveStats*)h1_TBA->GetListOfFunctions()->FindObject("stats");
    st_h1_TBA->SetX1NDC(0.12+shift); st_h1_TBA->SetX2NDC(0.35+shift);
    st_h1_TBA->SetY1NDC(0.72); st_h1_TBA->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c1->cd(8);
    h2_S1_total_TBA->SetTitle(cut_loop0_srt);
    h2_S1_total_TBA->GetXaxis()->SetTitle("S1_total [PE]");
    h2_S1_total_TBA->GetYaxis()->SetTitle("TBA");
    h2_S1_total_TBA->Draw("colz");
    gPad->Update();
    TPaveStats *st_h2_S1_total_TBA = (TPaveStats*)h2_S1_total_TBA->GetListOfFunctions()->FindObject("stats");
    st_h2_S1_total_TBA->SetX1NDC(0.12); st_h2_S1_total_TBA->SetX2NDC(0.35);
    st_h2_S1_total_TBA->SetY1NDC(0.72); st_h2_S1_total_TBA->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c1->cd(9);
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


    TCanvas *c2 = new TCanvas("c2","c2");
    c2->Divide(2,2,0.01,0.01);
    gStyle->SetOptFit(1);

    c2->cd(1);
    h1_Tdrift->GetXaxis()->SetTitle("Tdrift [us]");
    h1_Tdrift->Draw();

    c2->cd(2);
    h2_f90_S2_total->SetTitle(cut_loop2_srt);
    h2_f90_S2_total->GetXaxis()->SetTitle("S2_f90");
    h2_f90_S2_total->GetYaxis()->SetTitle("S2_total [PE]");
    h2_f90_S2_total->Draw("colz");
    gPad->Update();
    TPaveStats *st_h2_f90_S2_total = (TPaveStats*)h2_f90_S2_total->GetListOfFunctions()->FindObject("stats");
    st_h2_f90_S2_total->SetX1NDC(0.12+shift); st_h2_f90_S2_total->SetX2NDC(0.35+shift);
    st_h2_f90_S2_total->SetY1NDC(0.72); st_h2_f90_S2_total->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c2->cd(3);
    h1_S2_total->SetTitle(cut_loop2_srt);
    h1_S2_total->GetXaxis()->SetTitle("S2_total [PE]");
    h1_S2_total->Draw();
    //h1_S2_total->Fit("gaus");
    //TF1 *myfunc = h1_S1_total->GetFunction("gaus");
    //double mean = myfunc->GetParameter(1);
    //double sigma = myfunc->GetParameter(2);
    //h1_S1_total->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
    //gPad->Update();
//    TPaveStats *st_h1_S2_total = (TPaveStats*)h1_S2_total->GetListOfFunctions()->FindObject("stats");
//    st_h1_S2_total->SetX1NDC(0.12+shift+shift_x1); st_h1_S2_total->SetX2NDC(0.35+shift);
//    st_h1_S2_total->SetY1NDC(0.72+shift_y1); st_h1_S2_total->SetY2NDC(0.89);
//    gPad->Modified(); gPad->Update();

    c2->cd(4);
    h1_S2_rms_time->SetTitle(cut_loop2_srt);
    h1_S2_rms_time->GetXaxis()->SetTitle("S2_rms_time");
    h1_S2_rms_time->Draw();
    gPad->Update();
    TPaveStats *st_h1_S2_rms_time = (TPaveStats*)h1_S2_rms_time->GetListOfFunctions()->FindObject("stats");
    st_h1_S2_rms_time->SetX1NDC(0.12+shift); st_h1_S2_rms_time->SetX2NDC(0.35+shift);
    st_h1_S2_rms_time->SetY1NDC(0.72); st_h1_S2_rms_time->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();



//    TCanvas *c3 = new TCanvas("c3","c3");
//    h1_S1_f90->Draw();
//    h1_S1_f90->Fit("gaus","","",0.19,0.32);
//    TF1 *myfunc3 = h1_S1_f90->GetFunction("gaus");
//    double mean_3 = myfunc3->GetParameter(1);
//    double sigma_3 = myfunc3->GetParameter(2);
//    h1_S1_f90->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
//    //TF1 *myfunc2 = h2_f90_S1_total->GetFunction("gaus");

    TCanvas *c3 = new TCanvas("c3","c3");
    c3->Divide(3,3,0.01,0.01);
    c3->cd(7);
    h2_S1_TBA->SetTitle(cut_loop2_srt);
    h2_S1_TBA->GetXaxis()->SetTitle("TBA");
    h2_S1_TBA->GetYaxis()->SetTitle("S1");
    h2_S1_TBA->Draw("colz");
    c3->cd(8);
    TProfile *prof_h2_S1_TBA = h2_S1_TBA->ProfileX();
    prof_h2_S1_TBA->GetYaxis()->SetTitle("S1");
    prof_h2_S1_TBA->Draw();
    //TF1 *f1_exp_purity_S2_S1 = new TF1("f1_exp_purity_S2_S1","exp([0] + x*[1])",0,150);
    //prof_S2_S1_TDrift->Fit("f1_exp_purity_S2_S1","R","",left_lim,right_lim);
    //cout << "e- lifetime (S2/S1 vs T_drift) [us] = " << -1/f1_exp_purity_S2_S1->GetParameter(1) << " +- " << f1_exp_purity_S2_S1->GetParError(1)/pow(f1_exp_purity_S2_S1->GetParameter(1), 2.0) << endl;
    gPad->Modified(); gPad->Update();

    cout << "(number_of_clusters>1)/(number_of_clusters>0) = " <<  (double) n_ev_number_of_clusters_more_1 / n_ev_number_of_clusters_more_0 << endl;


}
