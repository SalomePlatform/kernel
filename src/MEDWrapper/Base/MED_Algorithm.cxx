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
  TElemGroup 
  GetElemsByEntity(TWrapper& theWrapper, 
		   const PMeshInfo& theMeshInfo,
		   const MED::TEntityInfo& theEntityInfo)
  {
    MSG(MYDEBUG,"GetElemsByEntity(...)");
    TElemGroup aGroup;
    MED::TEntityInfo::const_iterator anIter = theEntityInfo.begin();
    for(; anIter != theEntityInfo.end(); anIter++){
      const EEntiteMaillage& anEntity = anIter->first;
      if(anEntity == eNOEUD) continue;
      const MED::TGeom& aGeom = anIter->second;
      TElemSet& aElemSet = aGroup[anEntity];
      MED::TGeom::const_iterator anGeomIter = aGeom.begin();
      for(; anGeomIter != aGeom.end(); anGeomIter++){
	const EGeometrieElement& aGeo = anGeomIter->first;
	switch(aGeo){
	case ePOLYGONE:
	  {
	    PPolygoneInfo aPolygoneInfo = theWrapper.GetPPolygoneInfo(theMeshInfo,anEntity,aGeo);
	    aElemSet.insert(aPolygoneInfo);
	    TElemNum aConn  = aPolygoneInfo->GetConnectivite();
	    TElemNum aIndex = aPolygoneInfo->GetIndex();
	    TInt aNbIndex = aIndex.size();
	    TInt aIndex0 = aIndex[0];
	    INITMSG(MYDEBUG,"aGeo = "<<aGeo<<"; aNbElem = "<<aNbIndex-1<<": ");
	    for(TInt iElem = 1; iElem < aNbIndex; iElem++){
	      for (TInt i = aIndex0; i < aIndex[iElem];i++)
		ADDMSG(MYVALUEDEBUG,aConn[i-1]<<",");
	      ADDMSG(MYDEBUG," ");
	      aIndex0 = aIndex[iElem];
	    }
	    ADDMSG(MYDEBUG,endl);
	    ADDMSG(MYDEBUG,"           Indexes :");
	    for(TInt iElem = 0; iElem < aIndex.size(); iElem++){
	      ADDMSG(MYVALUEDEBUG,aIndex[iElem]<<",");
	    }
	    ADDMSG(MYDEBUG,endl);
	    break;
	  }
	default:
	  {
	    PCellInfo aCellInfo = theWrapper.GetPCellInfo(theMeshInfo,anEntity,aGeo);
	    aElemSet.insert(aCellInfo);
	  }
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
    TErr anErr;
    TFamilyGroup aGroup;
    TInt aNbFam = theWrapper.GetNbFamilies(*theMeshInfo);
    INITMSG(MYDEBUG,"GetNbFamilies() = "<<aNbFam<<"\n");
    for(TInt iFam = 1; iFam <= aNbFam; iFam++){
      PFamilyInfo aFamilyInfo = theWrapper.GetPFamilyInfo(theMeshInfo,iFam,&anErr);
      if(anErr >= 0)
	aGroup.insert(aFamilyInfo);
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

#ifdef _DEBUG_
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
#endif

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
      INITMSG(MYDEBUG,"aFieldName = '"<<aFieldInfo->GetName()<<
	      "'; aNbComp = "<<aFieldInfo->GetNbComp()<<"; ");
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
		      const TElemGroup& theElemGroup,
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
      
      if(!theElemGroup.empty()){
	TElemGroup::const_iterator anIter = theElemGroup.begin();
	for(; anIter != theElemGroup.end(); anIter++){
	  const EEntiteMaillage& anEntity = anIter->first;
	  TFamilyIdSet& aFamilyIdSet = aFamilyIdByEntity[anEntity];
	  const TElemSet& aElemSet = anIter->second;
	  TElemSet::const_iterator anElemIter = aElemSet.begin();
	  for(; anElemIter != aElemSet.end(); anElemIter++){
	    const PElemInfo& aElemInfo = *anElemIter;
	    if(TInt aNbElem = aElemInfo->GetNbElem()){
	      for(TInt i = 0; i < aNbElem; i++){
		aFamilyIdSet.insert(aElemInfo->GetFamNum(i));
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
