//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MED_Common_HeaderFile
#define MED_Common_HeaderFile

#include <string>
#include <vector>
#include <set>
#include <map>

extern "C"{
#include <med.h>
}  

#include <boost/shared_ptr.hpp>

namespace MED{
  using namespace std;
  using namespace boost;

  typedef vector<med_float> TFloatVector;
  typedef vector<string> TStringVector;
  typedef vector<med_int> TIntVector;
  typedef set<string> TStringSet;

  typedef map<med_geometrie_element,med_int> TGeom;
  typedef map<med_entite_maillage,TGeom> TEntityInfo;

  med_int GetNbConn(med_entite_maillage theMedEntity, 
		    med_geometrie_element theMedType,
		    med_int theMeshDim);

  struct TNameInfo;
  typedef shared_ptr<TNameInfo> PNameInfo;
  
  struct TMeshInfo;
  typedef shared_ptr<TMeshInfo> PMeshInfo;
  
  struct TFamilyInfo;
  typedef shared_ptr<TFamilyInfo> PFamilyInfo;

  struct TElemInfo;
  typedef shared_ptr<TElemInfo> PElemInfo;

  struct TNodeInfo;
  typedef shared_ptr<TNodeInfo> PNodeInfo;

  struct TCellInfo;
  typedef shared_ptr<TCellInfo> PCellInfo;

  struct TFieldInfo;
  typedef shared_ptr<TFieldInfo> PFieldInfo;

  struct TTimeStampInfo;
  typedef shared_ptr<TTimeStampInfo> PTimeStampInfo;
  
  struct TTimeStampVal;
  typedef shared_ptr<TTimeStampVal> PTimeStampVal;

  class TWrapper;
  typedef shared_ptr<TWrapper> PWrapper;
};


#endif
