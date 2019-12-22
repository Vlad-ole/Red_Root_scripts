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
#include "TLatex.h"

using namespace std;
using namespace TMath;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void article_quenching3()
{
    vector<string> text{"1269: 0/86/-744", "1268: 3780/86/-744", "1266: 5211/86/-744", "1267: 6500/86/-744"};
    vector<double> Va{0, 3780, 5211, 6500};
    vector<double> Ed_from_quenching{135.321, 173.3, 222.82, 283.312};
    TGraph *gr = new TGraph(Va.size(), &Va[0], &Ed_from_quenching[0]);
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kBlack);
    gr->SetMarkerSize(1.5);
    gr->Draw("AP");
    gr->GetYaxis()->SetTitle("E_{d} [V/cm]");
    gr->GetXaxis()->SetTitle("Anode voltage [V]");
    gr->SetTitle("");

//    TLatex lat;
//    lat.SetTextSize(0.030);
//    lat.SetTextAngle(90.);
//    for (int i=0; i < Va.size(); i++)
//    {
//        lat.DrawLatex(Va[i],Ed_from_quenching[i], text[i].c_str());
//    }

}
