//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MED_Wrapper.hxx"
#include "MED_Utilities.hxx"
 
#ifdef _DEBUG_
static int MYDEBUG = 0;
static int MYVALUEDEBUG = 0;
#else
static int MYDEBUG = 0;
static int MYVALUEDEBUG = 0;
#endif

namespace MED{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PMeshInfo TWrapper::GetPMeshInfo(TInt theId,
				   TErr* theErr)
  {
    PMeshInfo anInfo = CrMeshInfo();
    GetMeshInfo(theId,*anInfo,theErr);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PFamilyInfo TWrapper::GetPFamilyInfo(const PMeshInfo& theMeshInfo, 
				       TInt theId,
				       TErr* theErr)
  {
    TInt aNbAttr = GetNbFamAttr(theId,*theMeshInfo);
    TInt aNbGroup = GetNbFamGroup(theId,*theMeshInfo);
    PFamilyInfo anInfo = CrFamilyInfo(theMeshInfo,aNbGroup,aNbAttr);
    GetFamilyInfo(theId,*anInfo,theErr);

#ifdef _DEBUG_
    string aName = anInfo->GetName();
    INITMSG(MYDEBUG,
	    "aFamilyName = '"<<aName<<"'; andId = "<<anInfo->GetId()<<
	    "; aNbAttr = "<<aNbAttr<<"; aNbGroup = "<<aNbGroup<<"\n");
    for(TInt iGroup = 0; iGroup < aNbGroup; iGroup++){
      aName = anInfo->GetGroupName(iGroup);
      INITMSG(MYDEBUG,"aGroupName = '"<<aName<<"'\n");
    }
#endif
    
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PNodeInfo TWrapper::GetPNodeInfo(const PMeshInfo& theMeshInfo,
				   TErr* theErr)
  {
    TInt aNbElems = GetNbNodes(*theMeshInfo);
    PNodeInfo anInfo = CrNodeInfo(theMeshInfo,aNbElems);
    GetNodeInfo(*anInfo,theErr);

#ifdef _DEBUG_
    TInt aDim = theMeshInfo->myDim;
    TInt aNbElem = anInfo->GetNbElem();
    INITMSG(MYDEBUG,"GetPNodeInfo - aCoords: "<<aNbElem<<": ");
    TNodeCoord& aCoord = anInfo->myCoord;
    for(TInt iElem = 0; iElem < aNbElem; iElem++){
      for(TInt iDim = 0, anId = iElem*aDim; iDim < aDim; iDim++, anId++){
	ADDMSG(MYVALUEDEBUG,aCoord[anId]<<",");
      }
      ADDMSG(MYVALUEDEBUG," ");
    }
    ADDMSG(MYDEBUG,endl);

    BEGMSG(MYDEBUG,"GetPNodeInfo - GetFamNum: ");
    for(TInt iElem = 0; iElem < aNbElem; iElem++){
      ADDMSG(MYVALUEDEBUG,anInfo->GetFamNum(iElem)<<", ");
    }
    ADDMSG(MYDEBUG,endl);

    if(anInfo->IsElemNum()){
      BEGMSG(MYDEBUG,"GetPNodeInfo - GetElemNum: ");
      for(TInt iElem = 0; iElem < aNbElem; iElem++){
	ADDMSG(MYVALUEDEBUG,anInfo->GetElemNum(iElem)<<", ");
      }
      ADDMSG(MYDEBUG,endl);
    }

    ADDMSG(MYDEBUG,endl);
#endif
    
    return anInfo;
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PPolygoneInfo TWrapper::GetPPolygoneInfo(const PMeshInfo& theMeshInfo,
					   EEntiteMaillage theEntity, 
					   EGeometrieElement theGeom, 
					   EConnectivite theConn)
  {
    TInt aNbElem = GetNbPolygones(*theMeshInfo,theEntity,theGeom,theConn);
    TInt aNbConn = GetNbPolygoneConn(*theMeshInfo,theEntity,theGeom,theConn);
    PPolygoneInfo anInfo = CrPolygoneInfo(theMeshInfo,aNbElem,aNbConn,theEntity,theGeom,theConn);
    GetPolygoneInfo(*anInfo);
    return anInfo;
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PPolyedreInfo TWrapper::GetPPolyedreInfo(const PMeshInfo& theMeshInfo,
					   EEntiteMaillage theEntity, 
					   EGeometrieElement theGeom, 
					   EConnectivite theConn)
  {
    TInt aNbElem  = GetNbPolyedres(*theMeshInfo,theEntity,theGeom,theConn);
    TInt aNbConn = 0;
    TInt aNbFaces = 0;
    GetNbPolyedreConnF(*theMeshInfo,theConn,aNbFaces,aNbConn);
    PPolyedreInfo anInfo = CrPolyedreInfo(theMeshInfo,aNbElem,aNbConn,aNbFaces,theEntity,theGeom,theConn);
    GetPolyedreInfo(*anInfo);
    return anInfo;
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PCellInfo TWrapper::GetPCellInfo(const PMeshInfo& theMeshInfo,
				   EEntiteMaillage theEntity, 
				   EGeometrieElement theGeom, 
				   EConnectivite theConn,
				   TErr* theErr)
  {
    TInt aNbElem = GetNbCells(*theMeshInfo,theEntity,theGeom,theConn);
    PCellInfo anInfo = CrCellInfo(theMeshInfo,aNbElem,theEntity,theGeom,theConn);
    GetCellInfo(*anInfo,theErr);

#ifdef _DEBUG_
    TInt aConnDim = anInfo->GetConnDim();
    INITMSG(MYDEBUG,"GetPCellInfo - theEntity = "<<theEntity<<"; theGeom = "<<theGeom<<"; aConnDim: "<<aConnDim<<"\n");
    BEGMSG(MYDEBUG,"GetPCellInfo - aNbElem: "<<aNbElem<<": ");
    for(TInt iElem = 0; iElem < aNbElem; iElem++){
      for(TInt iConn = 0; iConn < aConnDim; iConn++){
	ADDMSG(MYVALUEDEBUG,anInfo->GetConn(iElem,iConn)<<",");
      }
      ADDMSG(MYVALUEDEBUG," ");
    }
    ADDMSG(MYDEBUG,endl);

    BEGMSG(MYDEBUG,"GetPCellInfo - GetFamNum: ");
    for(TInt iElem = 0; iElem < aNbElem; iElem++){
      ADDMSG(MYVALUEDEBUG,anInfo->GetFamNum(iElem)<<", ");
    }
    ADDMSG(MYDEBUG,endl);

    if(anInfo->IsElemNum()){
      BEGMSG(MYDEBUG,"GetPCellInfo - GetElemNum: ");
      for(TInt iElem = 0; iElem < aNbElem; iElem++){
	ADDMSG(MYVALUEDEBUG,anInfo->GetElemNum(iElem)<<", ");
      }
      ADDMSG(MYDEBUG,endl);
    }

    ADDMSG(MYDEBUG,endl);
#endif
    
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PFieldInfo TWrapper::GetPFieldInfo(const PMeshInfo& theMeshInfo, 
				     TInt theId,
				     TErr* theErr)
  {
    TInt aNbComp = GetNbComp(theId);
    PFieldInfo anInfo = CrFieldInfo(theMeshInfo,aNbComp);
    GetFieldInfo(theId,*anInfo,theErr);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PTimeStampInfo TWrapper::GetPTimeStampInfo(const PFieldInfo& theFieldInfo,
					     EEntiteMaillage theEntity,
					     const MED::TGeom& theGeom,
					     TInt theId,
					     TErr* theErr)
  {
    PTimeStampInfo anInfo = CrTimeStampInfo(theFieldInfo,theEntity,theGeom);
    GetTimeStampInfo(theId,*anInfo,theErr);
    return anInfo;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  PTimeStampVal TWrapper::GetPTimeStampVal(const PTimeStampInfo& theTimeStampInfo,
					   TErr* theErr)
  {
    PTimeStampVal anInfo = CrTimeStampVal(theTimeStampInfo);
    GetTimeStampVal(*anInfo,theErr);
    return anInfo;
  }
}
