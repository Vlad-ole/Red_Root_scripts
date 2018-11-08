//Qt
#include <QCoreApplication>

//C++
#include <iostream>

//root cern
#include "TGraph.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TCanvas.h"

using namespace std;

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    TApplication* app = new TApplication("App",0,0);

    //TCanvas C1 = new TCanvas("canv name", "canv title", 500, 500);
    TGraph gr(100);
    TH1F *hist = new TH1F("test hist", "title", 1000, 0, 10);
    hist->FillRandom("gaus", 5000);
    hist->Draw("");


    cout << "all is ok" << endl;
    app->Run();

    //system("pause");

    //Cleanup
    //delete app;

    return 0;
    //return a.exec();
}
