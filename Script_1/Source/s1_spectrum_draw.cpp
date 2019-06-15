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

using namespace std;


void s1_spectrum_draw()
{
    //start code
    std::cout << "You are in program." << std::endl;

    //main code
//    vector<double> file_list{744, 745, 748, 749, 750, 751, 752, 753, 767, 768,
//                          769, 770, 779, 782, 785, 789, 794, 797, 800, 810,
//                          814, 817, 820, 823, 826, 827, 828, 829, 830, 831};
    //vector<double> file_list{744, 745, 748, 749, 750};
    vector<int> file_list{1025, 1031, 1033, 1034};
    vector<TH1F*> TH1F_list;
    vector<double> f90_peak;

    string plot_type;
    plot_type = "S1_f90";
    //plot_type = "TBA";
    //plot_type = "S1_tot";
    //plot_type = "S1_top";
    //plot_type = "S1_bot";
    //plot_type = "baseline_mean_ch0";
    //plot_type = "baseline_mean_ch1";
    //plot_type = "baseline_mean_ch2";
    //plot_type = "baseline_mean_ch3";

    //ofstream file_out("/home/vlad/Reports/S1_LY/s1_spectrum/result.txt");

    int N_bins = 0;
    double hist_min = 0;
    double hist_max = 0;

    if(plot_type == "S1_f90") { N_bins = 400; hist_min = 0; hist_max = 1;}
    else if(plot_type == "TBA") {N_bins = 400; hist_min = -1; hist_max = 1;}
    else if(plot_type == "S1_tot") {N_bins = 400; hist_min = 0; hist_max = 5000;}
    else if(plot_type == "S1_top") {N_bins = 400; hist_min = 0; hist_max = 3000;}
    else if(plot_type == "S1_bot") {N_bins = 400; hist_min = 0; hist_max = 3000;}
    else if(plot_type == "baseline_mean_ch0" || plot_type == "baseline_mean_ch1" ||
            plot_type == "baseline_mean_ch2" || plot_type == "baseline_mean_ch3")
    {N_bins = 100; hist_min = 14300; hist_max = 14500;}



    for(int i = 0; i < file_list.size(); i++)
    {
        ostringstream path_in_file;

        if(plot_type == "S1_f90")
            path_in_file << "/home/vlad/Reports/run_properties/run_" << file_list[i] << "_S1_f90.txt";
        else if(plot_type == "TBA")
            path_in_file << "/home/vlad/Reports/run_properties/run_" << file_list[i] << "_TBA.txt";
        else if(plot_type == "S1_tot")
            path_in_file << "/home/vlad/Reports/run_properties/run_" << file_list[i] << "_S1_tot.txt";
        else if(plot_type == "S1_top")
            path_in_file << "/home/vlad/Reports/run_properties/run_" << file_list[i] << "_S1_top.txt";
        else if(plot_type == "S1_bot")
            path_in_file << "/home/vlad/Reports/run_properties/run_" << file_list[i] << "_S1_bot.txt";
        else if(plot_type == "baseline_mean_ch0" || plot_type == "baseline_mean_ch1" ||
                plot_type == "baseline_mean_ch2" || plot_type == "baseline_mean_ch3")
            path_in_file << "/home/vlad/Reports/run_properties/run_" << file_list[i] << "_baseline_mean_ch_0_1_2_3.txt";



        ifstream file_in(path_in_file.str().c_str());
        if(file_in.is_open())
        {
            cout << "File has been opened correctly: " << path_in_file.str().c_str() << endl;
            ostringstream TH1F_name;
            TH1F_name << "run_" << file_list[i];
            TString TH1F_name_tsting = TH1F_name.str().c_str();

            TH1F_list.push_back(new TH1F(TH1F_name_tsting, TH1F_name_tsting, N_bins, hist_min, hist_max));

            double val = 0;
            double x0, x1, x2, x3 = 0;
            while (file_in.good())
            {
                if(plot_type == "baseline_mean_ch0" || plot_type == "baseline_mean_ch1" ||
                        plot_type == "baseline_mean_ch2" || plot_type == "baseline_mean_ch3")
                {
                    file_in >> x0 >> x1 >> x2 >> x3;
                    if(plot_type == "baseline_mean_ch0")
                        TH1F_list.at(i)->Fill(x0);
                    else if(plot_type == "baseline_mean_ch1")
                        TH1F_list.at(i)->Fill(x1);
                    else if(plot_type == "baseline_mean_ch2")
                        TH1F_list.at(i)->Fill(x2);
                    else if(plot_type == "baseline_mean_ch3")
                        TH1F_list.at(i)->Fill(x3);

                }
                else
                {
                    file_in >> val;
                    TH1F_list.at(i)->Fill(val);
                }
            }

            //TH1F_list.at(i)->Scale(1/TH1F_list.at(i)->Integral());
            TH1F_list.at(i)->SetLineWidth(2);
            //TH1F_list.at(i)->Fit("gaus","","",0.19,0.32);
            //TF1 *myfunc = TH1F_list.at(i)->GetFunction("gaus");
            //double mean = myfunc->GetParameter(1);
            //double sigma = myfunc->GetParameter(2);
            //TH1F_list.at(i)->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
            //TF1 *myfunc2 = TH1F_list.at(i)->GetFunction("gaus");
            //f90_peak.push_back(mean);
            //file_out << file_list[i] << "\t" << mean << endl;

            ostringstream hist_name;
            hist_name << "run_" << file_list[i];

            if(file_list[i] == 1025)
                hist_name << "Am GP OFF";
            if(file_list[i] == 1031)
                hist_name << "Am GP 16V";
            if(file_list[i] == 1033)
                hist_name << " Am GP 24V";
            if(file_list[i] == 1034)
                hist_name << " Am GP OFF";


            TString TGraph_name_tsting = hist_name.str().c_str();
            TH1F_list.at(i)->SetTitle(TGraph_name_tsting);

            //cout << file_list[i] << "\t" << TGraph_name_tsting << "\t" << TH1F_list.at(i)->GetTitle() << endl;


        }
        else
        {
            cout << "Error: " << path_in_file.str().c_str() << endl;
            exit(1);
        }

    }

    auto legend = new TLegend(0.7,0.13,0.9,0.43);
    int nColors = gStyle->GetNumberOfColors();
    int nHistos = TH1F_list.size();
    //gStyle->SetPalette(kOcean);

    for(int i = 0; i < TH1F_list.size(); i++)
    {

        if(i == 0)
        {
           TH1F_list.at(i)->Draw("");
           TH1F_list.at(i)->SetStats(0);
           TH1F_list.at(i)->Scale(1/TH1F_list.at(i)->Integral());
           TH1F_list.at(i)->SetLineColor(kBlack);
        }
        else
        {
            TH1F_list.at(i)->Draw("SAME");
            TH1F_list.at(i)->Scale(1/TH1F_list.at(i)->Integral());
            //TH1F_list.at(i)->SetLineColor(kRed);
        }


        if(i == 1)
        {
            TH1F_list.at(i)->SetLineColor(kRed);
            //TGraph_list.at(i)->SetMarkerColor(kRed);
        }
        if(i == 2)
        {
            TH1F_list.at(i)->SetLineColor(kBlue);
            //TGraph_list.at(i)->SetMarkerColor(kBlue);
        }
        if(i == 3)
        {
            TH1F_list.at(i)->SetLineColor(kMagenta);
            //TGraph_list.at(i)->SetMarkerColor(kMagenta);
        }
        if(i == 4)
        {
            TH1F_list.at(i)->SetLineColor(kGreen);
            //TGraph_list.at(i)->SetMarkerColor(kGreen);
        }
        if(i == 5)
        {
            TH1F_list.at(i)->SetLineColor(kOrange);
            //TGraph_list.at(i)->SetMarkerColor(kOrange);
        }
        if(i == 6)
        {
            //TGraph_list.at(i)->SetLineColor(kTeal);
            //TGraph_list.at(i)->SetMarkerColor(kTeal );
            TH1F_list.at(i)->SetLineColor(kRed-3);
            //TGraph_list.at(i)->SetMarkerColor(kRed-3);
        }



        //int histoColor = (float)nColors / nHistos * (i+1) ;
        //TH1F_list.at(i)->SetLineColor(histoColor);

//        if( i == 0)
//        {
//            TH1F_list.at(i)->SetLineColor(kBlack);
//            TH1F_list.at(i)->SetMarkerSize(1);
//            TH1F_list.at(i)->SetMarkerColor(kBlack);
//        }
//        if(i==1)
//            TH1F_list.at(i)->SetLineColor(kRed);


        legend->AddEntry(TH1F_list.at(i),TH1F_list.at(i)->GetTitle(),"lep");

    }
    legend->Draw();

    if( !(plot_type == "baseline_mean_ch0" || plot_type == "baseline_mean_ch1" || plot_type == "baseline_mean_ch2" || plot_type == "baseline_mean_ch3"))
        TH1F_list.at(0)->SetTitle("clusters.size() == 1 && clusters.at(0)->charge > 1200");
    else
        TH1F_list.at(0)->SetTitle("all events");
    //TH1F_list.at(0)->GetXaxis()->SetTitle(" \"S1\" charge [pe]");

    if(plot_type == "S1_f90") { TH1F_list.at(0)->GetXaxis()->SetTitle(" \"S1\" f90");}
    else if(plot_type == "TBA") {TH1F_list.at(0)->GetXaxis()->SetTitle("TBA");}
    else if(plot_type == "S1_tot") {TH1F_list.at(0)->GetXaxis()->SetTitle(" \"S1\" total [pe]");}
    else if(plot_type == "S1_top") {TH1F_list.at(0)->GetXaxis()->SetTitle(" \"S1\" top [pe]");}
    else if(plot_type == "S1_bot") {TH1F_list.at(0)->GetXaxis()->SetTitle(" \"S1\" bottom [pe]");}
    else if(plot_type == "baseline_mean_ch0") {TH1F_list.at(0)->GetXaxis()->SetTitle(" baseline_mean_ch0");}
    else if(plot_type == "baseline_mean_ch1") {TH1F_list.at(0)->GetXaxis()->SetTitle(" baseline_mean_ch1");}
    else if(plot_type == "baseline_mean_ch2") {TH1F_list.at(0)->GetXaxis()->SetTitle(" baseline_mean_ch2");}
    else if(plot_type == "baseline_mean_ch3") {TH1F_list.at(0)->GetXaxis()->SetTitle(" baseline_mean_ch3");}


    //TH1F_list.at(0)->GetXaxis()->SetTitle(" \"S1\" f90");

//    TH1F_list.at(0)->Add(TH1F_list.at(1), -1);
//    TH1F_list.at(0)->Draw("");


    //TH1F_list.at(0)->GetYaxis()->SetRangeUser(0, 1);

//    TCanvas *c2 = new TCanvas("c2","c2");
//    TGraph *gr = new TGraph(f90_peak.size(), &file_list[0], &f90_peak[0]);
//    gr->Draw();
//    gr->SetMarkerStyle(20);
//    gr->SetMarkerSize(1);

//    TCanvas *c1 = new TCanvas("c1","c1");
//    h1_f90->Draw();
//    h1_f90->Sumw2(kTRUE);
//    h1_f90->GetXaxis()->SetTitle("S1 f90");
//    h1_f90->SetFillColor(kMagenta);
//    h1_f90->Scale(1/h1_f90->Integral());
//    h1_f90->SetStats(0); //delete statbox
//    gPad->Modified(); gPad->Update();


//    auto legend = new TLegend(0.7,0.13,0.9,0.43);
//    //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
//    //legend->AddEntry(h1_f90,"744","lep");
//    //legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
//    //legend->AddEntry("gr","Graph with error bars","lep");
//    legend->Draw();


}
