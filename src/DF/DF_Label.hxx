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

#ifndef DFLABEL_HXX
#define DFLABEL_HXX

#include "DF_definitions.hxx"
#include "DF_Attribute.hxx"

#include <string>
#include <vector>
#include <map>

class DF_Document;


class DF_LabelNode
{
public:
  DF_LabelNode();
  ~DF_LabelNode();
  void Reset();
private:
  int                                      _tag;
  int                                      _depth;
  DF_LabelNode*                            _father;
  DF_LabelNode*                            _previous;
  DF_LabelNode*                            _next;
  DF_LabelNode*                            _firstChild;
  DF_LabelNode*                            _lastChild;
  DF_Document*                             _document;
  std::map< std::string, DF_Attribute* >   _attributes;

  friend class DF_Document;
  friend class DF_Label;
  friend class DF_ChildIterator;
  friend class DF_Attribute;
};

//Class DF_Label defines a persistence reference in DF_Document that contains a tree of Labels.
//This reference is named "entry" and is a sequence of tags divided by ":". The root entry is "0:".
//For example "0:1:1" corresponds the following structure
// 0_
//   |
//   |_1_
//       |
//       |_ 1

class DF_Label {
public:

  //Returns a Label by its entry, if isCreated true the Label will be created if not exists
  Standard_EXPORT static DF_Label Label(const DF_Label& theLabel, const std::string& theEntry, bool isCreated = true);

  //Constructors: creates a root label
  Standard_EXPORT DF_Label();

  //Copy constructor
  Standard_EXPORT DF_Label(const DF_Label& theLabel);

  //Creates a Label from the LabelNode
  Standard_EXPORT DF_Label(DF_LabelNode* theNode);

  //Operator =
  Standard_EXPORT DF_Label& operator=(const DF_Label& theLabel);

  //Destructor
  Standard_EXPORT ~DF_Label();

  //Returns a smart pointer to Document which contains this Label
  Standard_EXPORT DF_Document* GetDocument() const;

  //Returns true if theLabel equals to this label
  Standard_EXPORT bool operator==(const DF_Label& theLabel);

  //Returns true if theLabel doesn't equals to this label
  Standard_EXPORT bool operator!=(const DF_Label& theLabel);

  //Returns a tag of this Label
  Standard_EXPORT int Tag() const;

  //Returns true if this Label is attached to the tree in the Document.
  Standard_EXPORT bool IsAttached();

  //Searches an Attribute with given ID located on this Label.
  //Returns true if the Attribute is found.
  Standard_EXPORT DF_Attribute* FindAttribute(const std::string& theID) const;

  //Returns true if there is an Attribute with given ID on this Label.
  Standard_EXPORT bool IsAttribute(const std::string& theID) const;

  //Adds theAttribute to the Label where this Attribute is located.
  //Returns true if theAttribute was added.
  Standard_EXPORT bool AddAttribute(DF_Attribute* theAttribute) const;

  //Forgets an Attribute with given ID located on the this Label.
  Standard_EXPORT bool ForgetAttribute(const std::string& theID) const;

  //Forgets all Attributes located on this Label.
  Standard_EXPORT bool ForgetAllAttributes(bool clearChildren = true) const;

  //Returns Father of this Label.
  Standard_EXPORT DF_Label Father() const;

  //Return true if the label is not initialized
  Standard_EXPORT bool IsNull() const;

  //Return true if the label is a Root label
  Standard_EXPORT bool IsRoot() const;

  //Returns true if this Label has Attributes.
  Standard_EXPORT bool HasAttributes() const;

  //Returns a list of Attributes of this Label.
  Standard_EXPORT std::vector<DF_Attribute*> GetAttributes() const;

  //Returns true if this Label has a child Label.
  Standard_EXPORT bool HasChild() const;

  //Returns a number of child Labels.
  Standard_EXPORT int NbChildren() const;

  //Returns the depth (a number of fathers required to identify the Label) of this Label in the tree.
  Standard_EXPORT int Depth() const;

  //Returns true if this Label is a descendant of theLabel.
  Standard_EXPORT bool IsDescendant(const DF_Label& theLabel);

  //Returns the root Label of a Label tree to which this Label belongs.
  Standard_EXPORT DF_Label Root() const;

  //Finds a child Label of this Label with a given tag. If isCreate = true and there is no child
  //Label with the given tag, the child Label is created.
  Standard_EXPORT DF_Label FindChild(int theTag, bool isCreate = true);

  //Creates a new child Label of this Label.
  Standard_EXPORT DF_Label NewChild();

  //Returns a tag of the last child
  Standard_EXPORT int LastChildTag() const;

  //Returns a string presentation of the entry
  Standard_EXPORT std::string Entry() const;

  //Returns true if theLabel is the same as me
  Standard_EXPORT bool IsEqual(const DF_Label& theLabel);

  Standard_EXPORT void dump();

private:
  //Nullifies the content of the label
  void Nullify();

friend class DF_Document;
friend class DF_ChildIterator;

private:
  DF_LabelNode* _node;
};

#endif
