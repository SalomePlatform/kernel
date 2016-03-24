// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

#include "DF_definitions.hxx"
#include "DF_Label.hxx"
#include "DF_Document.hxx"
#include "DF_Attribute.hxx"
#include "DF_ChildIterator.hxx"

#include <algorithm>

//Class DF_Label defines a persistence reference in DF_Document that contains a tree of Labels.
//This reference is named "entry" and is a sequence of tags divided by ":". The root entry is "0:".
//For example "0:1:1" corresponds the following structure
// 0_
//   |
//   |_1_
//       |
//       |_ 1

DF_Label DF_Label::Label(const DF_Label& theLabel, const std::string& theEntry, bool isCreated)
{
  if(theLabel.IsNull()) return DF_Label();
  
  DF_Label aLabel = theLabel.Root();
  if(theEntry == "0:") return aLabel;
  if(theEntry == "0:1") return theLabel.GetDocument()->Main();

  char* cc = (char*)theEntry.c_str();
  int n = 0;
  int i=0;

  while (*cc != '\0') {
    while ( *cc >= '0' && *cc <= '9') {
      n = 10*n + (*cc - '0');
      ++cc;
    }
    if (*cc == ':' || *cc == '\0') {
      if(i>0)
        {
          if(aLabel.IsNull())break;
          aLabel = aLabel.FindChild(n, isCreated);
        }
      i++;
      n = 0;
      if (*cc != '\0') ++cc;
    }
    else {
      return DF_Label();
    }
  }

  return aLabel;
}

DF_Label::DF_Label(DF_LabelNode* theNode)
  :_node(theNode)
{
}

//Constructor
DF_Label::DF_Label()
{
  _node = NULL;
}

//Copy constructor
DF_Label::DF_Label(const DF_Label& theLabel)
{
  _node = theLabel._node;
}

DF_Label& DF_Label::operator=(const DF_Label& theLabel)
{
  _node = theLabel._node;
  return *this;
}

//Destructor
DF_Label::~DF_Label()
{
  _node = NULL;
}

//Returns a smart pointer to Document which contains this Label
DF_Document* DF_Label::GetDocument() const
{
  if(!_node) return NULL;
  return _node->_document;
}

//Returns true if theLabel equals to this label
bool DF_Label::operator==(const DF_Label& theLabel)
{
  if(IsNull() || theLabel.IsNull()) return false;
  return (theLabel.Entry() == Entry());
}

//Returns true if theLabel doesn't equals to this label
bool DF_Label::operator!=(const DF_Label& theLabel)
{
  if(IsNull() || theLabel.IsNull()) return true;
  return (theLabel.Entry() != Entry());
}


//Returns a tag of this Label
int DF_Label::Tag() const
{
  if(!_node) return -1;
  return _node->_tag;
}

//Returns true if this Label is attached to the tree in the Document.
bool DF_Label::IsAttached()
{
  if(!_node) return false;
  return _node->_document != 0;
}

//Searches an Attribute with given ID located on this Label.
//Returns true if the Attribute is found.
DF_Attribute* DF_Label::FindAttribute(const std::string& theID) const
{
  if(!_node) return NULL;

  std::map< std::string, DF_Attribute* >::iterator it=_node->_attributes.find(theID);
  if(it == _node->_attributes.end()) return NULL;
  return it->second;

}

//Returns true if there is an Attribute with given ID on this Label.
bool DF_Label::IsAttribute(const std::string& theID) const
{
  if(!_node) return false;

  return (_node->_attributes.find(theID) != _node->_attributes.end());
}

//Adds theAttribute to the Label where this Attribute is located.
//Returns true if theAttribute was added.
bool DF_Label::AddAttribute(DF_Attribute* theAttribute) const
{
  if(!_node) return false;

  if(_node->_attributes.find(theAttribute->ID()) != _node->_attributes.end()) return false;
  theAttribute->_node = _node;
  _node->_attributes[theAttribute->ID()] = theAttribute;
  theAttribute->AfterAddition();    

  return true;
}

//Forgets an Attribute with given ID located on the this Label.
bool DF_Label::ForgetAttribute(const std::string& theID) const
{
  if(!_node) return false;

  if(_node->_attributes.find(theID) == _node->_attributes.end()) return false;
  DF_Attribute* attr = _node->_attributes[theID];
  attr->BeforeForget();
  _node->_attributes.erase(theID);
  delete attr;

  return true;
}

//Forgets all Attributes located on this Label.
bool DF_Label::ForgetAllAttributes(bool clearChildren) const
{
  if(!_node) return false;

  std::vector<DF_Attribute*> va = GetAttributes();
  _node->_attributes.clear();

  for(int i = 0, len = va.size(); i<len; i++) {
    va[i]->BeforeForget();
    delete va[i];
  }

  if(clearChildren) {
    DF_ChildIterator CI(*this, true);
    for(; CI.More(); CI.Next()) 
      CI.Value().ForgetAllAttributes(true);
  }

  return true;
}

//Returns Father of this Label.
DF_Label DF_Label::Father() const
{
  if(!_node) return DF_Label();

  return _node->_father;
}

//Returns is this Label is not initialized
bool DF_Label::IsNull() const
{
  return (!_node || (_node->_document == NULL));
}

//Returns is this Label is a Root label
bool DF_Label::IsRoot() const
{
  if(IsNull() || Father().IsNull()) return true;
  return false;
}


//Returns true if this Label has Attributes.
bool DF_Label::HasAttributes() const
{
  if(!_node) return false;

  return !(_node->_attributes.empty());
}

//Returns a list of Attributes of this Label.
std::vector<DF_Attribute*> DF_Label::GetAttributes() const
{
  std::vector<DF_Attribute*> attributes;
  if(!_node) return attributes;
  
  typedef std::map<std::string, DF_Attribute*>::const_iterator AI;
  std::vector<std::string> sorted;
  for(AI p = _node->_attributes.begin(); p!=_node->_attributes.end(); p++)
    sorted.push_back(p->first);
    
  sort(sorted.begin(), sorted.end());
  int len = sorted.size();    
  for(int i = 0; i<len; i++)
    attributes.push_back(_node->_attributes[sorted[i]]);

  return attributes;
}

//Returns true if this Label has a child Label.
bool DF_Label::HasChild() const
{
  if(!_node) return false;

  return _node->_firstChild != 0;
}

//Returns a number of child Labels.
int DF_Label::NbChildren() const
{
  if(!_node) return -1;

  if(!_node->_firstChild) return 0;
  int nb = 1;
  DF_LabelNode* next = _node->_firstChild->_next;
  while(next) {
    nb++;
    next = next->_next;
  }

  return nb;
}

//Returns the depth (a number of fathers required to identify the Label) of this Label in the tree.
int DF_Label::Depth() const
{
  if(!_node) return -1;

  return _node->_depth;
}

//Returns true if this Label is a descendant of theLabel.
bool DF_Label::IsDescendant(const DF_Label& theLabel)
{
  if(!_node) return false;

  DF_LabelNode* father = _node->_father;
  if(!father) return false;

  while(father) {
    if(father == theLabel._node) return true;
    father = father->_father;
  }

  return false;
}

//Returns the root Label of a Label tree to which this Label belongs.
DF_Label DF_Label::Root() const
{
  if(!_node) return DF_Label();

  return _node->_document->Main().Father();
}

//Finds a child Label of this Label with a given tag. If isCreate = true and there is no child
//Label with the given tag, the child Label is created.
DF_Label DF_Label::FindChild(int theTag, bool isCreate)
{
  if(!_node || IsNull()) return DF_Label();

  DF_LabelNode *aLabel = NULL, *aPrevious = NULL, *aNext = NULL;
  if(!_node->_firstChild && !isCreate) return DF_Label();

  if(_node->_firstChild && _node->_firstChild->_tag == theTag)
    return DF_Label(_node->_firstChild);
 
  if(_node->_lastChild) {
    if(_node->_lastChild->_tag == theTag) return DF_Label(_node->_lastChild);
    if(_node->_lastChild->_tag < theTag) aPrevious = _node->_lastChild;
  }
  
  if(!aPrevious) { 
    aLabel = _node->_firstChild;
    while(aLabel) {
      if(aLabel->_tag == theTag) return DF_Label(aLabel);
      if(aLabel->_tag > theTag) {
        aNext = aLabel;
        break;
      }
      if(aLabel->_tag < theTag) aPrevious = aLabel;
      aLabel = aLabel->_next;
    }
  }
  
  if(!isCreate) return DF_Label();

  DF_LabelNode* aChild = new DF_LabelNode();
  aChild->_father = this->_node;
  aChild->_document = _node->_document;
  aChild->_tag = theTag;
  aChild->_depth = _node->_depth+1;
  if(aNext) {
    aChild->_previous = aNext->_previous;
    aChild->_next = aNext;
    aNext->_previous = aChild;
  }
  if(aPrevious) {
    aChild->_previous = aPrevious;
    aChild->_next = aPrevious->_next;
    aPrevious->_next = aChild;
  }
    
  if(!_node->_firstChild || (aNext && aNext == _node->_firstChild) ) _node->_firstChild = aChild;
  if(!_node->_lastChild || !aNext) _node->_lastChild = aChild;
  
  return aChild;
}

//Creates a new child Label of this Label.
DF_Label DF_Label::NewChild()
{
  if(!_node || IsNull()) return DF_Label();

  int tag = 1;
  if(_node->_lastChild) tag = _node->_lastChild->_tag+1;
  
  return FindChild(tag, true);
}

//Returns a tag of the last child
int DF_Label::LastChildTag() const
{
  return _node->_lastChild ? _node->_lastChild->_tag : 0;
}

//Returns a string entry of this Label
std::string DF_Label::Entry() const
{
  DF_LabelNode* father = this->_node;
  if(!father->_father)return "0:";
  int tag;
  char buff[128];
  char* wstr= buff;
  char* str = buff;

  while(father)
    {
      tag=father->_tag;
      do{
         // Conversion. Number is reversed.
         *wstr++ = '0' + (tag % 10);
      }while(tag /= 10);
      father = father->_father;
      if(father)*wstr++ = ':';
    }
  *wstr-- = '\0';

  //reverse the buffer
  char aux;
  while (wstr > str)
    aux = *wstr, *wstr-- = *str, *str++ = aux;

  return buff;
}

bool DF_Label::IsEqual(const DF_Label& theLabel)
{
  if(theLabel.IsNull() || IsNull()) return false;
  DF_Label L(theLabel);
  return (L.Entry() == Entry());
}


void DF_Label::Nullify() 
{
  delete _node;
  _node = NULL;
}

void DF_Label::dump()
{
  if(!_node) std::cout << "DF_Label addr : " << this << " NULL " << std::endl;
  else {
    std::cout << "DF_Label addr : " << this->_node << " entry : " << Entry() << std::endl;
    if(_node->_father) std::cout << " Father : " << _node->_father << " entry : " << Father().Entry() << std::endl;
    else std::cout << " Father : NULL " << std::endl;

    if(_node->_firstChild) std::cout << " FirstChild : " << _node->_firstChild << " entry : " << DF_Label(_node->_firstChild).Entry() << std::endl;
    else std::cout << " FirstChild : NULL " << std::endl;

    if(_node->_lastChild) std::cout << " LastChild : " << _node->_lastChild << " entry : " << DF_Label(_node->_lastChild).Entry() << std::endl;
    else std::cout << " LastChild : NULL " << std::endl;

    if(_node->_previous) std::cout << " Previous : " << _node->_previous << " entry : " << DF_Label(_node->_previous).Entry() << std::endl;
    else std::cout << " Previous : NULL " << std::endl;

    if(_node->_next) std::cout << " Next : " << _node->_next << " entry : " << DF_Label(_node->_next).Entry() << std::endl;
    else std::cout << " Next : NULL " << std::endl;
  }
}


/*
 ###############################################
            DF_LabelNode methods
 ###############################################
*/

DF_LabelNode::DF_LabelNode()
{
  _depth = 0;
  _tag = 0;
  _attributes.clear();
  _document = NULL;
  _father = NULL;
  _firstChild = NULL;
  _lastChild = NULL;
  _previous = NULL;
  _next = NULL;
}

DF_LabelNode::~DF_LabelNode()
{
  std::vector<DF_Attribute*> va;
  typedef std::map<std::string, DF_Attribute*>::const_iterator AI;
  for(AI p = _attributes.begin(); p!=_attributes.end(); p++)
    va.push_back(p->second);

  for(int i = 0, len = va.size(); i<len; i++) 
    delete va[i];

  _attributes.clear();
}


void DF_LabelNode::Reset()
{
  _depth = 0;
  _tag = 0;

  std::vector<DF_Attribute*> va;
  typedef std::map<std::string, DF_Attribute*>::const_iterator AI;
  for(AI p = _attributes.begin(); p!=_attributes.end(); p++)
    va.push_back(p->second);

  for(int i = 0, len = va.size(); i<len; i++) 
    delete va[i];

  _attributes.clear();
  _document = NULL;
  _father = NULL;
  _firstChild = NULL;
  _lastChild = NULL;
  _previous = NULL;
  _next = NULL;  
}
