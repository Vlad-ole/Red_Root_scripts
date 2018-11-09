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
    cout << "You are in program." << endl;
    TApplication* app = new TApplication("App",0,0);



    //main code
    //TCanvas C1 = new TCanvas("canv name", "canv title", 500, 500);
//    TGraph gr(100);
    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    hist->FillRandom("gaus", 5000);
    hist->Draw("");

    //gROOT->ProcessLine(".class EvRec0");


    ostringstream path_root_file;
    path_root_file << "/home/vlad/Soft/Red_Soft/red-daq-light/src/Level1/" << "run_" << 393 << ".root";
    //cout << path_root_file.str().c_str() << endl; //error!?
    //COUT(path_root_file);

    //const char* filename = path_root_file.str().c_str();

    //TFile *f = new TFile(path_root_file.str().c_str(), "read");
    TFile *f = new TFile("/home/vlad/Soft/Red_Soft/red-daq-light/src/Level1/run_393.root", "read");

//    if (!(f->IsOpen()))
//    {
//      cout << "could not open file: " << path_root_file.str().c_str() << endl;
//      return 1;
//    }



    //finalization code
    cout << "all is ok" << endl;
    app->Run();


    //Cleanup
    //delete app;

    return 0;
    //return a.exec();
}
