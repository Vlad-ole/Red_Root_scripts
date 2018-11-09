//// Do NOT change. Changes will be lost next time file is generated

//#define R__DICTIONARY_FILENAME Dict

///*******************************************************************/
//#include <stddef.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#define G__DICTIONARY
//#include "RConfig.h"
//#include "TClass.h"
//#include "TDictAttributeMap.h"
//#include "TInterpreter.h"
//#include "TROOT.h"
//#include "TBuffer.h"
//#include "TMemberInspector.h"
//#include "TInterpreter.h"
//#include "TVirtualMutex.h"
//#include "TError.h"

//#ifndef G__ROOT
//#define G__ROOT
//#endif

//#include "RtypesImp.h"
//#include "TIsAProxy.h"
//#include "TFileMergeInfo.h"
//#include <algorithm>
//#include "TCollectionProxyInfo.h"
///*******************************************************************/

//#include "TDataMember.h"

//// Since CINT ignores the std namespace, we need to do so in this file.
//namespace std {} using namespace std;

//// Header files passed as explicit arguments
//#include "red-daq/EvHeader.hh"
//#include "red-daq/RDCluster.hh"
//#include "red-daq/RunHeader.hh"
//#include "red-daq/EvRec0.hh"
//#include "red-daq/EvRaw0.hh"

//// Header files passed via #pragma extra_include

//namespace ROOT {
//   static void *new_EvHeader(void *p = 0);
//   static void *newArray_EvHeader(Long_t size, void *p);
//   static void delete_EvHeader(void *p);
//   static void deleteArray_EvHeader(void *p);
//   static void destruct_EvHeader(void *p);
//   static void streamer_EvHeader(TBuffer &buf, void *obj);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EvHeader*)
//   {
//      ::EvHeader *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvHeader >(0);
//      static ::ROOT::TGenericClassInfo
//         instance("EvHeader", ::EvHeader::Class_Version(), "EvHeader.hh", 30,
//                  typeid(::EvHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &::EvHeader::Dictionary, isa_proxy, 16,
//                  sizeof(::EvHeader) );
//      instance.SetNew(&new_EvHeader);
//      instance.SetNewArray(&newArray_EvHeader);
//      instance.SetDelete(&delete_EvHeader);
//      instance.SetDeleteArray(&deleteArray_EvHeader);
//      instance.SetDestructor(&destruct_EvHeader);
//      instance.SetStreamerFunc(&streamer_EvHeader);
//      return &instance;
//   }
//   TGenericClassInfo *GenerateInitInstance(const ::EvHeader*)
//   {
//      return GenerateInitInstanceLocal((::EvHeader*)0);
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EvHeader*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
//} // end of namespace ROOT

//namespace ROOT {
//   static void *new_EvRaw0(void *p = 0);
//   static void *newArray_EvRaw0(Long_t size, void *p);
//   static void delete_EvRaw0(void *p);
//   static void deleteArray_EvRaw0(void *p);
//   static void destruct_EvRaw0(void *p);
//   static void streamer_EvRaw0(TBuffer &buf, void *obj);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EvRaw0*)
//   {
//      ::EvRaw0 *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvRaw0 >(0);
//      static ::ROOT::TGenericClassInfo
//         instance("EvRaw0", ::EvRaw0::Class_Version(), "EvRaw0.hh", 33,
//                  typeid(::EvRaw0), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &::EvRaw0::Dictionary, isa_proxy, 16,
//                  sizeof(::EvRaw0) );
//      instance.SetNew(&new_EvRaw0);
//      instance.SetNewArray(&newArray_EvRaw0);
//      instance.SetDelete(&delete_EvRaw0);
//      instance.SetDeleteArray(&deleteArray_EvRaw0);
//      instance.SetDestructor(&destruct_EvRaw0);
//      instance.SetStreamerFunc(&streamer_EvRaw0);
//      return &instance;
//   }
//   TGenericClassInfo *GenerateInitInstance(const ::EvRaw0*)
//   {
//      return GenerateInitInstanceLocal((::EvRaw0*)0);
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EvRaw0*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
//} // end of namespace ROOT

//namespace ROOT {
//   static void *new_RDCluster(void *p = 0);
//   static void *newArray_RDCluster(Long_t size, void *p);
//   static void delete_RDCluster(void *p);
//   static void deleteArray_RDCluster(void *p);
//   static void destruct_RDCluster(void *p);
//   static void streamer_RDCluster(TBuffer &buf, void *obj);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RDCluster*)
//   {
//      ::RDCluster *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RDCluster >(0);
//      static ::ROOT::TGenericClassInfo
//         instance("RDCluster", ::RDCluster::Class_Version(), "RDCluster.hh", 10,
//                  typeid(::RDCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &::RDCluster::Dictionary, isa_proxy, 16,
//                  sizeof(::RDCluster) );
//      instance.SetNew(&new_RDCluster);
//      instance.SetNewArray(&newArray_RDCluster);
//      instance.SetDelete(&delete_RDCluster);
//      instance.SetDeleteArray(&deleteArray_RDCluster);
//      instance.SetDestructor(&destruct_RDCluster);
//      instance.SetStreamerFunc(&streamer_RDCluster);
//      return &instance;
//   }
//   TGenericClassInfo *GenerateInitInstance(const ::RDCluster*)
//   {
//      return GenerateInitInstanceLocal((::RDCluster*)0);
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RDCluster*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
//} // end of namespace ROOT

//namespace ROOT {
//   static void *new_RunHeader(void *p = 0);
//   static void *newArray_RunHeader(Long_t size, void *p);
//   static void delete_RunHeader(void *p);
//   static void deleteArray_RunHeader(void *p);
//   static void destruct_RunHeader(void *p);
//   static void streamer_RunHeader(TBuffer &buf, void *obj);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RunHeader*)
//   {
//      ::RunHeader *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RunHeader >(0);
//      static ::ROOT::TGenericClassInfo
//         instance("RunHeader", ::RunHeader::Class_Version(), "RunHeader.hh", 24,
//                  typeid(::RunHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &::RunHeader::Dictionary, isa_proxy, 16,
//                  sizeof(::RunHeader) );
//      instance.SetNew(&new_RunHeader);
//      instance.SetNewArray(&newArray_RunHeader);
//      instance.SetDelete(&delete_RunHeader);
//      instance.SetDeleteArray(&deleteArray_RunHeader);
//      instance.SetDestructor(&destruct_RunHeader);
//      instance.SetStreamerFunc(&streamer_RunHeader);
//      return &instance;
//   }
//   TGenericClassInfo *GenerateInitInstance(const ::RunHeader*)
//   {
//      return GenerateInitInstanceLocal((::RunHeader*)0);
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RunHeader*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
//} // end of namespace ROOT

//namespace ROOT {
//   static void *new_EvRec0(void *p = 0);
//   static void *newArray_EvRec0(Long_t size, void *p);
//   static void delete_EvRec0(void *p);
//   static void deleteArray_EvRec0(void *p);
//   static void destruct_EvRec0(void *p);
//   static void streamer_EvRec0(TBuffer &buf, void *obj);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EvRec0*)
//   {
//      ::EvRec0 *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvRec0 >(0);
//      static ::ROOT::TGenericClassInfo
//         instance("EvRec0", ::EvRec0::Class_Version(), "EvRec0.hh", 32,
//                  typeid(::EvRec0), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &::EvRec0::Dictionary, isa_proxy, 16,
//                  sizeof(::EvRec0) );
//      instance.SetNew(&new_EvRec0);
//      instance.SetNewArray(&newArray_EvRec0);
//      instance.SetDelete(&delete_EvRec0);
//      instance.SetDeleteArray(&deleteArray_EvRec0);
//      instance.SetDestructor(&destruct_EvRec0);
//      instance.SetStreamerFunc(&streamer_EvRec0);
//      return &instance;
//   }
//   TGenericClassInfo *GenerateInitInstance(const ::EvRec0*)
//   {
//      return GenerateInitInstanceLocal((::EvRec0*)0);
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EvRec0*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
//} // end of namespace ROOT

////______________________________________________________________________________
//atomic_TClass_ptr EvHeader::fgIsA(0);  // static to hold class pointer

////______________________________________________________________________________
//const char *EvHeader::Class_Name()
//{
//   return "EvHeader";
//}

////______________________________________________________________________________
//const char *EvHeader::ImplFileName()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::EvHeader*)0x0)->GetImplFileName();
//}

////______________________________________________________________________________
//int EvHeader::ImplFileLine()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::EvHeader*)0x0)->GetImplFileLine();
//}

////______________________________________________________________________________
//TClass *EvHeader::Dictionary()
//{
//   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvHeader*)0x0)->GetClass();
//   return fgIsA;
//}

////______________________________________________________________________________
//TClass *EvHeader::Class()
//{
//   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvHeader*)0x0)->GetClass(); }
//   return fgIsA;
//}

////______________________________________________________________________________
//atomic_TClass_ptr EvRaw0::fgIsA(0);  // static to hold class pointer

////______________________________________________________________________________
//const char *EvRaw0::Class_Name()
//{
//   return "EvRaw0";
//}

////______________________________________________________________________________
//const char *EvRaw0::ImplFileName()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::EvRaw0*)0x0)->GetImplFileName();
//}

////______________________________________________________________________________
//int EvRaw0::ImplFileLine()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::EvRaw0*)0x0)->GetImplFileLine();
//}

////______________________________________________________________________________
//TClass *EvRaw0::Dictionary()
//{
//   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvRaw0*)0x0)->GetClass();
//   return fgIsA;
//}

////______________________________________________________________________________
//TClass *EvRaw0::Class()
//{
//   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvRaw0*)0x0)->GetClass(); }
//   return fgIsA;
//}

////______________________________________________________________________________
//atomic_TClass_ptr RDCluster::fgIsA(0);  // static to hold class pointer

////______________________________________________________________________________
//const char *RDCluster::Class_Name()
//{
//   return "RDCluster";
//}

////______________________________________________________________________________
//const char *RDCluster::ImplFileName()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::RDCluster*)0x0)->GetImplFileName();
//}

////______________________________________________________________________________
//int RDCluster::ImplFileLine()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::RDCluster*)0x0)->GetImplFileLine();
//}

////______________________________________________________________________________
//TClass *RDCluster::Dictionary()
//{
//   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RDCluster*)0x0)->GetClass();
//   return fgIsA;
//}

////______________________________________________________________________________
//TClass *RDCluster::Class()
//{
//   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RDCluster*)0x0)->GetClass(); }
//   return fgIsA;
//}

////______________________________________________________________________________
//atomic_TClass_ptr RunHeader::fgIsA(0);  // static to hold class pointer

////______________________________________________________________________________
//const char *RunHeader::Class_Name()
//{
//   return "RunHeader";
//}

////______________________________________________________________________________
//const char *RunHeader::ImplFileName()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::RunHeader*)0x0)->GetImplFileName();
//}

////______________________________________________________________________________
//int RunHeader::ImplFileLine()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::RunHeader*)0x0)->GetImplFileLine();
//}

////______________________________________________________________________________
//TClass *RunHeader::Dictionary()
//{
//   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RunHeader*)0x0)->GetClass();
//   return fgIsA;
//}

////______________________________________________________________________________
//TClass *RunHeader::Class()
//{
//   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RunHeader*)0x0)->GetClass(); }
//   return fgIsA;
//}

////______________________________________________________________________________
//atomic_TClass_ptr EvRec0::fgIsA(0);  // static to hold class pointer

////______________________________________________________________________________
//const char *EvRec0::Class_Name()
//{
//   return "EvRec0";
//}

////______________________________________________________________________________
//const char *EvRec0::ImplFileName()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::EvRec0*)0x0)->GetImplFileName();
//}

////______________________________________________________________________________
//int EvRec0::ImplFileLine()
//{
//   return ::ROOT::GenerateInitInstanceLocal((const ::EvRec0*)0x0)->GetImplFileLine();
//}

////______________________________________________________________________________
//TClass *EvRec0::Dictionary()
//{
//   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvRec0*)0x0)->GetClass();
//   return fgIsA;
//}

////______________________________________________________________________________
//TClass *EvRec0::Class()
//{
//   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvRec0*)0x0)->GetClass(); }
//   return fgIsA;
//}

////______________________________________________________________________________
//void EvHeader::Streamer(TBuffer &R__b)
//{
//   // Stream an object of class EvHeader.

//   UInt_t R__s, R__c;
//   if (R__b.IsReading()) {
//      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
//      TObject::Streamer(R__b);
//      R__b >> event_num;
//      { TString R__str; R__str.Streamer(R__b); date = R__str.Data(); }
//      { TString R__str; R__str.Streamer(R__b); run_type = R__str.Data(); }
//      { TString R__str; R__str.Streamer(R__b); rec_ver = R__str.Data(); }
//      { TString R__str; R__str.Streamer(R__b); input_file = R__str.Data(); }
//      R__b >> run_number;
//      R__b >> n_channels;
//      R__b >> peak_buffer;
//      R__b >> time;
//      {
//         vector<UInt_t> &R__stl =  boardtimes;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            unsigned int R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      R__b.CheckByteCount(R__s, R__c, EvHeader::IsA());
//   } else {
//      R__c = R__b.WriteVersion(EvHeader::IsA(), kTRUE);
//      TObject::Streamer(R__b);
//      R__b << event_num;
//      { TString R__str = date.c_str(); R__str.Streamer(R__b);}
//      { TString R__str = run_type.c_str(); R__str.Streamer(R__b);}
//      { TString R__str = rec_ver.c_str(); R__str.Streamer(R__b);}
//      { TString R__str = input_file.c_str(); R__str.Streamer(R__b);}
//      R__b << run_number;
//      R__b << n_channels;
//      R__b << peak_buffer;
//      R__b << time;
//      {
//         vector<UInt_t> &R__stl =  boardtimes;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<UInt_t>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      R__b.SetByteCount(R__c, kTRUE);
//   }
//}

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_EvHeader(void *p) {
//      return  p ? new(p) ::EvHeader : new ::EvHeader;
//   }
//   static void *newArray_EvHeader(Long_t nElements, void *p) {
//      return p ? new(p) ::EvHeader[nElements] : new ::EvHeader[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_EvHeader(void *p) {
//      delete ((::EvHeader*)p);
//   }
//   static void deleteArray_EvHeader(void *p) {
//      delete [] ((::EvHeader*)p);
//   }
//   static void destruct_EvHeader(void *p) {
//      typedef ::EvHeader current_t;
//      ((current_t*)p)->~current_t();
//   }
//   // Wrapper around a custom streamer member function.
//   static void streamer_EvHeader(TBuffer &buf, void *obj) {
//      ((::EvHeader*)obj)->::EvHeader::Streamer(buf);
//   }
//} // end of namespace ROOT for class ::EvHeader

////______________________________________________________________________________
//void EvRaw0::Streamer(TBuffer &R__b)
//{
//   // Stream an object of class EvRaw0.

//   UInt_t R__s, R__c;
//   if (R__b.IsReading()) {
//      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
//      TObject::Streamer(R__b);
//      R__b >> evheader;
//      {
//         map<int,vector<double>*> &R__stl =  wf;
//         R__stl.clear();
//         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> > *));
//         if (R__tcl2==0) {
//            Error("wf streamer","Missing the TClass object for class std::vector<double, class std::allocator<double> > *!");
//            return;
//         }
//         int R__i, R__n;
//         R__b >> R__n;
//         for (R__i = 0; R__i < R__n; R__i++) {
//            int R__t;
//            R__b >> R__t;
//            vector<double>* R__t2;
//            R__t2 = (vector<double>*)R__b.ReadObjectAny(R__tcl2);
//            typedef int Value_t;
//            std::pair<Value_t const, class std::vector<double, class std::allocator<double> > * > R__t3(R__t,R__t2);
//            R__stl.insert(R__t3);
//         }
//      }
//      R__b.CheckByteCount(R__s, R__c, EvRaw0::IsA());
//   } else {
//      R__c = R__b.WriteVersion(EvRaw0::IsA(), kTRUE);
//      TObject::Streamer(R__b);
//      R__b << evheader;
//      {
//         map<int,vector<double>*> &R__stl =  wf;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//         TClass *R__tcl2 = TBuffer::GetClass(typeid(class std::vector<double, class std::allocator<double> > *));
//         if (R__tcl2==0) {
//            Error("wfstreamer","Missing the TClass object for class std::vector<double, class std::allocator<double> > *!");
//            return;
//         }
//            map<int,vector<double>*>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << ((*R__k).first );
//            R__b.WriteObjectAny(((*R__k).second),R__tcl2);
//            }
//         }
//      }
//      R__b.SetByteCount(R__c, kTRUE);
//   }
//}

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_EvRaw0(void *p) {
//      return  p ? new(p) ::EvRaw0 : new ::EvRaw0;
//   }
//   static void *newArray_EvRaw0(Long_t nElements, void *p) {
//      return p ? new(p) ::EvRaw0[nElements] : new ::EvRaw0[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_EvRaw0(void *p) {
//      delete ((::EvRaw0*)p);
//   }
//   static void deleteArray_EvRaw0(void *p) {
//      delete [] ((::EvRaw0*)p);
//   }
//   static void destruct_EvRaw0(void *p) {
//      typedef ::EvRaw0 current_t;
//      ((current_t*)p)->~current_t();
//   }
//   // Wrapper around a custom streamer member function.
//   static void streamer_EvRaw0(TBuffer &buf, void *obj) {
//      ((::EvRaw0*)obj)->::EvRaw0::Streamer(buf);
//   }
//} // end of namespace ROOT for class ::EvRaw0

////______________________________________________________________________________
//void RDCluster::Streamer(TBuffer &R__b)
//{
//   // Stream an object of class RDCluster.

//   UInt_t R__s, R__c;
//   if (R__b.IsReading()) {
//      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
//      TObject::Streamer(R__b);
//      R__b >> start_time;
//      R__b >> start;
//      R__b >> stop;
//      R__b >> rep;
//      R__b >> charge;
//      R__b >> f90;
//      R__b >> mean_time;
//      R__b >> rms_time;
//      R__b >> min_x;
//      R__b >> min_y;
//      R__b >> pos_x;
//      R__b >> pos_y;
//      R__b >> cdf_time;
//      R__b >> tot_charge_top;
//      R__b >> tot_charge_bottom;
//      {
//         vector<double> &R__stl =  charge_top;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  charge_bottom;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      R__b.CheckByteCount(R__s, R__c, RDCluster::IsA());
//   } else {
//      R__c = R__b.WriteVersion(RDCluster::IsA(), kTRUE);
//      TObject::Streamer(R__b);
//      R__b << start_time;
//      R__b << start;
//      R__b << stop;
//      R__b << rep;
//      R__b << charge;
//      R__b << f90;
//      R__b << mean_time;
//      R__b << rms_time;
//      R__b << min_x;
//      R__b << min_y;
//      R__b << pos_x;
//      R__b << pos_y;
//      R__b << cdf_time;
//      R__b << tot_charge_top;
//      R__b << tot_charge_bottom;
//      {
//         vector<double> &R__stl =  charge_top;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  charge_bottom;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      R__b.SetByteCount(R__c, kTRUE);
//   }
//}

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_RDCluster(void *p) {
//      return  p ? new(p) ::RDCluster : new ::RDCluster;
//   }
//   static void *newArray_RDCluster(Long_t nElements, void *p) {
//      return p ? new(p) ::RDCluster[nElements] : new ::RDCluster[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_RDCluster(void *p) {
//      delete ((::RDCluster*)p);
//   }
//   static void deleteArray_RDCluster(void *p) {
//      delete [] ((::RDCluster*)p);
//   }
//   static void destruct_RDCluster(void *p) {
//      typedef ::RDCluster current_t;
//      ((current_t*)p)->~current_t();
//   }
//   // Wrapper around a custom streamer member function.
//   static void streamer_RDCluster(TBuffer &buf, void *obj) {
//      ((::RDCluster*)obj)->::RDCluster::Streamer(buf);
//   }
//} // end of namespace ROOT for class ::RDCluster

////______________________________________________________________________________
//void RunHeader::Streamer(TBuffer &R__b)
//{
//   // Stream an object of class RunHeader.

//   UInt_t R__s, R__c;
//   if (R__b.IsReading()) {
//      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
//      TObject::Streamer(R__b);
//      { TString R__str; R__str.Streamer(R__b); date = R__str.Data(); }
//      R__b >> run_number;
//      { TString R__str; R__str.Streamer(R__b); run_type = R__str.Data(); }
//      R__b >> n_events;
//      R__b >> fbk_bias;
//      R__b >> sensl_bias;
//      R__b >> anode;
//      R__b >> cathode;
//      R__b >> grid;
//      { TString R__str; R__str.Streamer(R__b); comments = R__str.Data(); }
//      R__b.CheckByteCount(R__s, R__c, RunHeader::IsA());
//   } else {
//      R__c = R__b.WriteVersion(RunHeader::IsA(), kTRUE);
//      TObject::Streamer(R__b);
//      { TString R__str = date.c_str(); R__str.Streamer(R__b);}
//      R__b << run_number;
//      { TString R__str = run_type.c_str(); R__str.Streamer(R__b);}
//      R__b << n_events;
//      R__b << fbk_bias;
//      R__b << sensl_bias;
//      R__b << anode;
//      R__b << cathode;
//      R__b << grid;
//      { TString R__str = comments.c_str(); R__str.Streamer(R__b);}
//      R__b.SetByteCount(R__c, kTRUE);
//   }
//}

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_RunHeader(void *p) {
//      return  p ? new(p) ::RunHeader : new ::RunHeader;
//   }
//   static void *newArray_RunHeader(Long_t nElements, void *p) {
//      return p ? new(p) ::RunHeader[nElements] : new ::RunHeader[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_RunHeader(void *p) {
//      delete ((::RunHeader*)p);
//   }
//   static void deleteArray_RunHeader(void *p) {
//      delete [] ((::RunHeader*)p);
//   }
//   static void destruct_RunHeader(void *p) {
//      typedef ::RunHeader current_t;
//      ((current_t*)p)->~current_t();
//   }
//   // Wrapper around a custom streamer member function.
//   static void streamer_RunHeader(TBuffer &buf, void *obj) {
//      ((::RunHeader*)obj)->::RunHeader::Streamer(buf);
//   }
//} // end of namespace ROOT for class ::RunHeader

////______________________________________________________________________________
//void EvRec0::Streamer(TBuffer &R__b)
//{
//   // Stream an object of class EvRec0.

//   UInt_t R__s, R__c;
//   if (R__b.IsReading()) {
//      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
//      TObject::Streamer(R__b);
//      R__b >> evheader;
//      R__b >> event_number;
//      R__b >> charge_total;
//      R__b >> f90_tot;
//      R__b >> charge_total_lsci;
//      R__b >> lsci_psd_tot;
//      R__b >> baseline_mean_all;
//      R__b >> baseline_rms_all;
//      {
//         vector<double> &R__stl =  baseline_mean;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  baseline_rms;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  charge;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  f90;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  start_time;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  xmin;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  xmax;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  ymin;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      {
//         vector<double> &R__stl =  ymax;
//         R__stl.clear();
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            double R__t;
//            R__b >> R__t;
//            R__stl.push_back(R__t);
//         }
//      }
//      R__b >> number_of_clusters;
//      {
//         vector<RDCluster*> &R__stl =  clusters;
//         R__stl.clear();
//         TClass *R__tcl1 = TBuffer::GetClass(typeid(class RDCluster *));
//         if (R__tcl1==0) {
//            Error("clusters streamer","Missing the TClass object for class RDCluster *!");
//            return;
//         }
//         int R__i, R__n;
//         R__b >> R__n;
//         R__stl.reserve(R__n);
//         for (R__i = 0; R__i < R__n; R__i++) {
//            RDCluster* R__t;
//            R__t = (RDCluster*)R__b.ReadObjectAny(R__tcl1);
//            R__stl.push_back(R__t);
//         }
//      }
//      R__b.CheckByteCount(R__s, R__c, EvRec0::IsA());
//   } else {
//      R__c = R__b.WriteVersion(EvRec0::IsA(), kTRUE);
//      TObject::Streamer(R__b);
//      R__b << evheader;
//      R__b << event_number;
//      R__b << charge_total;
//      R__b << f90_tot;
//      R__b << charge_total_lsci;
//      R__b << lsci_psd_tot;
//      R__b << baseline_mean_all;
//      R__b << baseline_rms_all;
//      {
//         vector<double> &R__stl =  baseline_mean;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  baseline_rms;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  charge;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  f90;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  start_time;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  xmin;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  xmax;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  ymin;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      {
//         vector<double> &R__stl =  ymax;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<double>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      R__b << number_of_clusters;
//      {
//         vector<RDCluster*> &R__stl =  clusters;
//         int R__n=int(R__stl.size());
//         R__b << R__n;
//         if(R__n) {
//            vector<RDCluster*>::iterator R__k;
//            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
//            R__b << (*R__k);
//            }
//         }
//      }
//      R__b.SetByteCount(R__c, kTRUE);
//   }
//}

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_EvRec0(void *p) {
//      return  p ? new(p) ::EvRec0 : new ::EvRec0;
//   }
//   static void *newArray_EvRec0(Long_t nElements, void *p) {
//      return p ? new(p) ::EvRec0[nElements] : new ::EvRec0[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_EvRec0(void *p) {
//      delete ((::EvRec0*)p);
//   }
//   static void deleteArray_EvRec0(void *p) {
//      delete [] ((::EvRec0*)p);
//   }
//   static void destruct_EvRec0(void *p) {
//      typedef ::EvRec0 current_t;
//      ((current_t*)p)->~current_t();
//   }
//   // Wrapper around a custom streamer member function.
//   static void streamer_EvRec0(TBuffer &buf, void *obj) {
//      ((::EvRec0*)obj)->::EvRec0::Streamer(buf);
//   }
//} // end of namespace ROOT for class ::EvRec0

//namespace ROOT {
//   static TClass *vectorlEunsignedsPintgR_Dictionary();
//   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
//   static void *new_vectorlEunsignedsPintgR(void *p = 0);
//   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
//   static void delete_vectorlEunsignedsPintgR(void *p);
//   static void deleteArray_vectorlEunsignedsPintgR(void *p);
//   static void destruct_vectorlEunsignedsPintgR(void *p);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
//   {
//      vector<unsigned int> *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
//      static ::ROOT::TGenericClassInfo
//         instance("vector<unsigned int>", -2, "vector", 339,
//                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
//                  sizeof(vector<unsigned int>) );
//      instance.SetNew(&new_vectorlEunsignedsPintgR);
//      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
//      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
//      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
//      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
//      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));
//      return &instance;
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

//   // Dictionary for non-ClassDef classes
//   static TClass *vectorlEunsignedsPintgR_Dictionary() {
//      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)0x0)->GetClass();
//      vectorlEunsignedsPintgR_TClassManip(theClass);
//   return theClass;
//   }

//   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
//   }

//} // end of namespace ROOT

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_vectorlEunsignedsPintgR(void *p) {
//      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
//   }
//   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
//      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_vectorlEunsignedsPintgR(void *p) {
//      delete ((vector<unsigned int>*)p);
//   }
//   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
//      delete [] ((vector<unsigned int>*)p);
//   }
//   static void destruct_vectorlEunsignedsPintgR(void *p) {
//      typedef vector<unsigned int> current_t;
//      ((current_t*)p)->~current_t();
//   }
//} // end of namespace ROOT for class vector<unsigned int>

//namespace ROOT {
//   static TClass *vectorlEdoublegR_Dictionary();
//   static void vectorlEdoublegR_TClassManip(TClass*);
//   static void *new_vectorlEdoublegR(void *p = 0);
//   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
//   static void delete_vectorlEdoublegR(void *p);
//   static void deleteArray_vectorlEdoublegR(void *p);
//   static void destruct_vectorlEdoublegR(void *p);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
//   {
//      vector<double> *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
//      static ::ROOT::TGenericClassInfo
//         instance("vector<double>", -2, "vector", 339,
//                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
//                  sizeof(vector<double>) );
//      instance.SetNew(&new_vectorlEdoublegR);
//      instance.SetNewArray(&newArray_vectorlEdoublegR);
//      instance.SetDelete(&delete_vectorlEdoublegR);
//      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
//      instance.SetDestructor(&destruct_vectorlEdoublegR);
//      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
//      return &instance;
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

//   // Dictionary for non-ClassDef classes
//   static TClass *vectorlEdoublegR_Dictionary() {
//      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
//      vectorlEdoublegR_TClassManip(theClass);
//   return theClass;
//   }

//   static void vectorlEdoublegR_TClassManip(TClass* ){
//   }

//} // end of namespace ROOT

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_vectorlEdoublegR(void *p) {
//      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
//   }
//   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
//      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_vectorlEdoublegR(void *p) {
//      delete ((vector<double>*)p);
//   }
//   static void deleteArray_vectorlEdoublegR(void *p) {
//      delete [] ((vector<double>*)p);
//   }
//   static void destruct_vectorlEdoublegR(void *p) {
//      typedef vector<double> current_t;
//      ((current_t*)p)->~current_t();
//   }
//} // end of namespace ROOT for class vector<double>

//namespace ROOT {
//   static TClass *vectorlERDClustermUgR_Dictionary();
//   static void vectorlERDClustermUgR_TClassManip(TClass*);
//   static void *new_vectorlERDClustermUgR(void *p = 0);
//   static void *newArray_vectorlERDClustermUgR(Long_t size, void *p);
//   static void delete_vectorlERDClustermUgR(void *p);
//   static void deleteArray_vectorlERDClustermUgR(void *p);
//   static void destruct_vectorlERDClustermUgR(void *p);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<RDCluster*>*)
//   {
//      vector<RDCluster*> *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<RDCluster*>));
//      static ::ROOT::TGenericClassInfo
//         instance("vector<RDCluster*>", -2, "vector", 339,
//                  typeid(vector<RDCluster*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &vectorlERDClustermUgR_Dictionary, isa_proxy, 0,
//                  sizeof(vector<RDCluster*>) );
//      instance.SetNew(&new_vectorlERDClustermUgR);
//      instance.SetNewArray(&newArray_vectorlERDClustermUgR);
//      instance.SetDelete(&delete_vectorlERDClustermUgR);
//      instance.SetDeleteArray(&deleteArray_vectorlERDClustermUgR);
//      instance.SetDestructor(&destruct_vectorlERDClustermUgR);
//      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<RDCluster*> >()));
//      return &instance;
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<RDCluster*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

//   // Dictionary for non-ClassDef classes
//   static TClass *vectorlERDClustermUgR_Dictionary() {
//      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<RDCluster*>*)0x0)->GetClass();
//      vectorlERDClustermUgR_TClassManip(theClass);
//   return theClass;
//   }

//   static void vectorlERDClustermUgR_TClassManip(TClass* ){
//   }

//} // end of namespace ROOT

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_vectorlERDClustermUgR(void *p) {
//      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RDCluster*> : new vector<RDCluster*>;
//   }
//   static void *newArray_vectorlERDClustermUgR(Long_t nElements, void *p) {
//      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<RDCluster*>[nElements] : new vector<RDCluster*>[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_vectorlERDClustermUgR(void *p) {
//      delete ((vector<RDCluster*>*)p);
//   }
//   static void deleteArray_vectorlERDClustermUgR(void *p) {
//      delete [] ((vector<RDCluster*>*)p);
//   }
//   static void destruct_vectorlERDClustermUgR(void *p) {
//      typedef vector<RDCluster*> current_t;
//      ((current_t*)p)->~current_t();
//   }
//} // end of namespace ROOT for class vector<RDCluster*>

//namespace ROOT {
//   static TClass *maplEintcOvectorlEdoublegRmUgR_Dictionary();
//   static void maplEintcOvectorlEdoublegRmUgR_TClassManip(TClass*);
//   static void *new_maplEintcOvectorlEdoublegRmUgR(void *p = 0);
//   static void *newArray_maplEintcOvectorlEdoublegRmUgR(Long_t size, void *p);
//   static void delete_maplEintcOvectorlEdoublegRmUgR(void *p);
//   static void deleteArray_maplEintcOvectorlEdoublegRmUgR(void *p);
//   static void destruct_maplEintcOvectorlEdoublegRmUgR(void *p);

//   // Function generating the singleton type initializer
//   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<double>*>*)
//   {
//      map<int,vector<double>*> *ptr = 0;
//      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<double>*>));
//      static ::ROOT::TGenericClassInfo
//         instance("map<int,vector<double>*>", -2, "map", 100,
//                  typeid(map<int,vector<double>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
//                  &maplEintcOvectorlEdoublegRmUgR_Dictionary, isa_proxy, 0,
//                  sizeof(map<int,vector<double>*>) );
//      instance.SetNew(&new_maplEintcOvectorlEdoublegRmUgR);
//      instance.SetNewArray(&newArray_maplEintcOvectorlEdoublegRmUgR);
//      instance.SetDelete(&delete_maplEintcOvectorlEdoublegRmUgR);
//      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEdoublegRmUgR);
//      instance.SetDestructor(&destruct_maplEintcOvectorlEdoublegRmUgR);
//      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<double>*> >()));
//      return &instance;
//   }
//   // Static variable to force the class initialization
//   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,vector<double>*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

//   // Dictionary for non-ClassDef classes
//   static TClass *maplEintcOvectorlEdoublegRmUgR_Dictionary() {
//      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<double>*>*)0x0)->GetClass();
//      maplEintcOvectorlEdoublegRmUgR_TClassManip(theClass);
//   return theClass;
//   }

//   static void maplEintcOvectorlEdoublegRmUgR_TClassManip(TClass* ){
//   }

//} // end of namespace ROOT

//namespace ROOT {
//   // Wrappers around operator new
//   static void *new_maplEintcOvectorlEdoublegRmUgR(void *p) {
//      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<double>*> : new map<int,vector<double>*>;
//   }
//   static void *newArray_maplEintcOvectorlEdoublegRmUgR(Long_t nElements, void *p) {
//      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<double>*>[nElements] : new map<int,vector<double>*>[nElements];
//   }
//   // Wrapper around operator delete
//   static void delete_maplEintcOvectorlEdoublegRmUgR(void *p) {
//      delete ((map<int,vector<double>*>*)p);
//   }
//   static void deleteArray_maplEintcOvectorlEdoublegRmUgR(void *p) {
//      delete [] ((map<int,vector<double>*>*)p);
//   }
//   static void destruct_maplEintcOvectorlEdoublegRmUgR(void *p) {
//      typedef map<int,vector<double>*> current_t;
//      ((current_t*)p)->~current_t();
//   }
//} // end of namespace ROOT for class map<int,vector<double>*>

//namespace {
//  void TriggerDictionaryInitialization_Dict_Impl() {
//    static const char* headers[] = {
//"EvRaw0.hh",
//"EvHeader.hh",
//"RDCluster.hh",
//"RunHeader.hh",
//"EvRec0.hh",
//0
//    };
//    static const char* includePaths[] = {
//"/home/vlad/Soft/root/builddir/include",
//"/usr/local/include/red-daq/",
//0
//    };
//    static const char* fwdDeclCode = R"DICTFWDDCLS(
//#line 1 "Dict dictionary forward declarations' payload"
//#pragma clang diagnostic ignored "-Wkeyword-compat"
//#pragma clang diagnostic ignored "-Wignored-attributes"
//#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
//extern int __Cling_Autoloading_Map;
//class __attribute__((annotate("$clingAutoload$EvHeader.hh")))  __attribute__((annotate("$clingAutoload$EvRaw0.hh")))  EvHeader;
//class __attribute__((annotate("$clingAutoload$EvRaw0.hh")))  EvRaw0;
//class __attribute__((annotate("$clingAutoload$RDCluster.hh")))  RDCluster;
//class __attribute__((annotate("$clingAutoload$RunHeader.hh")))  RunHeader;
//class __attribute__((annotate("$clingAutoload$EvRec0.hh")))  EvRec0;
//)DICTFWDDCLS";
//    static const char* payloadCode = R"DICTPAYLOAD(
//#line 1 "Dict dictionary payload"

//#ifndef G__VECTOR_HAS_CLASS_ITERATOR
//  #define G__VECTOR_HAS_CLASS_ITERATOR 1
//#endif

//#define _BACKWARD_BACKWARD_WARNING_H
//#include "EvRaw0.hh"
//#include "EvHeader.hh"
//#include "RDCluster.hh"
//#include "RunHeader.hh"
//#include "EvRec0.hh"

//#undef  _BACKWARD_BACKWARD_WARNING_H
//)DICTPAYLOAD";
//    static const char* classesHeaders[]={
//"EvHeader", payloadCode, "@",
//"EvRaw0", payloadCode, "@",
//"EvRec0", payloadCode, "@",
//"RDCluster", payloadCode, "@",
//"RunHeader", payloadCode, "@",
//nullptr};

//    static bool isInitialized = false;
//    if (!isInitialized) {
//      TROOT::RegisterModule("Dict",
//        headers, includePaths, payloadCode, fwdDeclCode,
//        TriggerDictionaryInitialization_Dict_Impl, {}, classesHeaders, /*has no C++ module*/false);
//      isInitialized = true;
//    }
//  }
//  static struct DictInit {
//    DictInit() {
//      TriggerDictionaryInitialization_Dict_Impl();
//    }
//  } __TheDictionaryInitializer;
//}
//void TriggerDictionaryInitialization_Dict() {
//  TriggerDictionaryInitialization_Dict_Impl();
//}
