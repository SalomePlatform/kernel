//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MED_Structures.hxx"
#include "MED_Utilities.hxx"
#include <string>
using namespace std;
using namespace MED;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

//---------------------------------------------------------------
string MED::GetString(med_int theId, med_int theStep,
		      const TString& theString)
{
  const char* aPos = &theString[theId*theStep];
  med_int aSize = min(med_int(strlen(aPos)),theStep);
  return string(aPos,aSize);
}

void MED::SetString(med_int theId, med_int theStep,
		    TString& theString, const string& theValue)
{
  med_int aSize = min(med_int(theValue.size()+1),theStep);
  if(aSize > theStep)
    EXCEPTION(runtime_error,"SetString(...): aSize =="<<aSize<<"of '"<<theValue<<"' > theStep");
  strncpy(&theString[theId*theStep],theValue.c_str(),aSize);
}

med_int MED::GetNbConn(med_entite_maillage theMedEntity, 
		       med_geometrie_element theMedType,
		       med_int theMeshDim)
{
  med_int anElemDim = theMedType / 100, nsup = 0;
  if(theMedEntity == MED_MAILLE && anElemDim < theMeshDim) nsup = 1;
  return nsup + theMedType % 100;
}


//---------------------------------------------------------------
TNameInfo::TNameInfo(const string& theValue):
  myName('\0',MED_TAILLE_NOM+1) 
{
  if(theValue != "") SetName(theValue);
}

string TNameInfo::GetName() const { 
  return GetString(0,MED_TAILLE_NOM,myName);
}

void TNameInfo::SetName(const string& theValue){
  SetString(0,MED_TAILLE_NOM,myName,theValue);
}


//---------------------------------------------------------------
TMeshInfo::TMeshInfo(med_int theDim,
		     const string& theValue):
  TNameInfo(theValue),
  myDim(theDim)
{}

//---------------------------------------------------------------
TFamilyInfo::TFamilyInfo(const PMeshInfo& theMeshInfo, 
			 med_int theNbGroup, 
			 med_int theNbAttr,
			 med_int theId,
			 const string& theValue):
  TNameInfo(theValue),
  myMeshInfo(theMeshInfo),
  myId(theId),
  myNbGroup(theNbGroup),
  myGroupNames('\0',theNbGroup*MED_TAILLE_LNOM+1),
  myNbAttr(theNbAttr), 
  myAttrId(theNbAttr), 
  myAttrVal(theNbAttr),
  myAttrDesc('\0',theNbAttr*MED_TAILLE_DESC+1)
{}  

TFamilyInfo::TFamilyInfo(const PMeshInfo& theMeshInfo,
			 const string& theValue,
			 med_int theId,
			 const TStringSet& theGroupNames, 
			 const TStringVector& theAttrDescs, 
			 const TIntVector& theAttrIds, 
			 const TIntVector& theAttrVals):
  TNameInfo(theValue),
  myMeshInfo(theMeshInfo),
  myId(theId),
  myNbGroup(theGroupNames.size()),
  myGroupNames('\0',theGroupNames.size()*MED_TAILLE_LNOM+1),
  myNbAttr(theAttrDescs.size()), 
  myAttrId(theAttrDescs.size()), 
  myAttrVal(theAttrDescs.size()),
  myAttrDesc('\0',theAttrDescs.size()*MED_TAILLE_DESC+1)
{
  if(myNbGroup){
    TStringSet::const_iterator anIter = theGroupNames.begin();
    for(med_int anId = 0; anIter != theGroupNames.end(); anIter++, anId++){
      const string& aVal = *anIter;
      SetGroupName(anId,aVal);
    }
  }
  if(myNbAttr){
    for(med_int anId = 0, anEnd = theAttrDescs.size(); anId < anEnd; anId++){
      const string& aVal = theAttrDescs[anId];
      SetAttrDesc(anId,aVal);
      myAttrId[anId] = theAttrIds[anId];
      myAttrVal[anId] = theAttrVals[anId];
    }
  }
}  

string TFamilyInfo::GetGroupName(med_int theId) const { 
  return GetString(theId,MED_TAILLE_LNOM,myGroupNames);
}

void TFamilyInfo::SetGroupName(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_LNOM,myGroupNames,theValue);
}

string TFamilyInfo::GetAttrDesc(med_int theId) const { 
  return GetString(theId,MED_TAILLE_DESC,myAttrDesc);
}

void TFamilyInfo::SetAttrDesc(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_DESC,myAttrDesc,theValue);
}


//---------------------------------------------------------------
TElemInfo::TElemInfo(const PMeshInfo& theMeshInfo, 
		     med_int theNbElem,
		     med_booleen theIsElemNum,
		     med_booleen theIsElemNames):
  myMeshInfo(theMeshInfo), 
  myNbElem(theNbElem),
  myFamNum(theNbElem),
  myIsElemNum(theIsElemNum), 
  myElemNum(theIsElemNum == MED_FAUX? 0: theNbElem),
  myIsElemNames(theIsElemNames),
  myElemNames('\0',(theIsElemNames == MED_FAUX? 0: theNbElem)*MED_TAILLE_PNOM+1)
{}

TElemInfo::TElemInfo(const PMeshInfo& theMeshInfo, 
		     const TIntVector& theFamilyNums,
		     const TIntVector& theElemNums,
		     const TStringVector& theElemNames):
  myMeshInfo(theMeshInfo), 
  myNbElem(theFamilyNums.size()),
  myFamNum(theFamilyNums.size()),
  myIsElemNum(theElemNums.size()? MED_VRAI: MED_FAUX), 
  myElemNum(theElemNums.size()),
  myIsElemNames(theElemNames.size()? MED_VRAI: MED_FAUX),
  myElemNames('\0',theElemNames.size()*MED_TAILLE_PNOM+1)
{
  if(myNbElem){
    for(med_int anId = 0; anId < myNbElem; anId++){
      myFamNum[anId] = theFamilyNums[anId];
    }
    if(myIsElemNum == MED_VRAI){
      for(med_int anId = 0; anId < myNbElem; anId++){
	myElemNum[anId] = theElemNums[anId];
      }
    }
    if(myIsElemNames == MED_VRAI){
      for(med_int anId = 0; anId < myNbElem; anId++){
	const string& aVal = theElemNames[anId];
	SetElemName(anId,aVal);
      }
    }
  }
}

string TElemInfo::GetElemName(med_int theId) const { 
  return GetString(theId,MED_TAILLE_PNOM,myElemNames);
}

void TElemInfo::SetElemName(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_PNOM,myElemNames,theValue);
}


//---------------------------------------------------------------
TNodeInfo::TNodeInfo(const PMeshInfo& theMeshInfo, 
		     med_int theNbElem,
		     med_repere theSystem, 
		     med_booleen theIsElemNum,
		     med_booleen theIsElemNames):
  TElemInfo(theMeshInfo,theNbElem,theIsElemNum,theIsElemNames),
  myCoord(theNbElem*theMeshInfo->myDim), 
  mySystem(theSystem), 
  myCoordNames('\0',theMeshInfo->myDim*MED_TAILLE_PNOM+1),
  myCoordUnits('\0',theMeshInfo->myDim*MED_TAILLE_PNOM+1)
{}

TNodeInfo::TNodeInfo(const PMeshInfo& theMeshInfo, 
		     med_repere theSystem, 
		     const TFloatVector& theNodeCoords,
		     const TStringVector& theCoordNames,
		     const TStringVector& theCoordUnits,
		     const TIntVector& theFamilyNums,
		     const TIntVector& theElemNums,
		     const TStringVector& theElemNames):
  TElemInfo(theMeshInfo,theFamilyNums,theElemNums,theElemNames),
  myCoord(theNodeCoords.size()), 
  mySystem(theSystem), 
  myCoordNames('\0',theMeshInfo->myDim*MED_TAILLE_PNOM+1),
  myCoordUnits('\0',theMeshInfo->myDim*MED_TAILLE_PNOM+1)
{
  if(myCoord.size() == myNbElem*theMeshInfo->myDim){
    for(med_int anId = 0, anEnd = myCoord.size(); anId < anEnd; anId++){
      myCoord[anId] = theNodeCoords[anId];
    }
  }else 
    EXCEPTION(runtime_error,"myCoord.size() != myNbElem*theMeshInfo->myDim");

  for(med_int anId = 0; anId < theMeshInfo->myDim; anId++){
#if defined __GNUC_2__
    const string& aVal = theCoordNames[anId];
#else
    const string& aVal = theCoordNames.at(anId);
#endif
 
    SetCoordName(anId,aVal);
  }

  for(med_int anId = 0; anId < theMeshInfo->myDim; anId++){
#if defined __GNUC_2__
    const string& aVal = theCoordUnits[anId];
#else
    const string& aVal = theCoordUnits.at(anId);
#endif
    SetCoordUnit(anId,aVal);
  }
}

string TNodeInfo::GetCoordName(med_int theId) const { 
  return GetString(theId,MED_TAILLE_PNOM,myCoordNames);
}

void TNodeInfo::SetCoordName(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_PNOM,myCoordNames,theValue);
}

string TNodeInfo::GetCoordUnit(med_int theId) const { 
  return GetString(theId,MED_TAILLE_PNOM,myCoordUnits);
}

void TNodeInfo::SetCoordUnit(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_PNOM,myCoordUnits,theValue);
}


//---------------------------------------------------------------
TCellInfo::TCellInfo(const PMeshInfo& theMeshInfo, 
		     med_int theNbElem,
		     med_entite_maillage theTEntity, 
		     med_geometrie_element theTGeom,
		     med_connectivite theTConn,
		     med_booleen theIsElemNum,
		     med_booleen theIsElemNames):
  TElemInfo(theMeshInfo,theNbElem,theIsElemNum,theIsElemNames),
  myTEntity(theTEntity), 
  myTGeom(theTGeom), 
  myTConn(theTConn),
  myConnDim(GetNbConn(myTEntity,myTGeom,theMeshInfo->myDim)),
  myConn(theNbElem*myConnDim)
{}

TCellInfo::TCellInfo(const PMeshInfo& theMeshInfo, 
		     med_entite_maillage theTEntity, 
		     med_geometrie_element theTGeom,
		     med_connectivite theTConn,
		     const TIntVector& theConnectivities,
		     const TIntVector& theFamilyNums,
		     const TIntVector& theElemNums,
		     const TStringVector& theElemNames):
  TElemInfo(theMeshInfo,theFamilyNums,theElemNums,theElemNames),
  myTEntity(theTEntity), 
  myTGeom(theTGeom), 
  myTConn(theTConn),
  myConnDim(GetNbConn(myTEntity,myTGeom,theMeshInfo->myDim)),
  myConn(theConnectivities.size())
{
  if(myConn.size() == myConnDim*theFamilyNums.size()){
    for(med_int anId = 0, anEnd = myConn.size(); anId < anEnd; anId++){
      myConn[anId] = theConnectivities[anId];
    }
  }else 
    EXCEPTION(runtime_error,"myConn.size() == myConnDim*theFamilyNums.size()");
}

//---------------------------------------------------------------
TFieldInfo::TFieldInfo(const PMeshInfo& theMeshInfo, 
		       med_int theNbComp,
		       med_type_champ theType,
		       const string& theValue):
  TNameInfo(theValue),
  myMeshInfo(theMeshInfo), 
  myNbComp(theNbComp),
  myType(theType),
  myCompNames('\0',theNbComp*MED_TAILLE_PNOM + 1),
  myUnitNames('\0',theNbComp*MED_TAILLE_PNOM + 1)
{}

string TFieldInfo::GetCompName(med_int theId) const { 
  return GetString(theId,MED_TAILLE_PNOM,myCompNames);
}

void TFieldInfo::SetCompName(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_PNOM,myCompNames,theValue);
}

string TFieldInfo::GetUnitName(med_int theId) const { 
  return GetString(theId,MED_TAILLE_PNOM,myUnitNames);
}

void TFieldInfo::SetUnitName(med_int theId, const string& theValue){
  SetString(theId,MED_TAILLE_PNOM,myUnitNames,theValue);
}


//---------------------------------------------------------------
TTimeStampInfo::TTimeStampInfo(const PFieldInfo& theFieldInfo, 
			       med_entite_maillage theEntity,
			       const TGeom& theGeom):
  myFieldInfo(theFieldInfo), 
  myEntity(theEntity), 
  myGeom(theGeom), 
  myUnitDt('\0',MED_TAILLE_PNOM+1)
{}

string TTimeStampInfo::GetUnitDt() const { 
  return GetString(0,MED_TAILLE_PNOM,myUnitDt);
}

void TTimeStampInfo::SetUnitDt(const string& theValue){
  SetString(0,MED_TAILLE_PNOM,myUnitDt,theValue);
}


//---------------------------------------------------------------
TTimeStampVal::TTimeStampVal(const PTimeStampInfo& theTimeStampInfo):
  myTimeStampInfo(theTimeStampInfo),
  myPflName('\0',MED_TAILLE_NOM+1)
{
  med_int aNbComp = theTimeStampInfo->myFieldInfo->myNbComp;
  med_int aNbGauss = theTimeStampInfo->myNbGauss;
  const TGeom& aTGeom = theTimeStampInfo->myGeom;
  TGeom::const_iterator anIter = aTGeom.begin();
  for(; anIter != aTGeom.end(); anIter++){
    const med_geometrie_element& aGeom = anIter->first;
    med_int aNb = anIter->second*aNbComp*aNbGauss;
    myMeshValue[aGeom].resize(aNb);
  }
}

string TTimeStampVal::GetPflName() const { 
  return GetString(0,MED_TAILLE_NOM,myPflName);
}

void TTimeStampVal::SetPflName(const string& theValue){
  SetString(0,MED_TAILLE_NOM,myPflName,theValue);
}


