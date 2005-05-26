//  File   : SALOMEDSImpl_UseCaseBuilder.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_Attributes.hxx"

#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_AttributeList.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_ChildIterator.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_UseCaseBuilder, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_UseCaseBuilder, MMgt_TShared )


#define USE_CASE_LABEL_TAG           2
#define USE_CASE_GUID                "AA43BB12-D9CD-11d6-945D-0050DA506788"


//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseBuilder::SALOMEDSImpl_UseCaseBuilder(const Handle(TDocStd_Document)& theDocument)
:_doc(theDocument)
{
  if(_doc.IsNull()) return;
  
  TDF_Label aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG); //Iterate all use cases
  if(!aLabel.FindAttribute(Standard_GUID(USE_CASE_GUID), _root)) {
    _root = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, Standard_GUID(USE_CASE_GUID));
  }
 
  Handle(SALOMEDSImpl_AttributeReference) aRef;
  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  

  Handle(SALOMEDSImpl_AttributeName) aNameAttr;
  if(!aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aNameAttr)) { 
    aNameAttr = SALOMEDSImpl_AttributeName::Set(aLabel, "Use cases"); 
  }  
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseBuilder::~SALOMEDSImpl_UseCaseBuilder()
{
}


//============================================================================
/*! Function : Append
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::Append(const Handle(SALOMEDSImpl_SObject)& theObject)
{
  if(_root.IsNull() || theObject.IsNull()) return false;

  TDF_Label aLabel = theObject->GetLabel();
  if(aLabel.IsNull()) return false;

  Handle(SALOMEDSImpl_AttributeTreeNode) aNode, aCurrentNode;
  aNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, _root->ID());
  aNode->Remove();

  Handle(SALOMEDSImpl_AttributeReference) aRef;
  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  

  TDF_Label aCurrent = aRef->Get();
  if(aCurrent.IsNull() || !aCurrent.FindAttribute(_root->ID(), aCurrentNode)) 
    aCurrentNode = _root;

  aCurrentNode->Append(aNode);

  return true;
}

 //============================================================================
/*! Function : Remove
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::Remove(const Handle(SALOMEDSImpl_SObject)& theObject)
{
  if(_root.IsNull() || theObject.IsNull()) return false;

  TDF_Label aLabel = theObject->GetLabel();   
  if(aLabel.IsNull()) return false;

  Handle(SALOMEDSImpl_AttributeTreeNode) aNode;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return false;

  aNode->Remove();

  TDF_AttributeList aList;
  aList.Append(aNode);

  Handle(SALOMEDSImpl_AttributeReference) aRef;
  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  
  TDF_Label aCurrent = aRef->Get();

  SALOMEDSImpl_ChildNodeIterator aChildItr(aNode, Standard_True);
  for(; aChildItr.More(); aChildItr.Next()) 
    aList.Append(aChildItr.Value());

  TDF_ListIteratorOfAttributeList anIterator(aList);
  for(; anIterator.More(); anIterator.Next()) {
    if(anIterator.Value()->Label() ==  aCurrent) { //The current node is removed
      aRef->Set(_root->Label()); //Reset the current node to the root
    }
    anIterator.Value()->Label().ForgetAttribute(_root->ID());
  }

  return true;
}


//============================================================================
/*! Function : AppendTo
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::AppendTo(const Handle(SALOMEDSImpl_SObject)& theFather, 
					   const Handle(SALOMEDSImpl_SObject)& theObject)
{
  if(_root.IsNull() || theFather.IsNull() || theObject.IsNull()) return false;

  TDF_Label aFatherLabel = theFather->GetLabel(), aLabel = theObject->GetLabel();
  Handle(SALOMEDSImpl_AttributeTreeNode) aFather, aNode;
  
  if(aFatherLabel.IsNull()) return false;
  if(!aFatherLabel.FindAttribute(_root->ID(), aFather)) return false;

  if(aLabel.IsNull()) return false;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) {
    aNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, _root->ID());
  }

  aNode->Remove();

  return aFather->Append(aNode);
}

//============================================================================
/*! Function : InsertBefore
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::InsertBefore(const Handle(SALOMEDSImpl_SObject)& theFirst, 
					       const Handle(SALOMEDSImpl_SObject)& theNext)
{
  if(_root.IsNull() || theFirst.IsNull() || theNext.IsNull()) return false;

  TDF_Label aFirstLabel = theFirst->GetLabel(), aLabel= theNext->GetLabel();
  Handle(SALOMEDSImpl_AttributeTreeNode) aFirstNode, aNode;
  
  if(aFirstLabel.IsNull()) return false;
  if(aFirstLabel.FindAttribute(_root->ID(), aFirstNode)) {
    aFirstNode->Remove();
    aFirstLabel.ForgetAttribute(aFirstNode->ID());
  }

  aFirstNode = SALOMEDSImpl_AttributeTreeNode::Set(aFirstLabel, _root->ID());
  
  if(aLabel.IsNull()) return false;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return false;

  aFirstNode->Remove();

  return aNode->InsertBefore(aFirstNode);
}


//============================================================================
/*! Function : SetCurrentObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::SetCurrentObject(const Handle(SALOMEDSImpl_SObject)& theObject)
{
  if(_root.IsNull() || theObject.IsNull()) return false;

  TDF_Label aLabel = theObject->GetLabel();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode;
  if(aLabel.IsNull()) return false;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return false;


  Handle(SALOMEDSImpl_AttributeReference) aRef;
  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), aNode->Label());  
  }
  
  aRef->Set(aNode->Label());

  return true;
}

//============================================================================
/*! Function : SetRootCurrent
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::SetRootCurrent()
{
  if(_root.IsNull()) return false;
   
  Handle(SALOMEDSImpl_AttributeReference) aRef;
  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) 
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  

  aRef->Set(_root->Label());
  return true;
}

//============================================================================
/*! Function : HasChildren
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::HasChildren(const Handle(SALOMEDSImpl_SObject)& theObject)
{
  if(_root.IsNull()) return false;

  TDF_Label aLabel;
  if (theObject.IsNull()) aLabel = _root->Label();
  else 
    aLabel = theObject->GetLabel(); 
  if(aLabel.IsNull()) return false;

  Handle(SALOMEDSImpl_AttributeTreeNode) aNode;
  if(!aLabel.FindAttribute(_root->ID(), aNode)) return false;

  return !(aNode->GetFirst().IsNull());
}

//============================================================================
/*! Function : SetName
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::SetName(const TCollection_AsciiString& theName) {
  if(_root.IsNull()) return false;

  Handle(SALOMEDSImpl_AttributeName) aNameAttrib;

  if (!_root->FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aNameAttrib))
    aNameAttrib = SALOMEDSImpl_AttributeName::Set(_root->Label(), theName);
  else    
    aNameAttrib->SetValue(theName);
    
  return true;
}


//============================================================================
/*! Function : GetCurrentObject
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_UseCaseBuilder::GetCurrentObject()
{
  if(_root.IsNull()) return NULL;

  Handle(SALOMEDSImpl_AttributeReference) aRef;
  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  
  TDF_Label aCurrent = aRef->Get();  
  if(aCurrent.IsNull()) return NULL;

  return SALOMEDSImpl_Study::SObject(aCurrent);
}

//============================================================================
/*! Function : GetName
 *  Purpose  :
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_UseCaseBuilder::GetName() 
{
  TCollection_AsciiString aString;
  if(_root.IsNull()) return aString;

  Handle(SALOMEDSImpl_AttributeName) aName;
  if (!_root->FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aName)) return aString;
  aString = TCollection_AsciiString(aName->Value());
  return aString;
}

//============================================================================ 
/*! Function :  IsUseCase
 *  Purpose  :  
 */ 
//============================================================================ 
bool SALOMEDSImpl_UseCaseBuilder::IsUseCase(const Handle(SALOMEDSImpl_SObject)& theObject)
{
  if(theObject.IsNull()) return false;
  TDF_Label aFather, aLabel = theObject->GetLabel(); 
  aFather = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG);
  if(aLabel.Father() == aFather) return true;
  return false;
}

//============================================================================ 
/*! Function : NewUseCase 
 *  Purpose  :  
 */ 
//============================================================================ 
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_UseCaseBuilder::AddUseCase(const TCollection_AsciiString& theName)
{
  Standard_GUID aBasicGUID(USE_CASE_GUID);

  //Create a use cases structure if it not exists 

  Handle(SALOMEDSImpl_AttributeTreeNode) aFatherNode, aNode;
  Handle(SALOMEDSImpl_AttributeInteger) anInteger;
  Handle(SALOMEDSImpl_AttributeReference) aRef;

  TDF_Label aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG);

  if(!_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aRef)) {
    aRef = SALOMEDSImpl_AttributeReference::Set(aLabel, aLabel);
  }
 
  if(!aRef->Get().FindAttribute(aBasicGUID, aFatherNode)) {
    aFatherNode = SALOMEDSImpl_AttributeTreeNode::Set(aRef->Get(), aBasicGUID);
  }

  if(!_root->FindAttribute(SALOMEDSImpl_AttributeInteger::GetID(), anInteger)) {
    anInteger = SALOMEDSImpl_AttributeInteger::Set(aLabel, 0);
  }

  //Create a new use case
  anInteger->SetValue(anInteger->Value()+1);
  TDF_Label aChild = aLabel.FindChild(anInteger->Value());
  aNode = SALOMEDSImpl_AttributeTreeNode::Set(aChild, aBasicGUID);
  aNode->Remove();
  aFatherNode->Append(aNode);
  SALOMEDSImpl_AttributeName::Set(aChild, theName);

  return SALOMEDSImpl_Study::SObject(aChild);
}

//============================================================================
/*! Function : GetUseCaseIterator
 *  Purpose  : Creates a new UseCase iterator, if anObject is null all use cases are iterated 
 */
//============================================================================
Handle(SALOMEDSImpl_UseCaseIterator) 
SALOMEDSImpl_UseCaseBuilder::GetUseCaseIterator(const Handle(SALOMEDSImpl_SObject)& theObject) 
{
  TDF_Label aLabel;

  if(!theObject.IsNull()) {
    aLabel = theObject->GetLabel(); //Iterate only sub tree in the use case
  }
  else {
    aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG); //Iterate all use cases
  }

  return new SALOMEDSImpl_UseCaseIterator(aLabel, USE_CASE_GUID, false); 
}


Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_UseCaseBuilder::GetSObject(const TCollection_AsciiString& theEntry)
{
   TDF_Label aLabel;    
   TDF_Tool::Label(_doc->GetData(), theEntry, aLabel);
   return SALOMEDSImpl_Study::SObject(aLabel);    
}
