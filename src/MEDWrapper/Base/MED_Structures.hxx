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

#ifndef MED_Structures_HeaderFile
#define MED_Structures_HeaderFile

#include "MED_Common.hxx"

namespace MED{

  //---------------------------------------------------------------
  typedef std::vector<char> TString;

  std::string GetString(TInt theId, TInt theStep, 
			const TString& theString);

  void SetString(TInt theId, TInt theStep, 
		 TString& theString, 
		 const std::string& theValue);

  //---------------------------------------------------------------
  struct TBase
  {
    virtual ~TBase() {} 
  };


  //---------------------------------------------------------------
  struct TNameInfo: virtual TBase
  {
    TString myName;
    virtual std::string GetName() const = 0;
    virtual void SetName(const std::string& theValue) = 0;
  };


  //---------------------------------------------------------------
  struct TMeshInfo: virtual TNameInfo
  {
    TInt myDim;
    TInt GetDim() const { return myDim;}

    EMaillage myType;
    EMaillage GetType() const { return myType;}

    TString myDesc;
    virtual std::string GetDesc() const = 0;
    virtual void SetDesc(const std::string& theValue) = 0;
  };
  

  //---------------------------------------------------------------
  typedef std::vector<TInt> TFamAttr;

  struct TFamilyInfo: virtual TNameInfo
  {
    PMeshInfo myMeshInfo;
    const PMeshInfo& GetMeshInfo() const { return myMeshInfo;}

    TInt myId;
    TInt GetId() const { return myId;}
    void SetId(TInt theId) { myId = theId;}

    TInt myNbGroup;
    TInt GetNbGroup() const { return myNbGroup;}

    TString myGroupNames;
    virtual std::string GetGroupName(TInt theId) const = 0;
    virtual void SetGroupName(TInt theId, const std::string& theValue) = 0;

    TInt myNbAttr;
    TInt GetNbAttr() const { return myNbAttr;}

    TFamAttr myAttrId;
    TInt GetAttrId(TInt theId) const;
    void SetAttrId(TInt theId, TInt theVal);

    TFamAttr myAttrVal;
    TInt GetAttrVal(TInt theId) const;
    void SetAttrVal(TInt theId, TInt theVal);

    TString myAttrDesc;
    virtual std::string GetAttrDesc(TInt theId) const = 0;
    virtual void SetAttrDesc(TInt theId, const std::string& theValue) = 0;
  };


  //---------------------------------------------------------------
  typedef std::vector<TInt> TElemNum;
  
  struct TElemInfo: virtual TBase
  {
    PMeshInfo myMeshInfo;
    const PMeshInfo& GetMeshInfo() const { return myMeshInfo;}

    TInt myNbElem;
    TInt GetNbElem() const { return myNbElem;}
    
    TElemNum myFamNum;
    TInt GetFamNum(TInt theId) const;
    void SetFamNum(TInt theId, TInt theVal);

    EBooleen myIsElemNum;
    EBooleen IsElemNum() const { return myIsElemNum;}

    TElemNum myElemNum;
    TInt GetElemNum(TInt theId) const;
    void SetElemNum(TInt theId, TInt theVal);

    EBooleen myIsElemNames;
    EBooleen IsElemNames() const { return myIsElemNames;}

    TString myElemNames;
    virtual std::string GetElemName(TInt theId) const = 0;
    virtual void SetElemName(TInt theId, const std::string& theValue) = 0;
  };


  //---------------------------------------------------------------
  typedef std::vector<TFloat> TNodeCoord;

  struct TNodeInfo: virtual TElemInfo
  {
    TNodeCoord myCoord;
    TFloat GetNodeCoord(TInt theId, TInt theComp) const;
    void SetNodeCoord(TInt theId, TInt theComp, TFloat theVal);

    ERepere mySystem;
    ERepere GetSystem() const { return mySystem;}
    void SetSystem(ERepere theSystem) { mySystem = theSystem;}

    TString myCoordNames;
    virtual std::string GetCoordName(TInt theId) const = 0;
    virtual void SetCoordName(TInt theId, const std::string& theValue) = 0;

    TString myCoordUnits;
    virtual std::string GetCoordUnit(TInt theId) const = 0;
    virtual void SetCoordUnit(TInt theId, const std::string& theValue) = 0;
  };


  //---------------------------------------------------------------
  struct TCellInfo: virtual TElemInfo
  {
    EEntiteMaillage myTEntity;
    EEntiteMaillage GetEntity() const { return myTEntity;}

    EGeometrieElement myTGeom;
    EGeometrieElement GetGeom() const { return myTGeom;}

    EConnectivite myTConn;
    EConnectivite GetConn() const { return myTConn;}

    virtual TInt GetConnDim() const = 0;

    TElemNum myConn;
    TInt GetConn(TInt theElemId, TInt theConnId) const;
    void SetConn(TInt theElemId, TInt theConnId, TInt theVal);
  };

  //---------------------------------------------------------------
  struct TPolygoneInfo: virtual TElemInfo
  {
    EEntiteMaillage myTEntity; // MED_FACE|MED_MAILLE
    EEntiteMaillage GetEntity() const { return myTEntity;}

    EGeometrieElement myTGeom; // ePOLYGONE
    EGeometrieElement GetGeom() const { return ePOLYGONE;}

    EConnectivite myTConn; // eNOD|eDESC(eDESC not used)
    EConnectivite GetConn() const { return myTConn;}

    TInt myConnDim;
    TInt GetConnDim() const { return myConnDim;}

    TElemNum myConn; // Table de connectivities
    TElemNum GetConnectivite() const { return myConn;}

    TElemNum myIndex; // Table de indexes
    TElemNum GetIndex() {return myIndex;}
    TInt GetNbConn(TInt theElemId) const;
  };

  //---------------------------------------------------------------
  struct TPolyedreInfo: virtual TElemInfo
  {
    EEntiteMaillage myTEntity; // MED_FACE|MED_MAILLE
    EEntiteMaillage GetEntity() const { return myTEntity;}

    EGeometrieElement myTGeom; // ePOLYEDRE
    EGeometrieElement GetGeom() const { return ePOLYEDRE;}

    EConnectivite myTConn; // eNOD|eDESC(eDESC not used)
    EConnectivite GetConn() const { return myTConn;}

    TInt myNbConn;
    TInt GetNbConn() const { return myNbConn;}

    TElemNum myConn; // Table de connectivities
    TElemNum GetConnectivite() const { return myConn;}
    
    TInt myNbFacesIndex;
    TInt GetNbFacesIndex() const { return myNbFacesIndex;}
    
    TElemNum myFacesIndex; // Table de faces indexes
    TElemNum GetFacesIndex() {return myFacesIndex;}
    
    TElemNum myIndex; // Table de indexes
    TElemNum GetIndex() {return myIndex;}
  };

  //---------------------------------------------------------------
  struct TFieldInfo: virtual TNameInfo
  {
    PMeshInfo myMeshInfo;
    const PMeshInfo& GetMeshInfo() const { return myMeshInfo;}

    ETypeChamp myType;
    ETypeChamp GetType() const { return myType;}

    TInt myNbComp;
    TInt GetNbComp() const { return myNbComp;}

    EBooleen myIsLocal;
    EBooleen GetIsLocal() const { return myIsLocal;}

    TInt myNbRef;
    TInt GetNbRef() const { return myNbRef;}

    TString myCompNames; 
    virtual std::string GetCompName(TInt theId) const = 0;
    virtual void SetCompName(TInt theId, const std::string& theValue) = 0;

    TString myUnitNames; 
    virtual std::string GetUnitName(TInt theId) const = 0;
    virtual void SetUnitName(TInt theId, const std::string& theValue) = 0;
  };


  //---------------------------------------------------------------
  struct TTimeStampInfo: virtual TBase
  {
    PFieldInfo myFieldInfo;
    const PFieldInfo& GetFieldInfo() const { return myFieldInfo;}

    EEntiteMaillage myEntity;
    EEntiteMaillage GetEntity() const { return myEntity;}

    TGeom myGeom;
    const TGeom& GetGeom() const { return myGeom;}

    TInt myNbGauss, myNumDt, myNumOrd;
    TInt GetNbGauss() const { return myNbGauss;}
    TInt GetNumDt() const { return myNumDt;}
    TInt GetNumOrd() const { return myNumOrd;}

    TFloat myDt;
    TFloat GetDt() const { return myDt;}

    TString myGaussName;
    virtual std::string GetGaussName() const = 0;
    virtual void SetGaussName(const std::string& theValue) = 0;

    TString myUnitDt;
    virtual std::string GetUnitDt() const = 0;
    virtual void SetUnitDt(const std::string& theValue) = 0;
  };
  

  //---------------------------------------------------------------
  typedef std::vector<TFloat> TValue;
  typedef std::map<EGeometrieElement,TValue> TMeshValue;

  struct TTimeStampVal: virtual TBase
  {
    PTimeStampInfo myTimeStampInfo;
    const PTimeStampInfo& GetTimeStampInfo() const { return myTimeStampInfo;}

    TMeshValue myMeshValue;
    TFloat GetVal(EGeometrieElement theGeom, TInt theId, 
		  TInt theComp, TInt theGauss = 0) const;

    void SetVal(EGeometrieElement theGeom, TInt theId, 
		TInt theComp, TFloat theVal, TInt theGauss = 0);
    EModeProfil myPflMode;
    EModeProfil GetPflMode() const { return myPflMode;}
    void GetPflMode(EModeProfil theVal) { myPflMode = theVal;}

    TString myPflName;
    virtual std::string GetPflName() const = 0;
    virtual void SetPflName(const std::string& theValue) = 0;
  };

}

#endif
