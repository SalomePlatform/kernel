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
//  File   : SALOMEDS_AttributeFileType_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeFileType_i_HeaderFile
#define SALOMEDS_AttributeFileType_i_HeaderFile

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_FileType.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

DEFINE_DERIVED_ATTR(AttributeFileType,SALOMEDS_FileType,true);

class SALOMEDS_AttributeFileType_i: 
  public virtual POA_SALOMEDS::AttributeFileType,
  public virtual SALOMEDS_TAttributeFileType_i 
{
  DEFINE_DERIVED_ATTR_METH_DEFAULT(AttributeFileType,SALOMEDS_FileType);
public:
  char* Value();
  void SetValue(const char* value);

  char* Store();
  void Restore(const char*);
  
};



#endif
