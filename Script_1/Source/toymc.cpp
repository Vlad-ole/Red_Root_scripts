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

//roofit
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "RooAddPdf.h"
#include "RooGenericPdf.h"
#include "RooArgusBG.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPlot.h"

using namespace RooFit;

void toymc()
{
//    // --- Observable ---

//    RooRealVar mes("mes","m_{ES} (GeV)",5.20,5.30) ;

//    // --- Build Gaussian signal PDF ---
//    RooRealVar sigmean("sigmean","B^{#pm} mass",5.28,5.20,5.30) ;
//    RooRealVar sigwidth("sigwidth","B^{#pm} width",0.0027,0.001,1.) ;
//    RooGaussian gauss("gauss","gaussian PDF",mes,sigmean,sigwidth) ;

//    // --- Build Argus background PDF ---
//    RooRealVar argpar("argpar","argus shape parameter",-20.0,-100.,-1.) ;
//    RooArgusBG argus("argus","Argus PDF",mes,RooConst(5.291),argpar) ;

//    // --- Construct signal+background PDF ---
//    RooRealVar nsig("nsig","#signal events",200,0.,10000) ;
//    RooRealVar nbkg("nbkg","#background events",800,0.,10000) ;
//    RooAddPdf sum("sum","g+a",RooArgList(gauss,argus),RooArgList(nsig,nbkg)) ;

//    // --- Generate a toyMC sample from composite PDF ---
//    RooDataSet *data = sum.generate(mes,2000) ;

//    // --- Perform extended ML fit of composite PDF to toy data ---
//    sum.fitTo(*data,Extended()) ;

//    // --- Plot toy data and composite PDF overlaid ---
//    RooPlot* mesframe = mes.frame() ;
//    data->plotOn(mesframe) ;
//    sum.plotOn(mesframe) ;
//    sum.plotOn(mesframe,Components(argus),LineStyle(kDashed)) ;
















    // S e t u p   m o d e l
      // ---------------------

      // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
      RooRealVar x("x","x",-10,10) ;
      RooRealVar mean("mean","mean of gaussian",1,-10,10) ;
      RooRealVar sigma("sigma","width of gaussian",1,0.1,10) ;

      // Build gaussian p.d.f in terms of x,mean and sigma
      RooGaussian gauss("gauss","gaussian PDF",x,mean,sigma) ;

      // Construct plot frame in 'x'
      RooPlot* xframe = x.frame(Title("Gaussian p.d.f.")) ;


      // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
      // ---------------------------------------------------------------------------

      // Plot gauss in frame (i.e. in x)
      gauss.plotOn(xframe) ;

      // Change the value of sigma to 3
      sigma.setVal(3) ;

      // Plot gauss in frame (i.e. in x) and draw frame on canvas
      gauss.plotOn(xframe,LineColor(kRed)) ;


      // G e n e r a t e   e v e n t s
      // -----------------------------

      // Generate a dataset of 1000 events in x from gauss
      RooDataSet* data = gauss.generate(x,10000) ;

      // Make a second plot frame in x and draw both the
      // data and the p.d.f in the frame
      RooPlot* xframe2 = x.frame(Title("Gaussian p.d.f. with data")) ;
      data->plotOn(xframe2) ;
      gauss.plotOn(xframe2) ;


      // F i t   m o d e l   t o   d a t a
      // -----------------------------

      // Fit pdf to data
      gauss.fitTo(*data) ;

      // Print values of mean and sigma (that now reflect fitted values and errors)
      mean.Print() ;
      sigma.Print() ;

      // Draw all frames on a canvas
      TCanvas* c = new TCanvas("rf101_basics","rf101_basics",800,400) ;
      c->Divide(2) ;
      c->cd(1) ; gPad->SetLeftMargin(0.15) ; xframe->GetYaxis()->SetTitleOffset(1.6) ; xframe->Draw() ;
      c->cd(2) ; gPad->SetLeftMargin(0.15) ; xframe2->GetYaxis()->SetTitleOffset(1.6) ; xframe2->Draw() ;

}
