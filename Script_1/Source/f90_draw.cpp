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


void f90_draw()
{
    //start code
    std::cout << "You are in program." << std::endl;

    //main code
//    vector<double> file_list{744, 745, 748, 749, 750, 751, 752, 753, 767, 768,
//                          769, 770, 779, 782, 785, 789, 794, 797, 800, 810,
//                          814, 817, 820, 823, 826, 827, 828, 829, 830, 831};
    //vector<double> file_list{744, 745, 748, 749, 750};
    vector<double> file_list{847};//field
    vector<TH1F*> TH1F_list;
    vector<double> f90_peak;

    ofstream file_out("/home/vlad/Reports/S1_LY/f90/result.txt");


    for(int i = 0; i < file_list.size(); i++)
    {
        ostringstream path_in_file;
        path_in_file << "/home/vlad/Reports/S1_LY/f90/run_" << file_list[i] << ".txt";
        ifstream file_in(path_in_file.str().c_str());
        if(file_in.is_open())
        {
            cout << "File has been opened correctly: " << path_in_file.str().c_str() << endl;
            ostringstream TH1F_name;
            TH1F_name << "run_" << file_list[i];
            TString TH1F_name_tsting = TH1F_name.str().c_str();
            TH1F_list.push_back(new TH1F(TH1F_name_tsting, TH1F_name_tsting, 200, 0, 1));

            double val = 0;
            while (file_in.good())
            {
                  //std::cout << c;
                  //c = ifs.get();
                //val = file_in.get();
                file_in >> val;
                //cout << "run = " << file_list[i] << "; val = " << val << endl;;
                TH1F_list.at(i)->Fill(val);
            }

            //TH1F_list.at(i)->Scale(1/TH1F_list.at(i)->Integral());
            TH1F_list.at(i)->SetLineWidth(2);
            TH1F_list.at(i)->Fit("gaus","","",0.19,0.32);
            TF1 *myfunc = TH1F_list.at(i)->GetFunction("gaus");
            double mean = myfunc->GetParameter(1);
            double sigma = myfunc->GetParameter(2);
            TH1F_list.at(i)->Fit("gaus","","",mean-1.5*sigma,mean+1.5*sigma);
            TF1 *myfunc2 = TH1F_list.at(i)->GetFunction("gaus");
            f90_peak.push_back(mean);
            file_out << file_list[i] << "\t" << mean << endl;
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
        }
        else
        {
            TH1F_list.at(i)->Draw("SAME");
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
    TH1F_list.at(0)->GetYaxis()->SetRangeUser(0, 10000);

    TCanvas *c2 = new TCanvas("c2","c2");
    TGraph *gr = new TGraph(f90_peak.size(), &file_list[0], &f90_peak[0]);
    gr->Draw();
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1);

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
