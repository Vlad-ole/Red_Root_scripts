//C++
#include <iostream>
#include <sstream>      // std::ostringstream
#include <fstream>

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

    COUT(Va);
    COUT(p);
    COUT(E_N);
    COUT(yield);

    return yield;
}

double Yield2(double E_N, double LGAr, double p, double anode_grid_distance)
{
    double N = N_cm(p);
    double Y_N = (1.87992 + -1.35974*E_N + 0.3639*pow(E_N, 2.0) + -0.0459*pow(E_N, 3.0) +
            0.00314*pow(E_N, 4.0) + -1.11514E-4*pow(E_N, 5.0) + 1.63053E-6*pow(E_N, 6.0)) * 1E-17;

    if(E_N < 4.1 || E_N > 21.2) Y_N = 0;

    double yield = Y_N * N * LGAr;

    COUT(N);

    return yield;
}

void article_s2_yield()
{
    double Va = 6620;
    double p = 1110;
    double LGAr = 1;
    double anode_grid_distance = 1;

    //Yield(Va, LGAr, p, anode_grid_distance);
    cout << Yield2(7, LGAr, p, anode_grid_distance) << endl;
}
