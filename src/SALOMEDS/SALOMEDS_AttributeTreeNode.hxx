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

//  File   : SALOMEDS_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeTreeNode_HeaderFile
#define SALOMEDS_AttributeTreeNode_HeaderFile

#include "SALOMEDSClient_AttributeTreeNode.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeTreeNode: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTreeNode
{
public:  
  SALOMEDS_AttributeTreeNode(SALOMEDSImpl_AttributeTreeNode* theAttr);
  SALOMEDS_AttributeTreeNode(SALOMEDS::AttributeTreeNode_ptr theAttr);
  ~SALOMEDS_AttributeTreeNode();

  virtual void SetFather(const _PTR(AttributeTreeNode)& value);
  virtual bool HasFather();
  virtual _PTR(AttributeTreeNode) GetFather();
  virtual void SetPrevious(const _PTR(AttributeTreeNode)& value);
  virtual bool HasPrevious();
  virtual _PTR(AttributeTreeNode) GetPrevious();
  virtual void SetNext(const _PTR(AttributeTreeNode)& value);
  virtual bool HasNext();
  virtual _PTR(AttributeTreeNode) GetNext();
  virtual void SetFirst(const _PTR(AttributeTreeNode)& value);
  virtual bool HasFirst();
  virtual _PTR(AttributeTreeNode) GetFirst();
  virtual void SetTreeID(const std::string& value);
  virtual std::string GetTreeID();
  virtual void Append(const _PTR(AttributeTreeNode)& value);
  virtual void Prepend(const _PTR(AttributeTreeNode)& value);
  virtual void InsertBefore(const _PTR(AttributeTreeNode)& value);
  virtual void InsertAfter(const _PTR(AttributeTreeNode)& value);
  virtual void Remove();
  virtual int Depth();
  virtual bool IsRoot();
  virtual bool IsDescendant(const _PTR(AttributeTreeNode)& value);
  virtual bool IsFather(const _PTR(AttributeTreeNode)& value);
  virtual bool IsChild(const _PTR(AttributeTreeNode)& value);
  virtual std::string Label();                                        
};

#endif
