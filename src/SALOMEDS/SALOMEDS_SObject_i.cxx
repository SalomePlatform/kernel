//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS_SObject_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include <TDF_Tool.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Reference.hxx>
#include <Standard_GUID.hxx>
#include <Standard_NoSuchObject.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>

#include <TCollection_AsciiString.hxx>
#include <TDF_AttributeIterator.hxx>

#include "SALOMEDS_SObject_i.hxx"

//SALOMEDS Headers
#include <SALOMEDS.hxx>
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"

#include "SALOMEDS_AttributeComment_i.hxx"

#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "SALOMEDS_AttributeUserID_i.hxx"

#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS_AttributeExternalFileDef_i.hxx"
#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS_AttributeName_i.hxx"
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfInteger_i.hxx"
#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS_AttributeSelectable_i.hxx"
#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_AttributePythonObject_i.hxx"

#include "SALOMEDS_AttributeGraphic_i.hxx"
#include "SALOMEDS_AttributeFlags_i.hxx"

#include "Utils_ExceptHandlers.hxx"
UNEXPECT_CATCH(GALockProtection, SALOMEDS::GenericAttribute::LockProtection);

#include "utilities.h"

using namespace std;
using namespace SALOMEDS;


inline bool operator<(const Standard_GUID& theLeft, const Standard_GUID& theRight)
{
  char aLeft[40] = "";
  theLeft.ToCString(aLeft);

  char aRight[40] = "";
  theRight.ToCString(aRight);
  
  return strcmp(aLeft,aRight) < 0;
}


namespace SALOMEDS{
  const char* Str(const TCollection_ExtendedString& theString)
  {
    return TCollection_AsciiString(theString).ToCString();
  }

  typedef std::string TAttributeID;

  typedef Standard_GUID (*TGetGUID)();
  typedef bool (*TIsCheckLockedStudy)();
  typedef Handle(TDF_Attribute) (*TNewAttribute)();
  typedef SALOMEDS_GenericAttribute_i* (*TNewInstance)(const Handle(TDF_Attribute)&, SALOMEDS_SObject_i*);
  
  struct TAttrFun{
    TAttrFun(const TGetGUID& theGetGUID,
	     const TIsCheckLockedStudy& theIsCheckLockedStudy,
	     const TNewAttribute& theNewAttribute,
	     const TNewInstance& theNewInstance):
      myGetGUID(theGetGUID),
      myIsCheckLockedStudy(theIsCheckLockedStudy),
      myNewAttribute(theNewAttribute),
      myNewInstance(theNewInstance)
    {
    }

    TGetGUID myGetGUID;
    TIsCheckLockedStudy myIsCheckLockedStudy;
    TNewAttribute myNewAttribute;
    TNewInstance myNewInstance;
  };
  
  typedef std::map<TAttributeID,TAttrFun> TAttrID2FunMap;
  static TAttrID2FunMap __AttrID2FunMap__;
  
  
  typedef std::map<Standard_GUID,TAttributeID> TGUID2AttrIDMap;
  static TGUID2AttrIDMap __GUID2AttrIDMap__;
  
  bool Init()
  {
    
#define ADD_ATTRID2FUNMAP_ITEM(theName) \
    __AttrID2FunMap__.insert( \
      TAttrID2FunMap::value_type(#theName,TAttrFun( \
	&(SALOMEDS_##theName##_i::GetGUID), \
	&(SALOMEDS_##theName##_i::IsCheckLockedStudy), \
	&(SALOMEDS_##theName##_i::NewAttribute), \
	&(SALOMEDS_##theName##_i::NewInstance) \
    )))
						       
    ADD_ATTRID2FUNMAP_ITEM(AttributeName);
    ADD_ATTRID2FUNMAP_ITEM(AttributeComment); 
    ADD_ATTRID2FUNMAP_ITEM(AttributeIOR);
    ADD_ATTRID2FUNMAP_ITEM(AttributeReal);
    ADD_ATTRID2FUNMAP_ITEM(AttributeInteger);
    ADD_ATTRID2FUNMAP_ITEM(AttributeSequenceOfInteger);
    ADD_ATTRID2FUNMAP_ITEM(AttributeSequenceOfReal);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTableOfInteger);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTableOfReal);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTableOfString);
    ADD_ATTRID2FUNMAP_ITEM(AttributeLocalID);
    ADD_ATTRID2FUNMAP_ITEM(AttributePythonObject);
    
    ADD_ATTRID2FUNMAP_ITEM(AttributeUserID);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTreeNode);

    ADD_ATTRID2FUNMAP_ITEM(AttributePersistentRef);
    ADD_ATTRID2FUNMAP_ITEM(AttributeDrawable);
    ADD_ATTRID2FUNMAP_ITEM(AttributeSelectable);
    ADD_ATTRID2FUNMAP_ITEM(AttributeExpandable);
    ADD_ATTRID2FUNMAP_ITEM(AttributeOpened);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTextColor);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTextHighlightColor);
    ADD_ATTRID2FUNMAP_ITEM(AttributePixMap);
    ADD_ATTRID2FUNMAP_ITEM(AttributeTarget);
    ADD_ATTRID2FUNMAP_ITEM(AttributeStudyProperties);
    ADD_ATTRID2FUNMAP_ITEM(AttributeExternalFileDef);
    ADD_ATTRID2FUNMAP_ITEM(AttributeFileType);

    ADD_ATTRID2FUNMAP_ITEM(AttributeGraphic);
    ADD_ATTRID2FUNMAP_ITEM(AttributeFlags);

    TAttrID2FunMap::const_iterator anIter = __AttrID2FunMap__.begin();
    TAttrID2FunMap::const_iterator anEnd = __AttrID2FunMap__.end();
    for(; anIter != anEnd; anIter++){
      const TAttrID2FunMap::key_type& aKey = anIter->first;
      const TAttrID2FunMap::mapped_type& aValue = anIter->second;
      __GUID2AttrIDMap__[aValue.myGetGUID()] = aKey;
    };

#undef ADD_ATTRID2FUNMAP_ITEM
    return true;
  }
  

  static bool __IsInitilized__ = Init();


  //============================================================================
  bool GetAttrFun(const Standard_GUID& theGUID, TAttrFun& theAttrFun)
  {
    TGUID2AttrIDMap::const_iterator anIter = __GUID2AttrIDMap__.find(theGUID);
    if(anIter != __GUID2AttrIDMap__.end())
    {
      const TAttributeID& anAttributeID = anIter->second;
      TAttrID2FunMap::const_iterator anIter2 = __AttrID2FunMap__.find(anAttributeID);
      if(anIter2 != __AttrID2FunMap__.end())
      {
	theAttrFun = anIter2->second;
	return true;
      }
    }
    return false;
  }


  //============================================================================
  Standard_GUID GetGUID(const char* theType)
  {
    TAttrID2FunMap::const_iterator anIter = __AttrID2FunMap__.find(theType);
    if(anIter != __AttrID2FunMap__.end()){
      const TAttrID2FunMap::mapped_type& aValue = anIter->second;
      return aValue.myGetGUID();
    }
    // create tree node GUID by name
    if(strncmp(theType,"AttributeTreeNodeGUID",21) == 0){
      char aGUIDString[40] = "";
      sprintf(aGUIDString,&theType[21]);
      return aGUIDString;
    }
    
    return Standard_GUID();
  }


  //============================================================================
  std::string GetType(const Handle(TDF_Attribute)& theAttr)
  {
    if(theAttr.IsNull())
      return CORBA::string_dup("");
    
    Standard_GUID aGUID = theAttr->ID();
    TGUID2AttrIDMap::const_iterator anIter = __GUID2AttrIDMap__.find(aGUID);
    if(anIter != __GUID2AttrIDMap__.end())
    {
      const TAttributeID& anAttributeID = anIter->second;
      return anAttributeID;
    }
    
    char aType[60] = "";
    {
      Handle(TDataStd_TreeNode) anAttr = Handle(TDataStd_TreeNode)::DownCast(theAttr);
      if (!anAttr.IsNull()) {
	char aGUID[40] = "";
	anAttr->ID().ToCString(aGUID);
	sprintf(aType, "AttributeTreeNodeGUID%s",aGUID);
	return aType;
      }
    }
    {
      Handle(TDataStd_UAttribute) anAttr = Handle(TDataStd_UAttribute)::DownCast(theAttr);
      if (!anAttr.IsNull()) {
	char aGUID[40] = "";
	anAttr->ID().ToCString(aGUID);
	sprintf(aType, "AttributeUserID_%s",aGUID); 
	return aType;
      }
    }
    return aType;
  }
  
}  

//static SALOMEDS::Mutex SObjMutex;
  
//============================================================================
SALOMEDS_Study_i::TSObjectHolder
SALOMEDS_SObject_i::New(SALOMEDS_Study_i* theStudy,
			const TDF_Label& theLabel)
{
  SALOMEDS_Study_i::TSObjectHolder aSObjectHolder;
  SALOMEDS_Study_i::TSObjectMap& anSObjectMap = theStudy->GetSObjectMap();
  SALOMEDS_Study_i::TSObjectMap::const_iterator anIter = anSObjectMap.find(theLabel);
  if(anIter != anSObjectMap.end())
    aSObjectHolder = anIter->second;
  else{
    SALOMEDS_SObject_i* aSObject = new SALOMEDS_SObject_i(theStudy,theLabel);
    aSObjectHolder.first = aSObject;
    aSObjectHolder.second = aSObject->_this();
    anSObjectMap[theLabel] = aSObjectHolder;

    //TCollection_AsciiString anEntry;
    //TDF_Tool::Entry(theLabel,anEntry);
    //cout<<"APO - SALOMEDS_SObject_i::New - anEntry = "<<anEntry.ToCString()<<endl;
  }
  return aSObjectHolder;
}

SALOMEDS_SObject_i*
SALOMEDS_SObject_i::NewPtr(SALOMEDS_Study_i* theStudy,
			   const TDF_Label& theLabel)
{
  return New(theStudy,theLabel).first;
}

SALOMEDS::SObject_var
SALOMEDS_SObject_i::NewRef(SALOMEDS_Study_i* theStudy,
			   const TDF_Label& theLabel)
{
  return New(theStudy,theLabel).second;
}

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SObject_i::SALOMEDS_SObject_i(SALOMEDS_Study_i* theStudy,
				       const TDF_Label& theLabel):
  _lab(theLabel),
  _study(theStudy)
{
}

//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SObject_i::~SALOMEDS_SObject_i()
{
}
  

//============================================================================
CORBA::ORB_var SALOMEDS_SObject_i::GetORB() const
{
  return _study->GetORB();
}


//============================================================================
PortableServer::POA_var SALOMEDS_SObject_i::GetPOA() const
{
  return _study->GetPOA();
}


//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetID()
{
  Locker lock;

  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(_lab,anEntry);
  return CORBA::string_dup(anEntry.ToCString());
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
TDF_Label SALOMEDS_SObject_i::GetFatherComponentLabel()
{
  TDF_Label aLabel = _lab;
  while(!SALOMEDS_SComponent_i::IsA(aLabel) && !aLabel.IsRoot())
    aLabel = aLabel.Father();

  return aLabel;
}

SALOMEDS::SComponent_ptr SALOMEDS_SObject_i::GetFatherComponent()
{
  Locker lock;

  TDF_Label aSCompLabel = GetFatherComponentLabel();

  return SALOMEDS_SComponent_i::NewRef(_study,aSCompLabel)._retn();
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_SObject_i::GetFather()
{
  Locker lock;

  return SALOMEDS_SObject_i::NewRef(_study,_lab.Father())._retn();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_SObject_i::GetStudy()
{
  return _study->_this();
}

//============================================================================
/*! Function : ReferencedObject
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::ReferencedObject(SALOMEDS::SObject_out theSObject)
{
  Locker lock;

  Handle(TDF_Reference) aRef;
  if (!_lab.FindAttribute(TDF_Reference::GetID(),aRef))
    return false;
  
  theSObject = SALOMEDS_SObject_i::NewRef(_study,aRef->Get())._retn(); 
  return true;
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindSubObject(CORBA::Long theTag, SALOMEDS::SObject_out theSObject)
{
  Locker lock;

  TDF_Label aLabel = _lab.FindChild(theTag,false);
  if(aLabel.IsNull()) 
    return false;
  
  theSObject = SALOMEDS_SObject_i::NewRef(_study,aLabel)._retn(); 
  return true;
}  

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::Name()
{
  Locker lock;

  return CORBA::string_dup(_name.c_str());
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
void  SALOMEDS_SObject_i::Name(const char* theName)
{
  Locker lock;

  _name = theName;
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Tag()
{
  Locker lock;

  return _lab.Tag();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Depth()
{
  Locker lock;

  return _lab.Depth();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Object_ptr SALOMEDS_SObject_i::GetObject()
{
  Locker lock;

  try {
    Handle(SALOMEDS_IORAttribute) anAttr;
    if(_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),anAttr)){
      CORBA::ORB_var anORB = _study->GetStudyManager()->GetORB();
      return anORB->string_to_object(Str(anAttr->Get()));
    }
  }catch(...){
  }
  return CORBA::Object::_nil();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetName() {
  Locker lock;

  Handle(TDataStd_Name) anAttr;
  if(_lab.FindAttribute(TDataStd_Name::GetID(),anAttr))
    return CORBA::string_dup(Str(anAttr->Get()));

  return CORBA::string_dup("");
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetComment() {
  Locker lock;

  Handle(TDataStd_Comment) anAttr;
  if(_lab.FindAttribute(TDataStd_Comment::GetID(), anAttr))
    return CORBA::string_dup(Str(anAttr->Get()));

  return CORBA::string_dup("");
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetIOR() {
  Locker lock;

  Handle(SALOMEDS_IORAttribute) anAttr;
  if(_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),anAttr))
    return CORBA::string_dup(Str(anAttr->Get()));

  return CORBA::string_dup("");
}


//============================================================================
/*! Function : GetAllAttributes
 *  Purpose  : Returns list of all attributes for this sobject
 */
//============================================================================
SALOMEDS_SObject_i::TAttrHolder 
SALOMEDS_SObject_i::_FindGenAttribute(const Handle(TDF_Attribute)& theAttr)
{
  std::string aType = GetType(theAttr);
  return _FindGenAttribute(aType.c_str());
}


SALOMEDS::ListOfAttributes* SALOMEDS_SObject_i::GetAllAttributes()
{
  Locker lock;

  SALOMEDS::ListOfAttributes_var aSeqOfAttr = new SALOMEDS::ListOfAttributes;
  if(_lab.NbAttributes() > 0){
    Standard_Integer i = 0;
    for(TDF_AttributeIterator iter(_lab); iter.More(); iter.Next()) {
      Handle(TDF_Attribute) anAttr = iter.Value();
      TAttrHolder anAttrHolder = _FindGenAttribute(anAttr);
      SALOMEDS::GenericAttribute_var anGenAttr = anAttrHolder.second;
      if(!anGenAttr->_is_nil())
      {
	aSeqOfAttr->length(++i);
	aSeqOfAttr[i-1] = anGenAttr._retn();
      }
    }
  }

  return aSeqOfAttr._retn();
}


//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
SALOMEDS_SObject_i::TAttrHolder 
SALOMEDS_SObject_i::_CreateGenAttribute(const Handle(TDF_Attribute)& theAttr,
					const char* theType) 
{
  SALOMEDS_GenericAttribute_i* anAttr;
  TAttrID2FunMap::const_iterator anIter = __AttrID2FunMap__.find(theType);
  if(anIter != __AttrID2FunMap__.end()){
    const TAttrID2FunMap::mapped_type& aValue = anIter->second;
    
    //if(aValue.myIsCheckLockedStudy()) // mpv 03.02.05: creation of CORBA objects does not modify the study
    //  _study->CheckLocked();
    
    anAttr = aValue.myNewInstance(theAttr,this);
    return TAttrHolder(anAttr,anAttr->_this());
  }
  
  if(strncmp(theType,"AttributeTreeNode",17) == 0){
    anAttr = new SALOMEDS_AttributeTreeNode_i(theAttr,this);
    return TAttrHolder(anAttr,anAttr->_this());
  }
  
  if(strncmp(theType,"AttributeUserID",15) == 0){
    anAttr =  new SALOMEDS_AttributeUserID_i(theAttr,this);
    return TAttrHolder(anAttr,anAttr->_this());
  }
  
  return TAttrHolder();
}


SALOMEDS_SObject_i::TAttrHolder 
SALOMEDS_SObject_i::_FindGenAttribute(const char* theType)
{
  TAttrHolder anAttrHolder;
  TAttrMap::const_iterator anIter = myAttrMap.find(theType);
  if(anIter != myAttrMap.end())
    anAttrHolder = anIter->second;

  Standard_GUID aGUID = ::GetGUID(theType);
  Handle(TDF_Attribute) anAttr;

  if(_lab.FindAttribute(aGUID,anAttr)){
    SALOMEDS_GenericAttribute_i* aGenAttr = anAttrHolder.first;
    if(aGenAttr != NULL){
      if(aGenAttr->GetAttribute() != anAttr)
	aGenAttr->SetAttribute(anAttr);
    }else{
      anAttrHolder = _CreateGenAttribute(anAttr,theType);
    }
    aGenAttr = anAttrHolder.first;
    if(aGenAttr != NULL)
      myAttrMap[theType] = anAttrHolder;
  }else{
    //myAttrMap.erase(theType);
    //if(anGenAttr != NULL)
    //  anGenAttr->Destroy();
    return TAttrHolder();
  }

  return anAttrHolder;
}


CORBA::Boolean 
SALOMEDS_SObject_i::FindAttribute(SALOMEDS::GenericAttribute_out theAttribute, 
				  const char* theType)
{
  Locker lock;

  TAttrHolder anAttr = _FindGenAttribute(theType);
  SALOMEDS::GenericAttribute_var anGenAttr = anAttr.second;
  if(!CORBA::is_nil(anGenAttr)){
    theAttribute = SALOMEDS::GenericAttribute::_duplicate(anGenAttr);
    return true;
  }
  return false;
}


//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
Handle(TDF_Attribute) 
  SALOMEDS_SObject_i::_AddAttribute(const char* theType) 
{
  Handle(TDF_Attribute) anAttr;
  TAttrID2FunMap::const_iterator anIter = __AttrID2FunMap__.find(theType);
  if(anIter != __AttrID2FunMap__.end()){
    const TAttrID2FunMap::mapped_type& aValue = anIter->second;
    
    if(aValue.myIsCheckLockedStudy())
      _study->CheckLocked();
    
    anAttr = aValue.myNewAttribute();
    _lab.AddAttribute(anAttr);
    return anAttr;
  }
  
  if(strncmp(theType, "AttributeTreeNode",17) == 0){
    Standard_GUID aGUID;
    if(strcmp(theType, "AttributeTreeNode") == 0){
      aGUID = TDataStd_TreeNode::GetDefaultTreeID();
    }else{
      char aString[40] = "";
      sprintf(aString, &theType[21]);
      aGUID = Standard_GUID(aString); // create tree node GUID by name
    }
    if(!_lab.FindAttribute(aGUID,anAttr)){
      _study->CheckLocked();
      anAttr = TDataStd_TreeNode::Set(_lab,aGUID);
      return anAttr;
    }
  }
  
  if(strncmp(theType, "AttributeUserID",15) == 0){
    Standard_GUID aGUID = SALOMEDS_AttributeUserID_i::GetGUID();
    if(!_lab.FindAttribute(aGUID,anAttr)){
      _study->CheckLocked();
      anAttr = TDataStd_UAttribute::Set(_lab,aGUID);
      return anAttr;
    }
  }
  
  
  return anAttr;
}


SALOMEDS::GenericAttribute_ptr 
SALOMEDS_SObject_i::FindOrCreateAttribute(const char* theType)
{
  Locker lock;

  TAttrHolder anAttrHolder = _FindGenAttribute(theType);
  SALOMEDS::GenericAttribute_var anGenAttr = anAttrHolder.second;
  if(!anGenAttr->_is_nil())
    return anGenAttr._retn();

  Handle(TDF_Attribute) anAttr = _AddAttribute(theType);
  if(!anAttr.IsNull()){
    anAttrHolder = _CreateGenAttribute(anAttr,theType);
    anGenAttr = anAttrHolder.second;
    if(!anGenAttr->_is_nil())
      return anGenAttr._retn();
  }

  return SALOMEDS::GenericAttribute::_nil();
}


//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
void SALOMEDS_SObject_i::RemoveAttribute(const char* theType)
{
  Locker lock;

  _study->CheckLocked();
  if(strcmp(theType, "AttributeIOR") == 0) { // postponed removing of CORBA objects
    Handle(SALOMEDS_IORAttribute) anAttr;
    if(_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
      _study->AddPostponed(Str(anAttr->Get()));
    else 
      return;
  }
  TAttrMap::iterator anIter = myAttrMap.find(theType);
  if(anIter != myAttrMap.end()){
    //myAttrMap.erase(anIter);
  }
  _lab.ForgetAttribute(::GetGUID(theType));
}


void SALOMEDS_SObject_i::OnRemove()
{
  Handle(TDF_Reference) aReference;
  if(_lab.FindAttribute(TDF_Reference::GetID(),aReference)){
    Handle(SALOMEDS_TargetAttribute) aTarget;
    if(aReference->Get().FindAttribute(SALOMEDS_TargetAttribute::GetID(),aTarget))
      aTarget->Remove(_lab);
  }

  Handle(SALOMEDS_IORAttribute) anAttr; // postponed removing of CORBA objects
  if(_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),anAttr)){
    _study->AddPostponed(TCollection_AsciiString(anAttr->Get()).ToCString());
  }

  //myAttrMap.clear();

  //SALOMEDS_Study_i::TSObjectMap& anSObjectMap = _study->GetSObjectMap();
  //anSObjectMap.erase(_lab);
}
