//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MEDA_Structures.hxx"
#include "MED_Structures.hxx"
#include "MED_Utilities.hxx"

using namespace std;
using namespace MEDA;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


//---------------------------------------------------------------
TNameInfo::TNameInfo(const MED::PNameInfo& theInfo): 
  myInfo(theInfo) 
{}

string TNameInfo::GetName() const{
  return myInfo->GetName();
}

void TNameInfo::SetName(const string& theValue){
  myInfo->SetName(theValue);
}


//---------------------------------------------------------------
TMeshInfo::TMeshInfo(const MED::PMeshInfo& theInfo): 
  TNameInfo(theInfo),
  myInfo(theInfo)
{}

med_int TMeshInfo::GetDim() const{
  return myInfo->myDim;
}


//---------------------------------------------------------------
TFamilyInfo::TFamilyInfo(const MED::PFamilyInfo& theInfo): 
  TNameInfo(theInfo),
  myInfo(theInfo) 
{}

med_int TFamilyInfo::GetId() const{
  return myInfo->myId;
}

void TFamilyInfo::SetId(med_int theId){
  myInfo->myId = theId;
}

med_int TFamilyInfo::GetNbGroup() const{
  return myInfo->myNbGroup;
}

string TFamilyInfo::GetGroupName(med_int theId) const{
  return myInfo->GetGroupName(theId);
}

void TFamilyInfo::SetGroupName(med_int theId, const string& theValue){
  myInfo->SetGroupName(theId,theValue);
}

med_int TFamilyInfo::GetNbAttr() const{
  return myInfo->myNbAttr;
}

string TFamilyInfo::GetAttrDesc(med_int theId) const{
  return myInfo->GetAttrDesc(theId);
}

void TFamilyInfo::SetAttrDesc(med_int theId, const string& theValue){
  myInfo->SetAttrDesc(theId,theValue);
}

med_int TFamilyInfo::GetAttrId(med_int theId) const{
  return myInfo->myAttrId[theId];
}

void TFamilyInfo::SetAttrId(med_int theId, med_int theVal){
  myInfo->myAttrId[theId] = theVal;
}

med_int TFamilyInfo::GetAttrVal(med_int theId) const{
  return myInfo->myAttrVal[theId];
}

void TFamilyInfo::SetAttrVal(med_int theId, med_int theVal){
  myInfo->myAttrVal[theId] = theVal;
}


//---------------------------------------------------------------
TElemInfo::TElemInfo(const MED::PElemInfo& theInfo): 
  myInfo(theInfo) 
{}

med_int TElemInfo::GetNbElem() const{
  return myInfo->myNbElem;
}

med_int TElemInfo::GetFamNum(med_int theId) const{
  return myInfo->myFamNum[theId];
}

void TElemInfo::SetFamNum(med_int theId, med_int theValue){
  myInfo->myFamNum[theId] = theValue;
}

med_booleen TElemInfo::IsElemNum() const{
  return myInfo->myIsElemNum;
}

med_int TElemInfo::GetElemNum(med_int theId) const{
  if(theId < 0 || myInfo->myElemNum.size() < theId)
    EXCEPTION(runtime_error,"TElemInfo::GetElemNum - myInfo->myElemNum.size() = "<<
	      myInfo->myElemNum.size()<<"; theId = "<<theId<<")");
  return myInfo->myElemNum[theId];
}

void TElemInfo::SetElemNum(med_int theId, med_int theValue){
  if(theId < 0 || myInfo->myElemNum.size() < theId)
    EXCEPTION(runtime_error,"TElemInfo::SetElemNum - myInfo->myElemNum.size() = "<<
	      myInfo->myElemNum.size()<<"; theId = "<<theId<<")");
  myInfo->myElemNum[theId] = theValue;
}

med_booleen TElemInfo::IsElemNames() const{
  return myInfo->myIsElemNames;
}

string TElemInfo::GetElemName(med_int theId) const{
  return myInfo->GetElemName(theId);
}

void TElemInfo::SetElemName(med_int theId, const string& theValue){
  myInfo->SetElemName(theId,theValue);
}


//---------------------------------------------------------------
TNodeInfo::TNodeInfo(const MED::PNodeInfo& theInfo): 
  TElemInfo(theInfo),
  myInfo(theInfo) 
{}

med_repere TNodeInfo::GetSystem() const{
  return myInfo->mySystem;
}

void TNodeInfo::GetSystem(med_repere theSystem){
  myInfo->mySystem = theSystem;
}

med_float TNodeInfo::GetNodeCoord(med_int theId, med_int theComp) const{
  return myInfo->myCoord[myInfo->myMeshInfo->myDim*theId + theComp];
}

void TNodeInfo::SetNodeCoord(med_int theId, med_int theComp, med_float theVal){
  myInfo->myCoord[myInfo->myMeshInfo->myDim*theId + theComp] = theVal;
}

string TNodeInfo::GetCoordName(med_int theId) const{
  return myInfo->GetCoordName(theId);
}

void TNodeInfo::SetCoordName(med_int theId, const string& theValue){
  myInfo->SetCoordName(theId,theValue);
}

string TNodeInfo::GetCoordUnit(med_int theId) const{
  return myInfo->GetCoordUnit(theId);
}

void TNodeInfo::SetCoordUnit(med_int theId, const string& theValue){
  myInfo->SetCoordUnit(theId,theValue);
}


//---------------------------------------------------------------
TCellInfo::TCellInfo(const MED::PCellInfo& theInfo): 
  TElemInfo(theInfo),
  myInfo(theInfo) 
{}

med_entite_maillage TCellInfo::GetEntity() const{
  return myInfo->myTEntity;
}

med_geometrie_element TCellInfo::GetGeom() const{
  return myInfo->myTGeom;
}

med_connectivite TCellInfo::GetConn() const{
  return myInfo->myTConn;
}

med_int TCellInfo::GetConnDim() const{
  return myInfo->myConnDim;
}

med_int TCellInfo::GetConn(med_int theElemId, med_int theConnId) const{
  med_int anId = myInfo->myConnDim*theElemId + theConnId;
  if(anId < 0 || myInfo->myConn.size() < anId)
    EXCEPTION(runtime_error,"TElemInfo::GetConn - myInfo->myConn.size() = "<<
	      myInfo->myConn.size()<<"; anId = "<<anId<<")");
  return myInfo->myConn[anId];
}
 
void TCellInfo::SetConn(med_int theElemId, med_int theConnId, med_int theVal){
  med_int anId = myInfo->myConnDim*theElemId + theConnId;
  if(anId < 0 || myInfo->myConn.size() < anId)
    EXCEPTION(runtime_error,"TElemInfo::SetConn - myInfo->myConn.size() = "<<
	      myInfo->myConn.size()<<"; anId = "<<anId<<")");
  myInfo->myConn[myInfo->myConnDim*theElemId + theConnId] = theVal;
}


//---------------------------------------------------------------
TFieldInfo::TFieldInfo(const MED::PFieldInfo& theInfo): 
  TNameInfo(theInfo),
  myInfo(theInfo) 
{}

med_type_champ TFieldInfo::GetType() const{
  return myInfo->myType;
}

med_int TFieldInfo::GetNbComp() const{
  return myInfo->myNbComp;
}

string TFieldInfo::GetCompName(med_int theId) const{
  return myInfo->GetCompName(theId);
}

void TFieldInfo::SetCompName(med_int theId, const string& theValue){
  myInfo->SetCompName(theId,theValue);
}

string TFieldInfo::GetUnitName(med_int theId) const{
  return myInfo->GetUnitName(theId);
}

void TFieldInfo::SetUnitName(med_int theId, const string& theValue){
  myInfo->SetUnitName(theId,theValue);
}


//---------------------------------------------------------------
TTimeStampInfo::TTimeStampInfo(const MED::PTimeStampInfo& theInfo): 
  myInfo(theInfo) 
{}

med_entite_maillage TTimeStampInfo::GetEntity() const{
  return myInfo->myEntity;
}

const MED::TGeom& TTimeStampInfo::GetGeom() const{
  return myInfo->myGeom;
}

med_int TTimeStampInfo::GetNbGauss() const{
  return myInfo->myNbGauss;
}

med_int TTimeStampInfo::GetNumDt() const{
  return myInfo->myNumDt;
}

med_int TTimeStampInfo::GetNumOrd() const{
  return myInfo->myNumOrd;
}

med_float TTimeStampInfo::GetDt() const{
  return myInfo->myDt;
}

string TTimeStampInfo::GetUnitDt() const{
  return myInfo->GetUnitDt();
}

void TTimeStampInfo::SetUnitDt(const string& theValue){
  myInfo->SetUnitDt(theValue);
}


//---------------------------------------------------------------
TTimeStampVal::TTimeStampVal(const MED::PTimeStampVal& theInfo): 
  myInfo(theInfo) 
{}

med_float TTimeStampVal::GetVal(med_geometrie_element theGeom, 
				med_int theId, 
				med_int theComp, 
				med_int theGauss) const
{
  med_int aNbComp = myInfo->myTimeStampInfo->myFieldInfo->myNbComp;
  med_int aNbGauss = myInfo->myTimeStampInfo->myNbGauss;
  med_int aStep = aNbComp*aNbGauss;
  return myInfo->myMeshValue[theGeom][theId*aStep + theComp*aNbGauss + theGauss];
}

void TTimeStampVal::SetVal(med_geometrie_element theGeom,
			   med_int theId, 
			   med_int theComp, 
			   med_float theVal, 
			   med_int theGauss)
{
  med_int aNbComp = myInfo->myTimeStampInfo->myFieldInfo->myNbComp;
  med_int aNbGauss = myInfo->myTimeStampInfo->myNbGauss;
  med_int aStep = aNbComp*aNbGauss;
  myInfo->myMeshValue[theGeom][theId*aStep + theComp*aNbGauss + theGauss] = theVal;
}

string TTimeStampVal::GetPflName() const{
  return myInfo->GetPflName();
}

void TTimeStampVal::SetPflName(const string& theValue){
  myInfo->SetPflName(theValue);
}

