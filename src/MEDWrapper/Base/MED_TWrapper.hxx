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

#ifndef MED_TWrapper_HeaderFile
#define MED_TWrapper_HeaderFile

#include "MED_TStructures.hxx"
#include "MED_Wrapper.hxx"

namespace MED{

  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM>
  class TTWrapper: public TWrapper{
  public:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PMeshInfo CrMeshInfo(TInt theDim = 0,
				 const std::string& theValue = "",
				 EMaillage theType = eNON_STRUCTURE,
				 const std::string& theDesc = "")
    {
      return PMeshInfo(new TTMeshInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theDim,
			theValue,
			theType,
			theDesc));
    }

    virtual PMeshInfo CrMeshInfo(const PMeshInfo& theInfo)
    {
      return PMeshInfo(new TTMeshInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>(theInfo));
    }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PFamilyInfo CrFamilyInfo(const PMeshInfo& theMeshInfo,
				     TInt theNbGroup = 0, 
				     TInt theNbAttr = 0,
				     TInt theId = 0,
				     const std::string& theValue = "")
    {
      return PFamilyInfo(new TTFamilyInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			 (theMeshInfo,
			  theNbGroup,
			  theNbAttr,
			  theId,
			  theValue));
    }

    virtual PFamilyInfo CrFamilyInfo(const PMeshInfo& theMeshInfo,
				     const std::string& theValue,
				     TInt theId,
				     const MED::TStringSet& theGroupNames, 
				     const MED::TStringVector& theAttrDescs = MED::TStringVector(), 
				     const MED::TIntVector& theAttrIds = MED::TIntVector(), 
				     const MED::TIntVector& theAttrVals = MED::TIntVector())
    {
      return PFamilyInfo(new TTFamilyInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			 (theMeshInfo,
			  theValue,
			  theId,
			  theGroupNames,
			  theAttrDescs,
			  theAttrIds,
			  theAttrVals));
    }

    virtual PFamilyInfo CrFamilyInfo(const PMeshInfo& theMeshInfo,
    				     const PFamilyInfo& theInfo)
    {
      return PFamilyInfo(new TTFamilyInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			 (theMeshInfo,
			  theInfo));
    }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PNodeInfo CrNodeInfo(const PMeshInfo& theMeshInfo, 
				 TInt theNbElem,
				 ERepere theSystem = eCART, 
				 EBooleen theIsElemNum = eVRAI,
				 EBooleen theIsElemNames = eVRAI)
    {
      return PNodeInfo(new TTNodeInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theMeshInfo,
			theNbElem,
			theSystem,
			theIsElemNum,
			theIsElemNames));
    }

    virtual PNodeInfo CrNodeInfo(const PMeshInfo& theMeshInfo, 
				 ERepere theSystem, 
				 const TFloatVector& theNodeCoords,
				 const TStringVector& theCoordNames,
				 const TStringVector& theCoordUnits,
				 const TIntVector& theFamilyNums,
				 const TIntVector& theElemNums,
				 const TStringVector& theElemNames = TStringVector())
    {
      return PNodeInfo(new TTNodeInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theMeshInfo,
			theSystem,
			theNodeCoords,
			theCoordNames,
			theCoordUnits,
			theFamilyNums,
			theElemNums,
			theElemNames));
    }

    virtual PNodeInfo CrNodeInfo(const PMeshInfo& theMeshInfo,
				 const PNodeInfo& theInfo)
    {
      return PNodeInfo(new TTNodeInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theMeshInfo,
			theInfo));
    }
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PCellInfo CrCellInfo(const PMeshInfo& theMeshInfo, 
				 TInt theNbElem,
				 EEntiteMaillage theTEntity, 
				 EGeometrieElement theTGeom,
				 EConnectivite theTConn = eNOD,
				 EBooleen theIsElemNum = eVRAI,
				 EBooleen theIsElemNames = eVRAI)
    {
      return PCellInfo(new TTCellInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theMeshInfo,
			theNbElem,
			theTEntity,
			theTGeom,
			theTConn,
			theIsElemNum,
			theIsElemNames));
    }

    virtual PCellInfo CrCellInfo(const PMeshInfo& theMeshInfo, 
				 EEntiteMaillage theTEntity, 
				 EGeometrieElement theTGeom,
				 EConnectivite theTConn,
				 const TIntVector& theConnectivities,
				 const TIntVector& theFamilyNums,
				 const TIntVector& theElemNums,
				 const TStringVector& theElemNames = TStringVector())
    {
      return PCellInfo(new TTCellInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theMeshInfo,
			theTEntity,
			theTGeom,
			theTConn,
			theConnectivities,
			theFamilyNums,
			theElemNums,
			theElemNames));
    }

    virtual PCellInfo CrCellInfo(const PMeshInfo& theMeshInfo,
				 const PCellInfo& theInfo)
    {
      return PCellInfo(new TTCellInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
		       (theMeshInfo,
			theInfo));
    }
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PFieldInfo CrFieldInfo(const PMeshInfo& theMeshInfo, 
				   TInt theNbComp = 0,
				   ETypeChamp theType = eFLOAT64,
				   const std::string& theValue = "",
				   EBooleen theIsLocal = eVRAI,
				   TInt theNbRef = 1)
    {
      return PFieldInfo(new TTFieldInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			(theMeshInfo,
			 theNbComp,
			 theType,
			 theValue,
			 theIsLocal,
			 theNbRef));
    }

    virtual PFieldInfo CrFieldInfo(const PMeshInfo& theMeshInfo,
				   const PFieldInfo& theInfo)
    {
      return PFieldInfo(new TTFieldInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			(theMeshInfo,
			 theInfo));
    }
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PTimeStampInfo CrTimeStampInfo(const PFieldInfo& theFieldInfo, 
					   EEntiteMaillage theEntity,
					   const TGeom& theGeom,
					   TInt theNbGauss = 0,
					   TInt theNumDt = 0,
					   TInt theNumOrd = 0,
					   TFloat theDt = 0,
					   const std::string& theUnitDt = "",
					   const std::string& theGaussName = "")
    {
      return PTimeStampInfo(new TTTimeStampInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			    (theFieldInfo,
			     theEntity,
			     theGeom,
			     theNbGauss,
			     theNumDt,
			     theNumOrd,
			     theDt,
			     theUnitDt,
			     theGaussName));
    }

    virtual PTimeStampInfo CrTimeStampInfo(const PFieldInfo& theFieldInfo,
					   const PTimeStampInfo& theInfo)
    {
      return PTimeStampInfo(new TTTimeStampInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			    (theFieldInfo,
			     theInfo));
    }
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    virtual PTimeStampVal CrTimeStampVal(const PTimeStampInfo& theTimeStampInfo,
					 const std::string& thePflName = "",
					 EModeProfil thePflMode = eCOMPACT)
    {
      return PTimeStampVal(new TTTimeStampVal<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			   (theTimeStampInfo,
			    thePflName,
			    thePflMode));
    }

    virtual PTimeStampVal CrTimeStampVal(const PTimeStampInfo& theTimeStampInfo,
					 const PTimeStampVal& theInfo)
    {
      return PTimeStampVal(new TTTimeStampVal<nPNOM,nDESC,nIDENT,nNOM,nLNOM>
			   (theTimeStampInfo,
			    theInfo));
    }
    
  };

}


#endif
