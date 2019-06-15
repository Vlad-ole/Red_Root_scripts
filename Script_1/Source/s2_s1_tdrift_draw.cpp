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


void s2_s1_tdrift_draw()
{
    //start code
    std::cout << "You are in program." << std::endl;

    //main code
    vector<int> file_list{1025, 1031, 1033, 1034};
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
        path_in_file << "/home/vlad/Reports/S2_S1/run_" << file_list[i] << ".txt";
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
            while (file_in.good())
            {
                 file_in >> x >> y;
                 xv.push_back(x);
                 yv.push_back(y);
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
                TGraph_name << "; Kr * const; GP 16V";
            if(file_list[i] == 993)
                TGraph_name << "; Am front; GP OFF";
            if(file_list[i] == 996)
                TGraph_name << "; Am front; GP 16V";
            if(file_list[i] == 1025)
                TGraph_name << "; Am; GP OFF";
            if(file_list[i] == 1030)
                TGraph_name << "; Am; GP 16V";
            if(file_list[i] == 1031)
                TGraph_name << "; Am; GP 16V";
            if(file_list[i] == 1033)
                TGraph_name << "; Am; GP 24V";
            if(file_list[i] == 1034)
                TGraph_name << "; Am; GP OFF";

            if(file_list[i] == 971)
            {
                for(int j = 0; j < yv.size(); j++)
                {
                    yv[j] *= 1.35;
                }
            }


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


    auto legend = new TLegend(0.7,0.13,0.9,0.43);
    for(int i = 0; i < TGraph_list.size(); i++)
    {
        TGraph_list.at(i)->SetMarkerStyle(20);

        if(i == 0)
        {
           TGraph_list.at(i)->Draw("AP");
           //TGraph_list.at(i)->SetStats(0);
           TGraph_list.at(i)->SetLineColor(kBlack);
           TGraph_list.at(i)->SetMarkerColor(kBlack);
           //TH1F_list.at(i)->GetXaxis()->SetTitle("TBA");
           //TH1F_list.at(i)->SetTitle("");
        }
        else
        {
            TGraph_list.at(i)->Draw("SAME P");
        }

        if(i == 1)
        {
            TGraph_list.at(i)->SetLineColor(kRed);
            TGraph_list.at(i)->SetMarkerColor(kRed);
        }
        if(i == 2)
        {
            TGraph_list.at(i)->SetLineColor(kBlue);
            TGraph_list.at(i)->SetMarkerColor(kBlue);
        }
        if(i == 3)
        {
            TGraph_list.at(i)->SetLineColor(kMagenta);
            TGraph_list.at(i)->SetMarkerColor(kMagenta);
        }
        if(i == 4)
        {
            TGraph_list.at(i)->SetLineColor(kGreen);
            TGraph_list.at(i)->SetMarkerColor(kGreen);
        }
        if(i == 5)
        {
            TGraph_list.at(i)->SetLineColor(kOrange);
            TGraph_list.at(i)->SetMarkerColor(kOrange);
        }
        if(i == 6)
        {
            //TGraph_list.at(i)->SetLineColor(kTeal);
            //TGraph_list.at(i)->SetMarkerColor(kTeal );
            TGraph_list.at(i)->SetLineColor(kRed-3);
            TGraph_list.at(i)->SetMarkerColor(kRed-3);
        }


//        kWhite =0,   kBlack =1,   kGray=920,
//                      kRed   =632, kGreen =416, kBlue=600, kYellow=400, kMagenta=616, kCyan=432,
//                      kOrange=800, kSpring=820, kTeal=840, kAzure =860, kViolet =880, kPink=900


        legend->AddEntry(TGraph_list.at(i),TGraph_list.at(i)->GetTitle(),"lep");

    }


    legend->Draw();
    TGraph_list.at(0)->GetXaxis()->SetTitle("Tdrift [us]");
    TGraph_list.at(0)->GetYaxis()->SetTitle("S2/S1");
    //TGraph_list.at(0)->GetYaxis()->SetTitle("S1_bottom [pe]");
    //TGraph_list.at(0)->GetYaxis()->SetTitle("S1_top [pe]");
    TGraph_list.at(0)->SetTitle("");

    TF1 *f1_exp_purity_S2_S1 = new TF1("f1_exp_purity_S2_S1","exp([0] + x*[1])",0,150);
    TGraph_list.at(0)->Fit("f1_exp_purity_S2_S1","R","",20,50);
    cout << "e- lifetime (S2/S1 vs T_drift) [us] = " << -1/f1_exp_purity_S2_S1->GetParameter(1) << " +- " << f1_exp_purity_S2_S1->GetParError(1)/pow(f1_exp_purity_S2_S1->GetParameter(1), 2.0) << endl;


}
