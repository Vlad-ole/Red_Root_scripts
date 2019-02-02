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

using namespace std;


int run_number = 749;

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
    path_root_file << "/media/vlad/Data/DS-data/reco/v1/" << "run_" << run_number << ".root";
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

    double S1_max = 1600;

    //TH1::SetDefaultSumw2(kTRUE);
    TH1F *h1_nc = new TH1F("h1_nc", "h1_nc", 200, 0, 10);
    TH1F *h1_S1_top = new TH1F("h1_S1_top", "h1_S1_top", 200, 0, S1_max*0.6);
    TH1F *h1_S1_bot = new TH1F("h1_S1_bot", "h1_S1_bot", 200, 0, S1_max*0.6);
    TH1F *h1_S1_total = new TH1F("h1 S1", "h1_S1_total", 400, 0, S1_max);
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 200, 0, 1);
    TH1F *h1_TBA = new TH1F("h1_TBA", "h1_TBA", 200, -1, 1);

    TH2F *h2_f90_S1_total = new TH2F("h2_f90_S1_total", "h2_f90_S1_total", 200, 0, 1, 200, 0, S1_max);
    TH2F *h2_S1_bot_S1_top = new TH2F("h2_S1_bot_S1_top", "h2_S1_bot_S1_top", 200, 0, S1_max/2.0, 200, 0, S1_max/2.0);
    TH2F *h2_S1_total_TBA = new TH2F("h2_S1_total_TBA", "h2_S1_total_TBA", 200, 0, S1_max, 200, -1, 1);


    //zero event loop
    TString total_cut_srt_loop0;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        BoolCut C_S1(clusters, 0);
        //C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35
        //REMEMBER_CUT_LOOP0(C_S1.nc == 1);
        REMEMBER_CUT_LOOP0(C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35);
        h1_nc->Fill(nc);

        if(cut_loop0_bool)
        {
           h1_S1_total->Fill(clusters.at(0)->charge);
           h1_S1_bot->Fill(clusters.at(0)->tot_charge_bottom);
           h1_S1_top->Fill(clusters.at(0)->tot_charge_top);
           h1_S1_f90->Fill(clusters.at(0)->f90);
           double TBA = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) /
                   (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);
           h1_TBA->Fill(TBA);

           h2_f90_S1_total->Fill(clusters.at(0)->f90, clusters.at(0)->charge);
           h2_S1_bot_S1_top->Fill(clusters.at(0)->tot_charge_bottom, clusters.at(0)->tot_charge_top);
           h2_S1_total_TBA->Fill(clusters.at(0)->charge, TBA);
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
    gPad->Update();
    TPaveStats *st_h1_S1_f90 = (TPaveStats*)h1_S1_f90->GetListOfFunctions()->FindObject("stats");
    st_h1_S1_f90->SetX1NDC(0.12+shift); st_h1_S1_f90->SetX2NDC(0.35+shift);
    st_h1_S1_f90->SetY1NDC(0.72); st_h1_S1_f90->SetY2NDC(0.89);
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



}
