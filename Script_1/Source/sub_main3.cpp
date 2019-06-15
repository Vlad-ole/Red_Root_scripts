//Qt
//#include <QCoreApplication>

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


//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void sub_main3()
{
    vector<double> power_v{0, 0.935, 2, 0};
    vector<double> cluster_ratio{0.0179241, 0.0154141, 0.0180474, 0.0225512};
    vector<double> run{1025, 1031, 1033, 1034};

    TGraph *gr = new TGraph(power_v.size(), &power_v[0], &cluster_ratio[0]);
    gr->Draw("AP");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(3);
    gr->GetXaxis()->SetRangeUser(-1, 3);
    gr->GetXaxis()->SetTitle("Heater power [W]");
    gr->GetYaxis()->SetTitle("(number_of_clusters>1)/(number_of_clusters>0)");


}
