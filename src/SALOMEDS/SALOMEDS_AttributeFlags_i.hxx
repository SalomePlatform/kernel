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
//  File   : SALOMEDS_AttributeFlags_i.hxx
//  Author : Sergey LITONIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeFlags_i_HeaderFile
#define SALOMEDS_AttributeFlags_i_HeaderFile

// IDL headers

#include "SALOMEDS_FlagsAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

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
  
                      SALOMEDS_AttributeFlags_i( const Handle(SALOMEDS_FlagsAttribute)&,
                                                 CORBA::ORB_ptr orb );
  virtual             ~SALOMEDS_AttributeFlags_i();

  CORBA::Long         GetFlags();
  void                SetFlags( CORBA::Long theFlags );

  CORBA::Boolean      Get( CORBA::Long theFlag );
  void                Set( CORBA::Long theFlag, CORBA::Boolean theValue );
  
};

#endif
