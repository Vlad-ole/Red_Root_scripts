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

void test1()
{
       TH2D *h_gaus = new TH2D("h_gaus", "", 48, 0, 48, 48, 0, 48);

       TF2 *TwoDGaus = new TF2("TwoDGaus","[0]*TMath::Gaus(x,[1],[2])*TMath::Gaus(y,[3],[4])",0,10,0,10);
       TwoDGaus->SetParameter(0,5);
       TwoDGaus->SetParameter(1,15);
       TwoDGaus->SetParameter(2,2);
       TwoDGaus->SetParameter(3,25);
       TwoDGaus->SetParameter(4,2);

       h_gaus->FillRandom("TwoDGaus",10000);

       Int_t MaxBin = h_gaus->GetMaximumBin();
       Int_t x,y,z;
       h_gaus->GetBinXYZ(MaxBin, x, y, z);

       printf("The bin having the maximum value is (%d,%d)\n",x,y);

       h_gaus->Draw("COLZ");
}
