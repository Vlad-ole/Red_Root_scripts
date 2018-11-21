//Qt
//#include <QCoreApplication>

//C++
#include <iostream>
#include <sstream>      // std::ostringstream

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


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;
//bool total_cut = false;
//bool tmp_cut = false;
//TString total_cut_srt_loop1;
//#define REMEMBER_CUT(x) total_cut_srt_loop1 = #x; /* tmp_cut = x;*/

//double h1_fill_value;
//TString h1_title;
//#define REMEMBER_h1_FILL_VALUE(x) h1_title = #x; h1_fill_value = x;

using namespace std;


//some predefined vars
//list of runs

//ph1
int run_number = 399; //ph1     Am241
//int run_number = 418; //ph1     Am241
//int run_number = 511; //ph1     Am241


//ph2
//int run_number = 448; //ph2     Am241 error
//int run_number = 532; //ph2    BEAM ON (E = 28 MeV, i = 12 nA)
//int run_number = 534; //ph2     backgroud
//int run_number = 537; //ph2     Am241
//int run_number = 540; //ph2     Cf252
//int run_number = 542; //ph2     Am241
//int run_number = 544; //ph2     Am241
//int run_number = 550; //ph2     Am241
//int run_number = 554; //ph2     Am241 error

//var1: 1.5 2.1 2.9 3.5
double area_cut_x1 = 1.5;//cm
double area_cut_x2 = 2.3;//cm
double area_cut_x3 = 2.7;//cm
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

    //cuts2
    bool cls0;//cluster 0
    bool cls1;//cluster 1
    bool is_S1;
    bool is_S2;

    bool cent_spot;
    bool edges;
    bool corners;

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

    cent_spot = (clusters.at(nc_i)->pos_x > area_cut_x2) && (clusters.at(nc_i)->pos_x < area_cut_x3) &&
            (clusters.at(nc_i)->pos_y > area_cut_y2) && (clusters.at(nc_i)->pos_y < area_cut_y3);

    //edges
    edge_left = (clusters.at(nc_i)->pos_x > area_cut_x1) && (clusters.at(nc_i)->pos_x < area_cut_x2) &&
            (clusters.at(nc_i)->pos_y > area_cut_y2) && (clusters.at(nc_i)->pos_y < area_cut_y3);

    edge_right = (clusters.at(nc_i)->pos_x > area_cut_x3) && (clusters.at(nc_i)->pos_x < area_cut_x4) &&
            (clusters.at(nc_i)->pos_y > area_cut_y2) && (clusters.at(nc_i)->pos_y < area_cut_y3);

    edge_bot = (clusters.at(nc_i)->pos_x > area_cut_x2) && (clusters.at(nc_i)->pos_x < area_cut_x3) &&
            (clusters.at(nc_i)->pos_y > area_cut_y1) && (clusters.at(nc_i)->pos_y < area_cut_y2);

    edge_top = (clusters.at(nc_i)->pos_x > area_cut_x2) && (clusters.at(nc_i)->pos_x < area_cut_x3) &&
            (clusters.at(nc_i)->pos_y > area_cut_y3) && (clusters.at(nc_i)->pos_y < area_cut_y4);

    edges = edge_left || edge_right || edge_bot || edge_top;

    //corners
    corner_left_bot = (clusters.at(nc_i)->pos_x > area_cut_x1) && (clusters.at(nc_i)->pos_x < area_cut_x2) &&
            (clusters.at(nc_i)->pos_y > area_cut_y1) && (clusters.at(nc_i)->pos_y < area_cut_y2);

    corner_right_bot = (clusters.at(nc_i)->pos_x > area_cut_x3) && (clusters.at(nc_i)->pos_x < area_cut_x4) &&
            (clusters.at(nc_i)->pos_y > area_cut_y1) && (clusters.at(nc_i)->pos_y < area_cut_y2);

    corner_left_top = (clusters.at(nc_i)->pos_x > area_cut_x1) && (clusters.at(nc_i)->pos_x < area_cut_x2) &&
            (clusters.at(nc_i)->pos_y > area_cut_y3) && (clusters.at(nc_i)->pos_y < area_cut_y4);

    corner_right_top = (clusters.at(nc_i)->pos_x > area_cut_x3) && (clusters.at(nc_i)->pos_x < area_cut_x4) &&
            (clusters.at(nc_i)->pos_y > area_cut_y3) && (clusters.at(nc_i)->pos_y < area_cut_y4);





    corners = corner_left_bot || corner_right_bot || corner_left_top || corner_right_top;

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




int main(/*int argc, char *argv[]*/)
{
    //start code
    std::cout << "You are in program." << std::endl;

    //
    TF1 *func_gauss_3p_1p = new TF1("gauss_3p_1p",gauss_3p_1p,-1000,10000,3);
    //func_gauss_3p_1p->SetParNames ("Constant","Mean","Sigma","Sigma/Mean");
    func_gauss_3p_1p->SetParNames ("Constant","Mean","Sigma");

    //main code
    ostringstream path_root_file;
    path_root_file << "/media/vlad/Data/DS-data/reco/rm3reco/lns/camp_V/v3/" << "run_" << run_number << ".root";
    //path_root_file << "/home/vlad/Soft/Red_Soft/red-daq-light/src/Level1/" << "run_" << run_number << ".root";
    TString filename = path_root_file.str().c_str();

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

    TH1F *h1;
    TH2F *h2 = new TH2F("h2 XY S2", "h2 title", 200, 0, 5, 200, 0, 5);//XY S2
    TH2F *h2_S1 = new TH2F("h2 XY S1", "h2 title", 200, 0, 5, 200, 0, 5);//XY S1
    TH1F *h1_S1 = new TH1F("h1 S1", "h1 title", 400, -100, 2000);//Am S1 charge
    TH1F *h1_S2 = new TH1F("h1 S2", "h1 title", 400, -100, 10000);//Am S2 charge
    TH1F *h1_S2_S1_ratio = new TH1F("h1 S2/S1", "h1 title", 300, -1, 30);//Am S2_charge/S1_charge
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 8000);//Am S2 tot_charge_top or tot_charge_bottom
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 1000);//Am S1 tot_charge_top or tot_charge_bottom
    //TH1F *h1 = new TH1F("h1 f90", "h1 title", 400, -0.2, 1);//f90
    //TH1F *h1 = new TH1F("h1 x or y", "h1 title", 400, -1, 5);//x ix or iy
    TH1F *h1_Tdrift = new TH1F("h1 Tdrift", "h1 title", 400, -1, 150);//
    h1 = h1_Tdrift;

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


//    vector<TH1F *> h1_S1_top_vec;
//    for(int ih1 = 0; ih1 < 24; ih1++)
//    {
//        ostringstream h1_S1_bottom_name;
//        h1_S1_bottom_name << "h1_S1_bottom_ch" << ih1;
//        h1_S1_top_vec.push_back(new TH1F("h1_S1_bottom_chF5", "h1 title", 200, -30, 200);
//    }


    vector<bool> is_in_cut(data->GetEntries(), false);

    //first event loop
    TString total_cut_srt_loop1_0;
    TString total_cut_srt_loop1;
    TString total_cut_srt_loop2;
    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        //cout << "line 106" << endl;
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();
        //cout << "Found n. " << nc << " clusters" << endl;


        if(nc)
        {

//            BoolCut C0(clusters, 0);
//            total_cut_srt_loop1_0 = "C0.nc == 2 && C0.cls0_is_full && C0.cls0_is_S1";
//            if(C0.nc == 2 && C0.cls0_is_full && C0.cls0_is_S1)
//            {
//                double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;
//                //

//                h1_Tdrift->Fill(Tdrift);
//            }


            for(int nc_i = 0; nc_i < nc; nc_i++)
            {
                double radius = sqrt( pow(clusters.at(nc_i)->pos_x - 2.62, 2.0) + pow(clusters.at(nc_i)->pos_y - 2.54, 2.0) );

                //total_cut = cuts(clusters, nc_i);
                BoolCut C1(clusters, nc_i);


                total_cut_srt_loop1 = "C1.is_S1_only";
                if ( C1.is_S1_only ) //cuts
                {
                    is_in_cut[ev] = true;
                    //cout << "   pos_x = " << clusters[nc_i]->pos_x << "; pos_y = " << clusters[nc_i]->pos_y << endl;
                    h2->Fill(clusters.at(nc_i)->pos_x, clusters.at(nc_i)->pos_y);
                    h1_S2->Fill(clusters.at(nc_i)->charge);

                    h1_S1_bottom_chF2->Fill(clusters.at(nc_i)->charge_bottom[0]);
                    h1_S1_bottom_chF3->Fill(clusters.at(nc_i)->charge_bottom[1]);
                    h1_S1_bottom_chF4->Fill(clusters.at(nc_i)->charge_bottom[2]);
                    h1_S1_bottom_chF5->Fill(clusters.at(nc_i)->charge_bottom[3]);

                    h1_S1_top_A1_A2_B1_B2_C1_C2->Fill(
                                clusters.at(nc_i)->charge_top[0] + clusters.at(nc_i)->charge_top[1] +
                            0 + clusters.at(nc_i)->charge_top[6] +
                            clusters.at(nc_i)->charge_top[10] + clusters.at(nc_i)->charge_top[11]);

                    h1_S1_top_A3_A4_A5_B4_B3_B5->Fill(
                                clusters.at(nc_i)->charge_top[2] + clusters.at(nc_i)->charge_top[3] +
                            clusters.at(nc_i)->charge_top[4] + clusters.at(nc_i)->charge_top[8] +
                            clusters.at(nc_i)->charge_top[7] + clusters.at(nc_i)->charge_top[9]);

                    h1_S1_top_C5_D1_D5_E2_E3_E4->Fill(
                                clusters.at(nc_i)->charge_top[2+12] + clusters.at(nc_i)->charge_top[3+12] +
                            clusters.at(nc_i)->charge_top[7+12] + clusters.at(nc_i)->charge_top[8+12] +
                            clusters.at(nc_i)->charge_top[9+12] + clusters.at(nc_i)->charge_top[10+12]);

                    h1_S1_top_C4_C3_D4_D3_D2_E5->Fill(
                                clusters.at(nc_i)->charge_top[1+12] + clusters.at(nc_i)->charge_top[0+12] +
                            clusters.at(nc_i)->charge_top[6+12] + clusters.at(nc_i)->charge_top[5+12] +
                            clusters.at(nc_i)->charge_top[4+12] + clusters.at(nc_i)->charge_top[11+12]);


                }


            }// end nc_i loop


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
            for(int nc_i = 0; nc_i < nc; nc_i++)
            {

                BoolCut C2(clusters, nc_i);
                total_cut_srt_loop2 = "C2.is_S1";
                if(C2.is_S1)
                {
                    h2_S1->Fill(clusters.at(nc_i)->pos_x, clusters.at(nc_i)->pos_y);
                    h1_S1->Fill(clusters.at(nc_i)->charge);
                }                

            }

            if(nc > 1)
                h1_S2_S1_ratio->Fill(clusters.at(1)->charge / clusters.at(0)->charge);

        }
    }


    //Draw options
    bool is_draw_h1 = 0;
    bool is_draw_h1_hist_overlap_var1 = 0;
    bool is_draw_h1_hist_overlap_var2 = 1;
    bool is_draw_h2 = 0;
    bool is_draw_h2_var1 = 0;
    bool is_draw_h2_var2 = 0;
    bool is_draw_h2_var3 = 0;
    bool is_draw_var4 = 0;

    if(is_draw_h1)
    {
        h1->SetTitle(total_cut_srt_loop2);
        //h1->SetTitle(total_cut_srt_loop1_0);
        //h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
        //h1->GetXaxis()->SetTitle("(clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000 [us]");
        h1->GetXaxis()->SetTitle("(clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000 [us]");
        h1->Draw();
    }

    if(is_draw_h1_hist_overlap_var1)
    {

        TCanvas *c1 = new TCanvas("c1","c1");
        gStyle->SetOptStat(0);

        h1_S1_bottom_chF2->SetTitle(total_cut_srt_loop1);
        h1_S1_bottom_chF2->GetXaxis()->SetTitle("clusters.at(nc_i)->charge_bottom[x] [PE]");

        h1_S1_bottom_chF2->SetLineWidth(2);
        h1_S1_bottom_chF2->SetLineColor(kBlue);

        h1_S1_bottom_chF3->SetLineWidth(2);
        h1_S1_bottom_chF3->SetLineColor(kRed);

        h1_S1_bottom_chF4->SetLineWidth(2);
        h1_S1_bottom_chF4->SetLineColor(kGreen);

        h1_S1_bottom_chF5->SetLineWidth(2);
        h1_S1_bottom_chF5->SetLineColor(kMagenta);

        h1_S1_bottom_chF2->Draw();
        h1_S1_bottom_chF3->Draw("same");
        h1_S1_bottom_chF4->Draw("same");
        h1_S1_bottom_chF5->Draw("same");

        h1_S1_bottom_chF2->GetYaxis()->SetRangeUser(0., 1600.);

        auto legend = new TLegend(0.7,0.6,0.9,0.9);
        //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
        legend->AddEntry(h1_S1_bottom_chF2,"F2 charge_bottom[0]","l");
        legend->AddEntry(h1_S1_bottom_chF3,"F3 charge_bottom[1]","l");
        legend->AddEntry(h1_S1_bottom_chF4,"F4 charge_bottom[2]","l");
        legend->AddEntry(h1_S1_bottom_chF5,"F5 charge_bottom[3]","l");
        legend->Draw();
    }

    if(is_draw_h1_hist_overlap_var2)
    {

        TCanvas *c1 = new TCanvas("c1","c1");
        c1->Divide(2,1,0.01,0.01);
        gStyle->SetOptStat(0);

        c1->cd(1);

        h1_S1_bottom_chF2->SetTitle(total_cut_srt_loop1);
        h1_S1_bottom_chF2->GetXaxis()->SetTitle("clusters.at(nc_i)->charge_bottom[x] [PE]");

        h1_S1_bottom_chF2->SetLineWidth(2);
        h1_S1_bottom_chF2->SetLineColor(kBlue);

        h1_S1_bottom_chF3->SetLineWidth(2);
        h1_S1_bottom_chF3->SetLineColor(kRed);

        h1_S1_bottom_chF5->SetLineWidth(2);
        h1_S1_bottom_chF5->SetLineColor(kGreen);

        h1_S1_bottom_chF4->SetLineWidth(2);
        h1_S1_bottom_chF4->SetLineColor(kMagenta);



        h1_S1_bottom_chF2->Draw();
        h1_S1_bottom_chF3->Draw("same");
        h1_S1_bottom_chF5->Draw("same");
        h1_S1_bottom_chF4->Draw("same");


        vector<double> max_values_h1;
        max_values_h1.push_back(h1_S1_bottom_chF2->GetMaximum());
        max_values_h1.push_back(h1_S1_bottom_chF3->GetMaximum());
        max_values_h1.push_back(h1_S1_bottom_chF4->GetMaximum());
        max_values_h1.push_back(h1_S1_bottom_chF5->GetMaximum());
        h1_S1_bottom_chF2->GetYaxis()->SetRangeUser(0., *max_element(max_values_h1.begin(), max_values_h1.end()) * 1.8);

        auto legend = new TLegend(0.35,0.6,0.9,0.9);
        legend->SetTextSize(0.03);
        //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
        legend->AddEntry(h1_S1_bottom_chF2,"F2 charge_bottom[0] (Left-Top)","l");
        legend->AddEntry(h1_S1_bottom_chF3,"F3 charge_bottom[1] (Right-Top)","l");
        legend->AddEntry(h1_S1_bottom_chF5,"F5 charge_bottom[3] (Left-Bot)","l");
        legend->AddEntry(h1_S1_bottom_chF4,"F4 charge_bottom[2] (Right-Bot)","l");
        legend->Draw();


        c1->cd(2);

        h1_S1_top_C5_D1_D5_E2_E3_E4->SetTitle(total_cut_srt_loop1);
        h1_S1_top_C5_D1_D5_E2_E3_E4->GetXaxis()->SetTitle("Summ of 6 ch: clusters.at(nc_i)->charge_top[x] [PE]");

        h1_S1_top_C5_D1_D5_E2_E3_E4->SetLineWidth(2);
        h1_S1_top_C5_D1_D5_E2_E3_E4->SetLineColor(kBlue);

        h1_S1_top_C4_C3_D4_D3_D2_E5->SetLineWidth(2);
        h1_S1_top_C4_C3_D4_D3_D2_E5->SetLineColor(kRed);

        h1_S1_top_A1_A2_B1_B2_C1_C2->SetLineWidth(2);
        h1_S1_top_A1_A2_B1_B2_C1_C2->SetLineColor(kGreen);

        h1_S1_top_A3_A4_A5_B4_B3_B5->SetLineWidth(2);
        h1_S1_top_A3_A4_A5_B4_B3_B5->SetLineColor(kMagenta);

        h1_S1_top_C5_D1_D5_E2_E3_E4->Draw();
        h1_S1_top_C4_C3_D4_D3_D2_E5->Draw("same");
        h1_S1_top_A1_A2_B1_B2_C1_C2->Draw("same");
        h1_S1_top_A3_A4_A5_B4_B3_B5->Draw("same");

        vector<double> max_values_h1_2;
        max_values_h1_2.push_back(h1_S1_top_C5_D1_D5_E2_E3_E4->GetMaximum());
        max_values_h1_2.push_back(h1_S1_top_C4_C3_D4_D3_D2_E5->GetMaximum());
        max_values_h1_2.push_back(h1_S1_top_A1_A2_B1_B2_C1_C2->GetMaximum());
        max_values_h1_2.push_back(h1_S1_top_A3_A4_A5_B4_B3_B5->GetMaximum());
        h1_S1_top_C5_D1_D5_E2_E3_E4->GetYaxis()->SetRangeUser(0., *max_element(max_values_h1_2.begin(), max_values_h1_2.end()) * 1.8 );

        auto legend_cd2 = new TLegend(0.35,0.6,0.9,0.9);
        legend_cd2->SetTextSize(0.03);
        //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
        legend_cd2->AddEntry(h1_S1_top_C5_D1_D5_E2_E3_E4,"C5_D1_D5_E2_E3_E4 (Left-Top)","l");
        legend_cd2->AddEntry(h1_S1_top_C4_C3_D4_D3_D2_E5,"C4_C3_D4_D3_D2_E5 (Right-Top)","l");
        legend_cd2->AddEntry(h1_S1_top_A1_A2_B1_B2_C1_C2,"A1_A2_B1_B2_C1_C2 (Left-Bot), B1=0","l");
        legend_cd2->AddEntry(h1_S1_top_A3_A4_A5_B4_B3_B5,"A3_A4_A5_B4_B3_B5 (Right-Bot)","l");
        legend_cd2->Draw();
    }

    if(is_draw_h2)
    {
        TCanvas *c1 = new TCanvas("c1","c1");
        c1->SetCanvasSize(950, 950);
        c1->SetWindowSize(980, 980);
        c1->Divide(2,2,0.01,0.01);
        //c1->Divide(2,1,0.01,0.01);

        c1->cd(1);
        h2->SetTitle(total_cut_srt_loop1);
        h2->GetXaxis()->SetTitle("x [cm]");
        h2->GetYaxis()->SetTitle("y [cm]");
        h2->GetXaxis()->SetRangeUser(1.5, 3.5);
        h2->GetYaxis()->SetRangeUser(1.5, 3.5);
        h2->Draw("colz");
        //h2->SetStats(0); //delete statbox
        gPad->Update();
        TPaveStats *st = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
        st->SetX1NDC(0.12); st->SetX2NDC(0.35);
        st->SetY1NDC(0.72); st->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        if(is_draw_h2_var1)
        {
            c1->cd(2);
            //TH2F *h2_copy = new TH2F(h2);
            TH2F *h2_copy = (TH2F*)h2->Clone("");//make clone to be free to delete statbox
            h2_copy->Draw("colz");
            h2_copy->GetXaxis()->SetRangeUser(0, 5);
            h2_copy->GetYaxis()->SetRangeUser(0, 5);
            h2_copy->SetStats(0); //delete statbox

            //draw SiPMs boxes
            double yw = 0.8;//cm
            double xw = 1.2;//cm
            for (int iy = 0; iy < 6; iy++)
            {
                double x_centr;
                double y_centr;
                for (int ix = 0; ix < 4; ix++)
                {
                    x_centr = ((0.5 + ix)*5/4);
                    y_centr = ((0.5 + iy)*5/6);

                    TBox *box = new TBox(x_centr - xw/2, y_centr - yw/2, x_centr + xw/2, y_centr + yw/2);
                    box->SetFillStyle(0);
                    box->SetLineColor(2);
                    box->SetLineWidth(2);
                    box->Draw();
                }
            }

            //propagation direction of the particles
            TArrow *ar1 = new TArrow(5, 0, 4, 1);
            ar1->SetLineWidth(4);
            ar1->SetLineColor(4);
            ar1->SetFillStyle(3008);
            ar1->SetFillColor(2);
            ar1->Draw();
        }
        else if (is_draw_h2_var2)
        {
            c1->cd(2);
            TH2F *h2_copy = (TH2F*)h2->Clone("");//make clone to be free to delete statbox
            h2_copy->Draw("colz");
            //h2_copy->GetXaxis()->SetRangeUser(0, 5);
            //h2_copy->GetYaxis()->SetRangeUser(0, 5);
            h2_copy->SetStats(0); //delete statbox

            Color_t box_color = kMagenta;
            int box_FillStyle = 0;
            int box_LineWidth = 2;
            double box_margin = 0.01;
            TBox *box1 = new TBox(area_cut_x1, area_cut_y1, area_cut_x2, area_cut_y2);
            box1->SetFillStyle(box_FillStyle);
            box1->SetLineColor(box_color);
            box1->SetLineWidth(box_LineWidth);
            box1->Draw();

            TBox *box2 = new TBox(area_cut_x3, area_cut_y1, area_cut_x4, area_cut_y2);
            box2->SetFillStyle(box_FillStyle);
            box2->SetLineColor(box_color);
            box2->SetLineWidth(box_LineWidth);
            box2->Draw();

            TBox *box3 = new TBox(area_cut_x1, area_cut_y3, area_cut_x2, area_cut_y4);
            box3->SetFillStyle(box_FillStyle);
            box3->SetLineColor(box_color);
            box3->SetLineWidth(box_LineWidth);
            box3->Draw();

            TBox *box4 = new TBox(area_cut_x3, area_cut_y3, area_cut_x4, area_cut_y4);
            box4->SetFillStyle(box_FillStyle);
            box4->SetLineColor(box_color);
            box4->SetLineWidth(box_LineWidth);
            box4->Draw();


            TBox *box5 = new TBox(area_cut_x2, area_cut_y2, area_cut_x3, area_cut_y3);
            box5->SetFillStyle(box_FillStyle);
            box5->SetLineColor(box_color);
            box5->SetLineWidth(box_LineWidth);
            box5->Draw();

        }
        else if (is_draw_h2_var3)
        {
            c1->cd(2);
            h1_Tdrift->SetTitle(total_cut_srt_loop1_0);
            //h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
            //h1_S2->GetXaxis()->SetTitle("clusters.at(nc_i)->charge [PE]");
            h1_Tdrift->GetXaxis()->SetTitle("(clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000 [us]");
            h1_Tdrift->Draw();
        }
        else if (is_draw_var4)
        {


                    //            c1->cd(2);
                    //            h2_S1->SetTitle(total_cut_srt_loop1);
                    //            h2_S1->GetXaxis()->SetTitle("x [cm]");
                    //            h2_S1->GetYaxis()->SetTitle("y [cm]");
                    //            h2_S1->GetXaxis()->SetRangeUser(1.5, 3.5);
                    //            h2_S1->GetYaxis()->SetRangeUser(1.5, 3.5);
                    //            h2_S1->Draw("colz");
                    //            //h2->SetStats(0); //delete statbox
                    //            gPad->Update();
                    //            TPaveStats *st2  = (TPaveStats*)h2_S1->GetListOfFunctions()->FindObject("stats");
                    //            st2->SetX1NDC(0.12); st2->SetX2NDC(0.35);
                    //            st2->SetY1NDC(0.72); st2->SetY2NDC(0.89);
                    //            gPad->Modified(); gPad->Update();


            c1->cd(2);
            gStyle->SetOptFit(1);
            h1_S2_S1_ratio->SetTitle(total_cut_srt_loop2);
            h1_S2_S1_ratio->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
            h1_S2_S1_ratio->Draw();
            gPad->Update();
            TPaveStats *st5  = (TPaveStats*)h1_S2_S1_ratio->GetListOfFunctions()->FindObject("stats");
            st5->SetX1NDC(0.60); st5->SetX2NDC(0.98);
            st5->SetY1NDC(0.50); st5->SetY2NDC(0.89);
            gPad->Modified(); gPad->Update();

            c1->cd(3);
            gStyle->SetOptFit(1);
            //gStyle->SetOptFit(0111);
            h1_S2->SetTitle(total_cut_srt_loop1);


            //h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
            h1_S2->GetXaxis()->SetTitle("clusters.at(nc_i)->charge [PE]");
            h1_S2->Draw();
            gPad->Update();
            TPaveStats *st3  = (TPaveStats*)h1_S2->GetListOfFunctions()->FindObject("stats");

            //func_gauss_3p_1p->SetParameters(500,h1_S2->GetMean(),h1_S2->GetRMS(),0.2);
            //h1_S2->Fit("gauss_3p_1p","R","",900, 1500);
//            st3->SetName("mystats");
//            TF1 *myfunc= h1_S2->GetFunction("gaus");

//            TList *list = st3->GetListOfLines();
//            //TLatex *myt = new TLatex(0, 0, "Test = 10");
//            TLatex *myt = new TLatex(0, 0, TString::Format("Ratio = %g", myfunc->GetParameter("Sigma") / myfunc->GetParameter("Mean")));
//            myt ->SetTextFont(42);
//            myt ->SetTextSize(0.04);
//            myt ->SetTextColor(kRed);
//            list->Add(myt);
            //h1_S2->SetStats(0);


            st3->SetX1NDC(0.60); st3->SetX2NDC(0.98);
            st3->SetY1NDC(0.50); st3->SetY2NDC(0.89);
            gPad->Modified(); gPad->Update();

            c1->cd(4);
            //gStyle->SetOptFit(1011);
            h1_S1->SetTitle(total_cut_srt_loop2);
            //h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
            h1_S1->GetXaxis()->SetTitle("clusters.at(nc_i)->charge [PE]");
            h1_S1->Draw();
            gPad->Update();
            TPaveStats *st4  = (TPaveStats*)h1_S1->GetListOfFunctions()->FindObject("stats");
            st4->SetX1NDC(0.60); st4->SetX2NDC(0.98);
            st4->SetY1NDC(0.50); st4->SetY2NDC(0.89);
            gPad->Modified(); gPad->Update();
        }

        gPad->Modified(); gPad->Update();
    }



    //finalization code
    cout << "all is ok" << endl;

    //Cleanup
    //delete app;

    return 0;
}
