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

    //test code
    //    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    //    hist->FillRandom("gaus", 5000);
    //    hist->Draw("");


    //main code
    ostringstream path_root_file;
    int run_number = /*532*/ 534;
    path_root_file << "/media/vlad/Data/DS-data/reco/rm3reco/lns/camp_V/v3/" << "run_" << run_number << ".root";
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


    //


    TCanvas *c1 = new TCanvas("c1","c1");
//    c1->SetCanvasSize(850, 850);
//    c1->SetWindowSize(900, 900);
    c1->Divide(2,1,0.01,0.01);


    c1->cd(1);
    h2->Draw("colz");
    gPad->Update();
    TPaveStats *st = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
    st->SetX1NDC(0.12); st->SetX2NDC(0.35);
    st->SetY1NDC(0.72); st->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    c1->cd(2);
    //TH2F *h2_copy = new TH2F(h2);
    TH2F *h2_copy = (TH2F*)h2->Clone("");
    h2_copy->Draw("colz");
    h2_copy->SetStats(0); //delete statbox
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
    gPad->Modified(); gPad->Update();

    //TBox *box = new TBox(x_centr - xw/2, y_centr - yw/2, x_centr + xw/2, y_centr + yw/2);
//    box->SetFillStyle(0);
//    box->SetLineColor(2);
//    box->SetLineWidth(2);
//    box ->Draw();
    //c1->Modified(); c1->Update();



    //finalization code
    cout << "all is ok" << endl;

    //Cleanup
    //delete app;

    return 0;
}
