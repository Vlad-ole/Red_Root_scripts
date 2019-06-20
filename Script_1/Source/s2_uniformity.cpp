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

//my
#include <functional>

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

using namespace std;


void s2_uniformity()
{
    //start code
    std::cout << "You are in program." << std::endl;

    TCanvas *c1 = new TCanvas("c1","c1");
    c1->SetGridx(10);

    //main code
    vector<int> file_list{971, 1059, 1061};
    //vector<int> file_list{921, 956, 971, 996, 1018, 1051, 1066};
    //vector<int> file_list{847, 884, 921, 956, 971, 996, 1018};
    //vector<int> file_list{847, 884, 921, 956, 996, 971, 1018};
    //vector<int> file_list{993, 996};
    //vector<int> file_list{952, 956};
    //vector<int> file_list{870, 872, 874, 878, 880, 882, 884};
    //vector<int> file_list{847, 884, 921, 956, 996};
    //vector<double> file_list{844, 872, 899, 921, 948, 996};
    //vector<double> file_list{844, 847, 852, 854, 870, 872, 921, 948, 996};
    vector<TGraph*> TGraph_list;


    for(int i = 0; i < file_list.size(); i++)
    {
        ostringstream path_in_file;
        path_in_file << "/home/vlad/Reports/XY/S2_uniformity/run_" << file_list[i] << ".txt";
        //path_in_file << "/home/vlad/Reports/S1_LY/s1_tdrift/run_" << file_list[i] << ".txt";
        //path_in_file << "/home/vlad/Reports/S1_LY/s1_bottom_tdrift/run_" << file_list[i] << ".txt";
        //path_in_file << "/home/vlad/Reports/S1_LY/s1_top_tdrift/run_" << file_list[i] << ".txt";
        ifstream file_in(path_in_file.str().c_str());
        if(file_in.is_open())
        {
            cout << "File has been opened correctly: " << path_in_file.str().c_str() << endl;

            double x = 0;
            double y = 0;
            vector<double> xv;
            vector<double> yv;
            double ch_num = 0;
            while (file_in.good())
            {
                 file_in >> y;
                 ch_num++;
                 xv.push_back(ch_num);
                 yv.push_back(y);
            }

            for(int k = 0; k < 10; k++)
            {
                xv.push_back(24 + k + 1);
                yv.push_back(0);
            }

            ostringstream TGraph_name;
            TGraph_name << "run_" << file_list[i];
            if(file_list[i] == 847)
                TGraph_name << "; Am front; GP ON";
            if(file_list[i] == 870)
                TGraph_name << "; Am front-center; GP OFF";
            if(file_list[i] == 872)
                TGraph_name << "; Am front-center; GP 5V";
            if(file_list[i] == 874)
                TGraph_name << "; Am front-center; GP 8V";
            if(file_list[i] == 878)
                TGraph_name << "; Am front-center; GP 11V";
            if(file_list[i] == 880)
                TGraph_name << "; Am front-center; GP 13V";
            if(file_list[i] == 882)
                TGraph_name << "; Am front-center; GP 15V";
            if(file_list[i] == 884)
                TGraph_name << "; Am front-center; GP 16V";
            if(file_list[i] == 921)
                TGraph_name << "; Am +180 deg; GP 16V";
            if(file_list[i] == 952)
                TGraph_name << "; Am front; GP OFF";
            if(file_list[i] == 956)
                TGraph_name << "; Am front; GP 16V";
            if(file_list[i] == 971)
                TGraph_name << "; Kr ; GP 16V";
            if(file_list[i] == 993)
                TGraph_name << "; Am front; GP OFF";
            if(file_list[i] == 996)
                TGraph_name << "; Am front; GP 16V";
            if(file_list[i] == 1018)
                TGraph_name << "; Am front; GP OFF";
            if(file_list[i] == 1051)
                TGraph_name << "; Am ; GP 20V";
            if(file_list[i] == 1059)
                TGraph_name << "; Bkg ; GP 20V";
            if(file_list[i] == 1061)
                TGraph_name << "; Kr ; GP 20V";
            if(file_list[i] == 1066)
                TGraph_name << "; Am ; GP 20V";

//            if(file_list[i] == 971)
//            {
//                for(int j = 0; j < yv.size(); j++)
//                {
//                    yv[j] *= 1.35;
//                }
//            }


            TString TGraph_name_tsting = TGraph_name.str().c_str();
            TGraph_list.push_back( new TGraph(xv.size(), &xv[0], &yv[0]) );
            TGraph_list.at(i)->SetTitle(TGraph_name_tsting);

        }
        else
        {
            cout << "Error: " << path_in_file.str().c_str() << endl;
            exit(1);
        }

    }

    vector<int> color = {kBlack, kRed, kBlue, kMagenta, kGreen, kOrange, 28};


    auto legend = new TLegend(0.7,0.13,0.9,0.43);
    for(int i = 0; i < TGraph_list.size(); i++)
    {
        TGraph_list.at(i)->SetMarkerStyle(20);

        if(i == 0)
        {
           TGraph_list.at(i)->Draw("APL");
           //TGraph_list.at(0)->GetXaxis()->SetRangeUser(20, 40);
           //TGraph_list.at(i)->Draw("APL");
           //TGraph_list.at(i)->SetStats(0);

           //TH1F_list.at(i)->GetXaxis()->SetTitle("TBA");
           //TH1F_list.at(i)->SetTitle("");
        }
        else
        {
            TGraph_list.at(i)->Draw("SAME PL");
        }

        TGraph_list.at(i)->SetLineColor(color[i]);
        TGraph_list.at(i)->SetMarkerColor(color[i]);


//        kWhite =0,   kBlack =1,   kGray=920,
//                      kRed   =632, kGreen =416, kBlue=600, kYellow=400, kMagenta=616, kCyan=432,
//                      kOrange=800, kSpring=820, kTeal=840, kAzure =860, kViolet =880, kPink=900


        legend->AddEntry(TGraph_list.at(i),TGraph_list.at(i)->GetTitle(),"lep");

    }





    legend->Draw();
    TGraph_list.at(0)->GetXaxis()->SetTitle("Ch number, S2_max_ch");
    TGraph_list.at(0)->GetYaxis()->SetTitle("S2_total rel. [a.u.]");
    //TGraph_list.at(0)->GetYaxis()->SetTitle("S1_bottom [pe]");
    //TGraph_list.at(0)->GetYaxis()->SetTitle("S1_top [pe]");
    TGraph_list.at(0)->SetTitle("");
    gPad->Modified(); gPad->Update();





}
