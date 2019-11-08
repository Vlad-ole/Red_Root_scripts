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
#include "TGaxis.h"


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;


using namespace std;


int run_number = 1269;

void s1_analysis()
{
    ostringstream path_root_file;
    if(run_number >= 743 && run_number <= 1034)
        path_root_file << "/media/vlad/Data/DS-data/reco/camp_VII/v3/" << "run_" << run_number << ".root";
    else if(run_number >= 1040)
        path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";

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

    double S1_max = 2000;

    TH1F *h1_S1_total = new TH1F("h1_S1_total", "h1_S1_total", 400, 0, S1_max);
    TH1F *h1_S1_f90 = new TH1F("h1_S1_f90", "h1_S1_f90", 400, -0.2, 1);

    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();

        if(clusters.size() == 1)
        {
            if(clusters.at(0)->f90 > 0.2)
            {
                h1_S1_total->Fill(clusters.at(0)->charge);
                h1_S1_f90->Fill(clusters.at(0)->f90);
            }
        }

    }


    h1_S1_total->Draw();
    double low = 0;
    double high = 100;
    gPad->Update();
    TGaxis *axis = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(),gPad->GetUymax(),low,high,510,"-L");
    //TGaxis *axis = new TGaxis(0,175,2000,175,low,high,510,"");
    //TGaxis *axis = new TGaxis(h1_S1_total->GetXaxis()->GetXmin(),h1_S1_total->GetYaxis()->GetXmin(),h1_S1_total->GetXaxis()->GetXmax(),h1_S1_total->GetYaxis()->GetXmin(),low,high,510,"");
    axis->Draw();


}

