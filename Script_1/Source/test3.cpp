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
#include "TF2.h"
#include "TExec.h"
#include "TMultiGraph.h"




void test3()
{
   TCanvas *c1 = new TCanvas("c1","multigraph",200,10,700,500);

   // draw a frame to define the range
   TMultiGraph *mg = new TMultiGraph();

   // create first graph
   Int_t n1 = 10;
   Double_t x1[]  = {-0.1, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
   Double_t y1[]  = {-1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
   TGraph *gr1 = new TGraph(n1,x1,y1);
   gr1->SetMarkerColor(kBlue);
   gr1->SetMarkerStyle(21);
   mg->Add(gr1);

   // create second graph
   Int_t n2 = 10;
   Float_t x2[]  = {-0.28, 0.005, 0.19, 0.29, 0.45, 0.56,0.65,0.80,0.90,1.01};
   Float_t y2[]  = {2.1,3.86,7,9,10,10.55,9.64,7.26,5.42,2};
   TGraph *gr2 = new TGraph(n2,x2,y2);
   gr2->SetMarkerColor(kRed);
   gr2->SetMarkerStyle(20);

   mg->Add(gr2);
   mg->Draw("alp");
   mg->GetXaxis()->SetTitle("X title");

   // draw text
   TLatex l;
   l.SetTextSize(0.025);
   l.SetTextAngle(30.);
   for (int i=0; i<n1; i++) {
      l.DrawLatex(x1[i],y1[i],Form("(%g,%g)",x1[i],y1[i]));
      l.DrawLatex(x2[i],y2[i],Form("(%g,%g)",x2[i],y2[i]));
   }
}
