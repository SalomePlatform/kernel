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
//  File   : SALOMEDS_AttributeTarget_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeTarget_i_HeaderFile
#define SALOMEDS_AttributeTarget_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_TargetAttribute.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"

DEFINE_DERIVED_ATTR(AttributeTarget,SALOMEDS_TargetAttribute,false);

class SALOMEDS_AttributeTarget_i: 
  public virtual POA_SALOMEDS::AttributeTarget,
  public virtual SALOMEDS_TAttributeTarget_i 
{
  DEFINE_DERIVED_ATTR_METH_DEFAULT(AttributeTarget,SALOMEDS_TargetAttribute);
public:
  virtual void Add(SALOMEDS::SObject_ptr anObject) ;
  virtual SALOMEDS::Study::ListOfSObject* Get();
  virtual void Remove(SALOMEDS::SObject_ptr anObject);

};



#endif
