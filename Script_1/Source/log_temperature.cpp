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

using namespace std;
using namespace TMath;

//useful directive
#define COUT(x) cout << #x " = " << x << endl;

void log_temperature()
{
    string path = "/media/vlad/Data/DS-data/sublogs/LakeShore336/";
    //vector<int> data = {191030,191031,191101,191102,191103,191104,191105};

    string filename_T_ch_0 = "ch0_191030_191105.txt";

    string full_file_name_ch0 = path + filename_T_ch_0;
    ifstream f_in(full_file_name_ch0);

    if(!f_in.is_open())
    {
        cout << "could not open file: " << full_file_name_ch0 << endl;
        exit(1);
    }
    else
    {
        cout << "File has been opened correctly: " << full_file_name_ch0 << endl;
    }


    vector<double> time_ch0;
    vector<double> time_ch1;
    vector<double> temp_ch0;
    vector<double> temp_ch1;

    while (f_in.good())
    {
        double x,y;
        f_in >> x >> y;
        time_ch0.push_back(x);
        temp_ch0.push_back(y);
    }

    TGraph *gr = new TGraph(time_ch0.size() ,&time_ch0[0], &temp_ch0[0]);
    gr->Draw();
    gr->GetXaxis()->SetTitle("Hours since 191030 00:00");
    gr->GetYaxis()->SetTitle("Temperature [K]");
    gr->SetTitle("ch0 (bottom)");

//    ostringstream path_root_file;
//    path_root_file << "/media/vlad/Data/DS-data/reco/camp_VIII/" << "run_" << run_number << ".root";

//    TString filename = path_root_file.str().c_str();
//    TFile *f = new TFile(filename, "read");
//    if (!(f->IsOpen()))
//    {
//        cout << "could not open file: " << filename << endl;
//        exit(1);
//    }
//    else
//    {
//        cout << "File has been opened correctly: " << filename << endl;
//    }


}
