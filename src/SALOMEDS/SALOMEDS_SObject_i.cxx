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

using namespace std;
#include "utilities.h"
#include "SALOMEDS_SObject_i.hxx"
//SALOMEDS Headers
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_IORAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_SequenceOfRealAttribute.hxx"
#include "SALOMEDS_SequenceOfIntegerAttribute.hxx"
#include "SALOMEDS_TableOfRealAttribute.hxx"
#include "SALOMEDS_TableOfIntegerAttribute.hxx"
#include "SALOMEDS_IORAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_StudyPropertiesAttribute.hxx"
#include "SALOMEDS_PythonObjectAttribute.hxx"
#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS_AttributeComment_i.hxx"
#include "SALOMEDS_AttributeExternalFileDef_i.hxx"
#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS_AttributeName_i.hxx"
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS_AttributeSelectable_i.hxx"
#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS_AttributeUserID_i.hxx"
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_AttributeTableOfInteger_i.hxx"
#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_AttributePythonObject_i.hxx"
#include <TDF_AttributeIterator.hxx>

//============================================================================
/*! Function : ReturnGUIDForAttribute
 *  Purpose  : 
 */
//============================================================================

Standard_GUID  
SALOMEDS_SObject_i::ReturnGUIDForAttribute(const char* aTypeOfAttribute)
{
   Standard_GUID TypeID;
  
   if      (strcmp(aTypeOfAttribute, "AttributeReal") == 0 )               return TDataStd_Real::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeInteger") == 0 )            return TDataStd_Integer::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfReal") == 0 )     return SALOMEDS_SequenceOfRealAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfInteger") == 0 )  return SALOMEDS_SequenceOfIntegerAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeName") == 0 )               return TDataStd_Name::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeComment") == 0 )            return TDataStd_Comment::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeExternalFileDef") == 0 )    return SALOMEDS_ExternalFileDef::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeFileType") == 0 )           return SALOMEDS_FileType::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0 )                return SALOMEDS_IORAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributePersistentRef") == 0 )      return SALOMEDS_PersRefAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeDrawable") == 0 )           return SALOMEDS_DrawableAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeSelectable") == 0 )         return SALOMEDS_SelectableAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeExpandable") == 0 )         return SALOMEDS_ExpandableAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeOpened") == 0 )             return SALOMEDS_OpenedAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTextColor") == 0 )          return SALOMEDS_TextColorAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTextHighlightColor") == 0 ) return SALOMEDS_TextHighlightColorAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributePixMap") == 0 )             return SALOMEDS_PixMapAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTreeNode") == 0 )           return TDataStd_TreeNode::GetDefaultTreeID();
   else if (strcmp(aTypeOfAttribute, "AttributeLocalID") == 0 )            return SALOMEDS_LocalIDAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTarget") == 0 )             return SALOMEDS_TargetAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTableOfInteger") == 0 )     return SALOMEDS_TableOfIntegerAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTableOfReal") == 0 )        return SALOMEDS_TableOfRealAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeTableOfString") == 0 )      return SALOMEDS_TableOfStringAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeStudyProperties") == 0 )    return SALOMEDS_StudyPropertiesAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributePythonObject") == 0 )       return SALOMEDS_PythonObjectAttribute::GetID();
   else if (strcmp(aTypeOfAttribute, "AttributeUserID") == 0 )             return SALOMEDS_AttributeUserID_i::DefaultID();
   else if (strncmp(aTypeOfAttribute,"AttributeTreeNodeGUID",21) == 0) {
     char* aGUIDString = new char[40];
     sprintf(aGUIDString, &(aTypeOfAttribute[21]));
     Standard_GUID aGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
     delete(aGUIDString);
     return aGUID;
   }
   else Standard_NoSuchObject::Raise("SALOMEDS_SObject_i::ReturnGUIDForAttribute");
}

//============================================================================
/*! Function : AttributeIDToType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::AttributeIDToType(Standard_GUID ID)
{
  if      (ID == TDataStd_Name::GetID())                        return "AttributeName";
  else if (ID == TDataStd_Comment::GetID())                     return "AttributeComment";
  else if (ID == SALOMEDS_ExternalFileDef::GetID())             return "AttributeExternalFileDef";
  else if (ID == SALOMEDS_FileType::GetID())                    return "AttributeFileType";
  else if (ID == SALOMEDS_IORAttribute::GetID())                return "AttributeIOR"; 
  else if (ID == SALOMEDS_PersRefAttribute::GetID())            return "AttributePersistentRef";
  else if (ID == TDataStd_Real::GetID())                        return "AttributeReal";
  else if (ID == TDataStd_Integer::GetID())                     return "AttributeInteger";
  else if (ID == SALOMEDS_SequenceOfRealAttribute::GetID())     return "AttributeSequenceOfReal";
  else if (ID == SALOMEDS_SequenceOfIntegerAttribute::GetID())  return "AttributeSequenceOfInteger";
  else if (ID == SALOMEDS_DrawableAttribute::GetID())           return "AttributeDrawable";
  else if (ID == SALOMEDS_SelectableAttribute::GetID())         return "AttributeSelectable";
  else if (ID == SALOMEDS_ExpandableAttribute::GetID())         return "AttributeExpandable";
  else if (ID == SALOMEDS_OpenedAttribute::GetID())             return "AttributeOpened";
  else if (ID == SALOMEDS_TextColorAttribute::GetID())          return "AttributeTextColor";
  else if (ID == SALOMEDS_TextHighlightColorAttribute::GetID()) return "AttributeTextHighlightColor";
  else if (ID == SALOMEDS_PixMapAttribute::GetID())             return "AttributePixMap";
  else if (ID == TDataStd_TreeNode::GetDefaultTreeID())         return "AttributeTreeNode";
  else if (ID == SALOMEDS_LocalIDAttribute::GetID())            return "AttributeLocalID";
  else if (ID == SALOMEDS_TargetAttribute::GetID())             return "AttributeTarget";
  else if (ID == SALOMEDS_TableOfIntegerAttribute::GetID())     return "AttributeTableOfInteger";
  else if (ID == SALOMEDS_TableOfRealAttribute::GetID())        return "AttributeTableOfReal";
  else if (ID == SALOMEDS_TableOfStringAttribute::GetID())      return "AttributeTableOfString";
  else if (ID == SALOMEDS_StudyPropertiesAttribute::GetID())    return "AttributeStudyProperties";
  else if (ID == SALOMEDS_PythonObjectAttribute::GetID())       return "AttributePythonObject";
  else if (ID == SALOMEDS_AttributeUserID_i::DefaultID())       return "AttributeUserID";
  // 08.01.2003 mpv: this method must be called only for attributes with constant GUID
  else Standard_NoSuchObject::Raise("SALOMEDS_SObject_i::AttributeIDToType");
}


//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SObject_i::SALOMEDS_SObject_i(const TDF_Label lab, CORBA::ORB_ptr orb)
  :_lab(lab)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _value = NULL;
  _type = NULL;
  _name = NULL;
  _liste_ba_type.resize(0);
}
  

//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SObject_i::~SALOMEDS_SObject_i()
{
  CORBA::string_free(_value);
  CORBA::string_free(_type);
  CORBA::string_free(_name);
}
  
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetID()
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry (_lab,anEntry);
  return CORBA::string_dup(anEntry.ToCString());
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_SObject_i::GetFatherComponent()
{
  TDF_Label LF = _lab;
  while (!SALOMEDS_SComponent_i::IsA(LF) && !LF.IsRoot()) {
    LF = LF.Father();
  }
  SALOMEDS_SComponent_i *  so_servant = new SALOMEDS_SComponent_i (LF,_orb);
  SALOMEDS::SComponent_var so;
  so= SALOMEDS::SComponent::_narrow(so_servant->SComponent::_this()); 
  return so;
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_SObject_i::GetFather()
{
  TDF_Label LF = _lab.Father();

  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (LF,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return so;
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_SObject_i::GetStudy()
{
  TDF_Label Root = _lab.Root();
  Handle(SALOMEDS_IORAttribute) Att;
  char* IOR;
  if (Root.FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){
    TCollection_AsciiString ch(Att->Get());
    IOR = CORBA::string_dup(ch.ToCString());
    CORBA::Object_var obj = _orb->string_to_object(IOR);
    SALOMEDS::Study_var Study = SALOMEDS::Study::_narrow(obj) ;
    ASSERT(!CORBA::is_nil(Study));
    return SALOMEDS::Study::_duplicate(Study); //return Study = abort...
  }
  INFOS("Problem GetStudy");
  return SALOMEDS::Study::_nil();
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindAttribute (SALOMEDS::GenericAttribute_out anAttribute, 
						  const char* aTypeOfAttribute)
{
  Handle(TDF_Attribute) anAttr;
  Standard_Boolean found = _lab.FindAttribute(SALOMEDS_SObject_i::ReturnGUIDForAttribute(aTypeOfAttribute), anAttr);
  if (found) {
    if (strcmp(aTypeOfAttribute, "AttributeReal") == 0 )  {
      SALOMEDS_AttributeReal_i* Attr = new SALOMEDS_AttributeReal_i(Handle(TDataStd_Real)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeReal::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeInteger") == 0 )  {
      SALOMEDS_AttributeInteger_i* Attr = new SALOMEDS_AttributeInteger_i(Handle(TDataStd_Integer)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeInteger::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfReal") == 0 )  {
      SALOMEDS_AttributeSequenceOfReal_i* Attr = new SALOMEDS_AttributeSequenceOfReal_i(Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeSequenceOfReal::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfInteger") == 0 )  {
      SALOMEDS_AttributeSequenceOfInteger_i* Attr = new SALOMEDS_AttributeSequenceOfInteger_i(Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeSequenceOfInteger::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeName") == 0 ) {
      SALOMEDS_AttributeName_i* Attr = new SALOMEDS_AttributeName_i(Handle(TDataStd_Name)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeName::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeComment") == 0 ) {
      SALOMEDS_AttributeComment_i* Attr = new SALOMEDS_AttributeComment_i(Handle(TDataStd_Comment)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeComment::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeExternalFileDef") == 0 ) {
      SALOMEDS_AttributeExternalFileDef_i* Attr = new SALOMEDS_AttributeExternalFileDef_i(Handle(SALOMEDS_ExternalFileDef)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeExternalFileDef::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeFileType") == 0 ) {
      SALOMEDS_AttributeFileType_i* Attr = new SALOMEDS_AttributeFileType_i(Handle(SALOMEDS_FileType)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeFileType::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0 ) {
      SALOMEDS_AttributeIOR_i* Attr = new SALOMEDS_AttributeIOR_i(Handle(SALOMEDS_IORAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeIOR::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributePersistentRef") == 0 )  {
      SALOMEDS_AttributePersistentRef_i* Attr = new SALOMEDS_AttributePersistentRef_i(Handle(SALOMEDS_PersRefAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributePersistentRef::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeDrawable") == 0 )  {
      SALOMEDS_AttributeDrawable_i* Attr = new SALOMEDS_AttributeDrawable_i(Handle(SALOMEDS_DrawableAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeDrawable::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeSelectable") == 0 )  {
      SALOMEDS_AttributeSelectable_i* Attr= new SALOMEDS_AttributeSelectable_i(Handle(SALOMEDS_SelectableAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeSelectable::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeExpandable") == 0 )  {
      SALOMEDS_AttributeExpandable_i* Attr = new SALOMEDS_AttributeExpandable_i(Handle(SALOMEDS_ExpandableAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeExpandable::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeOpened") == 0 )  {
      SALOMEDS_AttributeOpened_i* Attr= new SALOMEDS_AttributeOpened_i(Handle(SALOMEDS_OpenedAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeOpened::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTextColor") == 0 )  {
      SALOMEDS_AttributeTextColor_i* Attr= new SALOMEDS_AttributeTextColor_i(Handle(SALOMEDS_TextColorAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTextColor::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTextHighlightColor") == 0 )  {
      SALOMEDS_AttributeTextHighlightColor_i* Attr= new SALOMEDS_AttributeTextHighlightColor_i(Handle(SALOMEDS_TextHighlightColorAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTextHighlightColor::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributePixMap") == 0 )  {
      SALOMEDS_AttributePixMap_i* Attr= new SALOMEDS_AttributePixMap_i(Handle(SALOMEDS_PixMapAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributePixMap::_this();
      return Standard_True;
    }
    else if (strncmp(aTypeOfAttribute, "AttributeTreeNode",17) == 0 )  {
      SALOMEDS_AttributeTreeNode_i* Attr= new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTreeNode::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeLocalID") == 0 )  {
      SALOMEDS_AttributeLocalID_i* Attr= new SALOMEDS_AttributeLocalID_i(Handle(SALOMEDS_LocalIDAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeLocalID::_this();
      return Standard_True;
    }
    else if (strncmp(aTypeOfAttribute, "AttributeUserID",15) == 0 )  {
      SALOMEDS_AttributeUserID_i* Attr= new SALOMEDS_AttributeUserID_i(Handle(TDataStd_UAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeUserID::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTarget") == 0 )  {
      SALOMEDS_AttributeTarget_i* Attr= new SALOMEDS_AttributeTarget_i(Handle(SALOMEDS_TargetAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTarget::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTableOfInteger") == 0 )  {
      SALOMEDS_AttributeTableOfInteger_i* Attr = new SALOMEDS_AttributeTableOfInteger_i(Handle(SALOMEDS_TableOfIntegerAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTableOfInteger::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTableOfReal") == 0 )  {
      SALOMEDS_AttributeTableOfReal_i* Attr = new SALOMEDS_AttributeTableOfReal_i(Handle(SALOMEDS_TableOfRealAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTableOfReal::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTableOfString") == 0 )  {
      SALOMEDS_AttributeTableOfString_i* Attr = new SALOMEDS_AttributeTableOfString_i(Handle(SALOMEDS_TableOfStringAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTableOfString::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeStudyProperties") == 0 )  {
      SALOMEDS_AttributeStudyProperties_i* Attr = new SALOMEDS_AttributeStudyProperties_i(Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeStudyProperties::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributePythonObject") == 0 )  {
      SALOMEDS_AttributePythonObject_i* Attr = new SALOMEDS_AttributePythonObject_i(Handle(SALOMEDS_PythonObjectAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributePythonObject::_this();
      return Standard_True;
    }
  } else return Standard_False;
}

//============================================================================
/*! Function : GetAllAttributes
 *  Purpose  : Returns list of all attributes for this sobject
 */
//============================================================================

SALOMEDS::ListOfAttributes* SALOMEDS_SObject_i::GetAllAttributes()
{
  Standard_Integer NumAttr = _lab.NbAttributes();
  SALOMEDS::ListOfAttributes_var SeqOfAttr = new SALOMEDS::ListOfAttributes;
  SeqOfAttr->length(NumAttr);
  if (NumAttr != 0) {
    Standard_Integer i = 0;
    for(TDF_AttributeIterator iter(_lab);iter.More();iter.Next()) {
      Handle(TDF_Attribute) anAttr = iter.Value();
      SALOMEDS::GenericAttribute_var anAttribute;
      if (ReturnGUIDForAttribute("AttributeReal") == anAttr->ID()) {
        SALOMEDS_AttributeReal_i* Attr= new SALOMEDS_AttributeReal_i(Handle(TDataStd_Real)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeReal::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeInteger") == anAttr->ID()) {
        SALOMEDS_AttributeInteger_i* Attr= new SALOMEDS_AttributeInteger_i(Handle(TDataStd_Integer)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeInteger::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeSequenceOfReal") == anAttr->ID()) {
        SALOMEDS_AttributeSequenceOfReal_i* Attr= new SALOMEDS_AttributeSequenceOfReal_i(Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeSequenceOfReal::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeSequenceOfInteger") == anAttr->ID()) {
        SALOMEDS_AttributeSequenceOfInteger_i* Attr= new SALOMEDS_AttributeSequenceOfInteger_i(Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeSequenceOfInteger::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeName") == anAttr->ID()) {
        SALOMEDS_AttributeName_i* Attr= new SALOMEDS_AttributeName_i(Handle(TDataStd_Name)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeName::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeComment") == anAttr->ID()) {
        SALOMEDS_AttributeComment_i* Attr= new SALOMEDS_AttributeComment_i(Handle(TDataStd_Comment)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeComment::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeExternalFileDef") == anAttr->ID()) {
        SALOMEDS_AttributeExternalFileDef_i* Attr= new SALOMEDS_AttributeExternalFileDef_i(Handle(SALOMEDS_ExternalFileDef)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeExternalFileDef::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeFileType") == anAttr->ID()) {
        SALOMEDS_AttributeFileType_i* Attr= new SALOMEDS_AttributeFileType_i(Handle(SALOMEDS_FileType)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeFileType::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeIOR") == anAttr->ID()) {
        SALOMEDS_AttributeIOR_i* Attr= new SALOMEDS_AttributeIOR_i(Handle(SALOMEDS_IORAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeIOR::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributePersistentRef") == anAttr->ID()) {
        SALOMEDS_AttributePersistentRef_i* Attr= new SALOMEDS_AttributePersistentRef_i(Handle(SALOMEDS_PersRefAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributePersistentRef::_this();
      }
      else if (ReturnGUIDForAttribute("AttributeDrawable") == anAttr->ID()) {
        SALOMEDS_AttributeDrawable_i* Attr= new SALOMEDS_AttributeDrawable_i(Handle(SALOMEDS_DrawableAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeDrawable::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeSelectable") == anAttr->ID()) {
        SALOMEDS_AttributeSelectable_i* Attr= new SALOMEDS_AttributeSelectable_i(Handle(SALOMEDS_SelectableAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeSelectable::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeExpandable") == anAttr->ID()) {
        SALOMEDS_AttributeExpandable_i* Attr= new SALOMEDS_AttributeExpandable_i(Handle(SALOMEDS_ExpandableAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeExpandable::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeOpened") == anAttr->ID()) {
        SALOMEDS_AttributeOpened_i* Attr= new SALOMEDS_AttributeOpened_i(Handle(SALOMEDS_OpenedAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeOpened::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTextColor") == anAttr->ID()) {
        SALOMEDS_AttributeTextColor_i* Attr= new SALOMEDS_AttributeTextColor_i(Handle(SALOMEDS_TextColorAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTextColor::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTextHighlightColor") == anAttr->ID()) {
        SALOMEDS_AttributeTextHighlightColor_i* Attr= new SALOMEDS_AttributeTextHighlightColor_i(Handle(SALOMEDS_TextHighlightColorAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTextHighlightColor::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributePixMap") == anAttr->ID()) {
        SALOMEDS_AttributePixMap_i* Attr= new SALOMEDS_AttributePixMap_i(Handle(SALOMEDS_PixMapAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributePixMap::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTreeNode") == anAttr->ID()) {
        SALOMEDS_AttributeTreeNode_i* Attr= new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTreeNode::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeLocalID") == anAttr->ID()) {
        SALOMEDS_AttributeLocalID_i* Attr= new SALOMEDS_AttributeLocalID_i(Handle(SALOMEDS_LocalIDAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeLocalID::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTarget") == anAttr->ID()) {
        SALOMEDS_AttributeTarget_i* Attr= new SALOMEDS_AttributeTarget_i(Handle(SALOMEDS_TargetAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTarget::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTableOfInteger") == anAttr->ID()) {
        SALOMEDS_AttributeTableOfInteger_i* Attr= new SALOMEDS_AttributeTableOfInteger_i(Handle(SALOMEDS_TableOfIntegerAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTableOfInteger::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTableOfReal") == anAttr->ID()) {
        SALOMEDS_AttributeTableOfReal_i* Attr= new SALOMEDS_AttributeTableOfReal_i(Handle(SALOMEDS_TableOfRealAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTableOfReal::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeTableOfString") == anAttr->ID()) {
        SALOMEDS_AttributeTableOfString_i* Attr= new SALOMEDS_AttributeTableOfString_i(Handle(SALOMEDS_TableOfStringAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTableOfString::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributeStudyProperties") == anAttr->ID()) {
        SALOMEDS_AttributeStudyProperties_i* Attr= new SALOMEDS_AttributeStudyProperties_i(Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeStudyProperties::_this();
      } 
      else if (ReturnGUIDForAttribute("AttributePythonObject") == anAttr->ID()) {
        SALOMEDS_AttributePythonObject_i* Attr= new SALOMEDS_AttributePythonObject_i(Handle(SALOMEDS_PythonObjectAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributePythonObject::_this();
      } 
      else if (!Handle(TDataStd_UAttribute)::DownCast(anAttr).IsNull()) {
        SALOMEDS_AttributeUserID_i* Attr= new SALOMEDS_AttributeUserID_i(Handle(TDataStd_UAttribute)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeUserID::_this();
      } 
      else if (!Handle(TDataStd_TreeNode)::DownCast(anAttr).IsNull()) {
        SALOMEDS_AttributeTreeNode_i* Attr= new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(anAttr), _orb);
        anAttribute = Attr->AttributeTreeNode::_this();
      } else {
	// references attributes, for an example, never returns
	continue;
      }

      SeqOfAttr[i] =  anAttribute;
      i++;
    }
  }
  return SeqOfAttr._retn();
}


//============================================================================
/*! Function : ReferencedObject
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::ReferencedObject(SALOMEDS::SObject_out obj)
{
  Handle(TDF_Reference) Ref;
  if (!_lab.FindAttribute(TDF_Reference::GetID(),Ref))
    return false;
  
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (Ref->Get(),_orb);
  obj  = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return true;
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindSubObject(long atag, SALOMEDS::SObject_out obj)
{
  TDF_Label L = _lab.FindChild(atag,false);
  if (L.IsNull()) return false;
  
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (L,_orb);
  obj  = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return true;
    
}  

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::Name()
{
  return CORBA::string_dup(_name);
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
void  SALOMEDS_SObject_i::Name(const char* name)
{
  _name = CORBA::string_dup(name);
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Tag()
{
  return _lab.Tag();
}
