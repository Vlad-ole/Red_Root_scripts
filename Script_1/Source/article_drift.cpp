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
using namespace TMath ;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

double GetLArMobility( double T /*K*/, double E /*kV/cm*/ ) // [cm2/V/s]
{
  //  Global Fit in --> Astro-ph 1508.07059
  double a[6]={ 551.6, 7953.7, 4440.43, 4.29, 43.63, 0.2053 };
  double T0=89.;
  double val=a[0]+a[1]*E+a[2]*pow(E,3./2.)+a[3]*pow(E,5./2);
  val/=(1.+a[1]/a[0]*E+a[4]*E*E+a[5]*E*E*E);
  return val*pow(T/T0,-3./2.);
}

double GetTFromP(double p_mbar)
{
    double res = 0;

    if(p_mbar > 800 && p_mbar < 1800)
        res = 74.75644 + 0.01511*p_mbar + -2.75686E-6*p_mbar*p_mbar;

    return res;
}

void article_drift()
{
    double T = 87;
    double grid_cathode_cm = 5;
    double T_min = GetTFromP(1027);
    double T_max = GetTFromP(1120);

    COUT(GetTFromP(1027));
    COUT(GetTFromP(1120));

    double Td_min = 200.0 * GetLArMobility(T_min, 200/1000.0) * 1E-6;
    Td_min = grid_cathode_cm/Td_min;
    COUT(Td_min);

    vector<double> Td{};
    vector<double> Vd_exp;

    for (int i = 0; i < Td.size(); i++)
    {
        Vd_exp.push_back(grid_cathode_cm/Td[i]);
    }

    vector<double> E_th;
    vector<double> Vd_th;
    vector<double> Vd_th_min;
    vector<double> Vd_th_max;
    for(double E = 0.1; E < 1000; E+= 10)
    {
        E_th.push_back(E);
        Vd_th.push_back(E * GetLArMobility(T, E/1000.0) * 1E-6 );
        Vd_th_min.push_back(E * GetLArMobility(T_min, E/1000.0) * 1E-6 );
        Vd_th_max.push_back(E * GetLArMobility(T_max, E/1000.0) * 1E-6 );
    }


//    TGraph *gr = new TGraph(E_th.size(), &E_th[0], &Vd_th[0]);
//    gr->SetLineColor(kMagenta);
//    gr->SetLineWidth(2);


    int n = E_th.size();
    TGraph *grshade = new TGraph(2*n);
    for (int i=0; i<n; i++)
    {
        grshade->SetPoint(i,E_th[i],Vd_th_min[i]);
        grshade->SetPoint(n+i,E_th[n-i-1],Vd_th_max[n-i-1]);
    }
    grshade->SetFillStyle(1001/*3001*/);
    grshade->SetFillColor(kMagenta);
    ostringstream grshade_title;
    grshade_title << "lar.bnl.gov; " << std::setprecision(4) << T_min << " < T[K] < " << T_max;
    grshade->SetTitle(grshade_title.str().c_str());

    TMultiGraph *mg = new TMultiGraph();
    //mg->Add(gr);
    mg->Add(grshade, "f");

    mg->Draw("A");
    mg->SetTitle(";E drift [kV/cm];V drift [cm/us]");
    gPad->BuildLegend();
}
