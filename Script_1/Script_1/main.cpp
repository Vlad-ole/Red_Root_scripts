#include <QCoreApplication>

#include "TGraph.h"
#include "TH1F.h"

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    TGraph gr(100);
    TH1F *hist = new TH1F("test hist", "title", 100, 0, 100);
    hist->FillRandom("gaus", 5000);
    hist->Draw("APL");

    return 0;
    //return a.exec();
}
