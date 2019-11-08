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

void drawtext()
{
   Int_t i,n;
   Double_t x,y;
   TLatex *l;

   TGraph *g = (TGraph*)gPad->GetListOfPrimitives()->FindObject("Graph");
   n = g->GetN();
   for (i=1; i<n; i++) {
      g->GetPoint(i,x,y);
      l = new TLatex(x,y+0.2,Form("%4.2f",y));
      l->SetTextSize(0.025);
      l->SetTextFont(42);
      l->SetTextAlign(21);
      l->Paint();
   }
}


void test2()
{
    TCanvas *c = new TCanvas("c","A Simple Graph Example with Text",700,500);
      c->SetGrid();

      const Int_t n = 10;
      TGraph *gr = new TGraph(n);
      gr->SetTitle("A Simple Graph Example with Text");
      gr->SetMarkerStyle(20);
      TExec *ex = new TExec("ex","drawtext();");
      gr->GetListOfFunctions()->Add(ex);

      Double_t x, y;
      for (Int_t i=0;i<n;i++) {
         x = i*0.1;
         y = 10*sin(x+0.2);
         gr->SetPoint(i,x,y);

      }
      gr->Draw("ALP");
}
