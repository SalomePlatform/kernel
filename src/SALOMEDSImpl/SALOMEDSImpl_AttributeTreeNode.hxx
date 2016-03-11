// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  File   : SALOMEDSImpl_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef _SALOMEDSImpl_AttributeTreeNode_HeaderFile
#define _SALOMEDSImpl_AttributeTreeNode_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeTreeNode : 
  public SALOMEDSImpl_GenericAttribute 
{
public:

  const static std::string& GetDefaultTreeID();
  static SALOMEDSImpl_AttributeTreeNode* Set(const DF_Label& L, const std::string& ID);

  SALOMEDSImpl_AttributeTreeNode();
  void SetFather(const SALOMEDSImpl_AttributeTreeNode* value);
  void SetPrevious(const SALOMEDSImpl_AttributeTreeNode* value);
  void SetNext(const SALOMEDSImpl_AttributeTreeNode* value);
  void SetFirst(const SALOMEDSImpl_AttributeTreeNode* value);
  void SetTreeID(const std::string& value);

  bool Append(SALOMEDSImpl_AttributeTreeNode* value);  
  bool Prepend(SALOMEDSImpl_AttributeTreeNode* value);
  bool Remove();
  bool InsertBefore(SALOMEDSImpl_AttributeTreeNode* value);
  bool InsertAfter(SALOMEDSImpl_AttributeTreeNode* value);

  SALOMEDSImpl_AttributeTreeNode* GetFather() const { return myFather; }
  SALOMEDSImpl_AttributeTreeNode* GetPrevious() const { return myPrevious; }
  SALOMEDSImpl_AttributeTreeNode* GetNext() const { return myNext; }
  SALOMEDSImpl_AttributeTreeNode* GetFirst() const { return myFirst; }

  bool HasFather() { return (myFather); }
  bool HasPrevious() { return (myPrevious); }
  bool HasNext() {return (myNext); }
  bool HasFirst() { return (myFirst); }
  const std::string& GetTreeID() { return ID(); }
  int Depth() const;
  bool IsRoot() const ;
  SALOMEDSImpl_AttributeTreeNode* Root() const;
  bool IsAscendant (const SALOMEDSImpl_AttributeTreeNode* ofTN) const;
  bool IsDescendant(const SALOMEDSImpl_AttributeTreeNode* value) const;
  bool IsFather(const SALOMEDSImpl_AttributeTreeNode* value) const;
  bool IsChild(const SALOMEDSImpl_AttributeTreeNode* value) const;

  virtual std::string Save();
  virtual void Load(const std::string&); 
  virtual std::string Type(); 

  const std::string& ID() const;
  void Restore(DF_Attribute* with) ;
  void Paste(DF_Attribute* into);
  DF_Attribute* NewEmpty() const;
  ~SALOMEDSImpl_AttributeTreeNode() {;}

  virtual void AfterAddition();
  virtual void BeforeForget();

private:

  std::string myTreeID; 
  SALOMEDSImpl_AttributeTreeNode* myFather;
  SALOMEDSImpl_AttributeTreeNode* myPrevious;
  SALOMEDSImpl_AttributeTreeNode* myNext;
  SALOMEDSImpl_AttributeTreeNode* myFirst;

  friend class SALOMEDSImpl_ChildNodeIterator;

};

#endif
