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


int run_number = 831;

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


void f90()
{
    //start code
    std::cout << "You are in program." << std::endl;

    //main code
    ostringstream path_root_file;
    //path_root_file << "/media/vlad/Data/DS-data/reco/v1/" << "run_" << run_number << ".root";
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v1/" << "run_" << run_number << ".root";
    TString filename = path_root_file.str().c_str();

    ostringstream path_file_out;
    path_file_out << "/home/vlad/Reports/S1_LY/f90/run_" << run_number << ".txt";
    ofstream file_out(path_file_out.str().c_str());

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

    //double S1_max = 1600; //standart
    double S1_max = 2500; //V=35, V=70
    //double S1_max = 10000;//bkg

    //TH1::SetDefaultSumw2(kTRUE);
    TH1F *h1_f90 = new TH1F("h1_f90", "h1_f90", 200, 0, 1);

    //zero event loop
    TString total_cut_srt_loop0;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();

        BoolCut C_S1(clusters, 0);
        //C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35
        REMEMBER_CUT_LOOP0(C_S1.nc == 1);
        //REMEMBER_CUT_LOOP0(C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35);
        //REMEMBER_CUT_LOOP0(C_S1.nc == 1 && clusters.at(0)->f90 > 0.2 && clusters.at(0)->f90 < 0.35 && C_S1.cls0_is_full);

        if(cut_loop0_bool)
        {
            h1_f90->Fill(clusters.at(0)->f90);
            file_out << clusters.at(0)->f90 << endl;
        }

    }

    TCanvas *c1 = new TCanvas("c1","c1");
    h1_f90->Draw();
    h1_f90->Sumw2(kTRUE);
    h1_f90->GetXaxis()->SetTitle("S1 f90");
    h1_f90->SetFillColor(kMagenta);
    h1_f90->Scale(1/h1_f90->Integral());
    h1_f90->SetStats(0); //delete statbox
    gPad->Modified(); gPad->Update();




    //auto legend = new TLegend(0.7,0.13,0.9,0.43);
    //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
    //legend->AddEntry(h1_f90,"744","lep");
    //legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
    //legend->AddEntry("gr","Graph with error bars","lep");
    //legend->Draw();


}
