//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MEDA_Wrapper.hxx"
#include "MED_Wrapper.hxx"
#include "MED_Utilities.hxx"

using namespace std;
using namespace MEDA;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


//---------------------------------------------------------------
TWrapper::TWrapper(const string& theFileName):
  myWrapper(new MED::TWrapper(theFileName))
{}


//---------------------------------------------------------------
PMeshInfo TWrapper::CrMeshInfo(med_int theDim,
			       const string& theValue)
{
  MED::PMeshInfo anInfo(new MED::TMeshInfo(theDim,theValue));
  return PMeshInfo(new TMeshInfo(anInfo));
}

PMeshInfo TWrapper::CrMeshInfo(const PMeshInfo& theInfo){
  MED::PMeshInfo anInfo(new MED::TMeshInfo(*theInfo->myInfo));
  return PMeshInfo(new TMeshInfo(anInfo));
}

med_int TWrapper::GetNbMeshes(){
  return myWrapper->GetNbMeshes();
}

PMeshInfo TWrapper::GetMeshInfo(med_int theId){
  PMeshInfo anInfo = CrMeshInfo();
  myWrapper->GetMeshInfo(theId+1,*anInfo->myInfo);
  if(MYDEBUG){
    MESSAGE("GetMeshInfo(...)");
    MED::PrefixPrinter aPrefixPrinter;
    BEGMSG("aName = '"<<anInfo->GetName()<<"'; aDim = "<<anInfo->GetDim()<<"\n");
    ADDMSG("\n");
  }
  return anInfo;
}

void TWrapper::SetMeshInfo(const PMeshInfo& theInfo){
  myWrapper->SetMeshInfo(*theInfo->myInfo);
}


//---------------------------------------------------------------
PFamilyInfo TWrapper::CrFamilyInfo(const PMeshInfo& theMeshInfo, 
				   med_int theNbGroup,
				   med_int theNbAttr,
				   med_int theId,
				   const string& theValue)
{
  const MED::PMeshInfo& aMeshInfo = theMeshInfo->myInfo;
  MED::PFamilyInfo anInfo(new MED::TFamilyInfo(aMeshInfo,
					       theNbGroup,
					       theNbAttr,
					       theId,
					       theValue));
  return PFamilyInfo(new TFamilyInfo(anInfo));
}

PFamilyInfo TWrapper::CrFamilyInfo(const PMeshInfo& theMeshInfo,
				   const string& theValue,
				   med_int theId,
				   const MED::TStringSet& theGroupNames, 
				   const MED::TStringVector& theAttrDescs, 
				   const MED::TIntVector& theAttrIds, 
				   const MED::TIntVector& theAttrVals)
{
  const MED::PMeshInfo& aMeshInfo = theMeshInfo->myInfo;
  MED::PFamilyInfo anInfo(new MED::TFamilyInfo(aMeshInfo,
					       theValue,
					       theId,
					       theGroupNames,
					       theAttrDescs,
					       theAttrIds,
					       theAttrVals));
  return PFamilyInfo(new TFamilyInfo(anInfo));
}

PFamilyInfo TWrapper::CrFamilyInfo(const PMeshInfo& theMeshInfo,
				   const PFamilyInfo& theInfo)
{
  MED::PFamilyInfo anInfo(new MED::TFamilyInfo(*theInfo->myInfo));
  anInfo->myMeshInfo = theMeshInfo->myInfo;
  return PFamilyInfo(new TFamilyInfo(anInfo));
}

med_int TWrapper::GetNbFamilies(const PMeshInfo& theMeshInfo){
  return myWrapper->GetNbFamilies(*theMeshInfo->myInfo);
}

PFamilyInfo TWrapper::GetFamilyInfo(const PMeshInfo& theMeshInfo, med_int theId){
  med_int aNbAttr = myWrapper->GetNbFamAttr(theId+1,*theMeshInfo->myInfo);
  med_int aNbGroup = myWrapper->GetNbFamGroup(theId+1,*theMeshInfo->myInfo);
  PFamilyInfo anInfo = CrFamilyInfo(theMeshInfo,aNbGroup,aNbAttr);
  myWrapper->GetFamilyInfo(theId+1,*anInfo->myInfo);
  if(MYDEBUG){
    MESSAGE("GetFamilyInfo(...)");
    MED::PrefixPrinter aPrefixPrinter;
    BEGMSG("aName = '"<<anInfo->GetName()<<"'; anId = "<<anInfo->GetId()<<"\n");
    ADDMSG("\n");
  }
  return anInfo;
}

void TWrapper::SetFamilyInfo(const PFamilyInfo& theInfo){
  myWrapper->SetFamilyInfo(*theInfo->myInfo);
}


//---------------------------------------------------------------
PNodeInfo TWrapper::CrNodeInfo(const PMeshInfo& theMeshInfo, 
			       med_int theNbElem,
			       med_repere theSystem, 
			       med_booleen theIsElemNum,
			       med_booleen theIsElemNames)
{
  const MED::PMeshInfo& aMeshInfo = theMeshInfo->myInfo;
  MED::PNodeInfo anInfo(new MED::TNodeInfo(aMeshInfo,
					   theNbElem,
					   theSystem,
					   theIsElemNum,
					   theIsElemNames));
  return PNodeInfo(new TNodeInfo(anInfo));
}

PNodeInfo TWrapper::CrNodeInfo(const PMeshInfo& theMeshInfo, 
			       med_repere theSystem, 
			       const MED::TFloatVector& theNodeCoords,
			       const MED::TStringVector& theCoordNames,
			       const MED::TStringVector& theCoordUnits,
			       const MED::TIntVector& theFamilyNums,
			       const MED::TIntVector& theElemNums,
			       const MED::TStringVector& theElemNames)
{
  const MED::PMeshInfo& aMeshInfo = theMeshInfo->myInfo;
  MED::PNodeInfo anInfo(new MED::TNodeInfo(aMeshInfo,
					   theSystem,
					   theNodeCoords,
					   theCoordNames,
					   theCoordUnits,
					   theFamilyNums,
					   theElemNums,
					   theElemNames));
  return PNodeInfo(new TNodeInfo(anInfo));
}

PNodeInfo TWrapper::CrNodeInfo(const PMeshInfo& theMeshInfo,
			       const PNodeInfo& theInfo)
{
  MED::PNodeInfo anInfo(new MED::TNodeInfo(*theInfo->myInfo));
  anInfo->myMeshInfo = theMeshInfo->myInfo;
  return PNodeInfo(new TNodeInfo(anInfo));
}

PNodeInfo TWrapper::GetNodeInfo(const PMeshInfo& theMeshInfo){
  med_int aNbElems = myWrapper->GetNbNodes(*theMeshInfo->myInfo);
  PNodeInfo anInfo = CrNodeInfo(theMeshInfo,aNbElems);
  myWrapper->GetNodeInfo(*anInfo->myInfo);
  return anInfo;
}

void TWrapper::SetNodeInfo(const PNodeInfo& theInfo){
  myWrapper->SetNodeInfo(*theInfo->myInfo);
}


//---------------------------------------------------------------
MED::TEntityInfo TWrapper::GetEntityInfo(const PMeshInfo& theMeshInfo){
  return myWrapper->GetEntityInfo(*theMeshInfo->myInfo);
}

PCellInfo TWrapper::CrCellInfo(const PMeshInfo& theMeshInfo, 
			       med_int theNbElem,
			       med_entite_maillage theEntity, 
			       med_geometrie_element theGeom,
			       med_connectivite theConn,
			       med_booleen theIsElemNum,
			       med_booleen theIsElemNames)
{
  MED::PCellInfo anInfo(new MED::TCellInfo(theMeshInfo->myInfo,
					   theNbElem,
					   theEntity,
					   theGeom,
					   theConn,
					   theIsElemNum,
					   theIsElemNames));
  return PCellInfo(new TCellInfo(anInfo));
}

PCellInfo TWrapper::CrCellInfo(const PMeshInfo& theMeshInfo, 
			       med_entite_maillage theEntity, 
			       med_geometrie_element theGeom,
			       med_connectivite theConn,
			       const MED::TIntVector& theConnectivities,
			       const MED::TIntVector& theFamilyNums,
			       const MED::TIntVector& theElemNums,
			       const MED::TStringVector& theElemNames)
{
  MED::PCellInfo anInfo(new MED::TCellInfo(theMeshInfo->myInfo,
					   theEntity,
					   theGeom,
					   theConn,
					   theConnectivities,
					   theFamilyNums,
					   theElemNums,
					   theElemNames));
  return PCellInfo(new TCellInfo(anInfo));
}

PCellInfo TWrapper::CrCellInfo(const PMeshInfo& theMeshInfo,
			       const PCellInfo& theInfo)
{
  MED::PCellInfo anInfo(new MED::TCellInfo(*theInfo->myInfo));
  anInfo->myMeshInfo = theMeshInfo->myInfo;
  return PCellInfo(new TCellInfo(anInfo));
}

PCellInfo TWrapper::GetCellInfo(const PMeshInfo& theMeshInfo, 
				med_entite_maillage theEntity, 
				med_geometrie_element theGeom, 
				med_connectivite theConn)
{
  med_int aNbElem = myWrapper->GetNbCells(*theMeshInfo->myInfo,theEntity,theGeom,theConn);
  PCellInfo anInfo = CrCellInfo(theMeshInfo,aNbElem,theEntity,theGeom,theConn);
  myWrapper->GetCellInfo(*anInfo->myInfo);
  return anInfo;
}

void TWrapper::SetCellInfo(const PCellInfo& theInfo){
  myWrapper->SetCellInfo(*theInfo->myInfo);
}


//---------------------------------------------------------------
PFieldInfo TWrapper::CrFieldInfo(const PMeshInfo& theMeshInfo, 
				 med_int theNbComp,
				 med_type_champ theType,
				 const string& theValue)
{
  MED::PFieldInfo anInfo(new MED::TFieldInfo(theMeshInfo->myInfo,
					     theNbComp,
					     theType,
					     theValue));
  return PFieldInfo(new TFieldInfo(anInfo));
}

PFieldInfo TWrapper::CrFieldInfo(const PMeshInfo& theMeshInfo,
				 const PFieldInfo& theInfo)
{
  MED::PFieldInfo anInfo(new MED::TFieldInfo(*theInfo->myInfo));
  anInfo->myMeshInfo = theMeshInfo->myInfo;
  return PFieldInfo(new TFieldInfo(anInfo));
}

med_int TWrapper::GetNbFields(){
  return myWrapper->GetNbFields();
}

PFieldInfo TWrapper::GetFieldInfo(const PMeshInfo& theMeshInfo, med_int theId){
  med_int aNbComp = myWrapper->GetNbComp(theId+1);
  PFieldInfo anInfo = CrFieldInfo(theMeshInfo,aNbComp);
  myWrapper->GetFieldInfo(theId+1,*anInfo->myInfo);
  return anInfo;
}

void TWrapper::SetFieldInfo(const PFieldInfo& theInfo){
  myWrapper->SetFieldInfo(*theInfo->myInfo);
}


//---------------------------------------------------------------
PTimeStampInfo TWrapper::CrTimeStampInfo(const PFieldInfo& theFieldInfo, 
					 med_entite_maillage theEntity,
					 const MED::TGeom& theGeom)
{
  MED::PTimeStampInfo anInfo(new MED::TTimeStampInfo(theFieldInfo->myInfo,
						     theEntity,
						     theGeom));
  return PTimeStampInfo(new TTimeStampInfo(anInfo));
}

PTimeStampInfo TWrapper::CrTimeStampInfo(const PFieldInfo& theFieldInfo,
					 const PTimeStampInfo& theInfo)
{
  MED::PTimeStampInfo anInfo(new MED::TTimeStampInfo(*theInfo->myInfo));
  anInfo->myFieldInfo = theFieldInfo->myInfo;
  return PTimeStampInfo(new TTimeStampInfo(anInfo));
}

med_int TWrapper::GetNbTimeStamps(const PFieldInfo& theFieldInfo, 
				  const MED::TEntityInfo& theEntityInfo,
				  med_entite_maillage& theEntity,
				  MED::TGeom& theGeom)
{
  return myWrapper->GetNbTimeStamps(*theFieldInfo->myInfo,
				    theEntityInfo,
				    theEntity,
				    theGeom);
}

PTimeStampInfo TWrapper::GetTimeStampInfo(const PFieldInfo& theFieldInfo,
					  med_int theId,
					  med_entite_maillage theEntity,
					  const MED::TGeom& theGeom)
{
  PTimeStampInfo anInfo = CrTimeStampInfo(theFieldInfo,theEntity,theGeom);
  myWrapper->GetTimeStampInfo(theId+1,*anInfo->myInfo);
  return anInfo;
}


//---------------------------------------------------------------
PTimeStampVal TWrapper::CrTimeStampVal(const PTimeStampInfo& theTimeStampInfo)
{
  MED::PTimeStampVal anInfo(new MED::TTimeStampVal(theTimeStampInfo->myInfo));
  return PTimeStampVal(new TTimeStampVal(anInfo));
}

PTimeStampVal TWrapper::CrTimeStampVal(const PTimeStampInfo& theTimeStampInfo,
				       const PTimeStampVal& theInfo)
{
  MED::PTimeStampVal anInfo(new MED::TTimeStampVal(*theInfo->myInfo));
  anInfo->myTimeStampInfo = theTimeStampInfo->myInfo;
  return PTimeStampVal(new TTimeStampVal(anInfo));
}

PTimeStampVal TWrapper::GetTimeStampVal(const PTimeStampInfo& theTimeStampInfo)
{
  PTimeStampVal anInfo = CrTimeStampVal(theTimeStampInfo);
  myWrapper->GetTimeStampVal(*anInfo->myInfo);
  return anInfo;
}

void TWrapper::SetTimeStamp(const PTimeStampVal& theTimeStampVal){
  myWrapper->SetTimeStamp(*theTimeStampVal->myInfo);
}
