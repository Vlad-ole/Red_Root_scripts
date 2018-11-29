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
#include "TRandom3.h"


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

using namespace std;

void sumsignals()
{
    TH1F *h1 = new TH1F("h1","h1", 1000, -20, 1000);

    vector<double> bin_center;
    vector<double> bin_content;

    ifstream f_input("/home/vlad/Soft/Red_Root_scripts/Script_1/Source/mean_sigma.txt");
    vector<double> mean_v;
    vector<double> sigma_v;
    double mean = 0;
    double sigma = 0;
    while (f_input >> mean >> sigma)
    {
        mean_v.push_back(mean);
        sigma_v.push_back(sigma);
    }


    TRandom3 rndm3;

    for(int i = 0; i < 100000; i++)
    {
        double value = 0;

        //value = rndm3.Gaus(50, 10);

        for (int j = 0; j < mean_v.size(); j++)
        {
            value += rndm3.Gaus(mean_v[j], sigma_v[j]);
        }

        h1->Fill(value);
    }

    gStyle->SetOptFit(1);
    h1->Draw();
    h1->GetXaxis()->SetTitle("S1 charge as sum of all ch [PE]");

}
