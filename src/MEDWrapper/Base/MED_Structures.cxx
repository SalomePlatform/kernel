//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_Structure.cxx
//  Author : Eugeny NIKOLAEV
//  Module : VISU

#include "MED_Structures.hxx"
#include "MED_Utilities.hxx"
using namespace MED;

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

#if defined __GNUC_2__
#define GETINDEX(anArray,ind) anArray[ind]
#else
#define GETINDEX(anArray,ind) anArray.at(ind)
#endif

namespace MED{

  TInt GetNbConnectivities(EGeometrieElement typmai)
  {
    TInt taille = typmai%100;
    return taille;
  }

  template<>
  TInt GetNbConn<eV2_1>(EGeometrieElement typmai,
			TInt mdim)
  {
    TInt edim = typmai / 100;
    TInt nsup = 0;

    if (mdim  == 2 || mdim == 3)
      if (edim == 1)
	nsup = 1;
    
    if (mdim == 3)
      if (edim == 2)
	nsup = 1;
    
    TInt taille = nsup+typmai%100;
    return taille;
  }

  template<>
  TInt GetNbConn<eV2_2>(EGeometrieElement typmai,
			TInt mdim)
  {
    TInt taille = typmai%100;
    return taille;
  }

  std::string GetString(TInt theId, TInt theStep, 
			const TString& theString)
  {
    const char* aPos = &GETINDEX(theString,theId*theStep);
    TInt aSize = std::min(TInt(strlen(aPos)),theStep);
    return std::string(aPos,aSize);
  }

  void SetString(TInt theId, TInt theStep, 
		 TString& theString, 
		 const std::string& theValue)
  {
    TInt aSize = std::min(TInt(theValue.size()+1),theStep);
    char* aPos = &GETINDEX(theString,theId*theStep);
    strncpy(aPos,theValue.c_str(),aSize);
  }

}

//---------------------------------------------------------------
TInt TFamilyInfo::GetAttrId(TInt theId) const {
  return GETINDEX(myAttrId,theId);
}

TInt TFamilyInfo::GetAttrVal(TInt theId) const {
  return GETINDEX(myAttrVal,theId);
}

void TFamilyInfo::SetAttrId(TInt theId,TInt theVal) {
  GETINDEX(myAttrId,theId) = theVal;
}

void TFamilyInfo::SetAttrVal(TInt theId,TInt theVal) {
  GETINDEX(myAttrVal,theId) = theVal;
}

//---------------------------------------------------------------
TInt TElemInfo::GetFamNum(TInt theId) const {
  return GETINDEX(myFamNum,theId);
}

TInt TElemInfo::GetElemNum(TInt theId) const {
  return GETINDEX(myElemNum,theId);
}

void TElemInfo::SetFamNum(TInt theId,TInt theVal) {
  GETINDEX(myFamNum,theId) = theVal;
}

//---------------------------------------------------------------
TFloat TNodeInfo::GetNodeCoord(TInt theId,TInt theComp) const {
  return GETINDEX(myCoord,myMeshInfo->myDim*theId + theComp);
}

void TNodeInfo::SetNodeCoord(TInt theId,TInt theComp,TFloat theVal) {
  GETINDEX(myCoord,myMeshInfo->myDim*theId + theComp) = theVal;
}

//---------------------------------------------------------------
TInt TCellInfo::GetConn(TInt theElemId, TInt theConnId) const {
  return GETINDEX(myConn,GetConnDim()*theElemId + theConnId);
}

void TCellInfo::SetConn(TInt theElemId, TInt theConnId, TInt theVal){
  GETINDEX(myConn,GetConnDim()*theElemId + theConnId) = theVal;
}

TConstConnSlice 
TCellInfo::GetConnSlice(TInt theElemId) const
{
  return TConstConnSlice(myConn,std::slice(GetConnDim()*theElemId,GetNbConnectivities(myTGeom),1));
}

TConnSlice 
TCellInfo::GetConnSlice(TInt theElemId)
{
  return TConnSlice(myConn,std::slice(GetConnDim()*theElemId,GetNbConnectivities(myTGeom),1));
}

//---------------------------------------------------------------
TInt TPolygoneInfo::GetNbConn(TInt theElemId) const {
  TInt i1 = GETINDEX(myIndex,theElemId);
  TInt i2 = GETINDEX(myIndex,theElemId+1);
  TInt ret = i2 - i1;
  return ret;
}

//---------------------------------------------------------------
TFloat TTimeStampVal::GetVal(EGeometrieElement theGeom, TInt theId, 
			     TInt theComp, TInt theGauss) const {
  TInt aNbComp = myTimeStampInfo->myFieldInfo->myNbComp;
  TInt aNbGauss = myTimeStampInfo->myNbGauss;
  TInt aStep = aNbComp*aNbGauss;
  TMeshValue::const_iterator anIter = myMeshValue.find(theGeom);
  if(anIter != myMeshValue.end()){
    TFloat aRet=GETINDEX(anIter->second,theId*aStep + theComp*aNbGauss + theGauss);
    return aRet;
  }
  return TFloat();
}

void TTimeStampVal::SetVal(EGeometrieElement theGeom, TInt theId, 
			   TInt theComp, TFloat theVal, TInt theGauss)
{
  TInt aNbComp = myTimeStampInfo->myFieldInfo->myNbComp;
  TInt aNbGauss = myTimeStampInfo->myNbGauss;
  TInt aStep = aNbComp*aNbGauss;
  GETINDEX(myMeshValue[theGeom],theId*aStep + theComp*aNbGauss + theGauss) = theVal;
}
