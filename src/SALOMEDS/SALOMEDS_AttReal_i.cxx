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
//  File   : SALOMEDS_AttReal_i.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOMEDS_AttReal_i.hxx"
#include "utilities.h"
#include <stdio.h>
#include <cstring>
#include <SALOMEDSImpl_AttributeReal.hxx>


//============================================================================
/*! Function : Set
 *  Purpose  : Affect a value to the basic attribute
 */
//============================================================================
void SALOMEDS_AttReal_i::Set(CORBA::Double r)
{
  CheckLocked();
  SALOMEDSImpl_AttributeReal::Set (_Lab,r);
}

//============================================================================
/*! Function : Get
 *  Purpose  : Get basic attribute's value
 */
//============================================================================
CORBA::Double SALOMEDS_AttReal_i::Get()
{
  SALOMEDSImpl_AttributeReal* Att;
  CORBA::Double x;
  if ((Att=(SALOMEDSImpl_AttributeReal*)_Lab.FindAttribute(TDataStd_Real::GetID())))
    x = Att->Get ();
  return x;
}

//============================================================================
/*! Function : Save
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttReal_i::Save()
{
  BEGIN_OF("SALOMEDS_AttReal_i::Save");
  CORBA::Double x = this->Get();
  char* out= new char[12];
  sprintf(out,"%g",x);
  return out; 
}

//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_AttReal_i::Load()
{
  BEGIN_OF("SALOMEDS_AttReal_i::Load");
}

//============================================================================
/*! Function : GetPtr
 *  Purpose  : return sobject associated to the basic attribute (if exist)
 */
//============================================================================
CORBA::Boolean SALOMEDS_AttReal_i::GetPtr(SALOMEDS::SObject_ptr anObject)
{
  return _Lab.IsAttribute(SALOMEDSImpl_AttributeReal::GetID());
}

//============================================================================
/*! Function : GetType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_AttReal_i::GetType()
{
  return CORBA::string_dup(_type);
}
