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
//  File   : SALOMEDS_BasicAttributeFactory.cxx
//  Author : Estelle Deville, CEA
//  Module : SALOME
//  $Header$
//
#include "SALOMEDS_BasicAttributeFactory.hxx"
#include "utilities.h"


//============================================================================
/*! Function : Create
 *  Purpose  : create new basic attributes
 */
//============================================================================
SALOMEDS_BasicAttribute_i* BasicAttributeFactory::Create(const char* type)
    throw (SALOME_Exception)
{
  try {
    if (!strcmp(type,"AttReal"))
      {  
        SALOMEDS_AttReal_i *A = new SALOMEDS_AttReal_i();
        return A;
      }
    else if(!strcmp(type,"AttLong"))
      {
        SALOMEDS_AttLong_i *A = new SALOMEDS_AttLong_i();
        return A;
      }
    else throw(SALOME_Exception(LOCALIZED("bad attribute type name")));
  } catch (...) {throw(SALOME_Exception(LOCALIZED("Unexpected exception was caught")));}
}
