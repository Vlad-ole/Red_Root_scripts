QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
# DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    script.cpp \
    Dict.cpp \
    sumsignals.cpp \
    toymc.cpp \
    correlation_mc.cpp \
    chancorr.cpp \
    very_simple_mc.cpp \
    main2.cpp

INCLUDEPATH = /home/vlad/Soft/root/builddir/include/


#LIBS += -L/usr/local/lib -lmath
#-lCore

#root cern libs
LIBS += -L/home/vlad/Soft/root/builddir/lib  \
-lCore \
-lASImageGui \
-lASImage \
-lCling \
-lcomplexDict  \
-ldequeDict  \
-lEG  \
-lEve  \
-lFFTW  \
-lFitPanel  \
-lFITSIO  \
-lFoam  \
-lforward_listDict \
-lFTGL  \
-lFumili  \
-lGdml  \
-lGed  \
-lGenetic  \
-lGenVector  \
-lGeomBuilder  \
-lGeomPainter \
-lGeom  \
-lGLEW  \
-lGpad  \
-lGraf3d  \
-lGraf  \
-lGuiBld  \
-lGuiHtml  \
-lGui  \
-lGviz3d  \
-lGX11 \
-lGX11TTF \
-lHistFactory \
-lHistPainter \
-lHist \
-lHtml \
-lImt \
-lJupyROOT \
-llistDict \
-lmap2Dict \
-lmapDict  \
-lMathCore  \
-lMathMore  \
-lMatrix  \
-lMinuit  \
-lMLP \
-lmultimap2Dict  \
-lmultimapDict  \
-lMultiProc  \
-lmultisetDict \
-lNet  \
-lNew \
-lPhysics  \
-lPostscript  \
-lProofBench  \
-lProofDraw  \
-lProofPlayer  \
-lProof  \
-lPyMVA  \
-lPyROOT  \
-lQuadp  \
-lRDAVIX  \
-lRecorder \
-lRGL  \
-lRHTTPSniff  \
-lRHTTP  \
-lRint  \
-lRIO  \
-lRMySQL  \
-lRooFitCore  \
-lRooFit  \
-lRooStats  \
-lRootAuth \
-lROOTDataFrame  \
-lROOTVecOps  \
-lSessionViewer  \
-lsetDict  \
-lSmatrix  \
-lSpectrumPainter  \
-lSpectrum  \
-lSPlot  \
-lSQLIO  \
-lSrvAuth \
-ltbbmalloc_proxy \
-ltbbmalloc \
-ltbb \
-lThread  \
-lTMVAGui  \
-lTMVA  \
-lTreePlayer  \
-lTree  \
-lTreeViewer  \
-lUnfold \
-lunordered_mapDict  \
-lunordered_multimapDict  \
-lunordered_multisetDict  \
-lunordered_setDict  \
-lvalarrayDict  \
-lvdt \
-lvectorDict  \
-lVMC  \
-lX3d  \
-lXMLIO \
-lXMLParser

#Red libs
LIBS += -L/usr/local/lib  \
-llevelone \
-lmodules \
-lpadmeroot


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Linkdef.h

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib


#unix:!macx: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -llevelone

#INCLUDEPATH += $$PWD/../../../../../../usr/local/include
#DEPENDPATH += $$PWD/../../../../../../usr/local/include
#DEPENDS += "/home/peter/Workspace/build-Libtester-Desktop-Release/"
