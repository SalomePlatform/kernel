//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MED_Structures_HeaderFile
#define MED_Structures_HeaderFile

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif

#if defined __GNUC_2__
#include <vector>
#endif
#include <valarray>

#include "MED_Common.hxx"

namespace MED{
  using namespace std;
  using namespace boost;

//---------------------------------------------------------------
#if defined __GNUC_2__
  typedef vector<char> TString;
#else
  typedef valarray<char> TString;
#endif

  string GetString(med_int theId, med_int theStep, const TString& theString);
  void SetString(med_int theId, med_int theStep, TString& theString, 
		 const string& theValue);


//---------------------------------------------------------------
  struct TNameInfo{
    TNameInfo(const string& theValue = "");
    virtual ~TNameInfo() {} 

    TString myName;
    string GetName() const;
    void SetName(const string& theValue);
  };


//---------------------------------------------------------------
  struct TMeshInfo: TNameInfo{
    TMeshInfo(med_int theDim = 0,
	      const string& theValue = "");

    med_int myDim;
  };
  

//---------------------------------------------------------------
  typedef valarray<med_int> TFamAttr;

  struct TFamilyInfo: TNameInfo{
    TFamilyInfo(const PMeshInfo& theMeshInfo,
		med_int theNbGroup = 0, 
		med_int theNbAttr = 0,
		med_int theId = 0,
		const string& theValue = "");

    TFamilyInfo(const PMeshInfo& theMeshInfo,
		const string& theValue,
		med_int theId,
		const TStringSet& theGroupNames, 
		const TStringVector& theAttrDescs = TStringVector(), 
		const TIntVector& theAttrIds = TIntVector(), 
		const TIntVector& theAttrVals = TIntVector());

    PMeshInfo myMeshInfo;
    med_int myId;

    med_int myNbGroup;
    TString myGroupNames;
    string GetGroupName(med_int theId) const;
    void SetGroupName(med_int theId, const string& theValue);

    med_int myNbAttr;
    TFamAttr myAttrId;
    TFamAttr myAttrVal;

    TString myAttrDesc;
    string GetAttrDesc(med_int theId) const;
    void SetAttrDesc(med_int theId, const string& theValue);
  };


//---------------------------------------------------------------
  typedef valarray<med_int> TElemNum;

  struct TElemInfo{
    TElemInfo(const PMeshInfo& theMeshInfo, 
	      med_int theNbElem,
	      med_booleen theIsElemNum = MED_VRAI,
	      med_booleen theIsElemNames = MED_VRAI);
    TElemInfo(const PMeshInfo& theMeshInfo, 
	      const TIntVector& theFamilyNums,
	      const TIntVector& theElemNums,
	      const TStringVector& theElemNames = TStringVector());
    virtual ~TElemInfo() {} 

    PMeshInfo myMeshInfo;
    med_int myNbElem;
    
    TElemNum myFamNum;

    med_booleen myIsElemNum;
    TElemNum myElemNum;

    med_booleen myIsElemNames;
    TString myElemNames;
    string GetElemName(med_int theId) const;
    void SetElemName(med_int theId, const string& theValue);
  };


//---------------------------------------------------------------
  typedef valarray<med_float> TNodeCoord;

  struct TNodeInfo: TElemInfo{
    TNodeInfo(const PMeshInfo& theMeshInfo, 
	      med_int theNbElem,
	      med_repere theSystem = MED_CART, 
	      med_booleen theIsElemNum = MED_VRAI,
	      med_booleen theIsElemNames = MED_VRAI);

    TNodeInfo(const PMeshInfo& theMeshInfo, 
	      med_repere theSystem, 
	      const TFloatVector& theNodeCoords,
	      const TStringVector& theCoordNames,
	      const TStringVector& theCoordUnits,
	      const TIntVector& theFamilyNums,
	      const TIntVector& theElemNums,
	      const TStringVector& theElemNames = TStringVector());

    TNodeCoord myCoord;
    med_repere mySystem;

    TString myCoordNames;
    string GetCoordName(med_int theId) const;
    void SetCoordName(med_int theId, const string& theValue);

    TString myCoordUnits;
    string GetCoordUnit(med_int theId) const;
    void SetCoordUnit(med_int theId, const string& theValue);
  };


//---------------------------------------------------------------
  struct TCellInfo: TElemInfo{
    TCellInfo(const PMeshInfo& theMeshInfo, 
	      med_int theNbElem,
	      med_entite_maillage theTEntity, 
	      med_geometrie_element theTGeom,
	      med_connectivite theTConn = MED_NOD,
	      med_booleen theIsElemNum = MED_VRAI,
	      med_booleen theIsElemNames = MED_VRAI);

    TCellInfo(const PMeshInfo& theMeshInfo, 
	      med_entite_maillage theTEntity, 
	      med_geometrie_element theTGeom,
	      med_connectivite theTConn,
	      const TIntVector& theConnectivities,
	      const TIntVector& theFamilyNums,
	      const TIntVector& theElemNums,
	      const TStringVector& theElemNames = TStringVector());

    med_entite_maillage myTEntity;
    med_geometrie_element myTGeom;
    med_connectivite myTConn;

    med_int myConnDim;
    TElemNum myConn;
  };


//---------------------------------------------------------------
  struct TFieldInfo: TNameInfo{
    TFieldInfo(const PMeshInfo& theMeshInfo, 
	       med_int theNbComp = 0,
	       med_type_champ theType = MED_REEL64,
	       const string& theValue = "");
    
    PMeshInfo myMeshInfo;
    med_type_champ myType;
    med_int myNbComp;
    
    TString myCompNames; 
    string GetCompName(med_int theId) const;
    void SetCompName(med_int theId, const string& theValue);

    TString myUnitNames; 
    string GetUnitName(med_int theId) const;
    void SetUnitName(med_int theId, const string& theValue);
  };


//---------------------------------------------------------------
  struct TTimeStampInfo{
    TTimeStampInfo(const PFieldInfo& theFieldInfo, 
		   med_entite_maillage theEntity,
		   const TGeom& theGeom);

    PFieldInfo myFieldInfo;
    med_entite_maillage myEntity;
    TGeom myGeom;

    med_int myNbGauss, myNumDt, myNumOrd;
    med_float myDt;

    TString myUnitDt;
    string GetUnitDt() const;
    void SetUnitDt(const string& theValue);
  };
  

  typedef valarray<med_float> TValue;
  typedef map<med_geometrie_element,TValue> TMeshValue;
//---------------------------------------------------------------
  struct TTimeStampVal{
    TTimeStampVal(const PTimeStampInfo& theTimeStampInfo);

    PTimeStampInfo myTimeStampInfo;
    TMeshValue myMeshValue;

    TString myPflName;
    string GetPflName() const;
    void SetPflName(const string& theValue);
  };

};


#endif
