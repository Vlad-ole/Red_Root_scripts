// create dictionaries
//  echo $LD_LIBRARY_PATH
//  rootcling -f Dict.cpp -c EvRaw0.hh EvHeader.hh RDCluster.hh RunHeader.hh EvRec0.hh Linkdef.h

//run qtcreator from concole in order to say Qt where to find libs.
//"LIBS += " command does not work with liblevelone.so.0.
//
//check, that you have correct path to libs:
//
//  cd ~/Qt/Tools/QtCreator/bin/
//  ./qtcreator
