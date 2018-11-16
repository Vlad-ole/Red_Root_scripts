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
#include "TArrow.h"


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;
bool total_cut = false;
TString total_cut_srt;
#define REMEMBER_CUT(x) total_cut_srt = #x; total_cut = x;

//double h1_fill_value;
//TString h1_title;
//#define REMEMBER_h1_FILL_VALUE(x) h1_title = #x; h1_fill_value = x;

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    //start code
    std::cout << "You are in program." << std::endl;

    //test code
    //    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    //    hist->FillRandom("gaus", 5000);
    //    hist->Draw("");


    //some predefined vars
    double area_cut_x1 = 1.5;//cm
    double area_cut_x2 = 2.1;//cm
    double area_cut_x3 = 2.9;//cm
    double area_cut_x4 = 3.5;//cm
    double area_cut_y1 = area_cut_x1;
    double area_cut_y2 = area_cut_x2;
    double area_cut_y3 = area_cut_x3;
    double area_cut_y4 = area_cut_x4;



    //list of runs
    //int run_number = 532; //ph2    BEAM ON (E = 28 MeV, i = 12 nA)
    //int run_number = 534; //ph2     backgroud
    int run_number = 537; //ph2     Am241
    //int run_number = 540; //ph2     Cf252
    //int run_number = 542; //ph2     Am241
    //int run_number = 544; //ph2     Am241
    //int run_number = 554; //ph2     Am241 error

    //main code
    ostringstream path_root_file;
    path_root_file << "/media/vlad/Data/DS-data/reco/rm3reco/lns/camp_V/v3/" << "run_" << run_number << ".root";
    //path_root_file << "/home/vlad/Soft/Red_Soft/red-daq-light/src/Level1/" << "run_" << run_number << ".root";
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
    TH1F *h1 = new TH1F("h1 S1", "h1 title", 400, -100, 2000);//Am S1 charge
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 10000);//Am S2 charge
    //TH1F *h1 = new TH1F("h1 S2/S1", "h1 title", 400, -1, 20);//Am S2_charge/S1_charge
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 8000);//Am S2 tot_charge_top or tot_charge_bottom
    //TH1F *h1 = new TH1F("h1 S2", "h1 title", 400, -100, 1000);//Am S1 tot_charge_top or tot_charge_bottom
    //TH1F *h1 = new TH1F("h1 f90", "h1 title", 400, -0.2, 1);//f90
    //TH1F *h1 = new TH1F("h1 x or y", "h1 title", 400, -1, 5);//x ix or iy

    for (int ev = 0; ev < data->GetEntries(); ev++)
    {
        data->GetEntry(ev);
        //cout << "line 106" << endl;
        vector<RDCluster*> clusters = evReco->GetClusters();
        size_t nc = clusters.size();
        //cout << "Found n. " << nc << " clusters" << endl;


        if(nc)
        {
            //cuts1
            bool cls0_is_S1 = clusters.at(0)->f90 > 0.2;
            bool cls0_is_full = clusters.at(0)->rep == 1;
            bool S1_Am_peak = (clusters.at(0)->charge > 440) && (clusters.at(0)->charge < 630); // mean +- 1.5sigma using run 537
            //bool S1_Am_peak = (clusters.at(0)->charge > 419) && (clusters.at(0)->charge < 587); // mean +- 1.5sigma using run 542
            //bool S1_Am_peak = (clusters.at(0)->charge > 342) && (clusters.at(0)->charge < 482); // mean +- 1.5sigma using run 544
            //bool S1_Am_peak = (clusters.at(0)->charge > 252) && (clusters.at(0)->charge < 366); // mean +- 1.5sigma using run 554


            for(int nc_i = 0; nc_i < nc; nc_i++)
            {
                double radius = sqrt( pow(clusters.at(nc_i)->pos_x - 2.62, 2.0) + pow(clusters.at(nc_i)->pos_y - 2.54, 2.0) );


                //cuts2
                bool cls0 = nc_i == 0;//cluster 0
                bool cls1 = nc_i == 1;//cluster 1
                bool is_S1 =  nc == 2 && cls0 && cls0_is_full && cls0_is_S1;
                bool is_S2 =  nc == 2 && cls1 && cls0_is_full && cls0_is_S1;



                bool cent_spot = (clusters.at(nc_i)->pos_x > area_cut_x2) && (clusters.at(nc_i)->pos_x < area_cut_x3) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y2) && (clusters.at(nc_i)->pos_y < area_cut_y3);

                //edges
                bool edge_left = (clusters.at(nc_i)->pos_x > area_cut_x1) && (clusters.at(nc_i)->pos_x < area_cut_x2) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y2) && (clusters.at(nc_i)->pos_y < area_cut_y3);

                bool edge_right = (clusters.at(nc_i)->pos_x > area_cut_x3) && (clusters.at(nc_i)->pos_x < area_cut_x4) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y2) && (clusters.at(nc_i)->pos_y < area_cut_y3);

                bool edge_bot = (clusters.at(nc_i)->pos_x > area_cut_x2) && (clusters.at(nc_i)->pos_x < area_cut_x3) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y1) && (clusters.at(nc_i)->pos_y < area_cut_y2);

                bool edge_top = (clusters.at(nc_i)->pos_x > area_cut_x2) && (clusters.at(nc_i)->pos_x < area_cut_x3) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y3) && (clusters.at(nc_i)->pos_y < area_cut_y4);

                bool edges = edge_left || edge_right || edge_bot || edge_top;

                //corners
                bool corner_left_bot = (clusters.at(nc_i)->pos_x > area_cut_x1) && (clusters.at(nc_i)->pos_x < area_cut_x2) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y1) && (clusters.at(nc_i)->pos_y < area_cut_y2);

                bool corner_right_bot = (clusters.at(nc_i)->pos_x > area_cut_x3) && (clusters.at(nc_i)->pos_x < area_cut_x4) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y1) && (clusters.at(nc_i)->pos_y < area_cut_y2);

                bool corner_left_top = (clusters.at(nc_i)->pos_x > area_cut_x1) && (clusters.at(nc_i)->pos_x < area_cut_x2) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y3) && (clusters.at(nc_i)->pos_y < area_cut_y4);

                bool corner_right_top = (clusters.at(nc_i)->pos_x > area_cut_x3) && (clusters.at(nc_i)->pos_x < area_cut_x4) &&
                        (clusters.at(nc_i)->pos_y > area_cut_y3) && (clusters.at(nc_i)->pos_y < area_cut_y4);


                bool corners = corner_left_bot || corner_right_bot || corner_left_top || corner_right_top;

                //is_S2 && S1_Am_peak && edges
                REMEMBER_CUT(is_S1 && radius < 0.5);

                if (total_cut) //cuts
                {
                    //cout << "   pos_x = " << clusters[nc_i]->pos_x << "; pos_y = " << clusters[nc_i]->pos_y << endl;
                    h2->Fill(clusters.at(nc_i)->pos_x, clusters.at(nc_i)->pos_y);
                    h1->Fill(clusters.at(nc_i)->charge);
                }

//                int channumber = 0;
//                if (RDconfig::GetInstance()->GetChannelType(channumber) == RDconfig::kSiPMTop)
//                {
//                }
            }// end nc_i loop

            //        REMEMBER_CUT(nc == 2 && cls0_is_full && cls0_is_S1 && S1_Am_peak);
            //        if (total_cut) //cuts
            //        {
            //            h1->Fill(clusters.at(1)->charge / clusters.at(0)->charge);
            //        }


            //cout << endl;
        }

        if(ev%5000==0) cout << "Event " << ev << " processed" << endl;
    }

    //Draw options
    bool is_draw_h1 = 0;
    bool is_draw_h2 = 1;
    bool is_draw_h2_var1 = 0;
    bool is_draw_h2_var2 = 0;
    bool is_draw_h2_var3 = 1;

    if(is_draw_h1)
    {
        h1->SetTitle(total_cut_srt);
        //h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
        h1->GetXaxis()->SetTitle("clusters.at(nc_i)->charge [PE]");
        h1->Draw();
    }

    if(is_draw_h2)
    {
        TCanvas *c1 = new TCanvas("c1","c1");
        //    c1->SetCanvasSize(850, 850);
        //    c1->SetWindowSize(900, 900);
        c1->Divide(2,1,0.01,0.01);


        c1->cd(1);
        h2->SetTitle(total_cut_srt);
        h2->GetXaxis()->SetTitle("x [cm]");
        h2->GetYaxis()->SetTitle("y [cm]");
        h2->GetXaxis()->SetRangeUser(1.5, 3.5);
        h2->GetYaxis()->SetRangeUser(1.5, 3.5);
        h2->Draw("colz");
        //h2->SetStats(0); //delete statbox
        gPad->Update();
        TPaveStats *st = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
        st->SetX1NDC(0.12); st->SetX2NDC(0.35);
        st->SetY1NDC(0.72); st->SetY2NDC(0.89);
        gPad->Modified(); gPad->Update();

        if(is_draw_h2_var1)
        {
            c1->cd(2);
            //TH2F *h2_copy = new TH2F(h2);
            TH2F *h2_copy = (TH2F*)h2->Clone("");//make clone to be free to delete statbox
            h2_copy->Draw("colz");
            h2_copy->GetXaxis()->SetRangeUser(0, 5);
            h2_copy->GetYaxis()->SetRangeUser(0, 5);
            h2_copy->SetStats(0); //delete statbox

            //draw SiPMs boxes
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

            //propagation direction of the particles
            TArrow *ar1 = new TArrow(5, 0, 4, 1);
            ar1->SetLineWidth(4);
            ar1->SetLineColor(4);
            ar1->SetFillStyle(3008);
            ar1->SetFillColor(2);
            ar1->Draw();
        }
        else if (is_draw_h2_var2)
        {
            c1->cd(2);
            TH2F *h2_copy = (TH2F*)h2->Clone("");//make clone to be free to delete statbox
            h2_copy->Draw("colz");
            //h2_copy->GetXaxis()->SetRangeUser(0, 5);
            //h2_copy->GetYaxis()->SetRangeUser(0, 5);
            h2_copy->SetStats(0); //delete statbox

            Color_t box_color = kMagenta;
            int box_FillStyle = 0;
            int box_LineWidth = 2;
            double box_margin = 0.01;
            TBox *box1 = new TBox(area_cut_x1, area_cut_y1, area_cut_x2, area_cut_y2);
            box1->SetFillStyle(box_FillStyle);
            box1->SetLineColor(box_color);
            box1->SetLineWidth(box_LineWidth);
            box1->Draw();

            TBox *box2 = new TBox(area_cut_x3, area_cut_y1, area_cut_x4, area_cut_y2);
            box2->SetFillStyle(box_FillStyle);
            box2->SetLineColor(box_color);
            box2->SetLineWidth(box_LineWidth);
            box2->Draw();

            TBox *box3 = new TBox(area_cut_x1, area_cut_y3, area_cut_x2, area_cut_y4);
            box3->SetFillStyle(box_FillStyle);
            box3->SetLineColor(box_color);
            box3->SetLineWidth(box_LineWidth);
            box3->Draw();

            TBox *box4 = new TBox(area_cut_x3, area_cut_y3, area_cut_x4, area_cut_y4);
            box4->SetFillStyle(box_FillStyle);
            box4->SetLineColor(box_color);
            box4->SetLineWidth(box_LineWidth);
            box4->Draw();


            TBox *box5 = new TBox(area_cut_x2, area_cut_y2, area_cut_x3, area_cut_y3);
            box5->SetFillStyle(box_FillStyle);
            box5->SetLineColor(box_color);
            box5->SetLineWidth(box_LineWidth);
            box5->Draw();

        }
        else if (is_draw_h2_var3)
        {
            c1->cd(2);
            h1->SetTitle(total_cut_srt);
            //h1->GetXaxis()->SetTitle("clusters.at(1)->charge / clusters.at(0)->charge");
            h1->GetXaxis()->SetTitle("clusters.at(nc_i)->charge [PE]");
            h1->Draw();
        }


        gPad->Modified(); gPad->Update();
    }



    //finalization code
    cout << "all is ok" << endl;

    //Cleanup
    //delete app;

    return 0;
}
