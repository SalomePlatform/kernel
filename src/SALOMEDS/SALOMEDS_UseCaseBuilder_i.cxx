//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_UseCaseBuilder_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME

#include "SALOMEDS_UseCaseBuilder_i.hxx"
#include "SALOMEDS_AttributeComment_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_UseCaseIterator_i.hxx"
#include "utilities.h"
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_Reference.hxx>
#include <TDF_AttributeList.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_ChildIterator.hxx>
using namespace std;

#define USE_CASE_LABEL_TAG           2
#define USE_CASE_GUID                "AA43BB12-D9CD-11d6-945D-0050DA506788"


//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_UseCaseBuilder_i::SALOMEDS_UseCaseBuilder_i(const Handle(TDocStd_Document)& theDocument,
						     CORBA::ORB_ptr orb)
:_doc(theDocument)
{
  _orb = CORBA::ORB::_duplicate(orb);
  if(_doc.IsNull()) return;

  TDF_Label aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG); //Iterate all use cases
  if(!aLabel.FindAttribute(Standard_GUID(USE_CASE_GUID), _root)) {
    _root = TDataStd_TreeNode::Set(aLabel, Standard_GUID(USE_CASE_GUID));
  }

  Handle(TDF_Reference) aRef;
  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) {
    aRef = TDF_Reference::Set(_root->Label(), _root->Label());  
  }  

  Handle(TDataStd_Name) aNameAttr;
  if(!aLabel.FindAttribute(TDataStd_Name::GetID(), aNameAttr)) { 
    aNameAttr = TDataStd_Name::Set(aLabel, "Use cases"); 
  }

}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_UseCaseBuilder_i::~SALOMEDS_UseCaseBuilder_i()
{
}


//============================================================================
/*! Function : Append
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::Append(SALOMEDS::SObject_ptr theObject)
{
  if(_root.IsNull() || theObject->_is_nil()) return 0;

  TDF_Label aLabel;

  TDF_Tool::Label(_root->Label().Data(), theObject->GetID(), aLabel);
  if(aLabel.IsNull()) return 0;

  Handle(TDataStd_TreeNode) aNode, aCurrentNode;
  aNode = TDataStd_TreeNode::Set(aLabel, _root->ID());
  aNode->Remove();

  Handle(TDF_Reference) aRef;
  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) {
    aRef = TDF_Reference::Set(_root->Label(), _root->Label());  
  }  
  TDF_Label aCurrent = aRef->Get();

  if(aCurrent.IsNull() || !aCurrent.FindAttribute(_root->ID(), aCurrentNode)) 
    aCurrentNode = _root;

  return aCurrentNode->Append(aNode);
}

 //============================================================================
/*! Function : Remove
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::Remove(SALOMEDS::SObject_ptr theObject)
{
  if(_root.IsNull() || theObject->_is_nil()) return 0;

  TDF_Label aLabel;
  TDF_Tool::Label(_root->Label().Data(), theObject->GetID(), aLabel);
  if(aLabel.IsNull()) return 0;

  Handle(TDataStd_TreeNode) aNode;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return 0;

  aNode->Remove();

  TDF_AttributeList aList;
  aList.Append(aNode);

  Handle(TDF_Reference) aRef;
  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) {
    aRef = TDF_Reference::Set(_root->Label(), _root->Label());  
  }  
  TDF_Label aCurrent = aRef->Get();

  TDataStd_ChildNodeIterator aChildItr(aNode, Standard_True);
  for(; aChildItr.More(); aChildItr.Next()) 
    aList.Append(aChildItr.Value());

  TDF_ListIteratorOfAttributeList anIterator(aList);
  for(; anIterator.More(); anIterator.Next()) {
    if(anIterator.Value()->Label() ==  aCurrent) { //The current node is removed
      aRef->Set(_root->Label()); //Reset the current node to the root
    }
    anIterator.Value()->Label().ForgetAttribute(_root->ID());
  }

  return 1;
}


//============================================================================
/*! Function : AppendTo
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::AppendTo(SALOMEDS::SObject_ptr theFather, 
						   SALOMEDS::SObject_ptr theObject)
{
  if(_root.IsNull() || theFather->_is_nil() || theObject->_is_nil()) return 0;

  TDF_Label aFatherLabel, aLabel;
  Handle(TDataStd_TreeNode) aFather, aNode;
  
  TDF_Tool::Label(_root->Label().Data(), theFather->GetID(), aFatherLabel);
  if(aFatherLabel.IsNull()) return 0;
  if(!aFatherLabel.FindAttribute(_root->ID(), aFather)) return 0;

  TDF_Tool::Label(_root->Label().Data(), theObject->GetID(), aLabel);
  if(aLabel.IsNull()) return 0;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) {
    aNode = TDataStd_TreeNode::Set(aLabel, _root->ID());
  }
  else
    aNode->Remove();

  return aFather->Append(aNode);
}

//============================================================================
/*! Function : InsertBefore
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::InsertBefore(SALOMEDS::SObject_ptr theFirst, 
						       SALOMEDS::SObject_ptr theNext)
{
  if(_root.IsNull() || theFirst->_is_nil() || theNext->_is_nil()) return 0;

  TDF_Label aFirstLabel, aLabel;
  Handle(TDataStd_TreeNode) aFirstNode, aNode;
  
  TDF_Tool::Label(_root->Label().Data(), theFirst->GetID(), aFirstLabel);
  if(aFirstLabel.IsNull()) return 0;
  if(aFirstLabel.FindAttribute(_root->ID(), aFirstNode)) {
    aFirstNode->Remove();
    aFirstLabel.ForgetAttribute(aFirstNode->ID());
  }

  aFirstNode = TDataStd_TreeNode::Set(aFirstLabel, _root->ID());
  

  TDF_Tool::Label(_root->Label().Data(), theNext->GetID(), aLabel);
  if(aLabel.IsNull()) return 0;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return 0;

  return aNode->InsertBefore(aFirstNode);
}


//============================================================================
/*! Function : SetCurrentObject
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetCurrentObject(SALOMEDS::SObject_ptr theObject)
{
  if(_root.IsNull() || theObject->_is_nil()) return 0;

  TDF_Label aLabel;
  Handle(TDataStd_TreeNode) aNode;
  TDF_Tool::Label(_root->Label().Data(), theObject->GetID(), aLabel);
  if(aLabel.IsNull()) return 0;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return 0;


  Handle(TDF_Reference) aRef;
  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) {
    aRef = TDF_Reference::Set(_root->Label(), aNode->Label());  
  }
  
  aRef->Set(aNode->Label());

  return 1;
}

//============================================================================
/*! Function : SetRootCurrent
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetRootCurrent()
{
  if(_root.IsNull()) return 0;
   
  Handle(TDF_Reference) aRef;
  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) 
    aRef = TDF_Reference::Set(_root->Label(), _root->Label());  

  aRef->Set(_root->Label());
  return 1;
}

//============================================================================
/*! Function : HasChildren
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::HasChildren(SALOMEDS::SObject_ptr theObject)
{
  if(_root.IsNull()) return 0;

  TDF_Label aLabel;
  if (theObject->_is_nil()) aLabel = _root->Label();
  else TDF_Tool::Label(_root->Label().Data(), theObject->GetID(), aLabel);
  if(aLabel.IsNull()) return 0;

  Handle(TDataStd_TreeNode) aNode;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return 0;

  return !(aNode->First().IsNull());
}

//============================================================================
/*! Function : SetName
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetName(const char* theName) {
  if(_root.IsNull()) return 0;

  Handle(TDataStd_Name) aNameAttrib;
  TCollection_ExtendedString aName(strdup(theName));

  if (!_root->FindAttribute(TDataStd_Name::GetID(), aNameAttrib))
    aNameAttrib = TDataStd_Name::Set(_root->Label(), aName);
  else    
    aNameAttrib->Set(aName);
    
  return 1;
}


//============================================================================
/*! Function : GetCurrentObject
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_UseCaseBuilder_i::GetCurrentObject()
{
  if(_root.IsNull()) return NULL;

  Handle(TDF_Reference) aRef;
  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) {
    aRef = TDF_Reference::Set(_root->Label(), _root->Label());  
  }  
  TDF_Label aCurrent = aRef->Get();  
  if(aCurrent.IsNull()) return NULL;

  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (aCurrent, _orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return so._retn();
}

//============================================================================
/*! Function : GetName
 *  Purpose  :
 */
//============================================================================
char* SALOMEDS_UseCaseBuilder_i::GetName() {
  CORBA::String_var aString;
  if(_root.IsNull()) return aString._retn();

  Handle(TDataStd_Name) aName;
  if (!_root->FindAttribute(TDataStd_Name::GetID(), aName)) return aString._retn();
  aString = strdup(TCollection_AsciiString(aName->Get()).ToCString());
  return aString._retn();
}

//============================================================================ 
/*! Function :  IsUseCase
 *  Purpose  :  
 */ 
//============================================================================ 
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::IsUseCase(SALOMEDS::SObject_ptr theObject)
{
  if(theObject->_is_nil()) return false;
  TDF_Label aFather, aLabel; 
  TDF_Tool::Label(_doc->GetData(), theObject->GetID(), aLabel);
  aFather = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG);
  if(aLabel.Father() == aFather) return true;
  return false;
}

//============================================================================ 
/*! Function : NewUseCase 
 *  Purpose  :  
 */ 
//============================================================================ 
SALOMEDS::SObject_ptr SALOMEDS_UseCaseBuilder_i::AddUseCase(const char* theName)
{
  Standard_GUID aBasicGUID(USE_CASE_GUID);

  //Create a use cases structure if it not exists 

  Handle(TDataStd_TreeNode) aFatherNode, aNode;
  Handle(TDataStd_Integer) anInteger;
  Handle(TDF_Reference) aRef;

  TDF_Label aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG);

  if(!_root->FindAttribute(TDF_Reference::GetID(), aRef)) {
    aRef = TDF_Reference::Set(aLabel, aLabel);
  }
 
  if(!aRef->Get().FindAttribute(aBasicGUID, aFatherNode)) {
    aFatherNode = TDataStd_TreeNode::Set(aRef->Get());
  }

  if(!_root->FindAttribute(TDataStd_Integer::GetID(), anInteger)) {
    anInteger = TDataStd_Integer::Set(aLabel, 0);
  }

  //Create a new use case
  anInteger->Set(anInteger->Get()+1);
  TDF_Label aChild = aLabel.FindChild(anInteger->Get());
  aNode = TDataStd_TreeNode::Set(aChild, aBasicGUID);
  aFatherNode->Append(aNode);
  TDataStd_Name::Set(aChild, TCollection_ExtendedString(strdup(theName)));

  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (aChild, _orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 

  return so._retn();
}

//============================================================================
/*! Function : GetUseCaseIterator
 *  Purpose  : Creates a new UseCase iterator, if anObject is null all use cases are iterated 
 */
//============================================================================
SALOMEDS::UseCaseIterator_ptr SALOMEDS_UseCaseBuilder_i::GetUseCaseIterator(SALOMEDS::SObject_ptr anObject) 
{
  TDF_Label aLabel;

  if(!anObject->_is_nil()) {
    TDF_Tool::Label(_doc->GetData(), anObject->GetID(), aLabel); //Iterate only sub tree in the use case
  }
  else {
    aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG); //Iterate all use cases
  }

  SALOMEDS_UseCaseIterator_i* aServant = new SALOMEDS_UseCaseIterator_i(aLabel, USE_CASE_GUID, Standard_False, _orb);
  SALOMEDS::UseCaseIterator_var anIterator = SALOMEDS::UseCaseIterator::_narrow(aServant->_this());

  return anIterator._retn(); 
}
