// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_UseCaseBuilder.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_Attributes.hxx"

#include "DF_ChildIterator.hxx"

using namespace std;

#define USE_CASE_LABEL_TAG           2
#define USE_CASE_GUID                "AA43BB12-D9CD-11d6-945D-0050DA506788"


//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseBuilder::SALOMEDSImpl_UseCaseBuilder(DF_Document* theDocument)
:_doc(theDocument)
{
  if(!_doc) return;
  
  DF_Label aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG); //Iterate all use cases
  if(!(_root = (SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(std::string(USE_CASE_GUID)))) {
    _root = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, std::string(USE_CASE_GUID));
  }
 
  SALOMEDSImpl_AttributeReference* aRef = NULL;
  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  

  if(!aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID())) { 
    SALOMEDSImpl_AttributeName::Set(aLabel, "Use cases"); 
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
bool SALOMEDSImpl_UseCaseBuilder::Append(const SALOMEDSImpl_SObject& theObject)
{
  if(!_root || !theObject) return false;

  DF_Label aLabel = theObject.GetLabel();
  if(aLabel.IsNull()) return false;

  SALOMEDSImpl_AttributeTreeNode* aNode = NULL;
  SALOMEDSImpl_AttributeTreeNode* aCurrentNode = NULL;
  aNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, _root->ID());
  aNode->Remove();

  SALOMEDSImpl_AttributeReference* aRef;
  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  

  DF_Label aCurrent = aRef->Get();
  if(aCurrent.IsNull() || !(aCurrentNode=(SALOMEDSImpl_AttributeTreeNode*)aCurrent.FindAttribute(_root->ID()))) 
    aCurrentNode = _root;

  aCurrentNode->Append(aNode);

  return true;
}

 //============================================================================
/*! Function : Remove
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::Remove(const SALOMEDSImpl_SObject& theObject)
{
  if(!_root || !theObject) return false;

  DF_Label aLabel = theObject.GetLabel();   
  if(aLabel.IsNull()) return false;

  SALOMEDSImpl_AttributeTreeNode* aNode = NULL;
  if(!(aNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(_root->ID()))) return false;

  aNode->Remove();

  vector<DF_Attribute*> aList;
  aList.push_back(aNode);

  SALOMEDSImpl_AttributeReference* aRef = NULL;
  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  

  DF_Label aCurrent = aRef->Get();

  SALOMEDSImpl_ChildNodeIterator aChildItr(aNode, true);
  for(; aChildItr.More(); aChildItr.Next()) 
    aList.push_back(aChildItr.Value());

  for(int i = 0, len = aList.size(); i<len; i++) {
    if(aList[i]->Label() ==  aCurrent) { //The current node is removed
      aRef->Set(_root->Label()); //Reset the current node to the root
    }
    aList[i]->Label().ForgetAttribute(_root->ID());
  }

  return true;
}


//============================================================================
/*! Function : AppendTo
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::AppendTo(const SALOMEDSImpl_SObject& theFather, 
					   const SALOMEDSImpl_SObject& theObject)
{
  if(!_root || !theFather || !theObject) return false;

  DF_Label aFatherLabel = theFather.GetLabel(), aLabel = theObject.GetLabel();
  if(aFatherLabel == aLabel) return false;

  SALOMEDSImpl_AttributeTreeNode *aFather = false, *aNode = false;
  
  if(aFatherLabel.IsNull()) return false;
  if(!(aFather=(SALOMEDSImpl_AttributeTreeNode*)aFatherLabel.FindAttribute(_root->ID()))) return false;

  if(aLabel.IsNull()) return false;
  if(!(aNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(_root->ID()))) {
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
bool SALOMEDSImpl_UseCaseBuilder::InsertBefore(const SALOMEDSImpl_SObject& theFirst, 
					       const SALOMEDSImpl_SObject& theNext)
{
  if(!_root || !theFirst || !theNext) return false;

  DF_Label aFirstLabel = theFirst.GetLabel(), aLabel= theNext.GetLabel();
  if(aFirstLabel == aLabel) return false;

  SALOMEDSImpl_AttributeTreeNode *aFirstNode = NULL, *aNode = NULL;
  
  if(aFirstLabel.IsNull()) return false;
  if((aFirstNode=(SALOMEDSImpl_AttributeTreeNode*)aFirstLabel.FindAttribute(_root->ID()))) {
    aFirstNode->Remove();
    aFirstLabel.ForgetAttribute(aFirstNode->ID());
  }

  aFirstNode = SALOMEDSImpl_AttributeTreeNode::Set(aFirstLabel, _root->ID());
  
  if(aLabel.IsNull()) return false;
  if(!(aNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(_root->ID()))) return false;    

  aFirstNode->Remove();

  return aNode->InsertBefore(aFirstNode);
}


//============================================================================
/*! Function : SetCurrentObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::SetCurrentObject(const SALOMEDSImpl_SObject& theObject)
{
  if(!_root || !theObject) return false;

  DF_Label aLabel = theObject.GetLabel();
  SALOMEDSImpl_AttributeTreeNode* aNode = NULL;
  if(aLabel.IsNull()) return false;
  if(!(aNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(_root->ID()))) return false;

  SALOMEDSImpl_AttributeReference* aRef = NULL;
  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
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
  if(!_root) return false;
   
  SALOMEDSImpl_AttributeReference* aRef = NULL;
  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) 
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  

  aRef->Set(_root->Label());
  return true;
}

//============================================================================
/*! Function : HasChildren
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::HasChildren(const SALOMEDSImpl_SObject& theObject)
{
  if(!_root) return false;

  DF_Label aLabel;
  if (!theObject) aLabel = _root->Label();
  else 
    aLabel = theObject.GetLabel(); 
  if(aLabel.IsNull()) return false;

  SALOMEDSImpl_AttributeTreeNode* aNode = NULL;
  if(!(aNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(_root->ID()))) return false; 
  
  return (aNode->GetFirst());
}

//============================================================================
/*! Function : SetName
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_UseCaseBuilder::SetName(const string& theName) {
  if(!_root) return false;

  SALOMEDSImpl_AttributeName* aNameAttrib = NULL;

  if (!(aNameAttrib=(SALOMEDSImpl_AttributeName*)_root->FindAttribute(SALOMEDSImpl_AttributeName::GetID())))
    aNameAttrib = SALOMEDSImpl_AttributeName::Set(_root->Label(), theName);
     
  aNameAttrib->SetValue(theName);
    
  return true;
}


//============================================================================
/*! Function : GetCurrentObject
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_UseCaseBuilder::GetCurrentObject()
{
  SALOMEDSImpl_SObject so;
  if(!_root) return so;

  SALOMEDSImpl_AttributeReference* aRef = NULL;
  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    aRef = SALOMEDSImpl_AttributeReference::Set(_root->Label(), _root->Label());  
  }  
  
  DF_Label aCurrent = aRef->Get();  
  if(aCurrent.IsNull()) return so;

  return SALOMEDSImpl_Study::SObject(aCurrent);
}

//============================================================================
/*! Function : GetName
 *  Purpose  :
 */
//============================================================================
string SALOMEDSImpl_UseCaseBuilder::GetName() 
{
  string aString;
  if(!_root) return aString;
  
  SALOMEDSImpl_AttributeName* aName = NULL;
  if (!(aName=(SALOMEDSImpl_AttributeName*)_root->FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) return aString;
  return aName->Value();
}

//============================================================================ 
/*! Function :  IsUseCase
 *  Purpose  :  
 */ 
//============================================================================ 
bool SALOMEDSImpl_UseCaseBuilder::IsUseCase(const SALOMEDSImpl_SObject& theObject)
{
  if(!theObject) return false;
  DF_Label aFather, aLabel = theObject.GetLabel(); 
  aFather = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG);
  if(aLabel.Father() == aFather) return true;
  return false;
}

//============================================================================ 
/*! Function : NewUseCase 
 *  Purpose  :  
 */ 
//============================================================================ 
SALOMEDSImpl_SObject SALOMEDSImpl_UseCaseBuilder::AddUseCase(const string& theName)
{
  string aBasicGUID(USE_CASE_GUID);

  //Create a use cases structure if it not exists 

  SALOMEDSImpl_AttributeTreeNode *aFatherNode = NULL, *aNode = NULL;
  SALOMEDSImpl_AttributeInteger* anInteger = NULL;
  SALOMEDSImpl_AttributeReference* aRef = NULL;

  DF_Label aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG);

  if(!(aRef=(SALOMEDSImpl_AttributeReference*)_root->FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    aRef = SALOMEDSImpl_AttributeReference::Set(aLabel, aLabel);
  }
 
  if(!(aFatherNode=(SALOMEDSImpl_AttributeTreeNode*)aRef->Get().FindAttribute(aBasicGUID))) {
    aFatherNode = SALOMEDSImpl_AttributeTreeNode::Set(aRef->Get(), aBasicGUID);
  }

  if(!(anInteger=(SALOMEDSImpl_AttributeInteger*)_root->FindAttribute(SALOMEDSImpl_AttributeInteger::GetID()))) {
    anInteger = SALOMEDSImpl_AttributeInteger::Set(aLabel, 0);
  }    

  //Create a new use case
  anInteger->SetValue(anInteger->Value()+1);
  DF_Label aChild = aLabel.FindChild(anInteger->Value());
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
SALOMEDSImpl_UseCaseIterator
SALOMEDSImpl_UseCaseBuilder::GetUseCaseIterator(const SALOMEDSImpl_SObject& theObject) 
{
  DF_Label aLabel;

  if(theObject) {
    aLabel = theObject.GetLabel(); //Iterate only sub tree in the use case
  }
  else {
    aLabel = _doc->Main().Root().FindChild(USE_CASE_LABEL_TAG); //Iterate all use cases
  }

  return SALOMEDSImpl_UseCaseIterator(aLabel, USE_CASE_GUID, false); 
}


SALOMEDSImpl_SObject SALOMEDSImpl_UseCaseBuilder::GetSObject(const string& theEntry)
{
  DF_Label L = DF_Label::Label(_root->Label(), theEntry);
  return SALOMEDSImpl_Study::SObject(L);    
}
