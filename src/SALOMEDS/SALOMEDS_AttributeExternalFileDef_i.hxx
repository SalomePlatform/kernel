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
//  File   : SALOMEDS_AttributeExternalFileDef_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeExternalFileDef_i_HeaderFile
#define SALOMEDS_AttributeExternalFileDef_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_ExternalFileDef.hxx"

DEFINE_DERIVED_ATTR(AttributeExternalFileDef,SALOMEDS_ExternalFileDef,false);

class SALOMEDS_AttributeExternalFileDef_i: 
  public virtual POA_SALOMEDS::AttributeExternalFileDef,
  public virtual SALOMEDS_TAttributeExternalFileDef_i 
{
  DEFINE_DERIVED_ATTR_METH_DEFAULT(AttributeExternalFileDef,SALOMEDS_ExternalFileDef);
public:
  char* Value();
  void SetValue(const char* value);

  char* Store();
  void Restore(const char*);
  
};



#endif
