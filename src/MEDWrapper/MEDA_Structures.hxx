//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MEDA_Structures_HeaderFile
#define MEDA_Structures_HeaderFile

#include "MED_Common.hxx"

namespace MEDA{
  using namespace std;
  using namespace boost;

//---------------------------------------------------------------
  class TNameInfo{
    friend class TWrapper;
    TNameInfo();
  protected:
    MED::PNameInfo myInfo;
    TNameInfo(const MED::PNameInfo& theInfo);
  public:
    virtual ~TNameInfo() {} 

    string GetName() const;
    void SetName(const string& theValue);
  };


//---------------------------------------------------------------
  class TMeshInfo: public TNameInfo{
    friend class TWrapper;
    TMeshInfo();
  protected:
    MED::PMeshInfo myInfo;
    TMeshInfo(const MED::PMeshInfo& theInfo);

  public:
    med_int GetDim() const;
  };
  typedef shared_ptr<TMeshInfo> PMeshInfo;


//---------------------------------------------------------------
  class TFamilyInfo: public TNameInfo{
    friend class TWrapper;
    TFamilyInfo();
  protected:
    MED::PFamilyInfo myInfo;
    TFamilyInfo(const MED::PFamilyInfo& theInfo);
  public:
    med_int GetId() const;
    void SetId(med_int theId);

    med_int GetNbGroup() const;
    string GetGroupName(med_int theId) const;
    void SetGroupName(med_int theId, const string& theValue);

    med_int GetNbAttr() const;
    string GetAttrDesc(med_int theId) const;
    void SetAttrDesc(med_int theId, const string& theValue);
    
    med_int GetAttrId(med_int theId) const;
    void SetAttrId(med_int theId, med_int theVal);

    med_int GetAttrVal(med_int theId) const;
    void SetAttrVal(med_int theId, med_int theVal);
  };
  typedef shared_ptr<TFamilyInfo> PFamilyInfo;


//---------------------------------------------------------------
  class TElemInfo{
    friend class TWrapper;
    TElemInfo();
  protected:
    MED::PElemInfo myInfo;
    TElemInfo(const MED::PElemInfo& theInfo);
  public:
    virtual ~TElemInfo() {} 

    med_int GetNbElem() const;

    med_int GetFamNum(med_int theId) const;
    void SetFamNum(med_int theId, med_int theValue);

    med_booleen IsElemNum() const;
    med_int GetElemNum(med_int theId) const;
    void SetElemNum(med_int theId, med_int theValue);

    med_booleen IsElemNames() const;
    string GetElemName(med_int theId) const;
    void SetElemName(med_int theId, const string& theValue);
  };


//---------------------------------------------------------------
  class TNodeInfo: public TElemInfo{
    friend class TWrapper;
    TNodeInfo();
  protected:
    MED::PNodeInfo myInfo;
    TNodeInfo(const MED::PNodeInfo& theInfo);
  public:
    med_repere GetSystem() const;
    void GetSystem(med_repere theSystem);
    
    med_float GetNodeCoord(med_int theId, med_int theComp) const;
    void SetNodeCoord(med_int theId, med_int theComp, med_float theVal);

    string GetCoordName(med_int theId) const;
    void SetCoordName(med_int theId, const string& theValue);

    string GetCoordUnit(med_int theId) const;
    void SetCoordUnit(med_int theId, const string& theValue);
  };
  typedef shared_ptr<TNodeInfo> PNodeInfo;


//---------------------------------------------------------------
  class TCellInfo: public TElemInfo{
    friend class TWrapper;
    TCellInfo();
  protected:
    MED::PCellInfo myInfo;
    TCellInfo(const MED::PCellInfo& theInfo);
  public:
    med_entite_maillage GetEntity() const;
    med_geometrie_element GetGeom() const;
    med_connectivite GetConn() const;
    
    med_int GetConnDim() const;
    med_int GetConn(med_int theElemId, med_int theConnId) const;
    void SetConn(med_int theElemId, med_int theConnId, med_int theVal);
  };
  typedef shared_ptr<TCellInfo> PCellInfo;


//---------------------------------------------------------------
  class TFieldInfo: public TNameInfo{
    friend class TWrapper;
    TFieldInfo();
  protected:
    MED::PFieldInfo myInfo;
    TFieldInfo(const MED::PFieldInfo& theInfo);
  public:
    med_type_champ GetType() const;
    med_int GetNbComp() const;

    string GetCompName(med_int theId) const;
    void SetCompName(med_int theId, const string& theValue);

    string GetUnitName(med_int theId) const;
    void SetUnitName(med_int theId, const string& theValue);
  };
  typedef shared_ptr<TFieldInfo> PFieldInfo;


//---------------------------------------------------------------
  class TTimeStampInfo{
    friend class TWrapper;
    TTimeStampInfo();
  protected:
    MED::PTimeStampInfo myInfo;
    TTimeStampInfo(const MED::PTimeStampInfo& theInfo);
  public:
    med_entite_maillage GetEntity() const;
    const MED::TGeom& GetGeom() const;

    med_int GetNbGauss() const;
    med_int GetNumDt() const;
    med_int GetNumOrd() const;

    med_float GetDt() const;

    string GetUnitDt() const;
    void SetUnitDt(const string& theValue);
  };
  typedef shared_ptr<TTimeStampInfo> PTimeStampInfo;
  

//---------------------------------------------------------------
  class TTimeStampVal{
    friend class TWrapper;
    TTimeStampVal();
  protected:
    MED::PTimeStampVal myInfo;
    TTimeStampVal(const MED::PTimeStampVal& theInfo);
  public:
    med_float GetVal(med_geometrie_element theGeom, med_int theId, 
		     med_int theComp, med_int theGauss = 0) const;
    void SetVal(med_geometrie_element theGeom, med_int theId, 
		med_int theComp, med_float theVal, med_int theGauss = 0);
    
    string GetPflName() const;
    void SetPflName(const string& theValue);
  };
  typedef shared_ptr<TTimeStampVal> PTimeStampVal;
  
};

#endif
