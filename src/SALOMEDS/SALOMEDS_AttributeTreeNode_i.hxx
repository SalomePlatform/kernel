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
//  File   : SALOMEDS_AttributeTreeNode_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeTreeNode_i_HeaderFile
#define SALOMEDS_AttributeTreeNode_i_HeaderFile

// IDL headers
#include <TDataStd_TreeNode.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTreeNode_i: public virtual POA_SALOMEDS::AttributeTreeNode,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTreeNode_i(const Handle(TDataStd_TreeNode)& theAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theAttr; 
  };  
  ~SALOMEDS_AttributeTreeNode_i() {};

  void SetFather(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasFather();
  SALOMEDS::AttributeTreeNode_ptr GetFather();
  void SetPrevious(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasPrevious();
  SALOMEDS::AttributeTreeNode_ptr GetPrevious();
  void SetNext(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasNext();
  SALOMEDS::AttributeTreeNode_ptr GetNext();
  void SetFirst(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasFirst();
  SALOMEDS::AttributeTreeNode_ptr GetFirst();
  void SetTreeID(const char* value);
  char* GetTreeID();
  void Append(SALOMEDS::AttributeTreeNode_ptr value);
  void Prepend(SALOMEDS::AttributeTreeNode_ptr value);
  void InsertBefore(SALOMEDS::AttributeTreeNode_ptr value);
  void InsertAfter(SALOMEDS::AttributeTreeNode_ptr value);
  void Remove();
  CORBA::Long Depth();
  CORBA::Boolean IsRoot();
  CORBA::Boolean IsDescendant(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean IsFather(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean IsChild(SALOMEDS::AttributeTreeNode_ptr value);
  char* Label();

  char* Store();
  void Restore(const char*);

};

#endif
