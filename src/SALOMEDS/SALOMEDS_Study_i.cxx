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
//  File   : SALOMEDS_Study_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "utilities.h"
#include "SALOMEDS_Study_i.hxx"

#include "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"
#include <TColStd_SequenceOfExtendedString.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Owner.hxx>
#include <CDM_Document.hxx>
#include <CDM_Application.hxx>
#include <TDF_ChildIDIterator.hxx>
#include <SALOME_GenericObj_i.hh>
#include "SALOMEDS_LocalIDAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_UseCaseIterator_i.hxx"
using namespace std;


#define DIRECTORYID 16661
#define FILEID "FILE: "
#define FILELOCALID 26662 

//============================================================================
/*! Function : SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i constructor
 */
//============================================================================
SALOMEDS_Study_i::SALOMEDS_Study_i(const Handle(TDocStd_Document) doc,
				   CORBA::ORB_ptr                 orb,
				   const char* study_name)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _doc = doc;
  _name = new char[strlen(study_name) +1];
  strcpy(_name,study_name);
  _isSaved = false ;
  _URL = NULL;
  _StudyId = -1;
  _autoFill = true;
  myNbPostponed.Append(0);
  myNbUndos = 0;
}
  
//============================================================================
/*! Function : ~SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i destructor
 */
//============================================================================
SALOMEDS_Study_i::~SALOMEDS_Study_i()
{
  delete [] _name ;
  delete [] _URL ;
}  

//============================================================================
/*! Function : GetPersistentReference
 *  Purpose  : Get persistent reference of study (idem URL())
 */
//============================================================================
char* SALOMEDS_Study_i::GetPersistentReference()
{
  return URL();
}
//============================================================================
/*! Function : GetTransientReference
 *  Purpose  : Get IOR of the Study (registred in OCAF document in doc->Root)
 */
//============================================================================
char* SALOMEDS_Study_i::GetTransientReference()
{
  CORBA::String_var IOR;

  Handle(SALOMEDS_IORAttribute) Att;
  TDF_Label _lab = _doc->GetData()->Root();
  if (!_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){

    TCollection_AsciiString ch(Att->Get());
    IOR = CORBA::string_dup(ch.ToCString());
  }
  else IOR = CORBA::string_dup(""); // NULL ?

  return CORBA::string_dup(IOR); 
}

//============================================================================
/*! Function : IsEmpty
 *  Purpose  : Detect if study is empty
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsEmpty()
{
  if (_doc.IsNull()) return true;
  return _doc->IsEmpty();
}

//============================================================================
/*! Function : FindComponent
 *  Purpose  : Find a Component with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_Study_i::FindComponent (const char* aComponentName)
{
  bool _find = false;
  Standard_CString name;
  SALOMEDS::SComponentIterator_var itcomp = NewComponentIterator();
  SALOMEDS::SComponent_var compo;

  for (; itcomp->More(); itcomp->Next()) {
    SALOMEDS::SComponent_var SC = itcomp->Value();
    name = SC->ComponentDataType();
    //ED if ( TCollection_AsciiString(name).IsEqual(TCollection_AsciiString(strdup(aComponentName))) ) {
    if(strcmp(aComponentName,name) == 0){
      _find = true;
      return SALOMEDS::SComponent::_narrow(SC); 
    }
  }
  if(!_find)
    {
      return SALOMEDS::SComponent::_nil();
    }
  return compo;
}

//============================================================================
/*! Function : FindComponentID
 *  Purpose  : Find a Component from it's ID
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_Study_i::FindComponentID(const char* aComponentID)
{
  // Iterate on each components defined in the study
  // Get the component ID and compare with aComponentID 
  bool _find = false;
  char *ID;
  SALOMEDS::SComponent_ptr compo;

  SALOMEDS::SComponentIterator_var itcomp = NewComponentIterator();
  for (; itcomp->More(); itcomp->Next()) {
    SALOMEDS::SComponent_var SC = itcomp->Value();
    ID = SC->GetID();
    if(strcmp(aComponentID,ID)==0)
      {
	// ComponentID found
	_find = true;
        compo = SALOMEDS::SComponent::_narrow(SC);
      }
  }
  if(!_find)
    {
      compo = SALOMEDS::SComponent::_nil();
    }
  return compo;
}

//============================================================================
/*! Function : FindObject
 *  Purpose  : Find an Object with SALOMEDS::Name = anObjectName
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObject(const char* anObjectName)
{
  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool _find = false;
  SALOMEDS::SObject_ptr RefSO = SALOMEDS::SObject::_nil();

  SALOMEDS::SComponentIterator_var it = NewComponentIterator();
  for (; it->More();it->Next()){
    if(!_find)
      {
	SALOMEDS::SComponent_var SC = it->Value();
	SALOMEDS::GenericAttribute_var anAttr;
	if (SC->FindAttribute(anAttr,"AttributeName")) 
	{
	  SALOMEDS::AttributeName_var Name = SALOMEDS::AttributeName::_narrow(anAttr);
	  CORBA::String_var Val = Name->Value();
	  if (strcmp(Val, anObjectName) == 0)
	    {
	      _find = true;
	      RefSO = SALOMEDS::SObject::_narrow(SC);
	    }
	}
	if (!_find) RefSO =  _FindObject(SC,anObjectName, _find);
      }
  }
  return RefSO;
}

//============================================================================
/*! Function : FindObjectID
 *  Purpose  : Find an Object with ID = anObjectID
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectID(const char* anObjectID)
{
  // Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->GetData(), (char*)anObjectID, Lab);
  
  if (Lab.IsNull()) return SALOMEDS::SObject::_nil();
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (Lab,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return so;

}

//============================================================================
/*! Function : CreateObjectID
 *  Purpose  : Creates an Object with ID = anObjectID
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::CreateObjectID(const char* anObjectID)
{
  // Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->GetData(), (char*)anObjectID, Lab, Standard_True);
  
  if (Lab.IsNull()) return SALOMEDS::SObject::_nil();
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (Lab,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return so;

}

//============================================================================
/*! Function : FindObjectByName
 *  Purpose  : Find Objects with SALOMEDS::Name = anObjectName in a Component
 *           : with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDS::Study::ListOfSObject* SALOMEDS_Study_i::FindObjectByName( const char* anObjectName,
								    const char* aComponentName )
{
  SALOMEDS::Study::ListOfSObject_var listSO = new SALOMEDS::Study::ListOfSObject ;
  listSO->length(0);
  
  SALOMEDS::SComponent_ptr compo = FindComponent(aComponentName) ;
  if ( compo->_is_nil() ) {
    MESSAGE ("In FindObjectByName() :  Component named " << aComponentName << " not found ");
    return listSO._retn();
  }

  // Iterate on each object and subobject of the component
  // If objectName is found add it to the list of SObjects 
  char *name;
  char *childName ;
  SALOMEDS::SObject_ptr addSO = SALOMEDS::SObject::_nil();

  CORBA::String_var compoId = compo->GetID();
  SALOMEDS::ChildIterator_var it = NewChildIterator(compo);
  int length = 0 ;
  for ( ; it->More();it->Next() ) {
    
    SALOMEDS::SObject_var CSO = it->Value();
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::AttributeName_var    aName;
    if ( CSO->FindAttribute(anAttr, "AttributeName") ) {
      aName = SALOMEDS::AttributeName::_narrow(anAttr);
      name = aName->Value();
      if ( strcmp( name, anObjectName ) == 0) {
	addSO = SALOMEDS::SObject::_narrow(CSO);
	/* add to list */
	length++ ;
	listSO->length(length);
	listSO[length-1] = addSO ;
      }
      
      /* looks also for eventual children */
      bool found = false ;
      addSO = _FindObject( CSO, anObjectName, found ) ;
      if( found) {
	length++ ;
	listSO->length(length);
	listSO[length-1] = addSO ;
      }
    }
  }
  return listSO._retn() ;
}



//============================================================================
/*! Function : FindObjectIOR
 *  Purpose  : Find an Object with IOR = anObjectIOR
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectIOR(const char* anObjectIOR)
{
  // firstly searching in the datamap for optimization
  CORBA::String_var anIOR = CORBA::string_dup(anObjectIOR);
  if (myIORLabels.IsBound(TCollection_ExtendedString(anIOR))) {
    SALOMEDS_SObject_i* aResult = new SALOMEDS_SObject_i(myIORLabels.Find(TCollection_ExtendedString(anIOR)),_orb);
    // 11 oct 2002: forbidden attributes must be checked here
    SALOMEDS::GenericAttribute_var anAttr;
    if (!aResult->FindAttribute(anAttr,"AttributeIOR")) {
      myIORLabels.UnBind(TCollection_ExtendedString(anIOR));
    } else return aResult->_this();
  }
  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool _find = false;
  SALOMEDS::SObject_ptr RefSO = SALOMEDS::SObject::_nil();

  SALOMEDS::SComponentIterator_var it = NewComponentIterator();
  for (; it->More();it->Next()){
    if(!_find)
      {
	SALOMEDS::SComponent_var SC = it->Value();
	SALOMEDS::GenericAttribute_var anAttr;
	if (SC->FindAttribute(anAttr,"AttributeIOR")) 
	{
	  SALOMEDS::AttributeIOR_var IOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
          CORBA::String_var Val = IOR->Value();
	  if (strcmp(Val, anObjectIOR) == 0)
	    {
	      _find = true;
	      RefSO = SALOMEDS::SObject::_narrow(SC);
	    }
	}
	if (!_find) 
	  RefSO =  _FindObjectIOR(SC,anObjectIOR, _find);
      }
  }
  if (!RefSO->_is_nil()) MESSAGE("SALOMEDS_Study_i::FindObjectIOR: found label with old methods");

  return RefSO;
}

//============================================================================
/*! Function : FindObjectByPath
 *  Purpose  : Find an Object by its path = thePath
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectByPath(const char* thePath)
{
  TCollection_AsciiString aPath(CORBA::string_dup(thePath)), aToken;
  SALOMEDS::SObject_var aSO = SALOMEDS::SObject::_nil();
  int i = 1, aLength = aPath.Length();
  bool isRelative = false;

  if(aLength == 0) {  //Empty path - return the current context
    SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (_current, _orb);
    aSO = SALOMEDS::SObject::_narrow(so_servant->_this()); 
    return aSO._retn();
  }

  if(aPath.Value(1) != '/')  //Relative path 
    isRelative = true;

  TDF_ChildIterator anIterator;
  TDF_Label aLabel;
  Handle(TDataStd_Name) anAttr;

  if(isRelative) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext(); 
    anIterator.Initialize(_current, Standard_False);
  }
  else {
    if(aPath.Length() == 1 && aPath.Value(1) == '/') {    //Root
      SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (_doc->Main(), _orb);
      aSO = SALOMEDS::SObject::_narrow(so_servant->_this());
      return aSO._retn();
    }
    anIterator.Initialize(_doc->Main(), Standard_False);
  }

  while(i <= aLength) {

    aToken = aPath.Token("/", i);
    if(aToken.Length() == 0) break;

    for ( ; anIterator.More(); anIterator.Next() ) {
      aLabel = anIterator.Value();
      if(aLabel.FindAttribute(TDataStd_Name::GetID(), anAttr)) {
	if(anAttr->Get() == aToken) {
	  aToken = aPath.Token("/", i+1); //Check if it was the last part of the path
	  if(aToken.Length() == 0) {  //The searched label is found (no part of the path is left)
	      SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (aLabel, _orb);
	      aSO = SALOMEDS::SObject::_narrow(so_servant->_this()); 
	      return aSO._retn();
	  }

	  anIterator.Initialize(aLabel, Standard_False);
	  break;
	}
      }
    }

    i++;
  }

  return aSO._retn();
}

//============================================================================
/*! Function : GetObjectPath
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_Study_i::GetObjectPath(CORBA::Object_ptr theObject)
{
  TCollection_AsciiString aPath("");
  if(CORBA::is_nil(theObject)) return CORBA::string_dup(aPath.ToCString());

  SALOMEDS::SObject_var anObject = SALOMEDS::SObject::_narrow(theObject);
  if(anObject->_is_nil()) {
    anObject = FindObjectIOR(_orb->object_to_string(theObject));
    if(anObject->_is_nil()) return CORBA::string_dup(aPath.ToCString());
  }

  SALOMEDS::GenericAttribute_var anAttr;
  if(anObject->FindAttribute(anAttr, "AttributeName")) {
    SALOMEDS::AttributeName_var aName = SALOMEDS::AttributeName::_narrow(anAttr);
    if(anAttr->_is_nil()) return CORBA::string_dup(aPath.ToCString());
    TCollection_AsciiString aValue(aName->Value());
    aValue.Prepend("/");
    aValue += aPath;
    aPath = aValue;
    SALOMEDS::SObject_ptr aFather = anObject->GetFather();
    if(!aFather->_is_nil()) {
      TDF_Label aLabel;
      Handle(TDataStd_Name) aNameAttrib;
      TDF_Tool::Label(_doc->GetData(), aFather->GetID(), aLabel);
      if(aLabel.FindAttribute(TDataStd_Name::GetID(), aNameAttrib)) {
	  aValue = GetObjectPath(aFather);
	  aPath = aValue + aPath;
      }
    }
  }

  return  CORBA::string_dup(aPath.ToCString());
}


//============================================================================
/*! Function : SetContext
 *  Purpose  : Sets the current context
 */
//============================================================================
void SALOMEDS_Study_i::SetContext(const char* thePath) 
{
  if(thePath == NULL || strlen(thePath) == 0) throw SALOMEDS::Study::StudyInvalidDirectory();
  TCollection_AsciiString aPath(CORBA::string_dup(thePath)), aContext("");
  bool isInvalid = false, isFound = false;
  SALOMEDS::SObject_var aSO;
  
  if(aPath.Value(1) != '/') { //Relative path 
    aContext = TCollection_AsciiString(GetContext());
    aContext += '/';
    aContext += aPath;
  }
  else
    aContext = aPath;
  
  try {
    aSO = FindObjectByPath(aContext.ToCString());
  }
  catch( ... ) {
    isInvalid = true;
  }

  if(isInvalid || aSO->_is_nil()) throw SALOMEDS::Study::StudyInvalidContext();

  TDF_Label aLabel;
  TDF_Tool::Label(_doc->GetData(), aSO->GetID(), aLabel);
  if(aLabel.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();
  else
    _current = aLabel;  //Set the current context
  
}

//============================================================================
/*! Function : GetContext
 *  Purpose  : Gets the current context
 */
//============================================================================
char* SALOMEDS_Study_i::GetContext() 
{
  if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (_current, _orb);
  SALOMEDS::SObject_var aSO = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return GetObjectPath(aSO._retn());
}

//============================================================================
/*! Function : GetObjectNames
 *  Purpose  : method to get all object names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetObjectNames(const char* theContext) {
  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_Label aLabel;
  if (strlen(theContext) == 0) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  TDF_ChildIterator anIter(aLabel, Standard_False); // iterate all subchildren at all sublevels
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
//      Handle(TDF_Attribute) anAttribute;
//      if (aLabel.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttribute) ||
//  	aLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anAttribute)) {
    Handle(TDataStd_Name) aName;
    if (aLabel.FindAttribute(TDataStd_Name::GetID(), aName)) aResultSeq.Append(aName->Get());
//    }
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetDirectoryNames
 *  Purpose  : method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetDirectoryNames(const char* theContext) {
  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_Label aLabel;
  if (strlen(theContext) == 0) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  TDF_ChildIterator anIter(aLabel, Standard_False); // iterate first-level children at all sublevels
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDS_LocalIDAttribute) anID;
    if (aLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anID)) {
      if (anID->Get() == DIRECTORYID) {
	Handle(TDataStd_Name) aName;
	if (aLabel.FindAttribute(TDataStd_Name::GetID(), aName)) {
	  aResultSeq.Append(aName->Get());
	}
      }
    }
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetFileNames
 *  Purpose  : method to get all file names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetFileNames(const char* theContext) {
  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_Label aLabel;
  if (strlen(theContext) == 0) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  TDF_ChildIterator anIter(aLabel, Standard_False); // iterate all subchildren at all sublevels
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDS_LocalIDAttribute) anID;
    if (aLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anID)) {
      if (anID->Get() == FILELOCALID) {
	Handle(SALOMEDS_PersRefAttribute) aName;
	if(aLabel.FindAttribute(SALOMEDS_PersRefAttribute::GetID(), aName)) {
	  TCollection_ExtendedString aFileName = aName->Get();
	  if(aFileName.Length() > 0)
	    aResultSeq.Append(aFileName.Split(strlen(FILEID)));
	}
      }
    }
//      }
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetComponentNames
 *  Purpose  : method to get all components names
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetComponentNames(const char* theContext) {
  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_ChildIterator anIter(_doc->Main(), Standard_False); // iterate all subchildren at first level
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(TDataStd_Name) aName;
    if (aLabel.FindAttribute(TDataStd_Name::GetID(), aName)) aResultSeq.Append(aName->Get());
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : NewChildIterator
 *  Purpose  : Create a ChildIterator from an SObject
 */
//============================================================================
SALOMEDS::ChildIterator_ptr SALOMEDS_Study_i::NewChildIterator(SALOMEDS::SObject_ptr aSO)
{
  //Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->GetData(), aSO->GetID(), Lab);

  //Create iterator
  SALOMEDS_ChildIterator_i* it_servant = new SALOMEDS_ChildIterator_i(Lab,_orb);
  SALOMEDS::ChildIterator_var it = SALOMEDS::ChildIterator::_narrow(it_servant->_this()); 

  return it;
}


//============================================================================
/*! Function : NewComponentIterator
 *  Purpose  : Create a SComponentIterator
 */
//============================================================================
SALOMEDS::SComponentIterator_ptr SALOMEDS_Study_i::NewComponentIterator()
{
  SALOMEDS_SComponentIterator_i* it_servant = new SALOMEDS_SComponentIterator_i(_doc,_orb);
  SALOMEDS::SComponentIterator_var it = SALOMEDS::SComponentIterator::_narrow(it_servant->_this()); 
  return it;
}


//============================================================================
/*! Function : NewBuilder
 *  Purpose  : Create a StudyBuilder
 */
//============================================================================
SALOMEDS::StudyBuilder_ptr SALOMEDS_Study_i::NewBuilder()
{
  SALOMEDS_StudyBuilder_i* it_servant = new SALOMEDS_StudyBuilder_i(_doc,_orb);
  SALOMEDS::StudyBuilder_var it = SALOMEDS::StudyBuilder::_narrow(it_servant->_this()); 

  if(_autoFill) {
    SALOMEDS_Callback_i* callback = new SALOMEDS_Callback_i(GetUseCaseBuilder(), _orb);
    SALOMEDS::Callback_var cb = SALOMEDS::Callback::_narrow(callback->_this()); 

    it->SetOnAddSObject(cb);
    it->SetOnRemoveSObject(cb);
  }

  return it;

}
 
//============================================================================
/*! Function : Name
 *  Purpose  : get study name
 */
//============================================================================
char* SALOMEDS_Study_i::Name()
{
  return CORBA::string_dup(_name);
}

//============================================================================
/*! Function : Name
 *  Purpose  : set study name
 */
//============================================================================
void SALOMEDS_Study_i::Name(const char* name)
{
  _name = new char[strlen(name) +1];
  strcpy(_name,name);
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : get if study has been saved
 */
//============================================================================
CORBA::Boolean  SALOMEDS_Study_i::IsSaved()
{
  return _isSaved;
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : set if study has been saved
 */
//============================================================================
void SALOMEDS_Study_i::IsSaved(CORBA::Boolean save)
{
  _isSaved = save;
}

//============================================================================
/*! Function : IsModified
 *  Purpose  : Detect if a Study has been modified since it has been saved
 */
//============================================================================
CORBA::Boolean  SALOMEDS_Study_i::IsModified()
{
  // True if is modified and not saved
  if (_doc->IsModified())
    if (!_isSaved) return true;
    else return false;
  else return false;
}

//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
char* SALOMEDS_Study_i::URL()
{
  if(!_URL) {
    _URL = new char[1];
    _URL[0] = (char)0;
  }
  return CORBA::string_dup(_URL);
}

//============================================================================
/*! Function : URL
 *  Purpose  : set URL of the study (persistent reference of the study)
 */
//============================================================================
void SALOMEDS_Study_i::URL(const char* url)
{
  if (_URL) delete [] _URL;
  _URL = new char[strlen(url) +1];
  strcpy(_URL,url);
  SCRUTE(_URL);

  char *aName = _URL;
  char *adr = strtok(aName, "/");
  while (adr)
    {
      aName = adr;
      adr = strtok(NULL, "/");
    }
  strcpy(_URL,url);
  Name(aName);
}


//============================================================================
/*! Function : _FindObject
 *  Purpose  : Find an Object with SALOMEDS::Name = anObjectName
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::_FindObject(SALOMEDS::SObject_ptr SO,
						    const char* anObjectName, 
						    bool& _find)
{
  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  SALOMEDS::SObject_ptr RefSO = SALOMEDS::SObject::_nil();

  CORBA::String_var soid = SO->GetID();
  SALOMEDS::ChildIterator_var it = NewChildIterator(SO);
  for (; it->More();it->Next()){
    if(!_find)
      {
	SALOMEDS::SObject_var CSO = it->Value();
	SALOMEDS::GenericAttribute_var anAttr;
	if (CSO->FindAttribute(anAttr,"AttributeName")) 
	{
	  SALOMEDS::AttributeName_var Name = SALOMEDS::AttributeName::_narrow(anAttr);
          CORBA::String_var Val = Name->Value();
	  if (strcmp(Val, anObjectName) == 0)
	    {
	      RefSO = SALOMEDS::SObject::_narrow(CSO);
	      _find = true;
	    }
	}
	if (!_find) RefSO =  _FindObject(CSO, anObjectName, _find);
      }
  }
  return RefSO;
}

//============================================================================
/*! Function : _FindObject
 *  Purpose  : Find an Object with SALOMEDS::IOR = anObjectIOR
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_Study_i::_FindObjectIOR(SALOMEDS::SObject_ptr SO,
			      const char* anObjectIOR, 
			      bool& _find)
{
  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  SALOMEDS::SObject_ptr RefSO = SALOMEDS::SObject::_nil();

  SALOMEDS::ChildIterator_var it = NewChildIterator(SO);
  for (; it->More();it->Next()){
    if(!_find)
      {
	SALOMEDS::SObject_var CSO = it->Value();
	SALOMEDS::GenericAttribute_var anAttr;
	if (CSO->FindAttribute(anAttr,"AttributeIOR")) 
	{
	  SALOMEDS::AttributeIOR_var IOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
          CORBA::String_var Val = IOR->Value();
	  if (strcmp(Val, anObjectIOR) == 0)
	    {
	      RefSO = SALOMEDS::SObject::_narrow(CSO);
	      _find = true;
	    }
	}
	if (!_find) RefSO =  _FindObjectIOR(CSO, anObjectIOR, _find);
      }
  }
  return RefSO;
}

CORBA::Short SALOMEDS_Study_i::StudyId()
{
  return _StudyId;
}

void SALOMEDS_Study_i::StudyId(CORBA::Short id)
{
  _StudyId = id;
}

void SALOMEDS_Study_i::UpdateIORLabelMap(const char* anIOR,const char* anEntry) {
  TDF_Label aLabel;
  CORBA::String_var anEn = CORBA::string_dup(anEntry);
  CORBA::String_var IOR = CORBA::string_dup(anIOR);
  TDF_Tool::Label(_doc->GetData(),anEn,aLabel, Standard_True);
  if (myIORLabels.IsBound(TCollection_ExtendedString(IOR))) myIORLabels.UnBind(TCollection_ExtendedString(IOR));
  myIORLabels.Bind(TCollection_ExtendedString(IOR), aLabel);
}

SALOMEDS::Study_ptr SALOMEDS_Study_i::GetStudy(const TDF_Label theLabel, CORBA::ORB_ptr orb) {
  Handle(SALOMEDS_IORAttribute) Att;
  if (theLabel.Root().FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){
    char* IOR = CORBA::string_dup(TCollection_AsciiString(Att->Get()).ToCString());
    CORBA::Object_var obj = orb->string_to_object(IOR);
    SALOMEDS::Study_ptr aStudy = SALOMEDS::Study::_narrow(obj) ;
    ASSERT(!CORBA::is_nil(aStudy));
    return SALOMEDS::Study::_duplicate(aStudy);
  } else {
    MESSAGE("GetStudy: Problem to get study");
  }
  return SALOMEDS::Study::_nil();
}

void SALOMEDS_Study_i::IORUpdated(const Handle(SALOMEDS_IORAttribute) theAttribute, CORBA::ORB_ptr orb) {
  TCollection_AsciiString aString;
  TDF_Tool::Entry(theAttribute->Label(), aString);
  GetStudy(theAttribute->Label(), orb)->UpdateIORLabelMap(TCollection_AsciiString(theAttribute->Get()).ToCString(),
							  aString.ToCString());
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_Study_i::FindDependances(SALOMEDS::SObject_ptr anObject) {
  SALOMEDS::GenericAttribute_ptr aTarget;
  if (anObject->FindAttribute(aTarget,"AttributeTarget")) {
    return SALOMEDS::AttributeTarget::_narrow(aTarget)->Get();
  }
  SALOMEDS::Study::ListOfSObject* aList = new SALOMEDS::Study::ListOfSObject;
  aList->length(0);
  return aList;
}


SALOMEDS::AttributeStudyProperties_ptr SALOMEDS_Study_i::GetProperties() {
  SALOMEDS::GenericAttribute_ptr anAttr =  NewBuilder()->FindOrCreateAttribute(FindObjectID("0:1"),
									       "AttributeStudyProperties");
  return SALOMEDS::AttributeStudyProperties::_narrow(anAttr);
}

char* SALOMEDS_Study_i::GetLastModificationDate() {
  SALOMEDS::AttributeStudyProperties_var aProp = GetProperties();
  SALOMEDS::StringSeq_var aNames;
  SALOMEDS::LongSeq_var aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModificationsList(aNames , aMinutes ,aHours, aDays, aMonths, aYears, true);
  int aLastIndex = aNames->length() - 1;
  char aResult[20];
  sprintf(aResult, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", (int)(aDays[aLastIndex]),(int)(aMonths[aLastIndex]),
	  (int)(aYears[aLastIndex]), (int)(aHours[aLastIndex]), (int)(aMinutes[aLastIndex]));
  CORBA::String_var aResStr = CORBA::string_dup(aResult);
  return aResStr._retn();
}

SALOMEDS::ListOfDates* SALOMEDS_Study_i::GetModificationsDate() {
  SALOMEDS::AttributeStudyProperties_var aProp = GetProperties();
  SALOMEDS::StringSeq_var aNames;
  SALOMEDS::LongSeq_var aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModificationsList(aNames , aMinutes ,aHours, aDays, aMonths, aYears, false);

  int anIndex, aLength = aNames->length();
  SALOMEDS::ListOfDates_var aDates = new SALOMEDS::ListOfDates;
  aDates->length(aLength);

  for(anIndex = 0; anIndex < aLength; anIndex++) {
    char aDate[20];
    sprintf(aDate, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", (int)(aDays[anIndex]), (int)(aMonths[anIndex]),
	    (int)(aYears[anIndex]), (int)(aHours[anIndex]), (int)(aMinutes[anIndex]));
    aDates[anIndex] = CORBA::string_dup(aDate);
  }
  return aDates._retn();
}



//============================================================================
/*! Function : GetUseCaseBuilder
 *  Purpose  : Returns a UseCase builder
 */
//============================================================================
SALOMEDS::UseCaseBuilder_ptr SALOMEDS_Study_i::GetUseCaseBuilder() 
{
  SALOMEDS_UseCaseBuilder_i* _caseBuilder = new SALOMEDS_UseCaseBuilder_i(_doc, _orb);
  SALOMEDS::UseCaseBuilder_var aBuilder = SALOMEDS::UseCaseBuilder::_narrow(_caseBuilder->_this());
  return aBuilder._retn();
}


//============================================================================
/*! Function : Close
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::Close()
{
  SALOMEDS::SComponentIterator_var itcomponent = NewComponentIterator();

  for (; itcomponent->More(); itcomponent->Next()) {
    SALOMEDS::SComponent_var sco = itcomponent->Value();
	  
    MESSAGE ( "Look for an engine for data type :"<< sco->ComponentDataType());
    // if there is an associated Engine call its method for closing
    CORBA::String_var IOREngine;
    if (sco->ComponentIOR(IOREngine)) {
      // we have found the associated engine to write the data 
      MESSAGE ( "We have found an engine for data type :"<< sco->ComponentDataType());
      CORBA::Object_var obj = _orb->string_to_object(IOREngine);
      if (!CORBA::is_nil(obj)) {
	SALOMEDS::Driver_var anEngine = SALOMEDS::Driver::_narrow(obj) ;
	
	if (!anEngine->_is_nil())  
	  anEngine->Close(sco);
      }
    }
  }

  Handle(TDocStd_Application) anApp = Handle(TDocStd_Application)::DownCast(_doc->Application());
//    Handle(TDocStd_Owner) anOwner;
//    if (_doc->Main().Root().FindAttribute(TDocStd_Owner::GetID(), anOwner)) {
//      Handle(TDocStd_Document) anEmptyDoc;
//      anOwner->SetDocument(anEmptyDoc);
//    }
  if(!anApp.IsNull()) anApp->Close(_doc);
  _doc.Nullify();
}

//============================================================================
/*! Function : AddPostponed
 *  Purpose  : 
 */
 //============================================================================
void SALOMEDS_Study_i::AddPostponed(const char* theIOR) {
  if (!NewBuilder()->HasOpenCommand()) return;
  try {
    CORBA::Object_var obj = _orb->string_to_object(theIOR);
    if (!CORBA::is_nil(obj)) {
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj) ;
      if (!CORBA::is_nil(aGeneric)) {
	TCollection_AsciiString anIOR((char*)theIOR);
	anIOR.Prepend("d");
	myPostponedIORs.Append(anIOR); // add prefix: deleted
	myNbPostponed.SetValue(myNbPostponed.Length(), myNbPostponed.Last() + 1);
      }
    }
  } catch(...) {}
}

void SALOMEDS_Study_i::AddCreatedPostponed(const char* theIOR) {
  if (!NewBuilder()->HasOpenCommand()) return;
  try {
    CORBA::Object_var obj = _orb->string_to_object(theIOR);
    if (!CORBA::is_nil(obj)) {
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj) ;
      if (!CORBA::is_nil(aGeneric)) {
	TCollection_AsciiString anIOR((char*)theIOR);
	anIOR.Prepend("c");
	myPostponedIORs.Append(anIOR); // add prefix: created
	myNbPostponed.SetValue(myNbPostponed.Length(), myNbPostponed.Last() + 1);
      }
    }
  } catch(...) {}
}

//============================================================================
/*! Function : RemovePostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::RemovePostponed(const CORBA::Long theUndoLimit) {
  int anIndex;
  int anOld;

  int aUndoLimit = theUndoLimit;
  if (theUndoLimit < 0) aUndoLimit = 0;

  if (myNbUndos > 0) { // remove undone
    anOld = 0;
    for(anIndex = 1; anIndex < myNbPostponed.Length() - myNbUndos; anIndex++)
      anOld += myNbPostponed(anIndex);
    int aNew = myPostponedIORs.Length() - myNbPostponed.Last();

    for(anIndex = anOld + 1; anIndex <= aNew; anIndex++) {
      TCollection_AsciiString anIOR = myPostponedIORs(anIndex);
      if (anIOR.Value(1) == 'c') {
	CORBA::Object_var obj = _orb->string_to_object(anIOR.Split(1).ToCString());
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
      }
    }
    if (anOld < aNew) myPostponedIORs.Remove(anOld + 1, aNew);
    if (myNbPostponed.Length() > 0) myNbPostponed.Remove(myNbPostponed.Length() - myNbUndos, myNbPostponed.Length() - 1);

    myNbUndos = 0;
  }

  if (myNbPostponed.Length() > aUndoLimit) { // remove objects, that can not be undone
    anOld = 0;
    for(anIndex = myNbPostponed.Length() - aUndoLimit; anIndex >= 1; anIndex--)
      anOld += myNbPostponed(anIndex);
    for(anIndex = 1; anIndex <= anOld; anIndex++) {
      TCollection_AsciiString anIOR = myPostponedIORs(anIndex);
      if (anIOR.Value(1) == 'd') {
	CORBA::Object_var obj = _orb->string_to_object(anIOR.Split(1).ToCString());
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
      }
    }
    if (anOld > 0) myPostponedIORs.Remove(1, anOld);
    myNbPostponed.Remove(1, myNbPostponed.Length() - aUndoLimit);
  }

  if (theUndoLimit == -1) { // remove all IORs from the study on the study close
    TDF_ChildIDIterator anIter(_doc->GetData()->Root(), SALOMEDS_IORAttribute::GetID(), Standard_True);
    for(; anIter.More(); anIter.Next()) {
      Handle(SALOMEDS_IORAttribute) anAttr = Handle(SALOMEDS_IORAttribute)::DownCast(anIter.Value());
      CORBA::String_var anIOR = CORBA::string_dup(TCollection_AsciiString(anAttr->Get()).ToCString());
      try {
	CORBA::Object_var obj = _orb->string_to_object(anIOR);
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
      } catch (...) {}
    }
  } else myNbPostponed.Append(0);
}

//============================================================================
/*! Function : UndoPostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::UndoPostponed(const CORBA::Long theWay) {
  myNbUndos += theWay;
  // remove current postponed
  if (myNbPostponed.Last() > 0)
    myPostponedIORs.Remove(myPostponedIORs.Length() - myNbPostponed.Last() + 1, myPostponedIORs.Length());
  myNbPostponed(myNbPostponed.Length()) = 0;
}
