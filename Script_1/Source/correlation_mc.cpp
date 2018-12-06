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

vector<double> Get_Cov_r(vector<double> xv, vector<double> yv)
{
    vector<double> result;

    double mean_x = TMath::Mean(xv.begin(), xv.end());
    double mean_y = TMath::Mean(yv.begin(), yv.end());
    double rms_x = TMath::RMS(xv.begin(), xv.end());
    double rms_y = TMath::RMS(yv.begin(), yv.end());

    vector<double> tmp;
    for(int i = 0; i < xv.size(); i++)
    {
        tmp.push_back( (xv[i] - mean_x) * (yv[i] - mean_y) );
    }

    double cov = TMath::Mean(tmp.begin(), tmp.end());
    double r = cov / (rms_x * rms_y);

    result.push_back( cov );
    result.push_back( r );

    return result;
}

void correlation_mc()
{
    double x_max = 200;
    int n_binsx = x_max * 2;
    TH2F *h2_S1_ij = new TH2F("h2 S1_i vs S1_j", "h2 S1_i vs S1_j", n_binsx, 0, x_max, n_binsx, 0, x_max);
    TString total_cut_srt_loop2 = "h2";
    TString str_S1_i = "h1_i";
    TString str_S1_j = "h1_j";

    vector<double> S1_i_v;
    vector<double> S1_j_v;

    TRandom3 rndm3;

    for(int i = 0; i < 20000; i++)
    {

        double solid_angle_part1 = 0.5;
        double solid_angle_part2 = 0.5;
        double PDE1 = 0.5;
        double PDE2 = 0.5;
        double Na = 0;
        double p_ref = 0;//probabitily of reflection, i.e. change direction from left to right detector

        //int Ns = 60;//sourse intensity and distribution
        int Ns = rndm3.Binomial(100, 0.5);//sourse intensity and distribution
        //int Ns = rndm3.Poisson(50);//sourse intensity and distribution

        //without reflection
        double Ni0 = rndm3.Binomial(Ns, solid_angle_part1);//num of photons emitted in the left part
        double Nj0 = Ns - Ni0;//num of photons emitted in the right part
        //double Nj0 = rndm3.Binomial(Ns - Ni0, solid_angle_part2);//num of photons emitted in the right part. It's case when detectors does not cover 4pi



        //detecting
        double Ni = rndm3.Binomial(Ni0, PDE1);//num of photons detected by the left part
        double Nj = rndm3.Binomial(Nj0, PDE2);//num of photons detected by the right part

        //add x-talk
        double p_x_talk = 0.0;
        double q = 1 - p_x_talk;
        double Ni_xtalk = 0;
        double Nj_xtalk = 0;

        for(int k = 0; k < Ni ; k++)
        {
            //https://math.stackexchange.com/questions/485448/prove-the-way-to-generate-geometrically-distributed-random-numbers
            int Geametrical_rnd_value = 0;
            if(p_x_talk > 0)
                Geametrical_rnd_value = log( rndm3.Uniform() ) / log(1 - q);

            //Vinogradov's theory  DOI: 10.1109/NSSMIC.2009.5402300
            Ni_xtalk += 1 + Geametrical_rnd_value;
        }

        for(int k = 0; k < Nj ; k++)
        {
            int Geametrical_rnd_value = 0;
            if(p_x_talk > 0)
                Geametrical_rnd_value = log( rndm3.Uniform() ) / log(1 - q);

            Nj_xtalk += 1 + Geametrical_rnd_value;
        }


        double S1_i = Ni_xtalk;
        double S1_j = Nj_xtalk;
        h2_S1_ij->Fill(S1_i, S1_j);
        S1_i_v.push_back(S1_i);
        S1_j_v.push_back(S1_j);

    }




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

    vector<double> cov_r = Get_Cov_r(S1_i_v, S1_j_v);
    TPaveText *t_h2_S1_ij = new TPaveText(0.7,0.13,0.9,0.43,"brNDC");
    t_h2_S1_ij->AddText(Form("cov = %g", cov_r[0] ));
    t_h2_S1_ij->AddText(Form("r = %g", cov_r[1] ));
    t_h2_S1_ij->Draw();
}





















