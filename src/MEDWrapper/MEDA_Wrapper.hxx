//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MEDA_Wrapper_HeaderFile
#define MEDA_Wrapper_HeaderFile

#include "MEDA_Structures.hxx"

namespace MEDA{
  class TWrapper{
    TWrapper();
    TWrapper(const TWrapper&);
  protected:
    MED::PWrapper myWrapper;
    
  public:
    TWrapper(const string& theFileName);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static PMeshInfo CrMeshInfo(med_int theDim = 0,
				const string& theValue = "");

    static PMeshInfo CrMeshInfo(const PMeshInfo& theMeshInfo);

    med_int GetNbMeshes();

    PMeshInfo GetMeshInfo(med_int theId);

    void SetMeshInfo(const PMeshInfo& theInfo);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static PFamilyInfo CrFamilyInfo(const PMeshInfo& theMeshInfo,
				    med_int theNbGroup = 0, 
				    med_int theNbAttr = 0,
				    med_int theId = 0,
				    const string& theValue = "");

    static PFamilyInfo CrFamilyInfo(const PMeshInfo& theMeshInfo,
				    const string& theValue,
				    med_int theId,
				    const MED::TStringSet& theGroupNames, 
				    const MED::TStringVector& theAttrDescs = MED::TStringVector(), 
				    const MED::TIntVector& theAttrIds = MED::TIntVector(), 
				    const MED::TIntVector& theAttrVals = MED::TIntVector());

    static PFamilyInfo CrFamilyInfo(const PMeshInfo& theMeshInfo,
				    const PFamilyInfo& theInfo);

    med_int GetNbFamilies(const PMeshInfo& theMeshInfo);

    PFamilyInfo GetFamilyInfo(const PMeshInfo& theMeshInfo, med_int theId);

    void SetFamilyInfo(const PFamilyInfo& theInfo);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static PNodeInfo CrNodeInfo(const PMeshInfo& theMeshInfo, 
				med_int theNbElem,
				med_repere theSystem = MED_CART, 
				med_booleen theIsElemNum = MED_VRAI,
				med_booleen theIsElemNames = MED_VRAI);

    static PNodeInfo CrNodeInfo(const PMeshInfo& theMeshInfo, 
				med_repere theSystem, 
				const MED::TFloatVector& theNodeCoords,
				const MED::TStringVector& theCoordNames,
				const MED::TStringVector& theCoordUnits,
				const MED::TIntVector& theFamilyNums,
				const MED::TIntVector& theElemNums,
				const MED::TStringVector& theElemNames = MED::TStringVector());

    static PNodeInfo CrNodeInfo(const PMeshInfo& theMeshInfo,
				const PNodeInfo& theInfo);

    PNodeInfo GetNodeInfo(const PMeshInfo& theMeshInfo);

    void SetNodeInfo(const PNodeInfo& theInfo);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    MED::TEntityInfo GetEntityInfo(const PMeshInfo& theMeshInfo);

    static PCellInfo CrCellInfo(const PMeshInfo& theMeshInfo, 
				med_int theNbElem,
				med_entite_maillage theEntity, 
				med_geometrie_element theGeom,
				med_connectivite theConn = MED_NOD,
				med_booleen theIsElemNum = MED_VRAI,
				med_booleen theIsElemNames = MED_VRAI);

    static PCellInfo CrCellInfo(const PMeshInfo& theMeshInfo, 
				med_entite_maillage theEntity, 
				med_geometrie_element theGeom,
				med_connectivite theConn,
				const MED::TIntVector& theConnectivities,
				const MED::TIntVector& theFamilyNums,
				const MED::TIntVector& theElemNums,
				const MED::TStringVector& theElemNames = MED::TStringVector());

    static PCellInfo CrCellInfo(const PMeshInfo& theMeshInfo,
				const PCellInfo& theInfo);

    PCellInfo GetCellInfo(const PMeshInfo& theMeshInfo, 
			  med_entite_maillage theEntity, 
			  med_geometrie_element theGeom, 
			  med_connectivite theConn = MED_NOD);

    void SetCellInfo(const PCellInfo& theInfo);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static PFieldInfo CrFieldInfo(const PMeshInfo& theMeshInfo, 
				  med_int theNbComp = 0,
				  med_type_champ theType = MED_REEL64,
				  const string& theValue = "");

    static PFieldInfo CrFieldInfo(const PMeshInfo& theMeshInfo,
				  const PFieldInfo& theInfo);
    med_int GetNbFields();

    PFieldInfo GetFieldInfo(const PMeshInfo& theMeshInfo, med_int theId);

    void SetFieldInfo(const PFieldInfo& theInfo);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static PTimeStampInfo CrTimeStampInfo(const PFieldInfo& theFieldInfo, 
					  med_entite_maillage theEntity,
					  const MED::TGeom& theGeom);

    static PTimeStampInfo CrTimeStampInfo(const PFieldInfo& theFieldInfo,
					  const PTimeStampInfo& theInfo);

    med_int GetNbTimeStamps(const PFieldInfo& theFieldInfo, 
			    const MED::TEntityInfo& theEntityInfo,
			    med_entite_maillage& theEntity,
			    MED::TGeom& theGeom);

    PTimeStampInfo GetTimeStampInfo(const PFieldInfo& theFieldInfo,
				    med_int theId,
				    med_entite_maillage theEntity,
				    const MED::TGeom& theGeom);

    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    static PTimeStampVal CrTimeStampVal(const PTimeStampInfo& theTimeStampInfo);

    static PTimeStampVal CrTimeStampVal(const PTimeStampInfo& theTimeStampInfo,
					const PTimeStampVal& theInfo);

    PTimeStampVal GetTimeStampVal(const PTimeStampInfo& theTimeStampInfo);

    void SetTimeStamp(const PTimeStampVal& theTimeStampVal);
  };

};


#endif
