//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MED_Wrapper_HeaderFile
#define MED_Wrapper_HeaderFile

extern "C"{
#include <med.h>
}  
#include "MED_Structures.hxx"


namespace MED{
//---------------------------------------------------------------
  class TFile{
    TFile();
    TFile(const TFile&);
  public:
    TFile(const string& theFileName);
    ~TFile();
    
    void Open(med_mode_acces theMode);
    const med_idt& Id() const;
    void Close();

  protected:
    char* myFileName;
    med_idt myFid;
    int myCount;
  };


//---------------------------------------------------------------
  class TFileWrapper{
    TFile& myFile;
  public:
    TFileWrapper(TFile& theFile, med_mode_acces theMode): 
      myFile(theFile)
    {
      myFile.Open(theMode);
    }

    ~TFileWrapper(){
      myFile.Close();
    }
  };


//---------------------------------------------------------------
  class TWrapper{
    TWrapper();
    TWrapper(const TWrapper&);
    
  public:
    TWrapper(const string& theFileName);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    med_int GetNbMeshes();
    
    void GetMeshInfo(med_int theMeshId, TMeshInfo&);
    void SetMeshInfo(const TMeshInfo& theInfo);
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    med_int GetNbFamilies(const TMeshInfo& theMeshInfo);
    med_int GetNbFamAttr(med_int theFamId, const TMeshInfo& theInfo);
    med_int GetNbFamGroup(med_int theFamId, const TMeshInfo& theInfo);
    
    void GetFamilyInfo(med_int theFamId, TFamilyInfo& theInfo);
    void SetFamilyInfo(const TFamilyInfo& theInfo);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    med_int GetNbNodes(const TMeshInfo& theMeshInfo);
    
    void GetNodeInfo(TNodeInfo&);
    void SetNodeInfo(const TNodeInfo&);
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    TEntityInfo GetEntityInfo(const TMeshInfo& theMeshInfo);
    
    med_int GetNbCells(const TMeshInfo& theMeshInfo, 
		       med_entite_maillage, 
		       med_geometrie_element, 
		       med_connectivite theTConn = MED_NOD);
    
    void GetCellInfo(TCellInfo& theInfo);
    void SetCellInfo(const TCellInfo& theInfo);
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    med_int GetNbFields();
    
    med_int GetNbComp(med_int theFieldId);
    
    void GetFieldInfo(med_int theFieldId, TFieldInfo& theInfo);
    void SetFieldInfo(const TFieldInfo& theInfo);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    med_int GetNbTimeStamps(const TFieldInfo& theInfo, 
			    const TEntityInfo& theEntityInfo,
			    med_entite_maillage& theEntity,
			    TGeom& theGeom);

    void GetTimeStampInfo(med_int theTimeStampId, TTimeStampInfo&);

    void GetTimeStampVal(TTimeStampVal& theTimeStampVal);

    void SetTimeStamp(const TTimeStampVal& theTimeStampVal);

  protected:
    TFile myFile;
  };

};

#endif
