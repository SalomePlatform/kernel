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

//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDS_AttLong_i.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOMEDS_AttLong_i.hxx"
#include "utilities.h"
#include <stdio.h>
#include <SALOMEDSImpl_AttributeInteger.hxx>


//============================================================================
/*! Function : Set
 *  Purpose  : Affect a value to the basic attribute
 */
//============================================================================
void SALOMEDS_AttLong_i::Set(CORBA::Long i)
{
  CheckLocked();
  SALOMEDSImpl_AttributeInteger::Set (_Lab,i);
}

//============================================================================
/*! Function : Get
 *  Purpose  : Get basic attribute's value
 */
//============================================================================
CORBA::Long SALOMEDS_AttLong_i::Get()
{
  SALOMEDSImpl_AttributeInteger* Att = NULL;
  CORBA::Long x;
  if ((Att=(SALOMEDSImpl_AttributeInteger*)_Lab.FindAttribute(SALOMEDSImpl_AttributeInteger::GetID())))
    x = Att->Get ();
  return x;
}

//============================================================================
/*! Function : Save
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttLong_i::Save()
{
  BEGIN_OF("SALOMEDS_AttLong_i::Save");
  CORBA::Long x = this->Get();
  char* out= new char[12];
  sprintf(out,"%ld",x);
  return out; 
}

//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_AttLong_i::Load()
{
  BEGIN_OF("SALOMEDS_AttLong_i::Load");
}

//============================================================================
/*! Function : GetPtr
 *  Purpose  : return sobject associated to the basic attribute (if exist)
 */
//============================================================================
CORBA::Boolean SALOMEDS_AttLong_i::GetPtr(SALOMEDS::SObject_ptr anObject)
{
  return (_Lab.IsAttribute(SALOMEDSImpl_AttributeInteger::GetID()));
}

//============================================================================
/*! Function : GetType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttLong_i::GetType()
{
  return CORBA::string_dup(_type);
}
