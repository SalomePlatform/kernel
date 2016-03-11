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

//  File   : SALOMEDSClient_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDSClient_AttributeTreeNode_HeaderFile
#define SALOMEDSClient_AttributeTreeNode_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeTreeNode: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual void SetFather(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasFather() = 0;
  virtual _PTR(AttributeTreeNode) GetFather() = 0;
  virtual void SetPrevious(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasPrevious() = 0;
  virtual _PTR(AttributeTreeNode) GetPrevious() = 0;
  virtual void SetNext(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasNext() = 0;
  virtual _PTR(AttributeTreeNode) GetNext() = 0;
  virtual void SetFirst(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasFirst() = 0;
  virtual _PTR(AttributeTreeNode) GetFirst() = 0;
  virtual void SetTreeID(const std::string& value) = 0;
  virtual std::string GetTreeID() = 0;
  virtual void Append(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void Prepend(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void InsertBefore(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void InsertAfter(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void Remove() = 0;
  virtual int Depth() = 0;
  virtual bool IsRoot() = 0;
  virtual bool IsDescendant(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool IsFather(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool IsChild(const _PTR(AttributeTreeNode)& value) = 0;
  virtual std::string Label() = 0;

};

#endif
