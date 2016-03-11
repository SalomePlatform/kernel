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
#include "DF_Document.hxx"
#include "DF_Label.hxx"
#include "DF_ChildIterator.hxx"

//Class DF_Document is container for user's data stored as a tree of Labels
//with assigned Attributes

DF_Document::DF_Document(const std::string& theDocumentType)
{
  _id = -1;
  _type = theDocumentType;
  _modified = true;
}

DF_Document::~DF_Document()
{
  Clear();
}

DF_Application* DF_Document::GetApplication()
{
   return _appli;
}

//Returns a Label of this Document with entry "0:1"
DF_Label DF_Document::Main()
{
  if(!_main.IsNull()) return _main;

  if(_root.IsNull()) {
    _root = DF_Label(new DF_LabelNode());
    _root._node->_document = this;
  }

  _main = _root.FindChild(1, true);

  return _main;
}

//Returns a Label of this Document with entry "0:"
DF_Label DF_Document::Root()
{
  if(!_root.IsNull()) return _root;
  
  if(_root.IsNull()) {
    _root = DF_Label(new DF_LabelNode());
    _root._node->_document = this;
  }

  return _root;
}


//Returns an ID of this 
int DF_Document::GetDocumentID() const
{
  return _id;
}

//Returns a type of the Document
std::string DF_Document::GetDocumentType()
{
  return _type;
}

//Clears the content of this Document
void DF_Document::Clear()
{
  if(_root.IsNull()) return;

  std::vector<DF_LabelNode*> vn;
  DF_ChildIterator CI(_root, true);
  for(; CI.More(); CI.Next()) {
    DF_LabelNode* node =  CI.Value()._node; 
    if(node) vn.push_back(node);
  }

  for(int i = 0, len = vn.size(); i<len; i++)
    delete vn[i];

  _root._node->Reset();
  _root.Nullify();
}

//Returns true if this document is empty
bool DF_Document::IsEmpty()
{
  if(_root.IsNull()) return true;

  DF_ChildIterator CI(_root, true);
  for(; CI.More(); CI.Next()) {
    DF_LabelNode* node =  CI.Value()._node; 
    if(node->_attributes.size()) return false;
  }

  return true;
}

//Returns true if this document is modified
bool DF_Document::IsModified()
{
  return _modified;
}

//Sets whether a document is modified
void DF_Document::SetModified(bool isModified)
{
  _modified = isModified;
}


//Restores a content of the Document from the std::string theData
void DF_Document::Load(const std::string& theData)
{
   //Not implemented
}

//Converts a content of the Document into the std::string
std::string DF_Document::Save()
{
   //Not implemented
   return "";
}
