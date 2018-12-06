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
#include "TMath.h"

using namespace std;

bool is_true(TRandom3 &rndm3, double eff = 0.5)
{
    bool result = false;
    if(rndm3.Uniform() < eff)
        result = true;

    return result;
}

void very_simple_mc()
{
    //just change me. 0 <= p_ref < 1
    double p_ref = 0.5;//probabitily of reflection, i.e. change direction of photon from i to j (or from j to i) detector
    double solid_angle_i = 0.5;
    //double solid_angle_j = 0.5;
    double PDE_i = 1;
    double PDE_j = 1;
    int N_events = 50000;
    int verbosity = 0;

    double x_max = 50;
    int n_binsx = x_max * 2;
    TH2F *h2_S1_ij = new TH2F("h2 S1_i vs S1_j", "h2 S1_i vs S1_j", n_binsx, 0, x_max, n_binsx, 0, x_max);
    TString total_cut_srt_loop2 = "h2";
    TString str_S1_i = "h1_i";
    TString str_S1_j = "h1_j";

    vector<double> S1_i_v;
    vector<double> S1_j_v;

    TRandom3 rndm3;

    //balance in general: Ns_avr = Ni_avr + Nj_avr + Na_avr
    double Ns_avr = 0;
    double Na_avr = 0;
    double Ni_avr = 0;
    double Nj_avr = 0;

    //event loop
    for(int i = 0; i < N_events; i++)
    {
        if(verbosity)
            cout << endl << "event = " << i << endl;


        //sourse intensity and distribution
        //int Ns = 60;
        //int Ns = rndm3.Binomial(100, 0.5);
        int Ns = rndm3.Poisson(40);

        //balance in event: Ns = Ni + Nj + Na
        double Na = 0;//num of absorbed photons
        double Ni = 0;//num of photons detected by the i part
        double Nj = 0;//num of photons detected by the j part


        for(int k = 0; k < Ns; k++)
        {
            if(verbosity)
                cout << "\t k = " << k << endl;

            bool is_detected_by_i = false;
            bool is_detected_by_j = false;
            bool is_absorbed = false;


            while(1)
            {
                if(is_true(rndm3,solid_angle_i))//directon to i detector
                {
                    label_detector_i:
                    if(is_true(rndm3, p_ref))//reflected
                    {
                        if(verbosity)
                            cout << "\t \t det_i" << endl;

                        goto label_detector_j;
                    }
                    else//not reflected
                    {
                        if(is_true(rndm3,PDE_i))//detected
                        {
                            is_detected_by_i = true;
                            Ni++;
                        }
                        else//absorbed
                        {
                            is_absorbed = true;
                            Na++;
                        }
                    }
                }
                else//directon to j detector
                {
                    label_detector_j:
                    if(is_true(rndm3, p_ref))//reflected
                    {
                        if(verbosity)
                            cout << "\t \t det_j" << endl;

                        goto label_detector_i;
                    }
                    else//not reflected
                    {
                        if(is_true(rndm3,PDE_j))//detected
                        {
                            is_detected_by_j = true;
                            Nj++;
                        }
                        else//absorbed
                        {
                            is_absorbed = true;
                            Na++;
                        }
                    }
                }

                if(is_detected_by_i || is_detected_by_j || is_absorbed)
                    break;
            }//while


            if(verbosity)
                cout << "\t \t Ni =  " << is_detected_by_i << "; Nj = " << is_detected_by_j << "; Na = " << is_absorbed << endl;

            //check balance for 1 photon:
            if( (is_detected_by_i + is_detected_by_j + is_absorbed) != 1)
                cout << "error: (is_detected_by_i + is_detected_by_j + is_absorbed) != 1" << endl;

        }

        //check balance in event
        //cout << "Ni = " << Ni << "; Nj = " << Nj << "; Na = " << Na << "; Ni + Nj + Na = " << (Ni + Nj + Na) << "; Ns = " << Ns << endl;
        if( (Ni + Nj + Na) != Ns)
            cout << "error: (Ni + Nj + Na) != Ns" << endl;


        Ns_avr += Ns;
        Ni_avr += Ni;
        Nj_avr += Nj;
        Na_avr += Na;

        h2_S1_ij->Fill(Ni, Nj);

    }//event loop

    Ns_avr /= N_events;
    Ni_avr /= N_events;
    Nj_avr /= N_events;
    Na_avr /= N_events;

    //check balance in general
    cout << "Ni_avr = " << Ni_avr << "; Nj_avr = " << Nj_avr << "; Na_avr = " << Na_avr << "; Ni_avr + Nj_avr + Na_avr = " << (Ni_avr + Nj_avr + Na_avr) << "; Ns_avr = " << Ns_avr << endl;


    h2_S1_ij->SetTitle(total_cut_srt_loop2);
    h2_S1_ij->GetXaxis()->SetTitle(str_S1_i);
    h2_S1_ij->GetYaxis()->SetTitle(str_S1_j);
    h2_S1_ij->GetXaxis()->SetRangeUser(0, x_max);
    h2_S1_ij->GetYaxis()->SetRangeUser(0, x_max);
    h2_S1_ij->Draw("colz");
    cout << "r(i,j) from hist = " << h2_S1_ij->GetCorrelationFactor() << endl;
    gPad->Update();
    TPaveStats *st_h2_S1_ij = (TPaveStats*)h2_S1_ij->GetListOfFunctions()->FindObject("stats");
    st_h2_S1_ij->SetX1NDC(0.12); st_h2_S1_ij->SetX2NDC(0.35);
    st_h2_S1_ij->SetY1NDC(0.72); st_h2_S1_ij->SetY2NDC(0.89);
    gPad->Modified(); gPad->Update();

    TPaveText *t_h2_S1_ij = new TPaveText(0.7,0.13,0.9,0.43,"brNDC");
    t_h2_S1_ij->AddText(Form("r = %g", h2_S1_ij->GetCorrelationFactor() ));
    t_h2_S1_ij->Draw();

}
