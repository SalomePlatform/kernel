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

#include "MED_Utilities.hxx"
#include "MED_Algorithm.hxx"
#include "MED_Factory.hxx"

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif

static int MYVALUEDEBUG = 1;

static int MYWRITEDEBUG = 1;

using namespace MED;


void CheckMed(const std::string& theFileName)
{
  MSG(MYDEBUG,"CheckMed - theFileName = '"<<theFileName<<"'");
  {
    PWrapper aMed = CrWrapper(theFileName);
    TInt aNbMeshes = aMed->GetNbMeshes();
    INITMSG(MYDEBUG,"GetNbMeshes() = "<<aNbMeshes<<endl);
    for(TInt iMesh = 1; iMesh <= aNbMeshes; iMesh++){
      
      PMeshInfo aMeshInfo = aMed->GetPMeshInfo(iMesh);
      INITMSG(MYDEBUG,"aMeshInfo->GetName() = "<<aMeshInfo->GetName()<<endl);

      //continue;

      PNodeInfo aNodeInfo = aMed->GetPNodeInfo(aMeshInfo);
      
      TEntityInfo aEntityInfo = aMed->GetEntityInfo(aMeshInfo);
      
      TCellGroup aCellGroup = GetCellsByEntity(aMed,aMeshInfo,aEntityInfo);
      
      TFamilyGroup aFamilyGroup = GetFamilies(aMed,aMeshInfo);
      
      TFamilyByEntity aFamilyByEntity = GetFamiliesByEntity(aMed,aNodeInfo,aCellGroup,aFamilyGroup);
      
      TGroupInfo aGroupInfo = GetFamiliesByGroup(aFamilyGroup);
      
      TTimeStampGroup aTimeStampGroup = GetFieldsByEntity(aMed,aMeshInfo,aEntityInfo);
      
      TFieldGroup aFieldGroup = GetFieldsByEntity(aTimeStampGroup);
    }
  }
  MSG(MYDEBUG,"OK");
}


void CopyMed(const PWrapper& theMed, 
	     const PWrapper& theMed2, 
	     int theIncr)
{
  TInt aNbMeshes = theMed->GetNbMeshes();
  MSG(MYDEBUG,"aNbMeshes = "<<aNbMeshes);
  std::string aName;
  for(TInt iMesh = 0; iMesh < aNbMeshes; iMesh++){
    PMeshInfo aMeshInfo = theMed->GetPMeshInfo(iMesh+1);
    TInt aDim = aMeshInfo->myDim;
    aName = aMeshInfo->GetName();
    INITMSG(MYDEBUG,"GetMeshInfo - aName = '"<<aName<<"'; aDim = "<<aDim<<"\n");
    PMeshInfo aMeshInfo2 = theMed2->CrMeshInfo(aMeshInfo);
    if(MYWRITEDEBUG){
      aName = aMeshInfo2->GetName();

      aName[0] += theIncr;
      aMeshInfo2->SetName(aName);
      theMed2->SetMeshInfo(aMeshInfo2);
      BEGMSG(MYDEBUG,"aNbMeshes2 = "<<theMed2->GetNbMeshes()<<"\n");
    }
    
    TEntityInfo aEntityInfo = theMed->GetEntityInfo(aMeshInfo);

    //continue;

    TInt aNbFields = theMed->GetNbFields(); 
    MSG(MYDEBUG,"GetNbFields() = "<<aNbFields);
    for(TInt iField = 0; iField < aNbFields; iField++){
      PFieldInfo aFieldInfo = theMed->GetPFieldInfo(aMeshInfo,iField+1);
      TInt aNbComp = aFieldInfo->GetNbComp();
      INITMSG(MYDEBUG,"aName = '"<<aFieldInfo->GetName()<<"'; aNbComp = "<<aNbComp<<";\n");
      PFieldInfo aFieldInfo2 = theMed->CrFieldInfo(aMeshInfo2,aFieldInfo);

      if(MYWRITEDEBUG){
	aName = aFieldInfo->GetName();
	aName[0] += theIncr;
	aFieldInfo2->SetName(aName);
	theMed2->SetFieldInfo(aFieldInfo2);
      }

      EEntiteMaillage anEntity;
      TGeom aTGeom;
      TInt aNbTimeStamps = 
	theMed->GetNbTimeStamps(aFieldInfo,aEntityInfo,anEntity,aTGeom);
      {
	INITMSG(MYDEBUG,"GetNbTimeStamps = "<<aNbTimeStamps<<endl);
	for(TInt iTimeStamp = 0; iTimeStamp < aNbTimeStamps; iTimeStamp++){
	  PTimeStampInfo aTimeStampInfo = 
	  theMed->GetPTimeStampInfo(aFieldInfo,anEntity,aTGeom,iTimeStamp+1);
	  TInt aNumDt = aTimeStampInfo->GetNumDt();
	  TInt aNbGauss = aTimeStampInfo->GetNbGauss();
	  INITMSG(MYDEBUG,"aNbGauss = "<<aNbGauss<<"; aNumDt = "<<aNumDt<<"\n");
	  
	  PTimeStampInfo aTimeStampInfo2 = 
	    theMed->CrTimeStampInfo(aFieldInfo2,aTimeStampInfo);
	  
	  PTimeStampVal aTimeStampVal = theMed->GetPTimeStampVal(aTimeStampInfo);
	  
	  PTimeStampVal aTimeStampVal2 = theMed->CrTimeStampVal(aTimeStampInfo2,aTimeStampVal);
	  
	  if(MYWRITEDEBUG) theMed2->SetTimeStamp(aTimeStampVal2);
	  
	  TMeshValue& aMeshValue = aTimeStampVal->myMeshValue;
	  TMeshValue::iterator aMeshValueIter = aMeshValue.begin();
	  for(; aMeshValueIter != aMeshValue.end(); aMeshValueIter++){
	    EGeometrieElement aGeom = aMeshValueIter->first;
	    TValue aValue = aMeshValueIter->second;
	    INITMSG(MYDEBUG,"aGeom = "<<aGeom<<"; aValue = "<<aValue.size()<<": ");
	    for(TInt i = 0, iEnd = aValue.size()/aNbComp; i < iEnd; i++){
	      for(TInt j = 0, ij = i*aNbComp; j < aNbComp; j++, ij++){
		ADDMSG(MYVALUEDEBUG,aValue[ij]<<",");
	      }
	      ADDMSG(MYVALUEDEBUG," ");
	    }
	    ADDMSG(MYDEBUG,"\n");
	  }
	}
      }
    }

    TInt aNbFam = theMed->GetNbFamilies(aMeshInfo);
    MSG(MYDEBUG,"GetNbFamilies() = "<<aNbFam);
    for(TInt iFam = 0; iFam < aNbFam; iFam++){
      PFamilyInfo aFamilyInfo = theMed->GetPFamilyInfo(aMeshInfo,iFam+1);
      TInt aNbGroup = aFamilyInfo->GetNbGroup();
      TInt aNbAttr = aFamilyInfo->GetNbAttr();
      TInt anId = aFamilyInfo->GetId();
      aName = aFamilyInfo->GetName();
      INITMSG(MYDEBUG,"aName = '"<<aName<<"'; anId = "<<anId<<
	      "; aNbAttr = "<<aNbAttr<<"; aNbGroup = "<<aNbGroup<<"\n");
      PFamilyInfo aFamilyInfo2 = theMed->CrFamilyInfo(aMeshInfo2,aFamilyInfo);
      for(TInt iGroup = 0; iGroup < aNbGroup; iGroup++){
	aName = aFamilyInfo->GetGroupName(iGroup);
	INITMSG(MYDEBUG,"aGroupName = '"<<aName<<"'\n");
	aName[0] += theIncr;
	aFamilyInfo2->SetGroupName(iGroup,aName);
      }

      if(MYWRITEDEBUG){
	aName = aFamilyInfo->GetName();
	aName[0] += theIncr;
	aFamilyInfo2->SetName(aName);
	theMed2->SetFamilyInfo(aFamilyInfo2);
	INITMSG(MYDEBUG,"GetNbFamilies = "<<theMed2->GetNbFamilies(aMeshInfo2)<<endl);
      }
    }

    MSG(MYDEBUG,"GetEntityInfo - aNbEntities = "<<aEntityInfo.size());
    TEntityInfo::iterator anEntityInfoIter = aEntityInfo.begin();
    for(; anEntityInfoIter != aEntityInfo.end(); anEntityInfoIter++){
      const EEntiteMaillage& anEntity = anEntityInfoIter->first;
      INITMSG(MYDEBUG,"anEntity = "<<anEntity<<"\n");
      if(anEntity == eNOEUD){
	PNodeInfo aNodeInfo = theMed->GetPNodeInfo(aMeshInfo);
	TInt aNbNodes = aNodeInfo->GetNbElem();
	INITMSG(MYDEBUG,"GetNodeInfo - aNbNodes = "<<aNbNodes<<": ");
	TNodeCoord& aCoord = aNodeInfo->myCoord;
	for(TInt iNode = 0; iNode < aNbNodes; iNode++){
	  for(TInt iDim = 0, anId = iNode*aDim; iDim < aDim; iDim++, anId++){
	    ADDMSG(MYVALUEDEBUG,aCoord[anId]<<",");
	    aCoord[anId] += theIncr;
	  }
	  ADDMSG(MYVALUEDEBUG," ");
	}
	ADDMSG(MYDEBUG,endl);
	PNodeInfo aNodeInfo2 = theMed->CrNodeInfo(aMeshInfo2,aNodeInfo);
	if(MYWRITEDEBUG) theMed2->SetNodeInfo(aNodeInfo2);
	continue;
      }
      TGeom& aTGeom = anEntityInfoIter->second;
      TGeom::iterator anTGeomIter = aTGeom.begin();
      for(; anTGeomIter != aTGeom.end(); anTGeomIter++){
	const EGeometrieElement& aGeom = anTGeomIter->first;
	const TInt& aNbElem = anTGeomIter->second;
	INITMSG(MYDEBUG,"aGeom = "<<aGeom<<"; aNbElem = "<<aNbElem<<": ");
	PCellInfo aCellInfo = theMed->GetPCellInfo(aMeshInfo,anEntity,aGeom);
	TInt aConnDim = aCellInfo->GetConnDim();
	for(TInt iElem = 0; iElem < aNbElem; iElem++){
	  for(TInt iConn = 0; iConn < aConnDim; iConn++){
	    ADDMSG(MYVALUEDEBUG,aCellInfo->GetConn(iElem,iConn)<<",");
	  }
	  ADDMSG(MYVALUEDEBUG," ");
	}
	ADDMSG(MYDEBUG,endl);
	PCellInfo aCellInfo2 = theMed->CrCellInfo(aMeshInfo2,aCellInfo);
	if(MYWRITEDEBUG) theMed2->SetCellInfo(aCellInfo2);
      }
    }
    
  }
  MSG(MYDEBUG,"OK");
}


void CopyMed(const std::string& theFileName, 
	     const std::string& theFileName2, 
	     MED::EVersion theVersion,
	     int theNbCopy)
{
  MSG(MYDEBUG,"CopyMed - theFileName = '"<<theFileName<<"'; theFileName2 = '"<<theFileName2<<"'");

  PWrapper aMed = CrWrapper(theFileName);

  PWrapper aMed2 = CrWrapper(theFileName2,theVersion);

  for(int i = 0; i < theNbCopy; i++) 
    CopyMed(aMed,aMed2,i);
}


void ReadMed(const char* theFileName, 
	     const char* theFileName2,
	     MED::EVersion theVersion = eV2_1,
	     int theNbCopy = 1)
{
  MSG(MYDEBUG,"theFileName = '"<<theFileName<<"'; "<<
      "theFileName2 = '"<<theFileName2<<"'; "<<
      "theVersion = "<<theVersion<<"; "<<
      "theNbCopy = "<<theNbCopy);

  CopyMed(theFileName,theFileName2,theVersion,theNbCopy);
}


int main(int argc, char** argv){ 
  try{
    if(argc == 2)
      CheckMed(argv[1]);
    if(argc == 3)
      ReadMed(argv[1],argv[2]);
    if(argc == 4)
      ReadMed(argv[1],argv[2],MED::EVersion(atoi(argv[3])));
    if(argc == 5)
      ReadMed(argv[1],argv[2],MED::EVersion(atoi(argv[3])),atoi(argv[4]));
    return 0;
  }catch(std::exception& exc){
    MSG(MYDEBUG,"Follow exception was accured :\n"<<exc.what());
  }catch(...){
    MSG(MYDEBUG,"Unknown exception was accured in VISU_Convertor_impl");
  } 
  return 1;
}
