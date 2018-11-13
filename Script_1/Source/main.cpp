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

//useful directive
#define COUT(x) cout << #x " = " << x << endl;
bool total_cut = false;
TString total_cut_srt;
#define REMEMBER_CUT(x) total_cut_srt = #x; total_cut = x;

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    //start code
    std::cout << "You are in program." << std::endl;

    //test code
    //    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    //    hist->FillRandom("gaus", 5000);
    //    hist->Draw("");



    //list of runs
    //int run_number = 532; //ph2    BEAM ON (E = 28 MeV, i = 12 nA)
    //int run_number = 534; //ph2     backgroud
    int run_number = 537; //ph2     Am241
    //int run_number = 540; //ph2     Cf252

    //main code
    ostringstream path_root_file;    
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

    TH2F *h2 = new TH2F("h2 name", "h2 title", 200, 0, 5, 200, 0, 5);//XY
    //TH1F *h1 = new TH1F("h1 S1", "h1 title", 400, -100, 2000);//Am S1
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 8000);//Am S2
    TH1F *h1 = new TH1F("h1 S2/S1", "h1 title", 400, -1, 20);//Am S2/S1

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();
        //cout << "Found n. " << nc << " clusters" << endl;


        if(nc)
        {
            //cuts1
            bool cls0_is_S1 = clusters.at(0)->f90 > 0.2;
            bool cls0_is_full = clusters.at(0)->rep == 1;
            bool S1_Am_peak = (clusters.at(0)->charge > 440) && (clusters.at(0)->charge < 630); // mean +- 1.5sigma using run 537


            for(int nc_i = 0; nc_i < nc; nc_i++)
            {
                //cuts2
                bool cls0 = nc_i == 0;
                bool cls1 = nc_i == 1;
                bool is_S1 =  nc == 2 && cls0 && cls0_is_full && cls0_is_S1;
                bool is_S2 =  nc == 2 && cls1 && cls0_is_full && cls0_is_S1;


                bool cent_spot = (clusters.at(nc_i)->pos_x > 1) && (clusters.at(nc_i)->pos_x < 4) &&
                        (clusters.at(nc_i)->pos_y > 1) && (clusters.at(nc_i)->pos_y < 4);

                bool edge1 = (clusters.at(nc_i)->pos_x > 0) && (clusters.at(nc_i)->pos_x < 1) &&
                        (clusters.at(nc_i)->pos_y > 1) && (clusters.at(nc_i)->pos_y < 4);

                bool edge2 = (clusters.at(nc_i)->pos_x > 4) && (clusters.at(nc_i)->pos_x < 5) &&
                        (clusters.at(nc_i)->pos_y > 1) && (clusters.at(nc_i)->pos_y < 4);

                bool edge3 = (clusters.at(nc_i)->pos_x > 1) && (clusters.at(nc_i)->pos_x < 4) &&
                        (clusters.at(nc_i)->pos_y > 4) && (clusters.at(nc_i)->pos_y < 5);

                bool edge4 = (clusters.at(nc_i)->pos_x > 1) && (clusters.at(nc_i)->pos_x < 4) &&
                        (clusters.at(nc_i)->pos_y > 0) && (clusters.at(nc_i)->pos_y < 1);

                bool edges = edge1 || edge2 || edge3 || edge4;
                bool corners = false;

                //is_S2 && S1_Am_peak && edges
//                REMEMBER_CUT();

//                if (total_cut) //cuts
//                {
//                    //cout << "   pos_x = " << clusters[nc_i]->pos_x << "; pos_y = " << clusters[nc_i]->pos_y << endl;
//                    h2->Fill(clusters.at(nc_i)->pos_x, clusters.at(nc_i)->pos_y);
//                    h1->Fill(clusters.at(nc_i)->charge);
//                }
            }

            REMEMBER_CUT(nc == 2 && cls0_is_full && cls0_is_S1 && S1_Am_peak);
            if (total_cut) //cuts
            {
                h1->Fill(clusters.at(1)->charge / clusters.at(0)->charge);
            }

        }

        //cout << endl;


        if(ev%5000==0) cout << "Event " << ev << " processed" << endl;

    }

    //Draw h1

    {
        h1->SetTitle(total_cut_srt);
        h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
        h1->Draw();
    }

    //Draw h2

    {
//        TCanvas *c1 = new TCanvas("c1","c1");
//        //    c1->SetCanvasSize(850, 850);
//        //    c1->SetWindowSize(900, 900);
//        c1->Divide(2,1,0.01,0.01);


//        c1->cd(1);
//        h2->SetTitle(total_cut_srt);
//        h2->Draw("colz");
//        gPad->Update();
//        TPaveStats *st = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
//        st->SetX1NDC(0.12); st->SetX2NDC(0.35);
//        st->SetY1NDC(0.72); st->SetY2NDC(0.89);
//        gPad->Modified(); gPad->Update();

//        c1->cd(2);
//        //TH2F *h2_copy = new TH2F(h2);
//        TH2F *h2_copy = (TH2F*)h2->Clone("");//make clone to be free to delete statbox
//        h2_copy->Draw("colz");
//        h2_copy->SetStats(0); //delete statbox
//        double yw = 0.8;//cm
//        double xw = 1.2;//cm
//        for (int iy = 0; iy < 6; iy++)
//        {
//            double x_centr;
//            double y_centr;
//            for (int ix = 0; ix < 4; ix++)
//            {
//                x_centr = ((0.5 + ix)*5/4);
//                y_centr = ((0.5 + iy)*5/6);

//                TBox *box = new TBox(x_centr - xw/2, y_centr - yw/2, x_centr + xw/2, y_centr + yw/2);
//                box->SetFillStyle(0);
//                box->SetLineColor(2);
//                box->SetLineWidth(2);
//                box->Draw();
//            }
//        }
//        gPad->Modified(); gPad->Update();
    }



    //finalization code
    cout << "all is ok" << endl;

    //Cleanup
    //delete app;

    return 0;
}
