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


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//usefull directive
#define COUT(x) cout << #x " = " << x << endl;

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    //start code
    std::cout << "You are in program." << std::endl;

    //TApplication* app = new TApplication("App",0,0);


    //test code
    //    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    //    hist->FillRandom("gaus", 5000);
    //    hist->Draw("");


    //main code
    ostringstream path_root_file;
    //string path_root_file = "/home/vlad/Soft/Red_Soft/red-daq-light/src/Level1/";
    //string path_root_file = "/media/vlad/Data/DS-data/reco/rm3reco/lns/camp_V/v3/";
    int run_number = 532;
    path_root_file << "/media/vlad/Data/DS-data/reco/rm3reco/lns/camp_V/v3/" << "run_" << run_number << ".root";
    //path_root_file << "/media/vlad/Data/DS-data/reco/rm3reco/lns/camp_V/v3_vlad/" << "run_" << 537 << ".root";
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

    //int ev = 2;
//    data->GetEntry(ev);
//    size_t nchannels = evReco->GetCharge().size();
//    cout << "There are " << nchannels << " channnels" << endl;

//    vector<RDCluster*> clusters = evReco->GetClusters();
//    size_t nc = clusters.size();
//    cout << "Found n. " << nc << " clusters" << endl;

//    if(nc == 0)
//    {

//    }
//    else if (nc == 1)
//    {
//        cout << "pos_x = " << clusters[0]->pos_x << "; pos_y = " << clusters[0]->pos_y << endl;
//    }
    //

    ostringstream h2_title;
    h2_title << "run_" << run_number  << ". Cuts: nc == 2 && nc_i == 1 && clusters.at(0)->rep == 1";
    TH2F *h2 = new TH2F("h2 name", h2_title.str().c_str(), 200, 0, 5, 200, 0, 5);

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();
        //cout << "Found n. " << nc << " clusters" << endl;


        if(nc)
        {
            for(int nc_i = 0; nc_i < nc; nc_i++)
            {
                if (/*true*/ ( (nc == 2 && nc_i == 1) /*|| (nc == 1) */) && clusters.at(0)->rep == 1) //cuts
                {
                    //cout << "   pos_x = " << clusters[nc_i]->pos_x << "; pos_y = " << clusters[nc_i]->pos_y << endl;
                    h2->Fill(clusters.at(nc_i)->pos_x, clusters.at(nc_i)->pos_y);
                }
            }
        }

        //cout << endl;


        if(ev%1000==0) cout << "Event " << ev << " processed" << endl;

    }

    TCanvas *c1 = new TCanvas("c1","c1");
    c1->SetCanvasSize(850, 850);
    c1->SetWindowSize(900, 900);

    h2->Draw("colz");
    gPad->Update();
    TPaveStats *st = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
    //cout << st->GetX1NDC() << "; " << st->GetY1NDC() << endl;


//    st->SetX1NDC(0.12); st->SetX2NDC(0.31);
//    st->SetY1NDC(0.77); st->SetY2NDC(0.89);

    st->SetX1NDC(0.12); st->SetX2NDC(0.35);
    st->SetY1NDC(0.72); st->SetY2NDC(0.89);

    c1->Modified(); c1->Update();
    //gPad->Update();
    //h2->SetStats(1);



    //finalization code
    cout << "all is ok" << endl;
    //app->Run();


    //Cleanup
    //delete app;

    return 0;
}
