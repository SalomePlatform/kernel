//  
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : 
//  Author : 
//  Module : 
//  $Header$

#include "MED_Algorithm.hxx"
#include "MED_Utilities.hxx"
 
#ifdef _DEBUG_
static int MYDEBUG = 0;
static int MYVALUEDEBUG = 0;
#else
static int MYDEBUG = 0;
static int MYVALUEDEBUG = 0;
#endif

namespace MED{
  //---------------------------------------------------------------
  TCellGroup 
  GetCellsByEntity(TWrapper& theWrapper, 
		   const PMeshInfo& theMeshInfo,
		   const MED::TEntityInfo& theEntityInfo)
  {
    MSG(MYDEBUG,"GetCellsByEntity(...)");
    TCellGroup aGroup;
    MED::TEntityInfo::const_iterator anIter = theEntityInfo.begin();
    for(; anIter != theEntityInfo.end(); anIter++){
      const EEntiteMaillage& anEntity = anIter->first;
      if(anEntity == eNOEUD) continue;
      const MED::TGeom& aGeom = anIter->second;
      TCellSet& aCellSet = aGroup[anEntity];
      MED::TGeom::const_iterator anGeomIter = aGeom.begin();
      for(; anGeomIter != aGeom.end(); anGeomIter++){
	const EGeometrieElement& aGeo = anGeomIter->first;
	PCellInfo aCellInfo = theWrapper.GetPCellInfo(theMeshInfo,anEntity,aGeo);
	aCellSet.insert(aCellInfo);
	if(MYDEBUG){
	  TInt aNbElem = aCellInfo->GetNbElem();
	  INITMSG(MYDEBUG,"aGeo = "<<aGeo<<"; aNbElem = "<<aNbElem<<": ");
	  for(TInt iElem = 0; iElem < aNbElem; iElem++){
	    TInt iConnEnd = aCellInfo->GetConnDim();
	    for(TInt iConn = 0; iConn < iConnEnd; iConn++){
	      ADDMSG(MYVALUEDEBUG,aCellInfo->GetConn(iElem,iConn)<<",");
	    }
	    ADDMSG(MYVALUEDEBUG," ");
	  }
	  ADDMSG(MYDEBUG,"\n");
	}
      }
    }
    ADDMSG(MYDEBUG,"\n");
    return aGroup;
  }
  
  
  //---------------------------------------------------------------
  TFamilyGroup 
  GetFamilies(TWrapper& theWrapper,
	      const PMeshInfo& theMeshInfo)
  {
    MSG(MYDEBUG,"GetFamilies(...)");
    TFamilyGroup aGroup;
    TInt aNbFam = theWrapper.GetNbFamilies(*theMeshInfo);
    INITMSG(MYDEBUG,"GetNbFamilies() = "<<aNbFam<<"\n");
    for(TInt iFam = 1; iFam <= aNbFam; iFam++){
      PFamilyInfo aFamilyInfo = theWrapper.GetPFamilyInfo(theMeshInfo,iFam);
      aGroup.insert(aFamilyInfo);
      if(MYDEBUG){
	string aName = aFamilyInfo->GetName();
	TInt aNbAttr = aFamilyInfo->GetNbAttr();
	TInt aNbGroup = aFamilyInfo->GetNbGroup();
	INITMSG(MYDEBUG,
		"aFamilyName = '"<<aName<<"'; andId = "<<aFamilyInfo->GetId()<<
		"; aNbAttr = "<<aNbAttr<<"; aNbGroup = "<<aNbGroup<<"\n");
	for(TInt iGroup = 0; iGroup < aNbGroup; iGroup++){
	  aName = aFamilyInfo->GetGroupName(iGroup);
	  INITMSG(MYDEBUG,"aGroupName = '"<<aName<<"'\n");
	}
      }
    }
    ADDMSG(MYDEBUG,"\n");
    return aGroup;
  }


  TGroupInfo 
  GetFamiliesByGroup(const TFamilyGroup& theGroupInfo)
  {
    MSG(MYDEBUG,"GetFamiliesByGroup(...)");
    TGroupInfo aGroup;
    TFamilyGroup::const_iterator anIter = theGroupInfo.begin();
    for(; anIter != theGroupInfo.end(); anIter++){
      const PFamilyInfo& aFamilyInfo = *anIter;
      TInt aNbGroup = aFamilyInfo->GetNbGroup();
      for(TInt iGroup = 0; iGroup < aNbGroup; iGroup++){
	aGroup[aFamilyInfo->GetGroupName(iGroup)].insert(aFamilyInfo);
      } 
    }
    if(MYDEBUG){
      TGroupInfo::const_iterator anIter = aGroup.begin();
      for(; anIter != aGroup.end(); anIter++){
	string aName = anIter->first;
	INITMSG(MYDEBUG,"aGroupName = '"<<aName<<"'\n");
	const TFamilyGroup& aFamilyGroup = anIter->second;
	TFamilyGroup::const_iterator anFamIter = aFamilyGroup.begin();
	for(; anFamIter != aFamilyGroup.end(); anFamIter++){
	  const PFamilyInfo& aFamilyInfo = *anFamIter;
	  INITMSG(MYDEBUG,"aFamilyName = '"<<aFamilyInfo->GetName()<<"'\n");
	}
      }
      ADDMSG(MYDEBUG,"\n");
    }
    return aGroup;
  }


  //---------------------------------------------------------------
  TTimeStampGroup 
  GetFieldsByEntity(TWrapper& theWrapper, 
		    const PMeshInfo& theMeshInfo,
		    const MED::TEntityInfo& theEntityInfo)
  {
    MSG(MYDEBUG,"GetFieldsByEntity(...)");
    TTimeStampGroup aGroup;
    TInt aNbFields = theWrapper.GetNbFields();
    INITMSG(MYDEBUG,"GetNbFields() = "<<aNbFields<<"\n");
    for(TInt iField = 1; iField <= aNbFields; iField++){
      PFieldInfo aFieldInfo = theWrapper.GetPFieldInfo(theMeshInfo,iField);
      TInt aNbComp = aFieldInfo->GetNbComp();
      string aName = aFieldInfo->GetName();
      INITMSG(MYDEBUG,"aFieldName = '"<<aName<<"'; aNbComp = "<<aNbComp<<"; ");
      MED::TGeom aGeom;
      EEntiteMaillage anEntity = EEntiteMaillage(-1);
      TInt aNbTimeStamps = theWrapper.GetNbTimeStamps(aFieldInfo,theEntityInfo,anEntity,aGeom);
      ADDMSG(MYDEBUG,"anEntity = "<<anEntity<<"; GetNbTimeStamps = "<<aNbTimeStamps<<"\n");
      for(TInt iTimeStamp = 1; iTimeStamp <= aNbTimeStamps; iTimeStamp++){
	PTimeStampInfo aTimeStamp = 
	  theWrapper.GetPTimeStampInfo(aFieldInfo,anEntity,aGeom,iTimeStamp);
	aGroup[aFieldInfo].insert(aTimeStamp);
	INITMSG(MYDEBUG,"aDt = "<<aTimeStamp->GetDt()
		<<", Unit = \'"<<aTimeStamp->GetUnitDt()
		<<"\', aNbGauss = "<<aTimeStamp->GetNbGauss()<<"\n");
      }
    }
    ADDMSG(MYDEBUG,"\n");
    return aGroup;
  }
  

  TFieldGroup 
  GetFieldsByEntity(const TTimeStampGroup& theTimeStampGroup)
  {
    TFieldGroup aGroup;
    TTimeStampGroup::const_iterator anIter = theTimeStampGroup.begin();
    for(; anIter != theTimeStampGroup.end(); anIter++){
      //const PFieldInfo& aFieldInfo = anIter->first;
      const TTimeStampSet& aTimeStampSet = anIter->second;
      if(aTimeStampSet.empty()) continue;
      const PTimeStampInfo& aTimeStampInfo = *aTimeStampSet.begin();
      aGroup[aTimeStampInfo->GetEntity()].insert(*anIter);
    }
    return aGroup;
  }
  

  TFamilyByEntity
  GetFamiliesByEntity(TWrapper& theWrapper, 
		      const PNodeInfo& theNodeInfo,
		      const TCellGroup& theCellGroup,
		      const TFamilyGroup& theFamilyGroup)
  {
    MSG(MYDEBUG,"GetFamiliesByEntity(...)");
    TFamilyByEntity aFamilyByEntity;
    
    typedef map<TInt,PFamilyInfo> TFamilyByIdMap;
    TFamilyByIdMap aFamilyByIdMap;
    TFamilyGroup::const_iterator anIter = theFamilyGroup.begin();
    for(; anIter != theFamilyGroup.end(); anIter++){
      const PFamilyInfo& aFamilyInfo = *anIter;
      aFamilyByIdMap.insert(TFamilyByIdMap::value_type(aFamilyInfo->GetId(),aFamilyInfo));
    }
    
    if(!aFamilyByIdMap.empty()){
      typedef set<TInt> TFamilyIdSet;
      typedef map<EEntiteMaillage,TFamilyIdSet> TFamilyIdByEntity;
      TFamilyIdByEntity aFamilyIdByEntity;
      
      if(theNodeInfo!=0){
	if(TInt aNbElem = theNodeInfo->GetNbElem()){
	  TFamilyIdSet& aFamilyIdSet = aFamilyIdByEntity[eNOEUD];
	  for(TInt i = 0; i < aNbElem; i++){
	    aFamilyIdSet.insert(theNodeInfo->GetFamNum(i));
	  }
	}
      }
      
      if(!theCellGroup.empty()){
	TCellGroup::const_iterator anIter = theCellGroup.begin();
	for(; anIter != theCellGroup.end(); anIter++){
	  const EEntiteMaillage& anEntity = anIter->first;
	  TFamilyIdSet& aFamilyIdSet = aFamilyIdByEntity[anEntity];
	  const TCellSet& aCellSet = anIter->second;
	  TCellSet::const_iterator anCellIter = aCellSet.begin();
	  for(; anCellIter != aCellSet.end(); anCellIter++){
	    const PCellInfo& aCellInfo = *anCellIter;
	    if(TInt aNbElem = aCellInfo->GetNbElem()){
	      for(TInt i = 0; i < aNbElem; i++){
		aFamilyIdSet.insert(aCellInfo->GetFamNum(i));
	      }
	    }
	  }
	}
      }
      
      if(!aFamilyIdByEntity.empty()){
	TFamilyIdByEntity::const_iterator anIter = aFamilyIdByEntity.begin();
	for(; anIter != aFamilyIdByEntity.end(); anIter++){
	  const EEntiteMaillage& anEntity = anIter->first;
	  INITMSG(MYDEBUG,"anEntity = "<<anEntity<<":\n");
	  const TFamilyIdSet& aFamilyIdSet = anIter->second;
	  TFamilyIdSet::const_iterator anFamilyIdIter = aFamilyIdSet.begin();
	  for(; anFamilyIdIter != aFamilyIdSet.end(); anFamilyIdIter++){
	    const TInt& aFamilyId = *anFamilyIdIter;
	    TFamilyByIdMap::const_iterator 
	      anFamilyByIdMapIter = aFamilyByIdMap.find(aFamilyId);
	    if(anFamilyByIdMapIter != aFamilyByIdMap.end()){
	      const PFamilyInfo& aFamilyInfo = anFamilyByIdMapIter->second;
	    aFamilyByEntity[anEntity].insert(aFamilyInfo);
	    INITMSG(MYDEBUG,
		    "aFamilyName = '"<<aFamilyInfo->GetName()<<
		    "' anId = "<<aFamilyInfo->GetId()<<"\n");
	    }
	  }
	}
      }
    }    
    ADDMSG(MYDEBUG,"\n");
    return aFamilyByEntity;
  }
  
}
