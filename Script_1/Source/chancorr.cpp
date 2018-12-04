#include <iostream>
#include <ctime>

#include <TH2F.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TCanvas.h>

using namespace std;

void chancorr() {

   gStyle->SetPalette(55);

   TRandom3 *r = new TRandom3(time(0));

   int pe = 600; // Am241 peak with LY = 10;

   double Q1 = 0.4;  // quantum effieciencies
   double Q2 = 0.6;


   TH2F* h = new TH2F("h", "h; ch1; ch2", 200, 0, 600, 200, 0, 600);

   int counter = 0;
   for (int i = 0; i < 1e6; i++)
   {
      int npe = r->Poisson(pe);
      int Ni = r->Binomial(npe, Q1);
      int Nj = r->Binomial(npe, Q2);
      h->Fill(Ni, Nj);

      if( (Ni + Nj) > npe)
        counter++;
   }

   h->Draw("colz");


   cout << "h->GetCorrelationFactor()" << h->GetCorrelationFactor() << "\t; counter = " << counter << endl;

}
