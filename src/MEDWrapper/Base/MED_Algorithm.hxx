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

#ifndef MED_Algorithm_HeaderFile
#define MED_Algorithm_HeaderFile

#include "MED_Structures.hxx"
#include "MED_Wrapper.hxx"

#include <set>

namespace MED{
  //---------------------------------------------------------------
  typedef std::set<PElemInfo> TElemSet;
  typedef std::map<EEntiteMaillage,TElemSet> TElemGroup;

  TElemGroup GetElemsByEntity(TWrapper& theWrapper, 
			      const PMeshInfo& theMeshInfo,
			      const MED::TEntityInfo& theEntityInfo);


  //---------------------------------------------------------------
  typedef std::set<PFamilyInfo> TFamilyGroup;

  TFamilyGroup GetFamilies(TWrapper& theWrapper, 
			   const PMeshInfo& theMeshInfo);


  //---------------------------------------------------------------
  typedef std::map<EEntiteMaillage,TFamilyGroup> TFamilyByEntity;
  
  TFamilyByEntity GetFamiliesByEntity(TWrapper& theWrapper, 
				      const PNodeInfo& theNodeInfo, 
				      const TElemGroup& theElemGroup,
				      const TFamilyGroup& theFamilyGroup);


  //---------------------------------------------------------------
  typedef std::map<std::string,TFamilyGroup> TGroupInfo;
  
  TGroupInfo GetFamiliesByGroup(const TFamilyGroup& theGroupInfo);

  
  //---------------------------------------------------------------
  typedef std::set<PTimeStampInfo> TTimeStampSet;
  typedef std::map<PFieldInfo,TTimeStampSet> TTimeStampGroup;

  TTimeStampGroup GetFieldsByEntity(TWrapper& theWrapper, 
				    const PMeshInfo& theMeshInfo,
				    const MED::TEntityInfo& theEntityInfo);


  //---------------------------------------------------------------
  typedef std::map<EEntiteMaillage,TTimeStampGroup> TFieldGroup;

  TFieldGroup GetFieldsByEntity(const TTimeStampGroup& theTimeStampGroup);

}

#endif
