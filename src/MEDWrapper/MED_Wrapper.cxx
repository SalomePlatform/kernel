//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MED_Wrapper.hxx"
#include "MED_Utilities.hxx"

using namespace std;
using namespace MED;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif



//---------------------------------------------------------------
static med_geometrie_element NODEGEOM[1] = {MED_POINT1};

static med_geometrie_element EDGEGEOM[MED_NBR_GEOMETRIE_ARETE] = {
  MED_SEG2, MED_SEG3
};

static med_geometrie_element FACEGEOM[MED_NBR_GEOMETRIE_FACE] = {
  MED_TRIA3, MED_QUAD4, MED_TRIA6, MED_QUAD8
};

static med_geometrie_element  CELLGEOM[MED_NBR_GEOMETRIE_MAILLE] = {
  MED_POINT1, MED_SEG2, MED_SEG3, MED_TRIA3,
  MED_QUAD4, MED_TRIA6, MED_QUAD8, MED_TETRA4,
  MED_PYRA5, MED_PENTA6, MED_HEXA8, MED_TETRA10, 
  MED_PYRA13, MED_PENTA15, MED_HEXA20
};

void GetEntity2Geom(med_entite_maillage theEntity, med_geometrie_element*& theVector, int* theEnd){
  switch(theEntity){
  case MED_MAILLE: theVector = CELLGEOM; *theEnd = MED_NBR_GEOMETRIE_MAILLE; break;
  case MED_FACE: theVector = FACEGEOM; *theEnd = MED_NBR_GEOMETRIE_FACE; break;
  case MED_ARETE: theVector = EDGEGEOM; *theEnd = MED_NBR_GEOMETRIE_ARETE; break;
  case MED_NOEUD: theVector = NODEGEOM; *theEnd = 1; break;
  default:
    EXCEPTION(runtime_error,"GetEntity2Geom - theEntity == "<<theEntity<<" is uncorrect");
  }
}


//---------------------------------------------------------------
TFile::TFile(const std::string& theFileName): 
  myFid(0), 
  myCount(0),
  myFileName(strdup(theFileName.c_str()))
{}

TFile::~TFile(){ 
  free(myFileName);
  Close();
}


void TFile::Open(med_mode_acces theMode){
  if(myCount++ == 0){
    myFid = MEDouvrir(myFileName,theMode);
    if(myFid < 0)
      EXCEPTION(runtime_error,"TFile - MEDouvrir('"<<myFileName<<"',"<<theMode<<")");
  }
}
 
void TFile::Close(){ 
  if(--myCount == 0) {
    MEDfermer(myFid);
  }
}

const med_idt& TFile::Id() const { 
  if(myFid < 0)
    EXCEPTION(runtime_error,"TFile - GetFid() < 0");
  return myFid;
}


//---------------------------------------------------------------
TWrapper::TWrapper(const std::string& theFileName): 
  myFile(theFileName)
{}


med_int TWrapper::GetNbMeshes(){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  return MEDnMaa(myFile.Id());
}


void TWrapper::GetMeshInfo(med_int theMeshId, TMeshInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  med_err ret = MEDmaaInfo(myFile.Id(),
			   theMeshId,
			   &theInfo.myName[0],
			   &theInfo.myDim);
  if(ret < 0)
    EXCEPTION(runtime_error,"GetMeshInfo - MEDmaaInfo(...)");
}


void TWrapper::SetMeshInfo(const TMeshInfo& theInfo){
  med_err ret = -1;
  try{
    TFileWrapper aFileWrapper(myFile,MED_ECRI);
    
    TMeshInfo& anInfo = const_cast<TMeshInfo&>(theInfo);
    ret = MEDmaaCr(myFile.Id(),
  		   &anInfo.myName[0],
  		   anInfo.myDim);
  }catch(...){}
  if(ret < 0){
    TFileWrapper aFileWrapper(myFile,MED_REMP);
    
    TMeshInfo& anInfo = const_cast<TMeshInfo&>(theInfo);
    ret = MEDmaaCr(myFile.Id(),
		   &anInfo.myName[0],
		   anInfo.myDim);
  }
  if(ret < 0)
    EXCEPTION(runtime_error,"SetMeshInfo - MEDmaaCr(...)");
}


med_int TWrapper::GetNbFamilies(const TMeshInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& anInfo = const_cast<TMeshInfo&>(theInfo);
  return MEDnFam(myFile.Id(),
		 &anInfo.myName[0],
		 0,
		 MED_FAMILLE);
}


med_int TWrapper::GetNbFamAttr(med_int theFamId, const TMeshInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& anInfo = const_cast<TMeshInfo&>(theInfo);
  return MEDnFam(myFile.Id(),
		 &anInfo.myName[0],
		 theFamId,
		 MED_ATTR);
}


med_int TWrapper::GetNbFamGroup(med_int theFamId, const TMeshInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& anInfo = const_cast<TMeshInfo&>(theInfo);
  return MEDnFam(myFile.Id(),
		 &anInfo.myName[0],
		 theFamId,
		 MED_GROUPE);
}


void TWrapper::GetFamilyInfo(med_int theFamId, TFamilyInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& aMeshInfo = *theInfo.myMeshInfo;

  med_err ret = MEDfamInfo(myFile.Id(),
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
  if(ret < 0)
    EXCEPTION(runtime_error,"GetFamilyInfo - MEDfamInfo(...)");
}


void TWrapper::SetFamilyInfo(const TFamilyInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_ECRI);

  TFamilyInfo& anInfo = const_cast<TFamilyInfo&>(theInfo);
  TMeshInfo& aMeshInfo = *anInfo.myMeshInfo;

  med_err ret = MEDfamCr(myFile.Id(),
			 &aMeshInfo.myName[0],
			 &anInfo.myName[0],
			 anInfo.myId,
			 &anInfo.myAttrId[0],
			 &anInfo.myAttrVal[0],
			 &anInfo.myAttrDesc[0],
			 anInfo.myNbAttr,
			 &anInfo.myGroupNames[0],
			 anInfo.myNbGroup);
  if(ret < 0)
    EXCEPTION(runtime_error,"SetFamilyInfo - MEDfamCr(...)");
}


med_int TWrapper::GetNbNodes(const TMeshInfo& theMeshInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& aMeshInfo = const_cast<TMeshInfo&>(theMeshInfo);

  return MEDnEntMaa(myFile.Id(),
		    &aMeshInfo.myName[0],
		    MED_COOR,MED_NOEUD,
		    med_geometrie_element(0),
		    med_connectivite(0));
}


void TWrapper::GetNodeInfo(TNodeInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& aMeshInfo = *theInfo.myMeshInfo;

  med_err ret = MEDnoeudsLire(myFile.Id(),
			      &aMeshInfo.myName[0],
			      aMeshInfo.myDim,
			      &theInfo.myCoord[0],
			      MED_FULL_INTERLACE,
			      &theInfo.mySystem,
			      &theInfo.myCoordNames[0],
			      &theInfo.myCoordUnits[0],
			      &theInfo.myElemNames[0],
			      &theInfo.myIsElemNames,
			      &theInfo.myElemNum[0],
			      &theInfo.myIsElemNum,
			      &theInfo.myFamNum[0],
			      theInfo.myNbElem);
  if(ret < 0)
    EXCEPTION(runtime_error,"GetNodeInfo - MEDnoeudsLire(...)");
}


void TWrapper::SetNodeInfo(const TNodeInfo& theInfo)
{
  TFileWrapper aFileWrapper(myFile,MED_ECRI);

  TNodeInfo& anInfo = const_cast<TNodeInfo&>(theInfo);
  TMeshInfo& aMeshInfo = *anInfo.myMeshInfo;

  med_err ret = MEDnoeudsEcr(myFile.Id(),
			     &aMeshInfo.myName[0],
			     aMeshInfo.myDim,
			     &anInfo.myCoord[0],
			     MED_FULL_INTERLACE,
			     anInfo.mySystem,
			     &anInfo.myCoordNames[0],
			     &anInfo.myCoordUnits[0],
			     &anInfo.myElemNames[0],
			     anInfo.myIsElemNames,
			     &anInfo.myElemNum[0],
			     anInfo.myIsElemNum,
			     &anInfo.myFamNum[0],
			     anInfo.myNbElem,
			     MED_REMP);
  if(ret < 0)
    EXCEPTION(runtime_error,"SetNodeInfo - MEDnoeudsEcr(...)");
}


TEntityInfo TWrapper::GetEntityInfo(const TMeshInfo& theMeshInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TEntityInfo anInfo;
  if(med_int aNbElem = GetNbNodes(theMeshInfo)){
    anInfo[MED_NOEUD][MED_POINT1] = aNbElem;
    med_entite_maillage anEntityVector[3] = {MED_MAILLE, MED_FACE, MED_ARETE};
    for(med_int iEntity = 0; iEntity < 3; iEntity++){
      med_int iGeomEnd;
      med_entite_maillage& anEntity = anEntityVector[iEntity];
      med_geometrie_element* aGeomVector;
      GetEntity2Geom(anEntity,aGeomVector,&iGeomEnd);
      for(med_int iGeom = 0; iGeom < iGeomEnd; iGeom++) {
	med_geometrie_element& aGeom = aGeomVector[iGeom];
	if(med_int aNb = GetNbCells(theMeshInfo,anEntity,aGeom)){
	  anInfo[anEntity][aGeom] = aNb;
	}
      }
    }
  }
  return anInfo;
}


med_int TWrapper::GetNbCells(const TMeshInfo& theMeshInfo, 
			     med_entite_maillage theTEntity, 
			     med_geometrie_element theTGeom, 
			     med_connectivite theTConn)
{
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& aMeshInfo = const_cast<TMeshInfo&>(theMeshInfo);

  return MEDnEntMaa(myFile.Id(),
		    &aMeshInfo.myName[0],
		    MED_CONN,
		    theTEntity,
		    theTGeom,
		    theTConn); 
}


void TWrapper::GetCellInfo(TCellInfo& theInfo)
{
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  TMeshInfo& aMeshInfo = *theInfo.myMeshInfo;
  med_int aNbElem = theInfo.myElemNum.size();

  med_err ret = MEDelementsLire(myFile.Id(),
				&aMeshInfo.myName[0],
				aMeshInfo.myDim,
				&theInfo.myConn[0],
				MED_FULL_INTERLACE,
				&theInfo.myElemNames[0],
				&theInfo.myIsElemNames,
				&theInfo.myElemNum[0],
				&theInfo.myIsElemNum,
				&theInfo.myFamNum[0],
				aNbElem,
				theInfo.myTEntity,
				theInfo.myTGeom,
				theInfo.myTConn);
  if(ret < 0)
    EXCEPTION(runtime_error,"GetCellInfo - MEDelementsLire(...)");
}


void TWrapper::SetCellInfo(const TCellInfo& theInfo)
{
  TFileWrapper aFileWrapper(myFile,MED_ECRI);

  TCellInfo& anInfo = const_cast<TCellInfo&>(theInfo);
  TMeshInfo& aMeshInfo = *anInfo.myMeshInfo;
  med_int aNbElem = anInfo.myElemNum.size();

  med_err ret = MEDelementsEcr(myFile.Id(),
			       &aMeshInfo.myName[0],
			       aMeshInfo.myDim,
			       &anInfo.myConn[0],
			       MED_FULL_INTERLACE,
			       &anInfo.myElemNames[0],
			       anInfo.myIsElemNames,
			       &anInfo.myElemNum[0],
			       anInfo.myIsElemNum,
			       &anInfo.myFamNum[0],
			       anInfo.myNbElem,
			       anInfo.myTEntity,
			       anInfo.myTGeom,
			       anInfo.myTConn,
			       MED_REMP);

  if(ret < 0)
    EXCEPTION(runtime_error,"GetCellInfo - MEDelementsLire(...)");
}


med_int TWrapper::GetNbFields(){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  return MEDnChamp(myFile.Id(),0);
}


med_int TWrapper::GetNbComp(med_int theFieldId){
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  return MEDnChamp(myFile.Id(),theFieldId);
}


void TWrapper::GetFieldInfo(med_int theFieldId, TFieldInfo& theInfo){
  TFileWrapper aFileWrapper(myFile,MED_LECT);
  
  med_err ret = MEDchampInfo(myFile.Id(),
			     theFieldId,
			     &theInfo.myName[0],
			     &theInfo.myType,
			     &theInfo.myCompNames[0],
			     &theInfo.myUnitNames[0],
			     theInfo.myNbComp);
  if(ret < 0)
    EXCEPTION(runtime_error,"GetFieldInfo - MEDchampInfo(...)");
}


void TWrapper::SetFieldInfo(const TFieldInfo& theInfo)
{
  TFileWrapper aFileWrapper(myFile,MED_ECRI);
  
  TFieldInfo& anInfo = const_cast<TFieldInfo&>(theInfo);
  med_err ret = MEDchampCr(myFile.Id(),
			   &anInfo.myName[0],
			   anInfo.myType,
			   &anInfo.myCompNames[0],
			   &anInfo.myUnitNames[0],
			   anInfo.myNbComp);
  if(ret < 0)
    EXCEPTION(runtime_error,"SetFieldInfo - MEDchampEcr(...)");
}


med_int TWrapper::GetNbTimeStamps(const TFieldInfo& theInfo, 
				  const TEntityInfo& theEntityInfo,
				  med_entite_maillage& theEntity,
				  TGeom& theGeom)
{
  TFileWrapper aFileWrapper(myFile,MED_LECT);

  if(theEntityInfo.empty()) 
    EXCEPTION(runtime_error,"GetNbTimeStamps - There is no any Entity on the Mesh");

  theGeom.clear();
  med_int aNbTimeStamps = 0;
  TFieldInfo& anInfo = const_cast<TFieldInfo&>(theInfo);
  TEntityInfo::const_iterator anIter = theEntityInfo.begin();
  for(; anIter != theEntityInfo.end(); anIter++){
    const med_entite_maillage& anEntity = anIter->first;
    const TGeom& aTGeom = anIter->second;
    TGeom::const_iterator anGeomIter = aTGeom.begin();
    for(; anGeomIter != aTGeom.end(); anGeomIter++){
      const med_geometrie_element& aGeom = anGeomIter->first;
      aNbTimeStamps = MEDnPasdetemps(myFile.Id(),&anInfo.myName[0],anEntity,aGeom);
      if(aNbTimeStamps){
	theEntity = anEntity;
	theGeom[aGeom] = anGeomIter->second;
      }
    }
    if(aNbTimeStamps) break;
  }
  return aNbTimeStamps;
}


void TWrapper::GetTimeStampInfo(med_int theTimeStampId, TTimeStampInfo& theInfo)
{
  TFileWrapper aFileWrapper(myFile,MED_LECT);
  med_idt anId = myFile.Id();

  TFieldInfo& aFieldInfo = *theInfo.myFieldInfo;
  TMeshInfo& aMeshInfo = *aFieldInfo.myMeshInfo;

  med_entite_maillage anEntity = theInfo.myEntity;
  TGeom& aTGeom = theInfo.myGeom;
  if(aTGeom.empty())
    EXCEPTION(runtime_error,"GetTimeStampInfo - There is no any cell");

  TGeom::iterator anIter = aTGeom.begin();
  med_geometrie_element aGeom = anIter->first;

  med_err ret = MEDpasdetempsInfo(anId,
				  &aFieldInfo.myName[0],
				  anEntity,
				  aGeom,
				  theTimeStampId,
				  &aMeshInfo.myName[0],
				  &theInfo.myNbGauss,
				  &theInfo.myNumDt,
				  &theInfo.myUnitDt[0],
				  &theInfo.myDt,
				  &theInfo.myNumOrd);
  if(ret < 0)
    EXCEPTION(runtime_error,"GetTimeStampInfo - MEDpasdetempsInfo(...)");

  static med_int MAX_NB_GAUSS_POINTS = 32;
  if(theInfo.myNbGauss > MAX_NB_GAUSS_POINTS) 
    theInfo.myNbGauss = 1;
}

void TWrapper::SetTimeStamp(const TTimeStampVal& theVal)
{
  TFileWrapper aFileWrapper(myFile,MED_ECRI);
  
  TTimeStampVal& aVal = const_cast<TTimeStampVal&>(theVal);
  TTimeStampInfo& aTimeStampInfo = *aVal.myTimeStampInfo;
  TFieldInfo& aFieldInfo = *aTimeStampInfo.myFieldInfo;
  TMeshInfo& aMeshInfo = *aFieldInfo.myMeshInfo;
  TMeshValue& aMeshValue = aVal.myMeshValue;
  
  med_entite_maillage anEntity = aTimeStampInfo.myEntity;
  TMeshValue::iterator anIter = aMeshValue.begin();
  for(; anIter != aMeshValue.end(); anIter++){
    const med_geometrie_element& aGeom = anIter->first;
    TValue& aValue = aVal.myMeshValue[aGeom];
    med_int iEnd = aValue.size();
    med_int aNbVal = iEnd / aFieldInfo.myNbComp;
    
    med_err ret;
    switch(aFieldInfo.myType){
    case MED_REEL64: {
      valarray<med_float>& anArray = aValue;

      ret = MEDchampEcr(myFile.Id(),
			&aMeshInfo.myName[0],
			&aFieldInfo.myName[0],
			(unsigned char*)&anArray[0],
			MED_FULL_INTERLACE,
			aNbVal,
			aTimeStampInfo.myNbGauss,
			MED_ALL,
			&aVal.myPflName[0],
			MED_ECRI, 
			anEntity,
			aGeom,
			aTimeStampInfo.myNumDt,
			&aTimeStampInfo.myUnitDt[0],
			aTimeStampInfo.myDt,
			aTimeStampInfo.myNumOrd);
      break;
    }
    case MED_INT32:
    case MED_INT: {
      valarray<med_int> anArray(iEnd);
      for(med_int i = 0; i< iEnd; i++) anArray[i] = med_int(aValue[i]);

      ret = MEDchampEcr(myFile.Id(),
			&aMeshInfo.myName[0],
			&aFieldInfo.myName[0],
			(unsigned char*)&anArray[0],
			MED_FULL_INTERLACE,
			aNbVal,
			aTimeStampInfo.myNbGauss,
			MED_ALL,
			&aVal.myPflName[0],
			MED_ECRI, 
			anEntity,
			aGeom,
			aTimeStampInfo.myNumDt,
			&aTimeStampInfo.myUnitDt[0],
			aTimeStampInfo.myDt,
			aTimeStampInfo.myNumOrd);
      break;
    }
    default: 
      EXCEPTION(runtime_error,"SetTimeStamp - Type of the field == "<<aFieldInfo.myType<<" is wrong");
    }
    if(ret < 0)
      EXCEPTION(runtime_error,"SetTimeStamp - MEDchampEcr(...)");
  }
}

void TWrapper::GetTimeStampVal(TTimeStampVal& theVal)
{
  TFileWrapper aFileWrapper(myFile,MED_LECT);
  med_idt anId = myFile.Id();

  TTimeStampInfo& aTimeStampInfo = *theVal.myTimeStampInfo;
  TFieldInfo& aFieldInfo = *aTimeStampInfo.myFieldInfo;
  TMeshInfo& aMeshInfo = *aFieldInfo.myMeshInfo;
  TMeshValue& aMeshValue = theVal.myMeshValue;

  med_entite_maillage anEntity = aTimeStampInfo.myEntity;
  TGeom& aTGeom = aTimeStampInfo.myGeom;
  TGeom::iterator anIter = aTGeom.begin();
  for(; anIter != aTGeom.end(); anIter++){
    const med_geometrie_element& aGeom = anIter->first;
    med_int aNbVal = MEDnVal(anId,&aFieldInfo.myName[0],anEntity,aGeom,
			     aTimeStampInfo.myNumDt,aTimeStampInfo.myNumOrd);
    if(aNbVal <= 0)
      EXCEPTION(runtime_error,"GetTimeStampInfo - MEDnVal(...) - aNbVal == "<<aNbVal<<" <= 0");

    TValue& aValue = theVal.myMeshValue[aGeom];
    med_int iEnd = aNbVal * aFieldInfo.myNbComp;

    if(iEnd != aValue.size())
      EXCEPTION(runtime_error,"GetTimeStampInfo - iEnd == "<<iEnd<<" != aValue.size() == "<<aValue.size());

    med_err ret;
    switch(aFieldInfo.myType){
    case MED_REEL64: {
      valarray<med_float> anArray(iEnd);
      ret = MEDchampLire(myFile.Id(),
			 &aMeshInfo.myName[0],
			 &aFieldInfo.myName[0],
			 (unsigned char*)&anArray[0],
			 MED_FULL_INTERLACE,
			 MED_ALL,
			 &theVal.myPflName[0],
			 anEntity,
			 aGeom,
			 aTimeStampInfo.myNumDt,
			 aTimeStampInfo.myNumOrd);
      if(ret >= 0) 
	for(med_int i = 0; i < iEnd; i++) aValue[i] = anArray[i];
      break;
    }
    case MED_INT32:
    case MED_INT: {
      valarray<med_int> anArray(iEnd);
      ret = MEDchampLire(myFile.Id(),
			 &aMeshInfo.myName[0],
			 &aFieldInfo.myName[0],
			 (unsigned char*)&anArray[0],
			 MED_FULL_INTERLACE,
			 MED_ALL,
			 &theVal.myPflName[0],
			 anEntity,
			 aGeom,
			 aTimeStampInfo.myNumDt,
			 aTimeStampInfo.myNumOrd);
      if(ret >= 0) 
	for(med_int i = 0; i < iEnd; i++) aValue[i] = anArray[i];
      break;
    }
    default:
      EXCEPTION(runtime_error,"GetValTimeStamp - Type of the field == "<<aFieldInfo.myType<<" is wrong");
    }
    if(ret < 0)
      EXCEPTION(runtime_error,"GetValTimeStamp - MEDchampLire(...)");
  }
}
