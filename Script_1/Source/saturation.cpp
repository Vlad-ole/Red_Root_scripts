//C++
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>

//root cern
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
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
#include "TMultiGraph.h"

//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

using namespace std;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void saturation()
{
    int run_number = 1099;

    ostringstream run_number_osst;
    run_number_osst << "run " << run_number;
    string run_number_str = run_number_osst.str();

    ostringstream path_root_file;
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

    TTree *data = (TTree*)f->Get("reco");
    EvRec0* evReco = new EvRec0();
    data->SetBranchAddress("recoevent",&evReco);

    TGraph *gr_S2_amp_S2_charge;
    vector<double> S2_amp_v;
    vector<double> S2_charge_v;


//    //v1
//    for (int ev = 0; ev < data->GetEntries(); ev++)
//    {
//        data->GetEntry(ev);
//        vector<RDCluster*> clusters = evReco->GetClusters();

//        if(clusters.size() == 2)
//        {
//            double Tdrift = (clusters.at(1)->cdf_time - clusters.at(0)->cdf_time) * 2./1000;

//            bool cut_S1_total = clusters.at(0)->charge > 400 && clusters.at(0)->charge < 800;
//            bool cut_f90 = clusters.at(0)->f90 > 0.2 && clusters.at(1)->f90 < 0.2;

//            if(true && cut_f90 && cut_S1_total && clusters.at(0)->rep == 1 )
//            {
//                S2_amp_v.push_back( evReco->GetBaseMean().at(1) - evReco->GetYmin().at(1) );
//                S2_charge_v.push_back( clusters.at(1)->charge );

//                //COUT(ev);
//            }

//        }

//    }

//    gr_S2_amp_S2_charge = new TGraph(S2_amp_v.size(), &S2_charge_v[0], &S2_amp_v[0]);
//    gr_S2_amp_S2_charge->Draw("AP");
//    gr_S2_amp_S2_charge->GetXaxis()->SetRangeUser(0, 50000);
//    gr_S2_amp_S2_charge->GetXaxis()->SetTitle("Charge");
//    gr_S2_amp_S2_charge->GetYaxis()->SetTitle("Amplitude");
//    gr_S2_amp_S2_charge->SetMarkerSize(1.2);

    //v2
    //data->Draw("baseline_mean[1]-ymin[1]:clusters[0].charge", "clusters[0].f90<0.2 && number_of_clusters>=1");
    //data->Draw("baseline_mean[1]-ymin[1]:clusters[1].charge", "clusters[1].f90<0.2 && number_of_clusters==2");
    //data->Draw("baseline_mean[1]-ymin[1]:clusters[1].charge", "clusters[1].f90<0.2 && clusters[0].f90>0.2 && clusters[0].charge>400 && clusters[0].charge<800 && clusters[0].rep==1 && number_of_clusters==2");
    //data->Draw("clusters[0].f90>>h1(100, 0, 1)", "number_of_clusters==2 && clusters[0].charge>400 && clusters[0].charge<800");
    //data->Draw("(clusters[1].min_y:clusters[1].charge)>>h2(100, 0, 100E3, 100, -20, 100)", "clusters[1].f90<0.2 && number_of_clusters==2");
    //data->Draw("clusters[1].min_y:clusters[1].charge", "clusters[1].f90<0.2 && number_of_clusters==2");
    data->Draw("clusters[1].min_y:clusters[1].charge>>hist", "clusters[1].f90<0.2 && clusters[0].f90>0.2 && clusters[0].charge>400 && clusters[0].charge<800 && clusters[0].rep==1 && number_of_clusters==2");
    TH2F *hist = (TH2F*)gDirectory->Get("hist"); // or get hnew from the current Pad
    Int_t hist_nev = hist->GetEntries();
    COUT(hist_nev);
}
