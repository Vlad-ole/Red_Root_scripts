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


int run_number = 1079;


void s2_analysis()
{
    ostringstream path_root_file;
    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
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
//    double S2_max = 20000;
//    double S2_S1_max = 40;

    //Am E_EL = 4.2kV
//    double S1_max = 1000;
//    double S2_max = 2500;
//    double S2_S1_max = 5;

    //Am E_EL = 5.0kV
//    double S1_max = 2000;
//    double S2_max = 20000;
//    double S2_S1_max = 40;

    //Am arb.
    double S1_max = 2000;
    double S2_max = 25000;
    double S2_S1_max = 40;

    //double range_scale = 1;
    vector<double> x_centers = {0.625, 1.875, 3.125, 4.375};
    vector<double> y_centers = {0.416667, 1.25, 2.08333, 2.91667, 3.75, 4.58333};
    int n_events_test = 0;
    double left_lim = 15 /*15*/;
    double right_lim = 55 /*55*/;
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

    //S2 uniformity
    vector<TH1F*> h1_S2_maxch_vec;
    for(int i = 0; i < 24; i++)
    {
        ostringstream oss;
        oss << "h1_S2_maxch_vec" << i;
        h1_S2_maxch_vec.push_back( new TH1F(oss.str().c_str(), oss.str().c_str(), 400, -100, S2_max) );
    }
    TGraph* gr_S2_maxch;

    //general
    TH1F *h1_Tdrift = new TH1F("h1 Tdrift", "h1 Tdrift", (Tdrift_max + 1.0), -1, Tdrift_max);
    TH1F *h1_nc = new TH1F("h1_nc", "h1_nc", 200, 0, 10);


    double S2_ev_par0 = 911.403;
    double S2_ev_par1 = 0;





    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        h1_nc->Fill(clusters.size());

        if(clusters.size() == 2 /*&& ev > 15000*/ && ev < 8000)
        {
            double TBA_S1 = (clusters.at(0)->tot_charge_top - clusters.at(0)->tot_charge_bottom) / (clusters.at(0)->tot_charge_top + clusters.at(0)->tot_charge_bottom);
            double TBA_S2 = (clusters.at(1)->tot_charge_top - clusters.at(1)->tot_charge_bottom) / (clusters.at(1)->tot_charge_top + clusters.at(1)->tot_charge_bottom);
            double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;
            bool cut_S1_total = clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800;
            bool cut_S2_total = clusters.at(1)->charge > 500 && clusters.at(1)->charge < 1500;
//            bool cut_S1_total = true;
//            bool cut_S2_total = true;

            if(cut_f90 && cut_S1_total /*&& cut_S2_total*/ && clusters.at(0)->rep == 1 /*&&
                    clusters.at(1)->charge/clusters.at(0)->charge > 5*/
                    /*&& clusters.at(1)->f90 < 0.05 && clusters.at(1)->f90 > 0*/)
            {
                //general
                h1_Tdrift->Fill(Tdrift);


                //S1
                h2_S1_total_event->Fill(ev, clusters.at(0)->charge);
                h1_S1_total->Fill(clusters.at(0)->charge);
                h1_S1_f90->Fill(clusters.at(0)->f90);
                h1_S1_top->Fill(clusters.at(0)->tot_charge_top);
                h1_S1_bot->Fill(clusters.at(0)->tot_charge_bottom);
                h1_S1_TBA->Fill(TBA_S1);
                //h2_S1_TBA->Fill(TBA_S1, clusters.at(0)->charge);
                h2_S1_total_tdrift->Fill(Tdrift, clusters.at(0)->charge);

                //S2
                h1_S2_S1_ratio->Fill(clusters.at(1)->charge/clusters.at(0)->charge);
                h1_S2_total->Fill(clusters.at(1)->charge);
                h1_S2_f90->Fill(clusters.at(1)->f90);
                h1_S2_top->Fill(clusters.at(1)->tot_charge_top);
                h1_S2_bot->Fill(clusters.at(1)->tot_charge_bottom);
                h1_S2_TBA->Fill(TBA_S2);
                h2_S2_total_tdrift->Fill(Tdrift, clusters.at(1)->charge);
                h2_S2_TBA_ev->Fill(ev, TBA_S2);
                h2_S2_total_event->Fill(ev, clusters.at(1)->charge);
                h2_S2_S1_ratio_tdrift->Fill(Tdrift, clusters.at(1)->charge/clusters.at(0)->charge);

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
//                                    cout << clusters.at(1)->pos_x << "\t" << clusters.at(1)->pos_y << "\t" << k << "\t" << j << "\t" << x_centers[k] << "\t" << y_centers[j] << endl;
//                                }
                        }
                    }
                }


                //S2 ev_corr
                double S2_ev_corr = clusters.at(1)->charge + S2_ev_par1*(max_ev_number/2.0 - ev);
                h2_S2_total_evcorr_tdrift->Fill(Tdrift, S2_ev_corr);
                //h2_S2_TBA->Fill(TBA_S1, clusters.at(1)->charge);
                //cout << "S2 = " << clusters.at(1)->charge << "; S2_ev_corr = " << S2_ev_corr << endl;

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

    TCanvas *c1 = new TCanvas("S1_analysis","S1_analysis");
    c1->Divide(3,2,0.01,0.01);
    vector<TPaveStats*> st_h1_S1(6);

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
    h2_S1_total_event->Draw("colz");
    TProfile *prof_h2_S1_total_event = h2_S1_total_event->ProfileX();
    prof_h2_S1_total_event->Draw("same");
    prof_h2_S1_total_event->SetMarkerStyle(20);
    prof_h2_S1_total_event->SetMarkerColor(kRed);
    gPad->Modified(); gPad->Update();




    TCanvas *c2 = new TCanvas("S2_analysis","S2_analysis (part 1)");
    c2->Divide(3,2,0.01,0.01);
    vector<TPaveStats*> st_h1_S2(6);

    cd_i = 0;
    c2->cd(cd_i + 1);
    h1_S2_total->Draw();
    h1_S2_total->GetXaxis()->SetTitle("[PE]");
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




    TCanvas *c3 = new TCanvas("S2_analysis p2","S2_analysis (part 2)");
    c3->Divide(3,2,0.01,0.01);
    vector<TPaveStats*> st_h1_S2_p2(6);

    cd_i = 0;
    c3->cd(cd_i + 1);
    h2_S2_total_tdrift->Draw();
    h2_S2_total_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
    h2_S2_total_tdrift->GetYaxis()->SetTitle("S2 [PE]");
    h2_S2_total_tdrift->Draw("colz");
    gPad->Update();

    TProfile *prof_h2_S2_total_tdrift = h2_S2_total_tdrift->ProfileX();
    prof_h2_S2_total_tdrift->Draw("same");
    prof_h2_S2_total_tdrift->SetMarkerStyle(20);
    prof_h2_S2_total_tdrift->SetMarkerColor(kBlack);

    st_h1_S2_p2[cd_i] = (TPaveStats*)h2_S2_total_tdrift->GetListOfFunctions()->FindObject("stats");
    st_h1_S2_p2[cd_i]->SetX1NDC(0.45); st_h1_S2_p2[cd_i]->SetX2NDC(0.89);
    st_h1_S2_p2[cd_i]->SetY1NDC(0.8); st_h1_S2_p2[cd_i]->SetY2NDC(0.89);

    TF1 *f1_exp_purity = new TF1("f1_exp_purity","exp([0] + x*[1])",0,150);
    prof_h2_S2_total_tdrift->Fit("f1_exp_purity","R","",left_lim,right_lim);
    ostringstream TPaveStats_fit_info_l1;
    TPaveStats_fit_info_l1 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_exp_purity->GetParameter(1) << " +- " << f1_exp_purity->GetParError(1)/pow(f1_exp_purity->GetParameter(1), 2.0) << " us";

    st_h1_S2_p2[cd_i] = (TPaveStats*)gPad->GetPrimitive("stats");
    st_h1_S2_p2[cd_i]->SetName("mystats_c2_cd1");

    TList *listOfLines = st_h1_S2_p2[cd_i]->GetListOfLines();

    TText *tconst = st_h1_S2_p2[cd_i]->GetLineWith("Std");
    listOfLines->Remove(tconst);
    tconst = st_h1_S2_p2[cd_i]->GetLineWith("Std");
    listOfLines->Remove(tconst);
    tconst = st_h1_S2_p2[cd_i]->GetLineWith("Mean");
    listOfLines->Remove(tconst);
    tconst = st_h1_S2_p2[cd_i]->GetLineWith("Mean");
    listOfLines->Remove(tconst);
    tconst = st_h1_S2_p2[cd_i]->GetLineWith("h2");
    listOfLines->Remove(tconst);
    tconst = st_h1_S2_p2[cd_i]->GetLineWith("Entries");
    listOfLines->Remove(tconst);

    TLatex *myt = new TLatex(0,0,TPaveStats_fit_info_l1.str().c_str());
       myt ->SetTextFont(42);
       myt ->SetTextSize(0.04);
       myt ->SetTextColor(kRed);
       listOfLines->Add(myt);
       h2_S2_total_tdrift->SetStats(0);
    gPad->Modified();

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
    gPad->Update();
    TProfile *prof_h2_S2_TBA_ev = h2_S2_TBA_ev->ProfileX();
    prof_h2_S2_TBA_ev->Draw("same");
    prof_h2_S2_TBA_ev->SetMarkerStyle(20);
    prof_h2_S2_TBA_ev->SetMarkerColor(kBlack);
    gPad->Modified(); gPad->Update();


    {
        cd_i = 3;
        c3->cd(cd_i + 1);
        h2_S2_S1_ratio_tdrift->Draw();
        h2_S2_S1_ratio_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S2_S1_ratio_tdrift->GetYaxis()->SetTitle("S2/S1");
        h2_S2_S1_ratio_tdrift->Draw("colz");
        gPad->Update();

        TProfile *prof_h2_S2_S1_ratio_tdrift = h2_S2_S1_ratio_tdrift->ProfileX();
        prof_h2_S2_S1_ratio_tdrift->Draw("same");
        prof_h2_S2_S1_ratio_tdrift->SetMarkerStyle(20);
        prof_h2_S2_S1_ratio_tdrift->SetMarkerColor(kBlack);

        st_h1_S2_p2[cd_i] = (TPaveStats*)h2_S2_S1_ratio_tdrift->GetListOfFunctions()->FindObject("stats");
        st_h1_S2_p2[cd_i]->SetX1NDC(0.45); st_h1_S2_p2[cd_i]->SetX2NDC(0.89);
        st_h1_S2_p2[cd_i]->SetY1NDC(0.8); st_h1_S2_p2[cd_i]->SetY2NDC(0.89);

        TF1 *f1_exp_purity_3 = new TF1("f1_exp_purity_3","exp([0] + x*[1])",0,150);
        prof_h2_S2_S1_ratio_tdrift->Fit("f1_exp_purity_3","R","",left_lim,right_lim);
        ostringstream TPaveStats_fit_info_l1_3;
        TPaveStats_fit_info_l1_3 << "Lifetime = " << std::fixed << std::showpoint << std::setprecision(1) << -1/f1_exp_purity_3->GetParameter(1) << " +- " << f1_exp_purity_3->GetParError(1)/pow(f1_exp_purity_3->GetParameter(1), 2.0) << " us";

        st_h1_S2_p2[cd_i] = (TPaveStats*)gPad->GetPrimitive("stats");
        st_h1_S2_p2[cd_i]->SetName("mystats_c2_cd1");

        TList *listOfLines_3 = st_h1_S2_p2[cd_i]->GetListOfLines();

        TText *tconst_3 = st_h1_S2_p2[cd_i]->GetLineWith("Std");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_p2[cd_i]->GetLineWith("Std");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_p2[cd_i]->GetLineWith("Mean");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_p2[cd_i]->GetLineWith("Mean");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_p2[cd_i]->GetLineWith("h2");
        listOfLines_3->Remove(tconst_3);
        tconst_3 = st_h1_S2_p2[cd_i]->GetLineWith("Entries");
        listOfLines_3->Remove(tconst_3);

        TLatex *myt_3 = new TLatex(0,0,TPaveStats_fit_info_l1_3.str().c_str());
        myt_3 ->SetTextFont(42);
        myt_3 ->SetTextSize(0.04);
        myt_3 ->SetTextColor(kRed);
        listOfLines_3->Add(myt_3);
        h2_S2_S1_ratio_tdrift->SetStats(0);
        gPad->Modified();
    }


    {
        cd_i = 4;
        c3->cd(cd_i + 1);
        h2_S1_total_tdrift->Draw();
        h2_S1_total_tdrift->GetXaxis()->SetTitle("Tdrif [us]");
        h2_S1_total_tdrift->GetYaxis()->SetTitle("S1");
        h2_S1_total_tdrift->Draw("colz");
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



    TCanvas *c4 = new TCanvas("General_analysis","General_analysis");
    c4->Divide(2,2,0.01,0.01);
    vector<TPaveStats*> st_general_analysis(4);

    cd_i = 0;
    c4->cd(cd_i + 1);
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

    {
        TCanvas *c5 = new TCanvas("S2_uniformity (part 1)","S2_uniformity (part 1)");
        c5->Divide(4,6,0.01,0.01);
        vector<TPaveStats*> st_general_analysis(6);
        double S2_sum_of_means = 0;
        for(int i = 0; i < 24; i++)
            S2_sum_of_means += h1_S2_maxch_vec[i]->GetMean();

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
        TCanvas *c6 = new TCanvas("S2_uniformity (part 2)","S2_uniformity (part 2)");
        c6->Divide(1,1,0.01,0.01);

        vector<double> S2_mean_v;
        vector<double> ch_number_v;
        S2_mean_v.resize(24);
        ch_number_v.resize(24);
        double S2_sum_of_means = 0;
        int n_ch_not_null =0;
        for(int i = 0; i < 24; i++)
        {
            S2_sum_of_means += h1_S2_maxch_vec[i]->GetMean();
            if( h1_S2_maxch_vec[i]->GetMean() )
                n_ch_not_null++;
        }
        COUT(n_ch_not_null);

        S2_sum_of_means /= n_ch_not_null;
        for(int i = 0; i < 24; i++)
        {
            COUT(h1_S2_maxch_vec[i]->GetEntries());
            S2_mean_v[i] = h1_S2_maxch_vec[i]->GetMean()/S2_sum_of_means;
            ch_number_v[i] = i+1;
            file_out_s2_uniformity << S2_mean_v[i] << endl;
            file_out_xy_distr << h1_S2_maxch_vec[i]->GetEntries() << endl;
        }

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

}
