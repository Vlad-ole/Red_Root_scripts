//Qt
//#include <QCoreApplication>

//C++
#include <iostream>
#include <sstream>      // std::ostringstream

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

//Red
#include "red-daq/EvRec0.hh"
#include "red-daq/RDCluster.hh"
#include "red-daq/RDconfig.h"

//usefull directive
#define COUT(x) cout << #x " = " << x << endl;

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    //start code
    std::cout << "You are in program." << std::endl;
    TApplication* app = new TApplication("App",0,0);


    //main code
    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    hist->FillRandom("gaus", 5000);
    hist->Draw("");


    ostringstream path_root_file;
    path_root_file << "/home/vlad/Soft/Red_Soft/red-daq-light/src/Level1/" << "run_" << 393 << ".root";
    std::cout << path_root_file.str().c_str() << std::endl;

    TString filename = path_root_file.str().c_str();

    TFile *f = new TFile(filename, "read");
    if (!(f->IsOpen()))
    {
      cout << "could not open file: " << filename << endl;
      return 1;
    }



    //finalization code
    cout << "all is ok" << endl;
    app->Run();


    //Cleanup
    //delete app;

    return 0;
}
