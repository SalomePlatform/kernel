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

#include "MEDA_Structures.hxx"
#include "MEDA_Wrapper.hxx"
#include "MED_Utilities.hxx"

using namespace std;

#ifdef DEBUG
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


void ReadMed(const char* theFileName){
  using namespace MEDA;
  TWrapper aMed(theFileName);
  string aFileName = theFileName;
  aFileName += "+";
  TWrapper aMedW(aFileName);

  int aNbMeshes = aMed.GetNbMeshes();
  cout<<"GetNbMeshes() = "<<aNbMeshes<<endl;

  string aName;
  for(int iMesh = 0; iMesh < aNbMeshes; iMesh++){
    PMeshInfo aMeshInfo = aMed.GetMeshInfo(iMesh);
    int aDim = aMeshInfo->GetDim();
    PMeshInfo aMeshInfoW = TWrapper::CrMeshInfo(aMeshInfo);
    aName = aMeshInfoW->GetName();
    cout<<"GetMeshInfo - aName = '"<<aName<<"'; aDim = "<<aDim<<endl;
    aName[0] += 1;
    aMeshInfoW->SetName(aName);
    aMedW.SetMeshInfo(aMeshInfoW);
  
    int aNbFam = aMed.GetNbFamilies(aMeshInfo);
    cout<<"GetNbFamilies() = "<<aNbFam<<endl;
    for(int iFam = 0; iFam < aNbFam; iFam++){
      PFamilyInfo aFamilyInfo = aMed.GetFamilyInfo(aMeshInfo,iFam);
      PFamilyInfo aFamilyInfoW = TWrapper::CrFamilyInfo(aMeshInfoW,aFamilyInfo);
      aName = aFamilyInfo->GetName();
      int aNbAttr = aFamilyInfo->GetNbAttr();
      int aNbGroup = aFamilyInfo->GetNbGroup();
      cout<<"\taName = '"<<aName<<"'; aNbAttr = "<<aNbAttr<<"; aNbGroup = "<<aNbGroup<<endl;
      aName[0] += 1;
      aFamilyInfoW->SetName(aName);
      for(int iGroup = 0; iGroup < aNbGroup; iGroup++){
	aName = aFamilyInfo->GetGroupName(iGroup);
	cout<<"\t\taGroupName = '"<<aName<<"'\n";
	aName[0] += 1;
	aFamilyInfoW->SetGroupName(iGroup,aName);
      }
      aMedW.SetFamilyInfo(aFamilyInfoW);
    }

    MED::TEntityInfo aEntityInfo = aMed.GetEntityInfo(aMeshInfo);
    cout<<"GetEntityInfo - aNbEntities = "<<aEntityInfo.size()<<endl;
    MED::TEntityInfo::iterator anEntityIter = aEntityInfo.begin();
    for(; anEntityIter != aEntityInfo.end(); anEntityIter++){
      const med_entite_maillage& anEntity = anEntityIter->first;
      cout<<"\tanEntity = "<<anEntity<<endl;
      if(anEntity == MED_NOEUD){
	PNodeInfo aNodeInfo = aMed.GetNodeInfo(aMeshInfo);
	PNodeInfo aNodeInfoW = TWrapper::CrNodeInfo(aMeshInfoW,aNodeInfo);
	int aNbNodes = aNodeInfo->GetNbElem();
	cout<<"\tGetNodeInfo - aNbNodes = "<<aNbNodes<<": ";
	for(int iNode = 0; iNode < aNbNodes; iNode++){
	  for(int iDim = 0; iDim < aDim; iDim++){
	    //cout<<aNodeInfo->GetNodeCoord(iNode,iDim)<<",";
	  }
	  cout<<" ";
	}
	cout<<endl;
	aMedW.SetNodeInfo(aNodeInfoW);
	continue;
      }
      const MED::TGeom& aTGeom = anEntityIter->second;
      MED::TGeom::const_iterator anTGeomIter = aTGeom.begin();
      for(; anTGeomIter != aTGeom.end(); anTGeomIter++){
	const med_geometrie_element& aGeom = anTGeomIter->first;
	PCellInfo aCellInfo = aMed.GetCellInfo(aMeshInfo,anEntity,aGeom);
	PCellInfo aCellInfoW = TWrapper::CrCellInfo(aMeshInfoW,aCellInfo);
	med_int aNbElem = aCellInfo->GetNbElem();
	cout<<"\t\taGeom = "<<aGeom<<"; aNbElem = "<<aNbElem<<": ";
	for(int iElem = 0; iElem < aNbElem; iElem++){
	  int iConnEnd = aCellInfo->GetConnDim();
	  for(int iConn = 0; iConn < iConnEnd; iConn++){
	    //cout<<aCellInfo->GetConn(iElem,iConn)<<",";
	  }
	  cout<<" ";
	}
	cout<<endl;
	aMedW.SetCellInfo(aCellInfoW);
      }
    }

    med_int aNbFields = aMed.GetNbFields(); 
    cout<<"GetNbFields() = "<<aNbFields<<endl;
    for(int iField = 0; iField < aNbFields; iField++){
      PFieldInfo aFieldInfo = aMed.GetFieldInfo(aMeshInfo,iField);
      PFieldInfo aFieldInfoW = TWrapper::CrFieldInfo(aMeshInfoW,aFieldInfo);
      med_int aNbComp = aFieldInfo->GetNbComp();
      aName = aFieldInfo->GetName();
      cout<<"\taName = '"<<aName<<"'; aNbComp = "<<aNbComp<<"; ";
      aName[0] += 1;
      aFieldInfoW->SetName(aName);
      aMedW.SetFieldInfo(aFieldInfoW);
      MED::TGeom aTGeom;
      med_entite_maillage anEntity;
      med_int aNbTimeStamps = aMed.GetNbTimeStamps(aFieldInfo,aEntityInfo,anEntity,aTGeom);
      cout<<"anEntity = "<<anEntity<<"; GetNbTimeStamps = "<<aNbTimeStamps<<endl;
      for(int iTimeStamp = 0; iTimeStamp < aNbTimeStamps; iTimeStamp++){
	PTimeStampInfo aTimeStamp = aMed.GetTimeStampInfo(aFieldInfo,iTimeStamp,
							  anEntity,aTGeom);
	PTimeStampInfo aTimeStampW = TWrapper::CrTimeStampInfo(aFieldInfoW,aTimeStamp);
	med_int aNbGauss = aTimeStamp->GetNbGauss();
	cout<<"\t\taDt = "<<aTimeStamp->GetDt()<<", "<<aTimeStamp->GetUnitDt()<<endl;
	PTimeStampVal aTimeStampVal = aMed.GetTimeStampVal(aTimeStamp);
	PTimeStampVal aTimeStampValW = TWrapper::CrTimeStampVal(aTimeStampW,aTimeStampVal);
	MED::TGeom::const_iterator anTGeomIter = aTGeom.begin();
	for(; anTGeomIter != aTGeom.end(); anTGeomIter++){
	  const med_geometrie_element& aGeom = anTGeomIter->first;
	  med_int aNbElem = anTGeomIter->second;
	  cout<<"\t\taGeom = "<<aGeom<<"; aNbElem = "<<aNbElem<<": ";
	  for(int iElem = 0; iElem < aNbElem; iElem++){
	    for(int j = 0; j < aNbComp; j++){
	      for(int k = 0; k < aNbGauss; k++){
		//cout<<aTimeStampValW->GetVal(aGeom,iElem,j,k)<<",";
	      }
	    }
	    cout<<" ";
	  }
	  cout<<"\n";
	}
	aMedW.SetTimeStamp(aTimeStampValW);
      }
    }    
  }
  cout<<"OK"<<endl;
}


int main(int argc, char** argv){ 
    if(argc > 1)
      ReadMed(argv[1]);
    return 0;
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
