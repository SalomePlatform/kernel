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

#include "MED_V2_2_Wrapper.hxx"
#include "MED_Utilities.hxx"

extern "C"{
#include <med.h>
}

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif



namespace MED{
  namespace V2_2{

    //---------------------------------------------------------------
    class TFile{
      TFile();
      TFile(const TFile&);
      
    public:
      TFile(const std::string& theFileName): 
	myFid(0), 
	myCount(0),
	myFileName(theFileName)
      {}
      
      ~TFile(){ 
	Close();
      }
      
      void Open(EModeAcces theMode, TErr* theErr = NULL){
	if(myCount++ == 0){
	  char* aFileName = const_cast<char*>(myFileName.c_str());
	  myFid = MEDouvrir(aFileName,med_mode_acces(theMode));
	}
	if(theErr)
	  *theErr = TErr(myFid > 0);
	else if(myFid < 0)
	  EXCEPTION(runtime_error,"TFile - MEDouvrir('"<<myFileName<<"',"<<theMode<<")");
      }

      const TIdt& Id() const { 
	if(myFid < 0)
	  EXCEPTION(runtime_error,"TFile - GetFid() < 0");
	return myFid;
      }

      void Close(){ 
	if(--myCount == 0)
	  MEDfermer(myFid);
      }

    protected:
      std::string myFileName;
      TInt myCount;
      TIdt myFid;
    };


    //---------------------------------------------------------------
    class TFileWrapper{
      PFile myFile;

    public:
      TFileWrapper(const PFile& theFile, EModeAcces theMode, TErr* theErr = NULL): 
	myFile(theFile)
      {
	myFile->Open(theMode,theErr);
      }
      
      ~TFileWrapper(){
	myFile->Close();
      }
    };


    //---------------------------------------------------------------
    TVWrapper::TVWrapper(const std::string& theFileName): 
      myFile(new TFile(theFileName))
    {
    }
    
    
    TInt TVWrapper::GetNbMeshes(TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      return MEDnMaa(myFile->Id());
    }
    
    
    void TVWrapper::GetMeshInfo(TInt theMeshId, 
			       MED::TMeshInfo& theInfo,
			       TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return;
      
      med_maillage& aType = static_cast<med_maillage>(theInfo.myType);

      TErr aRet = MEDmaaInfo(myFile->Id(),
			     theMeshId,
			     &theInfo.myName[0],
			     &theInfo.myDim,
			     &aType,
			     &theInfo.myDesc[0]);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetMeshInfo - MEDmaaInfo(...)");
    }
    
    
    void TVWrapper::SetMeshInfo(const MED::TMeshInfo& theInfo,
			       EModeAcces theMode,
			       TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,theMode,theErr);
      
      if(theErr && !*theErr)
	return;
      
      MED::TMeshInfo& anInfo = const_cast<MED::TMeshInfo&>(theInfo);
      
      med_maillage& aType = static_cast<med_maillage>(theInfo.myType);

      TErr aRet = MEDmaaCr(myFile->Id(),
			   &anInfo.myName[0],
			   anInfo.myDim,
			   aType,
			   &anInfo.myDesc[0]);
      if(aRet == 0){
	aRet = MEDunvCr(myFile->Id(),
			&anInfo.myName[0]);
      }
      
      INITMSG(MYDEBUG,"TVWrapper::SetMeshInfo - MED_MODE_ACCES = "<<theMode<<"; aRet = "<<aRet<<endl);
      
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"SetMeshInfo - MEDmaaCr(...)");
    }
    
    
    void TVWrapper::SetMeshInfo(const MED::TMeshInfo& theInfo,
			       TErr* theErr)
    {
      TErr aRet;
      SetMeshInfo(theInfo,eLECTURE_ECRITURE,&aRet);
      
      if(aRet < 0)
	SetMeshInfo(theInfo,eLECTURE_AJOUT,&aRet);

      if(aRet < 0)
	SetMeshInfo(theInfo,eCREATION,theErr);
    }
    
    
    TInt TVWrapper::GetNbFamilies(const MED::TMeshInfo& theInfo,
				 TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      MED::TMeshInfo& anInfo = const_cast<MED::TMeshInfo&>(theInfo);
      return MEDnFam(myFile->Id(),
		     &anInfo.myName[0]);
    }
    
    
    TInt TVWrapper::GetNbFamAttr(TInt theFamId, 
				const MED::TMeshInfo& theInfo,
				TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      MED::TMeshInfo& anInfo = const_cast<MED::TMeshInfo&>(theInfo);
      return MEDnAttribut(myFile->Id(),
			  &anInfo.myName[0],
			  theFamId);
    }
    
    
    TInt TVWrapper::GetNbFamGroup(TInt theFamId, 
				 const MED::TMeshInfo& theInfo,
				 TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      MED::TMeshInfo& anInfo = const_cast<MED::TMeshInfo&>(theInfo);
      return MEDnGroupe(myFile->Id(),
			&anInfo.myName[0],
			theFamId);
    }
    
    
    void TVWrapper::GetFamilyInfo(TInt theFamId, 
				 MED::TFamilyInfo& theInfo,
				 TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return;
      
      MED::TMeshInfo& aMeshInfo = *theInfo.myMeshInfo;
      
      TErr aRet = MEDfamInfo(myFile->Id(),
			     &aMeshInfo.myName[0],
			     theFamId,
			     &theInfo.myName[0],
			     &theInfo.myId,
			     &theInfo.myAttrId[0],
			     &theInfo.myAttrVal[0],
			     &theInfo.myAttrDesc[0],
			     &theInfo.myNbAttr,
			     &theInfo.myGroupNames[0],
			     &theInfo.myNbGroup);
      
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetFamilyInfo - MEDfamInfo(...) - "<<
		  " aMeshInfo.myName = '"<<&aMeshInfo.myName[0]<<
		  "'; theFamId = "<<theFamId<<
		  "; theInfo.myNbGroup = "<<theInfo.myNbGroup<<
		  "; theInfo.myNbAttr = "<<theInfo.myNbAttr);

      INITMSG(MYDEBUG,"GetFamilyInfo - MEDfamInfo(...) - "<<
	      " aMeshInfo.myName = '"<<&aMeshInfo.myName[0]<<
	      "'; theFamId = "<<theFamId<<
	      "; theInfo.myNbGroup = "<<theInfo.myNbGroup<<
	      "; theInfo.myNbAttr = "<<theInfo.myNbAttr);
    }
    
    
    void TVWrapper::SetFamilyInfo(const MED::TFamilyInfo& theInfo,
				 EModeAcces theMode,
				 TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,theMode,theErr);
      
      if(theErr && !*theErr)
	return;
      
      MED::TFamilyInfo& anInfo = const_cast<MED::TFamilyInfo&>(theInfo);
      MED::TMeshInfo& aMeshInfo = *anInfo.myMeshInfo;
      
      TErr aRet = MEDfamCr(myFile->Id(),
			   &aMeshInfo.myName[0],
			   &anInfo.myName[0],
			   anInfo.myId,
			   &anInfo.myAttrId[0],
			   &anInfo.myAttrVal[0],
			   &anInfo.myAttrDesc[0],
			   anInfo.myNbAttr,
			   &anInfo.myGroupNames[0],
			   anInfo.myNbGroup);
      
      INITMSG(MYDEBUG,"TVWrapper::GetFamilyInfo - MED_MODE_ACCES = "<<theMode<<"; aRet = "<<aRet<<endl);
      
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"SetFamilyInfo - MEDfamCr(...)");
    }
    
    
    void TVWrapper::SetFamilyInfo(const MED::TFamilyInfo& theInfo,
				 TErr* theErr)
    {
      TErr aRet;
      SetFamilyInfo(theInfo,eLECTURE_ECRITURE,&aRet);
      
      if(aRet < 0)
	SetFamilyInfo(theInfo,eLECTURE_AJOUT,theErr);
    }
    
    
    TInt TVWrapper::GetNbNodes(const MED::TMeshInfo& theMeshInfo,
			      TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      MED::TMeshInfo& aMeshInfo = const_cast<MED::TMeshInfo&>(theMeshInfo);
      
      return MEDnEntMaa(myFile->Id(),
			&aMeshInfo.myName[0],
			MED_COOR,
			MED_NOEUD,
			med_geometrie_element(0),
			med_connectivite(0));
    }
    
    
    void TVWrapper::GetNodeInfo(MED::TNodeInfo& theInfo,
			       TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return;
      
      MED::TMeshInfo& aMeshInfo = *theInfo.myMeshInfo;

      med_repere& aRepere = static_cast<med_repere>(theInfo.mySystem);
      med_booleen& anIsElemNames = static_cast<med_booleen>(theInfo.myIsElemNames);
      med_booleen& anIsElemNum = static_cast<med_booleen>(theInfo.myIsElemNum);

      TErr aRet = MEDnoeudsLire(myFile->Id(),
				&aMeshInfo.myName[0],
				aMeshInfo.myDim,
				&theInfo.myCoord[0],
				MED_FULL_INTERLACE,
				&aRepere,
				&theInfo.myCoordNames[0],
				&theInfo.myCoordUnits[0],
				&theInfo.myElemNames[0],
				&anIsElemNames,
				&theInfo.myElemNum[0],
				&anIsElemNum,
				&theInfo.myFamNum[0],
				theInfo.myNbElem);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetNodeInfo - MEDnoeudsLire(...)");
    }
    
    
    void TVWrapper::SetNodeInfo(const MED::TNodeInfo& theInfo,
			       EModeAcces theMode,
			       TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,theMode,theErr);
      
      if(theErr && !*theErr)
	return;
      
      MED::TNodeInfo& anInfo = const_cast<MED::TNodeInfo&>(theInfo);
      MED::TMeshInfo& aMeshInfo = *anInfo.myMeshInfo;
      
      med_repere& aRepere = static_cast<med_repere>(theInfo.mySystem);
      med_booleen& anIsElemNames = static_cast<med_booleen>(theInfo.myIsElemNames);
      med_booleen& anIsElemNum = static_cast<med_booleen>(theInfo.myIsElemNum);

      TErr aRet = MEDnoeudsEcr(myFile->Id(),
			       &aMeshInfo.myName[0],
			       aMeshInfo.myDim,
			       &anInfo.myCoord[0],
			       MED_FULL_INTERLACE,
			       aRepere,
			       &anInfo.myCoordNames[0],
			       &anInfo.myCoordUnits[0],
			       &anInfo.myElemNames[0],
			       anIsElemNames,
			       &anInfo.myElemNum[0],
			       anIsElemNum,
			       &anInfo.myFamNum[0],
			       anInfo.myNbElem);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"SetNodeInfo - MEDnoeudsEcr(...)");
    }
    
    
    void TVWrapper::SetNodeInfo(const MED::TNodeInfo& theInfo,
			       TErr* theErr)
    {
      TErr aRet;
      SetNodeInfo(theInfo,eLECTURE_ECRITURE,&aRet);
      
      if(aRet < 0)
	SetNodeInfo(theInfo,eLECTURE_AJOUT,theErr);
    }
    
    
    TEntityInfo TVWrapper::GetEntityInfo(const MED::TMeshInfo& theMeshInfo,
					EConnectivite theTConn,
					TErr* theErr)
    {
      TEntityInfo anInfo;
      
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return anInfo;
      
      if(TInt aNbElem = GetNbNodes(theMeshInfo)){
	anInfo[eNOEUD][ePOINT1] = aNbElem;
	const TEntity2GeomSet& anEntity2GeomSet = GetEntity2GeomSet();
	TEntity2GeomSet::const_iterator anIter = anEntity2GeomSet.begin();
	TEntity2GeomSet::const_iterator anIterEnd = anEntity2GeomSet.end();
	for(; anIter != anIterEnd; anIter++){
	  const EEntiteMaillage& anEntity = anIter->first;
	  const TGeomSet& aGeomSet = anIter->second;
	  TGeomSet::const_iterator anIter2 = aGeomSet.begin();
	  TGeomSet::const_iterator anIterEnd2 = aGeomSet.end();
	  for(; anIter2 != anIterEnd2; anIter2++){
	    const EGeometrieElement& aGeom = *anIter2;
	    if(TInt aNb = GetNbCells(theMeshInfo,anEntity,aGeom,theTConn,theErr)){
	      anInfo[anEntity][aGeom] = aNb;
	    }
	  }
	}
      }
      return anInfo;
    }
    
    
    TInt TVWrapper::GetNbCells(const MED::TMeshInfo& theMeshInfo, 
			      EEntiteMaillage theTEntity, 
			      EGeometrieElement theTGeom, 
			      EConnectivite theTConn,
			      TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      MED::TMeshInfo& aMeshInfo = const_cast<MED::TMeshInfo&>(theMeshInfo);
      
      med_entite_maillage& anEntity = static_cast<med_entite_maillage>(theTEntity);
      med_geometrie_element& aGeom = static_cast<med_geometrie_element>(theTGeom);
      med_connectivite& aConn = static_cast<med_connectivite>(theTConn);

      return MEDnEntMaa(myFile->Id(),
			&aMeshInfo.myName[0],
			MED_CONN,
			anEntity,
			aGeom,
			aConn); 
    }
    
    
    void TVWrapper::GetCellInfo(MED::TCellInfo& theInfo,
			       TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);

      if(theErr && !*theErr)
	return;
      
      MED::TMeshInfo& aMeshInfo = *theInfo.myMeshInfo;
      TInt aNbElem = theInfo.myElemNum.size();

      med_booleen& anIsElemNames = static_cast<med_booleen>(theInfo.myIsElemNames);
      med_booleen& anIsElemNum = static_cast<med_booleen>(theInfo.myIsElemNum);
      med_entite_maillage& anEntity = static_cast<med_entite_maillage>(theInfo.myTEntity);
      med_geometrie_element& aGeom = static_cast<med_geometrie_element>(theInfo.myTGeom);
      med_connectivite& aConn = static_cast<med_connectivite>(theInfo.myTConn);

      TErr aRet = MEDelementsLire(myFile->Id(),
				  &aMeshInfo.myName[0],
				  aMeshInfo.myDim,
				  &theInfo.myConn[0],
				  MED_FULL_INTERLACE,
				  &theInfo.myElemNames[0],
				  &anIsElemNames,
				  &theInfo.myElemNum[0],
				  &anIsElemNum,
				  &theInfo.myFamNum[0],
				  aNbElem,
				  anEntity,
				  aGeom,
				  aConn);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetCellInfo - MEDelementsLire(...)");
    }
    
    
    void TVWrapper::SetCellInfo(const MED::TCellInfo& theInfo,
			       EModeAcces theMode,
			       TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,theMode,theErr);
      
      if(theErr && !*theErr)
	return;

      MED::TCellInfo& anInfo = const_cast<MED::TCellInfo&>(theInfo);
      MED::TMeshInfo& aMeshInfo = *anInfo.myMeshInfo;

      med_booleen& anIsElemNames = static_cast<med_booleen>(theInfo.myIsElemNames);
      med_booleen& anIsElemNum = static_cast<med_booleen>(theInfo.myIsElemNum);
      med_entite_maillage& anEntity = static_cast<med_entite_maillage>(theInfo.myTEntity);
      med_geometrie_element& aGeom = static_cast<med_geometrie_element>(theInfo.myTGeom);
      med_connectivite& aConn = static_cast<med_connectivite>(theInfo.myTConn);

      TErr aRet = MEDelementsEcr(myFile->Id(),
				 &aMeshInfo.myName[0],
				 aMeshInfo.myDim,
				 &anInfo.myConn[0],
				 MED_FULL_INTERLACE,
				 &anInfo.myElemNames[0],
				 anIsElemNames,
				 &anInfo.myElemNum[0],
				 anIsElemNum,
				 &anInfo.myFamNum[0],
				 anInfo.myNbElem,
				 anEntity,
				 aGeom,
				 aConn);
      
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetCellInfo - MEDelementsLire(...)");
    }
    

    void TVWrapper::SetCellInfo(const MED::TCellInfo& theInfo,
			       TErr* theErr)
    {
      SetCellInfo(theInfo,eLECTURE_ECRITURE,theErr);
    }
    

    TInt TVWrapper::GetNbFields(TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      return MEDnChamp(myFile->Id(),0);
    }
    
    
    TInt TVWrapper::GetNbComp(TInt theFieldId,
			     TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return -1;
      
      return MEDnChamp(myFile->Id(),theFieldId);
    }
    
    
    void TVWrapper::GetFieldInfo(TInt theFieldId, 
				MED::TFieldInfo& theInfo,
				TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return;
      
      med_type_champ& aType = static_cast<med_type_champ>(theInfo.myType);

      TErr aRet = MEDchampInfo(myFile->Id(),
			       theFieldId,
			       &theInfo.myName[0],
			       &aType,
			       &theInfo.myCompNames[0],
			       &theInfo.myUnitNames[0],
			       theInfo.myNbComp);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetFieldInfo - MEDchampInfo(...)");
    }
    
    
    void TVWrapper::SetFieldInfo(const MED::TFieldInfo& theInfo,
				EModeAcces theMode,
				TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,theMode,theErr);
      
      if(theErr && !*theErr)
	return;
      
      MED::TFieldInfo& anInfo = const_cast<MED::TFieldInfo&>(theInfo);
      
      med_type_champ& aType = static_cast<med_type_champ>(theInfo.myType);
      
      TErr aRet = MEDchampCr(myFile->Id(),
			     &anInfo.myName[0],
			     aType,
			     &anInfo.myCompNames[0],
			     &anInfo.myUnitNames[0],
			     anInfo.myNbComp);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"SetFieldInfo - MEDchampEcr(...)");
    }
    
    
    void TVWrapper::SetFieldInfo(const MED::TFieldInfo& theInfo,
				TErr* theErr)
    {
      TErr aRet;
      SetFieldInfo(theInfo,eLECTURE_ECRITURE,&aRet);
      
      if(aRet < 0)
	SetFieldInfo(theInfo,eLECTURE_AJOUT,theErr);
    }
    
    
    TInt TVWrapper::GetNbTimeStamps(const MED::TFieldInfo& theInfo, 
				   const MED::TEntityInfo& theEntityInfo,
				   EEntiteMaillage& theEntity,
				   TGeom& theGeom,
				   TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr){
	*theErr &= !theEntityInfo.empty();
	if(!*theErr)
	  return -1;
      }
      
      if(theEntityInfo.empty()) 
	EXCEPTION(runtime_error,"GetNbTimeStamps - There is no any Entity on the Mesh");
      
      theGeom.clear();
      TInt aNbTimeStamps = 0;
      MED::TFieldInfo& anInfo = const_cast<MED::TFieldInfo&>(theInfo);
      TEntityInfo::const_iterator anIter = theEntityInfo.begin();
      for(; anIter != theEntityInfo.end(); anIter++){
	const med_entite_maillage& anEntity = static_cast<const med_entite_maillage>(anIter->first);
	const TGeom& aTGeom = anIter->second;
	TGeom::const_iterator anGeomIter = aTGeom.begin();
	for(; anGeomIter != aTGeom.end(); anGeomIter++){
	  const med_geometrie_element& aGeom = static_cast<const med_geometrie_element>(anGeomIter->first);
	  aNbTimeStamps = MEDnPasdetemps(myFile->Id(),&anInfo.myName[0],anEntity,aGeom);
	  if(aNbTimeStamps){
	    theEntity = EEntiteMaillage(anEntity);
	    theGeom[EGeometrieElement(aGeom)] = anGeomIter->second;
	  }
	}
	if(aNbTimeStamps) 
	  break;
      }
      return aNbTimeStamps;
    }
    
    
    void TVWrapper::GetTimeStampInfo(TInt theTimeStampId, 
				    MED::TTimeStampInfo& theInfo,
				    TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      TGeom& aTGeom = theInfo.myGeom;
      
      if(theErr){
	*theErr &= !aTGeom.empty();
	if(!*theErr)
	  return;
      }
      
      if(aTGeom.empty())
	EXCEPTION(runtime_error,"GetTimeStampInfo - There is no any cell");
      
      MED::TFieldInfo& aFieldInfo = *theInfo.myFieldInfo;
      MED::TMeshInfo& aMeshInfo = *aFieldInfo.myMeshInfo;
      
      med_entite_maillage& anEntity = static_cast<med_entite_maillage>(theInfo.myEntity);
      
      TGeom::iterator anIter = aTGeom.begin();
      med_geometrie_element& aGeom = static_cast<med_geometrie_element>(anIter->first);

      med_booleen& anIsLocal = static_cast<med_booleen>(aFieldInfo.myIsLocal);
      
      TErr aRet = MEDpasdetempsInfo(myFile->Id(),
				    &aFieldInfo.myName[0],
				    anEntity,
				    aGeom,
				    theTimeStampId,
				    &theInfo.myNbGauss,
				    &theInfo.myNumDt,
				    &theInfo.myNumOrd,
				    &theInfo.myUnitDt[0],
				    &theInfo.myDt,
				    &aMeshInfo.myName[0],
				    &anIsLocal,
				    &aFieldInfo.myNbRef);
      if(theErr) 
	*theErr = aRet;
      else if(aRet < 0)
	EXCEPTION(runtime_error,"GetTimeStampInfo - MEDpasdetempsInfo(...)");
      
      static TInt MAX_NB_GAUSS_POINTS = 32;
      if(theInfo.myNbGauss > MAX_NB_GAUSS_POINTS) 
	theInfo.myNbGauss = 1;
    }
    

    void TVWrapper::GetTimeStampVal(MED::TTimeStampVal& theVal,
				   TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,eLECTURE,theErr);
      
      if(theErr && !*theErr)
	return;
      
      TIdt anId = myFile->Id();
      
      MED::TTimeStampInfo& aTimeStampInfo = *theVal.myTimeStampInfo;
      MED::TFieldInfo& aFieldInfo = *aTimeStampInfo.myFieldInfo;
      MED::TMeshInfo& aMeshInfo = *aFieldInfo.myMeshInfo;
      
      med_entite_maillage& anEntity = static_cast<med_entite_maillage>(aTimeStampInfo.myEntity);
      med_mode_profil& aPflMode = static_cast<med_mode_profil>(theVal.myPflMode);
      TGeom& aTGeom = aTimeStampInfo.myGeom;
      TGeom::iterator anIter = aTGeom.begin();
      for(; anIter != aTGeom.end(); anIter++){
	med_geometrie_element& aGeom = static_cast<med_geometrie_element>(anIter->first);
	TInt aNbVal = MEDnVal(anId,
			      &aFieldInfo.myName[0],
			      anEntity,
			      aGeom,
			      aTimeStampInfo.myNumDt,
			      aTimeStampInfo.myNumOrd,
			      &aMeshInfo.myName[0],
			      aPflMode);
	if(aNbVal <= 0){
	  if(theErr){
	    *theErr = MED_FAUX;
	    return;
	  }
	  EXCEPTION(runtime_error,"GetTimeStampInfo - MEDnVal(...) - aNbVal == "<<aNbVal<<" <= 0");
	}
	
	TValue& aValue = theVal.myMeshValue[EGeometrieElement(aGeom)];
	TInt iEnd = aNbVal*aFieldInfo.myNbComp;
	
	if(iEnd != aValue.size()){
	  if(theErr){
	    *theErr = MED_FAUX;
	    return;
	  }
	  EXCEPTION(runtime_error,"GetTimeStampInfo - iEnd == "<<iEnd<<" != aValue.size() == "<<aValue.size());
	}
	
	TErr aRet;
	switch(aFieldInfo.myType){
	case eFLOAT64: {
	  std::vector<TFloat> anArray(iEnd);
	  aRet = MEDchampLire(anId,
			      &aMeshInfo.myName[0],
			      &aFieldInfo.myName[0],
			      (unsigned char*)&anArray[0],
			      MED_FULL_INTERLACE,
			      MED_ALL,
			      &aTimeStampInfo.myGaussName[0],
			      &theVal.myPflName[0],
			      aPflMode,
			      anEntity,
			      aGeom,
			      aTimeStampInfo.myNumDt,
			      aTimeStampInfo.myNumOrd);
	  if(aRet >= 0) 
	    for(TInt i = 0; i < iEnd; i++) 
	      aValue[i] = anArray[i];
	  break;
	}
	default: {
	  std::vector<TInt> anArray(iEnd);
	  aRet = MEDchampLire(anId,
			      &aMeshInfo.myName[0],
			      &aFieldInfo.myName[0],
			      (unsigned char*)&anArray[0],
			      MED_FULL_INTERLACE,
			      MED_ALL,
			      &aTimeStampInfo.myGaussName[0],
			      &theVal.myPflName[0],
			      aPflMode,
			      anEntity,
			      aGeom,
			      aTimeStampInfo.myNumDt,
			      aTimeStampInfo.myNumOrd);
	  if(aRet >= 0) 
	    for(med_int i = 0; i < iEnd; i++) 
	      aValue[i] = anArray[i];
	  break;
	}}
	if(aRet < 0){
	  if(theErr){
	    *theErr = MED_FAUX;
	    return;
	  }
	  EXCEPTION(runtime_error,"GetValTimeStamp - MEDchampLire(...)");
	}
      }
    }
    
    
    void TVWrapper::SetTimeStamp(const MED::TTimeStampVal& theVal,
				 EModeAcces theMode,
				 TErr* theErr)
    {
      TFileWrapper aFileWrapper(myFile,theMode,theErr);
      
      if(theErr && !*theErr)
	return;
      
      TErr aRet;
      TIdt anId = myFile->Id();
      
      MED::TTimeStampVal& aVal = const_cast<MED::TTimeStampVal&>(theVal);
      MED::TTimeStampInfo& aTimeStampInfo = *aVal.myTimeStampInfo;
      MED::TFieldInfo& aFieldInfo = *aTimeStampInfo.myFieldInfo;
      MED::TMeshInfo& aMeshInfo = *aFieldInfo.myMeshInfo;
      MED::TMeshValue& aMeshValue = aVal.myMeshValue;
      
      med_entite_maillage& anEntity = static_cast<med_entite_maillage>(aTimeStampInfo.myEntity);
      med_mode_profil& aPflMode = static_cast<med_mode_profil>(theVal.myPflMode);
      TMeshValue::iterator anIter = aMeshValue.begin();
      for(; anIter != aMeshValue.end(); anIter++){
	med_geometrie_element& aGeom = static_cast<med_geometrie_element>(anIter->first);
	TValue& aValue = aVal.myMeshValue[EGeometrieElement(aGeom)];
	med_int iEnd = aValue.size();
	med_int aNbVal = iEnd / aFieldInfo.myNbComp;
	
	switch(aFieldInfo.myType){
	case eFLOAT64: {
	  std::vector<TFloat>& anArray = aValue;
	  
	  aRet = MEDchampEcr(anId,
			     &aMeshInfo.myName[0],
			     &aFieldInfo.myName[0],
			     (unsigned char*)&anArray[0],
			     MED_FULL_INTERLACE,
			     aNbVal,
			     &aTimeStampInfo.myGaussName[0],
			     MED_ALL,
			     &aVal.myPflName[0],
			     aPflMode,
			     anEntity,
			     aGeom,
			     aTimeStampInfo.myNumDt,
			     &aTimeStampInfo.myUnitDt[0],
			     aTimeStampInfo.myDt,
			     aTimeStampInfo.myNumOrd);
	  break;
	}
	default: {
	  vector<TInt> anArray(iEnd);
	  for(TInt i = 0; i< iEnd; i++) anArray[i] = TInt(aValue[i]);
	  
	  aRet = MEDchampEcr(anId,
			     &aMeshInfo.myName[0],
			     &aFieldInfo.myName[0],
			     (unsigned char*)&anArray[0],
			     MED_FULL_INTERLACE,
			     aNbVal,
			     &aTimeStampInfo.myGaussName[0],
			     MED_ALL,
			     &aVal.myPflName[0],
			     aPflMode,
			     anEntity,
			     aGeom,
			     aTimeStampInfo.myNumDt,
			     &aTimeStampInfo.myUnitDt[0],
			     aTimeStampInfo.myDt,
			     aTimeStampInfo.myNumOrd);
	  break;
	}}
	
	if(aRet < 0){
	  if(theErr){
	    *theErr = MED_FAUX;
	    break;
	  }
	  EXCEPTION(runtime_error,"SetTimeStamp - MEDchampEcr(...)");
	}
	
      }
      
      INITMSG(MYDEBUG,"TVWrapper::SetMeshInfo - MED_MODE_ACCES = "<<theMode<<"; aRet = "<<aRet<<endl);
    }

    
    void TVWrapper::SetTimeStamp(const MED::TTimeStampVal& theVal,
				TErr* theErr)
    {
      TErr aRet;
      SetTimeStamp(theVal,eLECTURE_ECRITURE,&aRet);
      
      if(aRet < 0)
	SetTimeStamp(theVal,eLECTURE_AJOUT,theErr);
    }
    
  }
}
