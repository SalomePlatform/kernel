//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MED_Wrapper.hxx"
#include "MED_Utilities.hxx"
 
namespace MED{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PMeshInfo TWrapper::GetPMeshInfo(TInt theId)
  {
    PMeshInfo anInfo = CrMeshInfo();
    GetMeshInfo(theId,*anInfo);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PFamilyInfo TWrapper::GetPFamilyInfo(const PMeshInfo& theMeshInfo, TInt theId)
  {
    TInt aNbAttr = GetNbFamAttr(theId,*theMeshInfo);
    TInt aNbGroup = GetNbFamGroup(theId,*theMeshInfo);
    PFamilyInfo anInfo = CrFamilyInfo(theMeshInfo,aNbGroup,aNbAttr);
    GetFamilyInfo(theId,*anInfo);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PNodeInfo TWrapper::GetPNodeInfo(const PMeshInfo& theMeshInfo)
  {
    TInt aNbElems = GetNbNodes(*theMeshInfo);
    PNodeInfo anInfo = CrNodeInfo(theMeshInfo,aNbElems);
    GetNodeInfo(*anInfo);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PCellInfo TWrapper::GetPCellInfo(const PMeshInfo& theMeshInfo,
				   EEntiteMaillage theEntity, 
				   EGeometrieElement theGeom, 
				   EConnectivite theConn)
  {
    TInt aNbElem = GetNbCells(*theMeshInfo,theEntity,theGeom,theConn);
    PCellInfo anInfo = CrCellInfo(theMeshInfo,aNbElem,theEntity,theGeom,theConn);
    GetCellInfo(*anInfo);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PFieldInfo TWrapper::GetPFieldInfo(const PMeshInfo& theMeshInfo, TInt theId)
  {
    TInt aNbComp = GetNbComp(theId);
    PFieldInfo anInfo = CrFieldInfo(theMeshInfo,aNbComp);
    GetFieldInfo(theId,*anInfo);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PTimeStampInfo TWrapper::GetPTimeStampInfo(const PFieldInfo& theFieldInfo,
					     EEntiteMaillage theEntity,
					     const MED::TGeom& theGeom,
					     TInt theId)
  {
    PTimeStampInfo anInfo = CrTimeStampInfo(theFieldInfo,theEntity,theGeom);
    GetTimeStampInfo(theId,*anInfo);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PTimeStampVal TWrapper::GetPTimeStampVal(const PTimeStampInfo& theTimeStampInfo)
  {
    PTimeStampVal anInfo = CrTimeStampVal(theTimeStampInfo);
    GetTimeStampVal(*anInfo);
    return anInfo;
  }
}
