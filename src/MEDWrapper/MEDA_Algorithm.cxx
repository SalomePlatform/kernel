//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MEDA_Algorithm.hxx"
#include "MED_Utilities.hxx"
 
using namespace std;
using namespace MEDA;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


//---------------------------------------------------------------
TCellGroup MEDA::GetCellsByEntity(TWrapper& theWrapper, 
				  const PMeshInfo& theMeshInfo,
				  const MED::TEntityInfo theEntityInfo)
{
  if(MYDEBUG) MESSAGE("GetCellsByEntity(...)");
  TCellGroup aGroup;
  MED::TEntityInfo::const_iterator anIter = theEntityInfo.begin();
  for(; anIter != theEntityInfo.end(); anIter++){
    const med_entite_maillage& anEntity = anIter->first;
    if(anEntity == MED_NOEUD) continue;
    const MED::TGeom& aGeom = anIter->second;
    TCellSet& aCellSet = aGroup[anEntity];
    MED::TGeom::const_iterator anGeomIter = aGeom.begin();
    for(; anGeomIter != aGeom.end(); anGeomIter++){
      const med_geometrie_element& aGeo = anGeomIter->first;
      PCellInfo aCellInfo = theWrapper.GetCellInfo(theMeshInfo,anEntity,aGeo);
      aCellSet.insert(aCellInfo);
      if(MYDEBUG){
	med_int aNbElem = aCellInfo->GetNbElem();
	MED::PrefixPrinter aPrefixPrinter;
	BEGMSG("aGeo = "<<aGeo<<"; aNbElem = "<<aNbElem<<": ");
	for(int iElem = 0; iElem < aNbElem; iElem++){
	  int iConnEnd = aCellInfo->GetConnDim();
	  for(int iConn = 0; iConn < iConnEnd; iConn++){
	    ADDMSG(aCellInfo->GetConn(iElem,iConn)<<",");
	  }
	  ADDMSG(" ");
	}
	ADDMSG("\n");
      }
    }
  }
  if(MYDEBUG) ADDMSG("\n");
  return aGroup;
}


//---------------------------------------------------------------
TFamilyGroup 
MEDA::GetFamilies(TWrapper& theWrapper,
		  const PMeshInfo& theMeshInfo)
{
  if(MYDEBUG) MESSAGE("GetFamilies(...)");
  TFamilyGroup aGroup;
  int aNbFam = theWrapper.GetNbFamilies(theMeshInfo);
  MED::PrefixPrinter aPrefixPrinter;
  if(MYDEBUG) BEGMSG("GetNbFamilies() = "<<aNbFam<<"\n");
  for(int iFam = 0; iFam < aNbFam; iFam++){
    PFamilyInfo aFamilyInfo = theWrapper.GetFamilyInfo(theMeshInfo,iFam);
    aGroup.insert(aFamilyInfo);
    if(MYDEBUG){
      MED::PrefixPrinter aPrefixPrinter;
      string aName = aFamilyInfo->GetName();
      med_int aNbAttr = aFamilyInfo->GetNbAttr();
      med_int aNbGroup = aFamilyInfo->GetNbGroup();
      BEGMSG("aFamilyName = '"<<aName<<"'; andId = "<<aFamilyInfo->GetId()<<
	     "; aNbAttr = "<<aNbAttr<<"; aNbGroup = "<<aNbGroup<<"\n");
      for(int iGroup = 0; iGroup < aNbGroup; iGroup++){
	aName = aFamilyInfo->GetGroupName(iGroup);
	MED::PrefixPrinter aPrefixPrinter;
	BEGMSG("aGroupName = '"<<aName<<"'\n");
      }
    }
  }
  if(MYDEBUG) ADDMSG("\n");
  return aGroup;
}


TGroupInfo 
MEDA::GetFamiliesByGroup(const TFamilyGroup& theGroupInfo)
{
  if(MYDEBUG) MESSAGE("GetFamiliesByGroup(...)");
  TGroupInfo aGroup;
  TFamilyGroup::const_iterator anIter = theGroupInfo.begin();
  for(; anIter != theGroupInfo.end(); anIter++){
    const PFamilyInfo& aFamilyInfo = *anIter;
    int aNbGroup = aFamilyInfo->GetNbGroup();
    for(int iGroup = 0; iGroup < aNbGroup; iGroup++){
      aGroup[aFamilyInfo->GetGroupName(iGroup)].insert(aFamilyInfo);
    } 
  }
  if(MYDEBUG){
    TGroupInfo::const_iterator anIter = aGroup.begin();
    for(; anIter != aGroup.end(); anIter++){
      string aName = anIter->first;
      MED::PrefixPrinter aPrefixPrinter;
      BEGMSG("aGroupName = '"<<aName<<"'\n");
      const TFamilyGroup& aFamilyGroup = anIter->second;
      TFamilyGroup::const_iterator anFamIter = aFamilyGroup.begin();
      for(; anFamIter != aFamilyGroup.end(); anFamIter++){
	const PFamilyInfo& aFamilyInfo = *anFamIter;
	MED::PrefixPrinter aPrefixPrinter;
	BEGMSG("aFamilyName = '"<<aFamilyInfo->GetName()<<"'\n");
      }
    }
    ADDMSG("\n");
  }
  return aGroup;
}


//---------------------------------------------------------------
TTimeStampGroup 
MEDA::GetFieldsByEntity(TWrapper& theWrapper, 
			const PMeshInfo& theMeshInfo,
			const MED::TEntityInfo theEntityInfo)
{
  if(MYDEBUG) MESSAGE("GetFieldsByEntity(...)");
  TTimeStampGroup aGroup;
  med_int aNbFields = theWrapper.GetNbFields(); 
  MED::PrefixPrinter aPrefixPrinter;
  if(MYDEBUG) BEGMSG("GetNbFields() = "<<aNbFields<<"\n");
  for(int iField = 0; iField < aNbFields; iField++){
    PFieldInfo aFieldInfo = theWrapper.GetFieldInfo(theMeshInfo,iField);
    med_int aNbComp = aFieldInfo->GetNbComp();
    string aName = aFieldInfo->GetName();
    MED::PrefixPrinter aPrefixPrinter;
    if(MYDEBUG) BEGMSG("aFieldName = '"<<aName<<"'; aNbComp = "<<aNbComp<<"; ");
    MED::TGeom aGeom;
    med_entite_maillage anEntity;
    med_int aNbTimeStamps = theWrapper.GetNbTimeStamps(aFieldInfo,theEntityInfo,anEntity,aGeom);
    if(MYDEBUG) BEGMSG("anEntity = "<<anEntity<<"; GetNbTimeStamps = "<<aNbTimeStamps<<"\n");
    for(int iTimeStamp = 0; iTimeStamp < aNbTimeStamps; iTimeStamp++){
      PTimeStampInfo aTimeStamp = 
	theWrapper.GetTimeStampInfo(aFieldInfo,iTimeStamp,anEntity,aGeom);
      aGroup[aFieldInfo].insert(aTimeStamp);
      med_int aNbGauss = aTimeStamp->GetNbGauss();
      MED::PrefixPrinter aPrefixPrinter;
      if(MYDEBUG) BEGMSG("aDt = "<<aTimeStamp->GetDt()<<", "<<aTimeStamp->GetUnitDt()<<"\n");
    }
  }
  if(MYDEBUG) ADDMSG("\n");
  return aGroup;
}


TFieldGroup 
MEDA::GetFieldsByEntity(const TTimeStampGroup& theTimeStampGroup)
{
  TFieldGroup aGroup;
  TTimeStampGroup::const_iterator anIter = theTimeStampGroup.begin();
  for(; anIter != theTimeStampGroup.end(); anIter++){
    const PFieldInfo& aFieldInfo = anIter->first;
    const TTimeStampSet& aTimeStampSet = anIter->second;
    if(aTimeStampSet.empty()) continue;
    const PTimeStampInfo& aTimeStampInfo = *aTimeStampSet.begin();
    aGroup[aTimeStampInfo->GetEntity()].insert(*anIter);
  }
  return aGroup;
}


TFamilyByEntity
MEDA::GetFamiliesByEntity(TWrapper& theWrapper, 
			  const PNodeInfo& theNodeInfo,
			  const TCellGroup& theCellGroup,
			  const TFamilyGroup& theFamilyGroup)
{
  if(MYDEBUG) MESSAGE("GetFamiliesByEntity(...)");
  TFamilyByEntity aFamilyByEntity;

  typedef map<med_int,PFamilyInfo> TFamilyByIdMap;
  TFamilyByIdMap aFamilyByIdMap;
  TFamilyGroup::const_iterator anIter = theFamilyGroup.begin();
  for(; anIter != theFamilyGroup.end(); anIter++){
    const PFamilyInfo& aFamilyInfo = *anIter;
    aFamilyByIdMap.insert(TFamilyByIdMap::value_type(aFamilyInfo->GetId(),aFamilyInfo));
  }

  if(!aFamilyByIdMap.empty()){
    typedef set<med_int> TFamilyIdSet;
    typedef map<med_entite_maillage,TFamilyIdSet> TFamilyIdByEntity;
    TFamilyIdByEntity aFamilyIdByEntity;

    if(theNodeInfo!=0){
      if(med_int aNbElem = theNodeInfo->GetNbElem()){
	TFamilyIdSet& aFamilyIdSet = aFamilyIdByEntity[MED_NOEUD];
	for(med_int i = 0; i < aNbElem; i++){
	  aFamilyIdSet.insert(theNodeInfo->GetFamNum(i));
	}
      }
    }
    
    if(!theCellGroup.empty()){
      TCellGroup::const_iterator anIter = theCellGroup.begin();
      for(; anIter != theCellGroup.end(); anIter++){
	const med_entite_maillage& anEntity = anIter->first;
	TFamilyIdSet& aFamilyIdSet = aFamilyIdByEntity[anEntity];
	const TCellSet& aCellSet = anIter->second;
	TCellSet::const_iterator anCellIter = aCellSet.begin();
	for(; anCellIter != aCellSet.end(); anCellIter++){
	  const PCellInfo& aCellInfo = *anCellIter;
	  if(med_int aNbElem = aCellInfo->GetNbElem()){
	    for(med_int i = 0; i < aNbElem; i++){
	      aFamilyIdSet.insert(aCellInfo->GetFamNum(i));
	    }
	  }
	}
      }
    }
    
    if(!aFamilyIdByEntity.empty()){
      TFamilyIdByEntity::const_iterator anIter = aFamilyIdByEntity.begin();
      for(; anIter != aFamilyIdByEntity.end(); anIter++){
	const med_entite_maillage& anEntity = anIter->first;
	MED::PrefixPrinter aPrefixPrinter;
	if(MYDEBUG) BEGMSG("anEntity = "<<anEntity<<":\n");
	const TFamilyIdSet& aFamilyIdSet = anIter->second;
	TFamilyIdSet::const_iterator anFamilyIdIter = aFamilyIdSet.begin();
	for(; anFamilyIdIter != aFamilyIdSet.end(); anFamilyIdIter++){
	  const med_int& aFamilyId = *anFamilyIdIter;
	  TFamilyByIdMap::const_iterator 
	    anFamilyByIdMapIter = aFamilyByIdMap.find(aFamilyId);
	  if(anFamilyByIdMapIter != aFamilyByIdMap.end()){
	    const PFamilyInfo& aFamilyInfo = anFamilyByIdMapIter->second;
	    aFamilyByEntity[anEntity].insert(aFamilyInfo);
	    MED::PrefixPrinter aPrefixPrinter;
	    if(MYDEBUG) 
	      BEGMSG("aFamilyName = '"<<aFamilyInfo->GetName()<<
		     "' anId = "<<aFamilyInfo->GetId()<<"\n");
	  }
	}
      }
    }
  }    
  if(MYDEBUG) ADDMSG("\n");
  return aFamilyByEntity;
}

