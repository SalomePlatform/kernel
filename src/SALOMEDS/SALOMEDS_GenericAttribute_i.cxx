//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_GenericAttribute_i.cxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//  $Header$

#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>

#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS_AttributeSelectable_i.hxx"
#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS_AttributeFlags_i.hxx"
#include "SALOMEDS_AttributeGraphic_i.hxx"
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_AttributeTableOfInteger_i.hxx"
#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_AttributePythonObject_i.hxx"
#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "SALOMEDS_AttributeUserID_i.hxx"
#include "SALOMEDS_AttributeExternalFileDef_i.hxx"
#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS_StudyBuilder_i.hxx"

#include "SALOMEDS_SequenceOfRealAttribute.hxx"
#include "SALOMEDS_SequenceOfIntegerAttribute.hxx"
#include "SALOMEDS_IORAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_DrawableAttribute.hxx"
#include "SALOMEDS_SelectableAttribute.hxx"
#include "SALOMEDS_ExpandableAttribute.hxx"
#include "SALOMEDS_OpenedAttribute.hxx"
#include "SALOMEDS_FlagsAttribute.hxx"
#include "SALOMEDS_GraphicAttribute.hxx"
#include "SALOMEDS_TextColorAttribute.hxx"
#include "SALOMEDS_TextHighlightColorAttribute.hxx"
#include "SALOMEDS_PixMapAttribute.hxx"
#include "SALOMEDS_LocalIDAttribute.hxx"
#include "SALOMEDS_PythonObjectAttribute.hxx"
#include "SALOMEDS_TargetAttribute.hxx"
#include "SALOMEDS_StudyPropertiesAttribute.hxx"
#include "SALOMEDS_TableOfIntegerAttribute.hxx"
#include "SALOMEDS_TableOfRealAttribute.hxx"
#include "SALOMEDS_TableOfStringAttribute.hxx"
#include "SALOMEDS_ExternalFileDef.hxx"
#include "SALOMEDS_FileType.hxx"
#include "Utils_ExceptHandlers.hxx"

using namespace std;

#define RegisteredAttributes 28

UNEXPECT_CATCH(GALockProtection, SALOMEDS::GenericAttribute::LockProtection);

const char AttributesTypeNames[RegisteredAttributes][30] = {
  "AttributeReal", "AttributeInteger",
  "AttributeSequenceOfReal", "AttributeSequenceOfInteger",
  "AttributeName", "AttributeComment",
  "AttributeIOR", "AttributePersistentRef",
  "AttributeDrawable", "AttributeSelectable",
  "AttributeExpandable", "AttributeOpened",
  "AttributeTextColor", "AttributeTextHighlightColor", "AttributePixMap",
  "AttributeTreeNode", "AttributeLocalID",
  "AttributeTarget",
  "AttributeTableOfInteger", "AttributeTableOfReal", "AttributeTableOfString",
  "AttributeStudyProperties",
  "AttributePythonObject",
  "AttributeUserID",
  "AttributeExternalFileDef", "AttributeFileType",
  "AttributeFlags",
  "AttributeGraphic"
};

const Standard_GUID AttributesGUIDs[RegisteredAttributes] = {
  TDataStd_Real::GetID(), TDataStd_Integer::GetID(),
  SALOMEDS_SequenceOfRealAttribute::GetID(), SALOMEDS_SequenceOfIntegerAttribute::GetID(),
  TDataStd_Name::GetID(), TDataStd_Comment::GetID(),
  SALOMEDS_IORAttribute::GetID(), SALOMEDS_PersRefAttribute::GetID(),
  SALOMEDS_DrawableAttribute::GetID(), SALOMEDS_SelectableAttribute::GetID(),
  SALOMEDS_ExpandableAttribute::GetID(), SALOMEDS_OpenedAttribute::GetID(),
  SALOMEDS_TextColorAttribute::GetID(), SALOMEDS_TextHighlightColorAttribute::GetID(), SALOMEDS_PixMapAttribute::GetID(),
  TDataStd_TreeNode::GetDefaultTreeID(), SALOMEDS_LocalIDAttribute::GetID(),
  SALOMEDS_TargetAttribute::GetID(),
  SALOMEDS_TableOfIntegerAttribute::GetID(), SALOMEDS_TableOfRealAttribute::GetID(), SALOMEDS_TableOfStringAttribute::GetID(),
  SALOMEDS_StudyPropertiesAttribute::GetID(),
  SALOMEDS_PythonObjectAttribute::GetID(),
  Standard_GUID("FFFFFFFF-D9CD-11d6-945D-1050DA506788"),
  SALOMEDS_ExternalFileDef::GetID(), SALOMEDS_FileType::GetID(),
  SALOMEDS_FlagsAttribute::GetID(),
  SALOMEDS_GraphicAttribute::GetID()
};

void SALOMEDS_GenericAttribute_i::CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection) {
  Unexpect aCatch(GALockProtection);
  if (!_myAttr.IsNull() && _myAttr->IsValid() && !CORBA::is_nil(_myOrb)) {
    Handle(SALOMEDS_IORAttribute) Att;
    if (_myAttr->Label().Root().FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){
      TCollection_AsciiString ch(Att->Get());
      char* IOR = CORBA::string_dup(ch.ToCString());
      CORBA::Object_var obj = _myOrb->string_to_object(IOR);
      SALOMEDS::Study_ptr aStudy = SALOMEDS::Study::_narrow(obj);
//  	ASSERT(!CORBA::is_nil(aStudy));
      if (aStudy->NewBuilder()->HasOpenCommand()) return;
      if (aStudy->GetProperties()->IsLocked()) {
//  	  MESSAGE("Locked document modification !!!");
	throw SALOMEDS::GenericAttribute::LockProtection();
      }
    }
  }
}

SALOMEDS::SObject_ptr SALOMEDS_GenericAttribute_i::GetSObject() {
  if (_myAttr.IsNull() || _myAttr->Label().IsNull()) return SALOMEDS::SObject::_nil();
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (_myAttr->Label(),_myOrb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return so._retn();
}

Standard_GUID SALOMEDS_GenericAttribute_i::GetGUID(const char* theType) {
  Standard_Integer i;
  for(i = 0; i < RegisteredAttributes; i++) {
    if (strcmp(AttributesTypeNames[i], theType) == 0) return AttributesGUIDs[i];
  }

  if (strncmp(theType, "AttributeTreeNodeGUID",21) == 0) {
    char* aGUIDString = new char[40];
    sprintf(aGUIDString, &(theType[21]));
    Standard_GUID aGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
    delete(aGUIDString);
    return aGUID;
  }
  return Standard_GUID();
}

char* SALOMEDS_GenericAttribute_i::Type() {
  if (_myAttr.IsNull()) return "";
  int i;
  for(i = 0; i < RegisteredAttributes; i++) {
    if (_myAttr->ID() == AttributesGUIDs[i]) {
      CORBA::String_var aString(AttributesTypeNames[i]);
      return aString._retn();
    }
  }
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  if (!aNode.IsNull()) {
    char* aNodeName = new char[60];
    char aGUID[40];
    aNode->ID().ToCString(aGUID);
    sprintf(aNodeName, "AttributeTreeNodeGUID%s",aGUID);
    return aNodeName;
  }

  Handle(TDataStd_UAttribute) aUAttr = Handle(TDataStd_UAttribute)::DownCast(_myAttr);
  if (!aUAttr.IsNull()) {
    char* aUAttrName = new char[60];
    char aGUID[40];
    aUAttr->ID().ToCString(aGUID);
    sprintf(aUAttrName, "AttributeUserID_%s",aGUID); 
    return aUAttrName;
  }

  return "";
}

SALOMEDS::GenericAttribute_ptr SALOMEDS_GenericAttribute_i::CreateAttribute(CORBA::ORB_ptr theOrb,
									    const Handle(TDF_Attribute)& theAttr) {

  __ReturnCORBAAttribute(TDataStd_Real, AttributeReal);
  __ReturnCORBAAttribute(TDataStd_Integer, AttributeInteger);
  __ReturnCORBAAttribute(SALOMEDS_SequenceOfRealAttribute, AttributeSequenceOfReal);
  __ReturnCORBAAttribute(SALOMEDS_SequenceOfIntegerAttribute, AttributeSequenceOfInteger);
  __ReturnCORBAAttribute(TDataStd_Name, AttributeName);
  __ReturnCORBAAttribute(TDataStd_Comment, AttributeComment);
  __ReturnCORBAAttribute(SALOMEDS_IORAttribute, AttributeIOR);
  __ReturnCORBAAttribute(SALOMEDS_PersRefAttribute, AttributePersistentRef);
  __ReturnCORBAAttribute(SALOMEDS_DrawableAttribute, AttributeDrawable);
  __ReturnCORBAAttribute(SALOMEDS_SelectableAttribute, AttributeSelectable);
  __ReturnCORBAAttribute(SALOMEDS_ExpandableAttribute, AttributeExpandable);
  __ReturnCORBAAttribute(SALOMEDS_OpenedAttribute, AttributeOpened);
  __ReturnCORBAAttribute(SALOMEDS_TextColorAttribute, AttributeTextColor);
  __ReturnCORBAAttribute(SALOMEDS_TextHighlightColorAttribute, AttributeTextHighlightColor);
  __ReturnCORBAAttribute(SALOMEDS_PixMapAttribute, AttributePixMap);
  __ReturnCORBAAttribute(SALOMEDS_LocalIDAttribute, AttributeLocalID);
  __ReturnCORBAAttribute(SALOMEDS_TargetAttribute, AttributeTarget);
  __ReturnCORBAAttribute(SALOMEDS_TableOfIntegerAttribute, AttributeTableOfInteger);
  __ReturnCORBAAttribute(SALOMEDS_TableOfRealAttribute, AttributeTableOfReal);
  __ReturnCORBAAttribute(SALOMEDS_TableOfStringAttribute, AttributeTableOfString);
  __ReturnCORBAAttribute(SALOMEDS_StudyPropertiesAttribute, AttributeStudyProperties);
  __ReturnCORBAAttribute(SALOMEDS_PythonObjectAttribute, AttributePythonObject);
  __ReturnCORBAAttribute(SALOMEDS_ExternalFileDef, AttributeExternalFileDef);
  __ReturnCORBAAttribute(SALOMEDS_FileType, AttributeFileType);
  __ReturnCORBAAttribute(SALOMEDS_FlagsAttribute, AttributeFlags);
  __ReturnCORBAAttribute(SALOMEDS_GraphicAttribute, AttributeGraphic);

  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(theAttr);
  if (!aNode.IsNull()) {
    SALOMEDS_AttributeTreeNode_i* Attr = new SALOMEDS_AttributeTreeNode_i(aNode, theOrb); \
    return Attr->AttributeTreeNode::_this(); \
  }

  Handle(TDataStd_UAttribute) aUAttr = Handle(TDataStd_UAttribute)::DownCast(theAttr);
  if (!aUAttr.IsNull()) {
    SALOMEDS_AttributeUserID_i* Attr = new SALOMEDS_AttributeUserID_i(aUAttr, theOrb); \
    return Attr->AttributeUserID::_this(); \
  }
  return SALOMEDS::GenericAttribute::_nil();
}
