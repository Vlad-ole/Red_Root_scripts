//C++
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>
#include <algorithm>

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

using namespace std;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

double N_cm(double p)
{
    double pho = 0.00748 + 1.39212E-4 * p + -4.1215E-9 * p * p;
    double NA = 6.02214076E23;
    return pho * NA / 1000;;
}

double extraxtion_eff(double LGAr)
{
    double LLAr = 1 - 0.6;
    double e_LAr = 1.505;
    double E = 5211/(LGAr + LLAr/e_LAr);
    return E/e_LAr;
}

double Yield(double Va, double LGAr, double p, double anode_grid_distance)
{
    double LLAr = anode_grid_distance - LGAr;
    double e_LAr = 1.505;
    double E = Va/(LGAr + LLAr/e_LAr);
    //double NA = 6.02214076E23;
    //double pho = 0.00748 + 1.39212E-4 * p + -4.1215E-9 * p * p;
    //double N = pho * NA / 1000;
    double N = N_cm(p);
    double E_N = E / N * 1E17;
    double Y_N = (1.87992 + -1.35974*E_N + 0.3639*pow(E_N, 2.0) + -0.0459*pow(E_N, 3.0) +
            0.00314*pow(E_N, 4.0) + -1.11514E-4*pow(E_N, 5.0) + 1.63053E-6*pow(E_N, 6.0)) * 1E-17;

    if(E_N < 4.1 || E_N > 21.2) Y_N = 0;

    double yield = Y_N * N * LGAr;

    return yield;
}

void article_gas_pocket_thickness()
{
    //v1
//    double anode_grid_distance = 1.25;//cm
//    vector<double> LGAr_v{anode_grid_distance, 0.5, 0.3};
//    double LGAr_for_normalisation = LGAr_v[1];//cm

    //v2
//    double anode_grid_distance = 1.25;//cm
//    vector<double> LGAr_v{anode_grid_distance, 0.5, 0.3};
//    double LGAr_for_normalisation = LGAr_v[1];//cm

    //v3
    double anode_grid_distance = 1.08;//cm
    vector<double> LGAr_v{anode_grid_distance, 0.6, 0.3};
    double LGAr_for_normalisation = LGAr_v[1];//cm

    double p = 1030 /*1120*/ /*1030*/;
    COUT(p);
    double p_min = p - 10;
    double p_max = p + 10;

    vector<double> Va_v;
    vector<double> Y_v;
    vector<double> Y_min_v;
    vector<double> Y_max_v;

    vector<double> Va_v2;
    vector<double> Y_v2;
    vector<double> Y_min_v2;
    vector<double> Y_max_v2;

    vector<double> Va_v3;
    vector<double> Y_v3;
    vector<double> Y_min_v3;
    vector<double> Y_max_v3;




    for (int Va = 3500; Va < 7000 ; Va+= 50)
    {
        double LGAr = LGAr_v[0];
        double y = Yield(Va, LGAr, p, anode_grid_distance);
        double y_min = Yield(Va, LGAr, p_min, anode_grid_distance);
        double y_max = Yield(Va, LGAr, p_max, anode_grid_distance);
        if( y>0 && y_min>0 && y_max>0 )
        {
            Va_v.push_back(Va);
            Y_v.push_back(y);
            Y_min_v.push_back(y_min);
            Y_max_v.push_back(y_max);
        }

        LGAr = LGAr_v[1];
        y = Yield(Va, LGAr, p, anode_grid_distance);
        y_min = Yield(Va, LGAr, p_min, anode_grid_distance);
        y_max = Yield(Va, LGAr, p_max, anode_grid_distance);
        if( y>0 && y_min>0 && y_max>0 )
        {
            Va_v2.push_back(Va);
            Y_v2.push_back(y);
            Y_min_v2.push_back(y_min);
            Y_max_v2.push_back(y_max);
        }

        LGAr = LGAr_v[2];
        y = Yield(Va, LGAr, p, anode_grid_distance);
        y_min = Yield(Va, LGAr, p_min, anode_grid_distance);
        y_max = Yield(Va, LGAr, p_max, anode_grid_distance);
        if( y>0 && y_min>0 && y_max>0 )
        {
            Va_v3.push_back(Va);
            Y_v3.push_back(y);
            Y_min_v3.push_back(y_min);
            Y_max_v3.push_back(y_max);
        }

    }

    TMultiGraph *mg = new TMultiGraph();

    //TGraph *gr = new TGraph(Va_v.size(), &Va_v[0], &Y_v[0]);
    //TGraph *gr_min = new TGraph(Va_v.size(), &Va_v[0], &Y_min_v[0]);
    //TGraph *gr_max = new TGraph(Va_v.size(), &Va_v[0], &Y_max_v[0]);

    //TGraph *gr_min2 = new TGraph(Va_v2.size(), &Va_v2[0], &Y_min_v2[0]);
    //TGraph *gr_max2 = new TGraph(Va_v2.size(), &Va_v2[0], &Y_max_v2[0]);

    //TGraph *gr_min3 = new TGraph(Va_v3.size(), &Va_v3[0], &Y_min_v3[0]);
    //TGraph *gr_max3 = new TGraph(Va_v3.size(), &Va_v3[0], &Y_max_v3[0]);

    string grshade_title_common = "Theory: gas pocket thickness = ";
    int n = Va_v.size();
    TGraph *grshade = new TGraph(2*n);
    for (int i=0; i<n; i++)
    {
        grshade->SetPoint(i,Va_v[i],Y_max_v[i]);
        grshade->SetPoint(n+i,Va_v[n-i-1],Y_min_v[n-i-1]);
    }
    grshade->SetFillStyle(3001);
    grshade->SetFillColor(kBlue);
    ostringstream grshade_title;
    grshade_title << grshade_title_common << LGAr_v[0] << " cm";
    grshade->SetTitle(grshade_title.str().c_str());

    n = Va_v2.size();
    TGraph *grshade2 = new TGraph(2*n);
    for (int i=0; i<n; i++)
    {
        grshade2->SetPoint(i,Va_v2[i],Y_max_v2[i]);
        grshade2->SetPoint(n+i,Va_v2[n-i-1],Y_min_v2[n-i-1]);
    }
    grshade2->SetFillStyle(3001);
    grshade2->SetFillColor(kRed);
    ostringstream grshade_title2;
    grshade_title2 << grshade_title_common << LGAr_v[1] << " cm";
    grshade2->SetTitle(grshade_title2.str().c_str());
    //grshade2->SetTitle("Theory: gas pocket thickness = 0.5 cm");

    n = Va_v3.size();
    TGraph *grshade3 = new TGraph(2*n);
    for (int i=0; i<n; i++)
    {
        grshade3->SetPoint(i,Va_v3[i],Y_max_v3[i]);
        grshade3->SetPoint(n+i,Va_v3[n-i-1],Y_min_v3[n-i-1]);
    }
    grshade3->SetFillStyle(3001);
    grshade3->SetFillColor(kGreen);
    ostringstream grshade_title3;
    grshade_title3 << grshade_title_common << LGAr_v[2] << " cm";
    grshade3->SetTitle(grshade_title3.str().c_str());
    //grshade3->SetTitle("Theory: gas pocket thickness = 0.3 cm");


    vector<double> Va_v_exper{5211, 5500, 6000, 6500};
    vector<double> Va_v_exper_err{0, 0, 0, 0};
    vector<double> S2_S1{20.7, 27.3, 38.2, 47.4};
    vector<double> S2_S1_err{0.3, 0.4, 0.8, 0.9};
    double myconstant = Yield(Va_v_exper[3], LGAr_for_normalisation, p, anode_grid_distance)/S2_S1[3];
    cout << "myconstant = " << myconstant << endl;
    std::transform(S2_S1.begin(), S2_S1.end(), S2_S1.begin(), [myconstant](double& c){return c*myconstant;});
    std::transform(S2_S1_err.begin(), S2_S1_err.end(), S2_S1_err.begin(), [myconstant](double& c){return c*myconstant;});
    TGraphErrors *gr2 = new TGraphErrors(Va_v_exper.size(), &Va_v_exper[0], &S2_S1[0], &Va_v_exper_err[0], &S2_S1_err[0]);
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerSize(1.5);
    gr2->SetTitle("Normalized experimental data");


    //mg->Add(gr, "l");
    //mg->Add(gr_min, "l");
    //mg->Add(gr_max, "l");
    mg->Add(grshade, "f");
    mg->Add(grshade2, "f");
    mg->Add(grshade3, "f");
    mg->Add(gr2, "pe");
    mg->Draw("A");


    ostringstream oss;
    oss << "p = " << p << " mbar" << "; Anode voltage [V]; S2 yield [photons/e-]";
    mg->SetTitle(oss.str().c_str());

    gPad->BuildLegend();

    //TCanvas *c2 = new TCanvas("c2", "c2");
    COUT(extraxtion_eff(0.8))
}










