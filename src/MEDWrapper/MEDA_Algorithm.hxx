//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MEDA_Algorithm_HeaderFile
#define MEDA_Algorithm_HeaderFile

#include "MEDA_Structures.hxx"
#include "MEDA_Wrapper.hxx"

#include <set>

namespace MEDA{
  using namespace std;
  using namespace boost;


  //---------------------------------------------------------------
  typedef set<PCellInfo> TCellSet;
  typedef map<med_entite_maillage,TCellSet> TCellGroup;

  TCellGroup GetCellsByEntity(TWrapper& theWrapper, 
			      const PMeshInfo& theMeshInfo,
			      const MED::TEntityInfo theEntityInfo);


  //---------------------------------------------------------------
  typedef set<PFamilyInfo> TFamilyGroup;

  TFamilyGroup GetFamilies(TWrapper& theWrapper, 
			   const PMeshInfo& theMeshInfo);


  //---------------------------------------------------------------
  typedef map<med_entite_maillage,TFamilyGroup> TFamilyByEntity;
  
  TFamilyByEntity GetFamiliesByEntity(TWrapper& theWrapper, 
				      const PNodeInfo& theNodeInfo, 
				      const TCellGroup& theCellGroup,
				      const TFamilyGroup& theFamilyGroup);


  //---------------------------------------------------------------
  typedef map<string,TFamilyGroup> TGroupInfo;
  
  TGroupInfo GetFamiliesByGroup(const TFamilyGroup& theGroupInfo);


  //---------------------------------------------------------------
  typedef set<PTimeStampInfo> TTimeStampSet;
  typedef map<PFieldInfo,TTimeStampSet> TTimeStampGroup;

  TTimeStampGroup GetFieldsByEntity(TWrapper& theWrapper, 
				    const PMeshInfo& theMeshInfo,
				    const MED::TEntityInfo theEntityInfo);


  //---------------------------------------------------------------
  typedef map<med_entite_maillage,TTimeStampGroup> TFieldGroup;

  TFieldGroup GetFieldsByEntity(const TTimeStampGroup& theTimeStampGroup);

};

#endif
