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

//  File   : SALOMEDS_AttributeFlags_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:
//
#ifndef SALOMEDS_AttributeFlags_i_HeaderFile
#define SALOMEDS_AttributeFlags_i_HeaderFile

// IDL headers

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeFlags.hxx"

/*
  Class       : SALOMEDS_AttributeFlags_i
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).

                Avalable attributes:

                IS_VISIBLE - is equal to 1 if object is visible in 3D view (0 - overwise).
                             This attribute is valid for active view only.
*/

class SALOMEDS_AttributeFlags_i: public virtual POA_SALOMEDS::AttributeFlags,
                                 public virtual SALOMEDS_GenericAttribute_i
{
public:
  
                      SALOMEDS_AttributeFlags_i( SALOMEDSImpl_AttributeFlags* theAttr, CORBA::ORB_ptr orb )
                        :SALOMEDS_GenericAttribute_i(theAttr, orb) {};

  virtual             ~SALOMEDS_AttributeFlags_i() {};

  CORBA::Long         GetFlags();
  void                SetFlags( CORBA::Long theFlags );

  CORBA::Boolean      Get( CORBA::Long theFlag );
  void                Set( CORBA::Long theFlag, CORBA::Boolean theValue );
  
};

#endif
