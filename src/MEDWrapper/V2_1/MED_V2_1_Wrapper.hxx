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

#ifndef MED_V2_1_Wrapper_HeaderFile
#define MED_V2_1_Wrapper_HeaderFile

#include "MED_TWrapper.hxx"

namespace MED{
  namespace V2_1{

    const TInt PNOM = 8;

    typedef MED::TTMeshInfo<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVMeshInfo;

    typedef MED::TTFamilyInfo<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVFamilyInfo;

    typedef MED::TTNodeInfo<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVNodeInfo;

    typedef MED::TTCellInfo<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVCellInfo;

    typedef MED::TTFieldInfo<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVFieldInfo;

    typedef MED::TTTimeStampInfo<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVTimeStampInfo;

    typedef MED::TTTimeStampVal<PNOM,DESC,IDENT,NOM,LNOM,eV2_1> TVTimeStampVal;

    //---------------------------------------------------------------
    class TFile;
    typedef boost::shared_ptr<TFile> PFile;
    
    typedef enum {eLECT, eECRI, eREMP} EModeAcces; 

    //---------------------------------------------------------------
    class TVWrapper: public MED::TTWrapper<PNOM,DESC,IDENT,NOM,LNOM,eV2_1>
    {
      TVWrapper();
      TVWrapper(const TVWrapper&);
      TVWrapper& operator=(const TVWrapper&);
      
    public:
      TVWrapper(const std::string& theFileName);


      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      virtual TInt GetNbMeshes(TErr* theErr = NULL);
      
      virtual void GetMeshInfo(TInt theMeshId, MED::TMeshInfo&,
			       TErr* theErr = NULL);

      virtual void SetMeshInfo(const MED::TMeshInfo& theInfo,
			       TErr* theErr = NULL);
      
      void SetMeshInfo(const MED::TMeshInfo& theInfo,
		       EModeAcces theMode,
		       TErr* theErr = NULL);

      
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      virtual TInt GetNbFamilies(const MED::TMeshInfo& theMeshInfo,
				 TErr* theErr = NULL);
      virtual TInt GetNbFamAttr(TInt theFamId, 
				const MED::TMeshInfo& theInfo,
				TErr* theErr = NULL);
      virtual TInt GetNbFamGroup(TInt theFamId, 
				 const MED::TMeshInfo& theInfo,
				 TErr* theErr = NULL);
      
      virtual void GetFamilyInfo(TInt theFamId, 
				 MED::TFamilyInfo& theInfo,
				 TErr* theErr = NULL);

      virtual void SetFamilyInfo(const MED::TFamilyInfo& theInfo,
				 TErr* theErr = NULL);
      
      void SetFamilyInfo(const MED::TFamilyInfo& theInfo,
			 EModeAcces theMode,
			 TErr* theErr = NULL);

      
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      virtual TInt GetNbNodes(const MED::TMeshInfo& theMeshInfo,
			      TErr* theErr = NULL);
      
      virtual void GetNodeInfo(MED::TNodeInfo& theInfo,
			       TErr* theErr = NULL);

      virtual void SetNodeInfo(const MED::TNodeInfo& theInfo,
			       TErr* theErr = NULL);
      
      void SetNodeInfo(const MED::TNodeInfo& theInfo,
		       EModeAcces theMode,
		       TErr* theErr = NULL);

      
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      virtual TEntityInfo GetEntityInfo(const MED::TMeshInfo& theMeshInfo,
					EConnectivite theTConn = eNOD,
					TErr* theErr = NULL);
      
      virtual TInt GetNbCells(const MED::TMeshInfo& theMeshInfo, 
			      EEntiteMaillage, 
			      EGeometrieElement, 
			      EConnectivite theTConn = eNOD,
			      TErr* theErr = NULL);
      
      virtual void GetCellInfo(MED::TCellInfo& theInfo,
			       TErr* theErr = NULL);

      virtual void SetCellInfo(const MED::TCellInfo& theInfo,
			       TErr* theErr = NULL);
      
      void SetCellInfo(const MED::TCellInfo& theInfo,
		       EModeAcces theMode,
		       TErr* theErr = NULL);

      
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      virtual TInt GetNbFields(TErr* theErr = NULL);
      
      virtual TInt GetNbComp(TInt theFieldId,
			     TErr* theErr = NULL);
      
      virtual void GetFieldInfo(TInt theFieldId, 
				MED::TFieldInfo& theInfo,
				TErr* theErr = NULL);

      virtual void SetFieldInfo(const MED::TFieldInfo& theInfo,
				TErr* theErr = NULL);
      
      void SetFieldInfo(const MED::TFieldInfo& theInfo,
			EModeAcces theMode,
			TErr* theErr = NULL);

      
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      virtual TInt GetNbTimeStamps(const MED::TFieldInfo& theInfo, 
				   const MED::TEntityInfo& theEntityInfo,
				   EEntiteMaillage& theEntity,
				   TGeom& theGeom,
				   TErr* theErr = NULL);
      
      virtual void GetTimeStampInfo(TInt theTimeStampId, 
				    MED::TTimeStampInfo& theInfo,
				    TErr* theErr = NULL);

      virtual void GetTimeStampVal(MED::TTimeStampVal& theVal,
				   TErr* theErr = NULL);
      
      virtual void SetTimeStamp(const MED::TTimeStampVal& theTimeStampVal,
				TErr* theErr = NULL);
      
      void SetTimeStamp(const MED::TTimeStampVal& theTimeStampVal,
			EModeAcces theMode,
			TErr* theErr = NULL);

    protected:
      PFile myFile;
    };
  }
}

#endif
