//  VISU OBJECT : interactive object for VISU entities implementation
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
//  File:    VISUConvertor.cxx
//  Author:  Alexey PETROV
//  Module : VISU

#include "MED_Structures.hxx"
#include "MED_Wrapper.hxx"
#include "MED_Utilities.hxx"

using namespace std;

#ifdef DEBUG
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


void ReadMed(const char* theFileName){
  using namespace MED;
  TWrapper aMed(theFileName);
  string aFileName = theFileName;
  aFileName += "-";
  TWrapper aMedW(aFileName);

  int aNbMeshes = aMed.GetNbMeshes();
  cout<<"GetNbMeshes() = "<<aNbMeshes<<endl;

  string aName;
  for(int iMesh = 0; iMesh < aNbMeshes; iMesh++){
    PMeshInfo aMeshInfo(new TMeshInfo());
    PMeshInfo aNewMeshInfo(new TMeshInfo());
    aMed.GetMeshInfo(iMesh+1,*aMeshInfo);
    int aDim = aMeshInfo->myDim;
    aName = aMeshInfo->GetName();
    cout<<"GetMeshInfo - aName = '"<<aName<<"'; aDim = "<<aDim<<endl;
    *aNewMeshInfo = *aMeshInfo;
    aName[0] += 1;
    aNewMeshInfo->SetName(aName);
    aMedW.SetMeshInfo(*aNewMeshInfo);

    TEntityInfo aEntityInfo = aMed.GetEntityInfo(*aMeshInfo);

    med_int aNbFields = aMed.GetNbFields(); 
    cout<<"GetNbFields() = "<<aNbFields<<endl;
    for(int iField = 0; iField < aNbFields; iField++){
      med_int aNbComp = aMed.GetNbComp(iField+1);
      PFieldInfo aFieldInfo(new TFieldInfo(aMeshInfo,aNbComp));
      aMed.GetFieldInfo(iField+1,*aFieldInfo);
      cout<<"\taName = '"<<aFieldInfo->GetName()<<"'; aNbComp = "<<aNbComp<<"; ";
      aMedW.SetFieldInfo(*aFieldInfo);
      med_entite_maillage anEntity;
      TGeom aTGeom;
      med_int aNbTimeStamps = aMed.GetNbTimeStamps(*aFieldInfo,aEntityInfo,
						   anEntity,aTGeom);
      cout<<"GetNbTimeStamps = "<<aNbTimeStamps<<endl;
      PTimeStampInfo aTimeStampInfo(new TTimeStampInfo(aFieldInfo,anEntity,aTGeom));
      for(int iTimeStamp = 0; iTimeStamp < aNbTimeStamps; iTimeStamp++){
	aMed.GetTimeStampInfo(iTimeStamp+1, *aTimeStampInfo);

	TTimeStampVal aTimeStampVal(aTimeStampInfo);
	aMed.GetTimeStampVal(aTimeStampVal);
	aMedW.SetTimeStamp(aTimeStampVal);

	TMeshValue& aMeshValue = aTimeStampVal.myMeshValue;
	TMeshValue::iterator aMeshValueIter = aMeshValue.begin();
	for(; aMeshValueIter != aMeshValue.end(); aMeshValueIter++){
	  med_geometrie_element aGeom = aMeshValueIter->first;
	  TValue aValue = aMeshValueIter->second;
	  cout<<"\t\taGeom = "<<aGeom<<"; aValue = "<<aValue.size()<<": ";
	  for(int i = 0, iEnd = aValue.size()/aNbComp; i < iEnd; i++){
	    for(int j = 0, ij = i*aNbComp; j < aNbComp; j++, ij++){
	      //cout<<aValue[ij]<<",";
	    }
	    //cout<<" ";
	  }
	  cout<<"\n";
	}
      }
    }

    int aNbFam = aMed.GetNbFamilies(*aMeshInfo);
    cout<<"GetNbFamilies() = "<<aNbFam<<endl;
    for(int iFam = 0; iFam < aNbFam; iFam++){
      int aNbAttr = aMed.GetNbFamAttr(iFam+1,*aMeshInfo);
      int aNbGroup = aMed.GetNbFamGroup(iFam+1,*aMeshInfo);
      PFamilyInfo aFamilyInfo(new TFamilyInfo(aMeshInfo,aNbGroup,aNbAttr));
      aMed.GetFamilyInfo(iFam+1,*aFamilyInfo);
      aName = aFamilyInfo->GetName();
      cout<<"\taName = '"<<aName<<"'; aNbAttr = "<<aNbAttr<<"; aNbGroup = "<<aNbGroup<<endl;
      aName[0] += 1;
      aFamilyInfo->SetName(aName);
      aFamilyInfo->myMeshInfo = aNewMeshInfo;
      aName = aFamilyInfo->GetName();
      for(int iGroup = 0; iGroup < aNbGroup; iGroup++){
	aName = aFamilyInfo->GetGroupName(iGroup);
	cout<<"\t\taGroupName = '"<<aName<<"'\n";
	aName[0] += 1;
	aFamilyInfo->SetGroupName(iGroup,aName);
      }

      aMedW.SetFamilyInfo(*aFamilyInfo);
    }

    cout<<"GetEntityInfo - aNbEntities = "<<aEntityInfo.size()<<endl;
    TEntityInfo::iterator anEntityInfoIter = aEntityInfo.begin();
    for(; anEntityInfoIter != aEntityInfo.end(); anEntityInfoIter++){
      const med_entite_maillage& anEntity = anEntityInfoIter->first;
      cout<<"\tanEntity = "<<anEntity<<endl;
      if(anEntity == MED_NOEUD){
	int aNbNodes = aMed.GetNbNodes(*aMeshInfo);
	PNodeInfo aNodeInfo(new TNodeInfo(aMeshInfo,aNbNodes));
	aMed.GetNodeInfo(*aNodeInfo);
	cout<<"\tGetNodeInfo - aNbNodes = "<<aNbNodes<<": ";
	TNodeCoord& aCoord = aNodeInfo->myCoord;
	for(int iNode = 0; iNode < aNbNodes; iNode++){
	  for(int iDim = 0, anId = iNode*aDim; iDim < aDim; iDim++, anId++){
	    //cout<<aCoord[anId]<<",";
	    aCoord[anId] += 1.0;
	  }
	  //cout<<" ";
	}
	cout<<endl;
	aNodeInfo->myMeshInfo = aNewMeshInfo;
	aMedW.SetNodeInfo(*aNodeInfo);
	continue;
      }
      TGeom& aTGeom = anEntityInfoIter->second;
      TGeom::iterator anTGeomIter = aTGeom.begin();
      for(; anTGeomIter != aTGeom.end(); anTGeomIter++){
	const med_geometrie_element& aGeom = anTGeomIter->first;
	med_int& aNbElem = anTGeomIter->second;
	cout<<"\t\taGeom = "<<aGeom<<"; aNbElem = "<<aNbElem<<": ";
	PCellInfo aCellInfo(new TCellInfo(aMeshInfo,aNbElem,anEntity,aGeom));
	aMed.GetCellInfo(*aCellInfo);
	for(int iElem = 0; iElem < aCellInfo->myNbElem; iElem++){
	  int i = iElem*aCellInfo->myConnDim;
	  for(int iConn = 0; iConn < aCellInfo->myConnDim; iConn++, i++){
	    //cout<<aCellInfo->myConn[i]<<",";
	  }
	  //cout<<" ";
	}
	cout<<endl;
	aCellInfo->myMeshInfo = aNewMeshInfo;
	aMedW.SetCellInfo(*aCellInfo);
      }
    }
    
  }
  cout<<"OK"<<endl;
}


int main(int argc, char** argv){ 
  try{
    if(argc > 1)
      ReadMed(argv[1]);
    return 0;
  }catch(std::exception& exc){
    cout<<"Follow exception was accured :\n"<<exc.what()<<endl;
  }catch(...){
    cout<<"Unknown exception was accured in VISU_Convertor_impl"<<endl;
  } 
  return 1;
}
