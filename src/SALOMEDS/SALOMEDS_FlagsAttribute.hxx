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
//  File   : SALOMEDS_FlagsAttribute.hxx
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef SALOMEDS_FlagsAttribute_HeaderFile
#define SALOMEDS_FlagsAttribute_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDataStd_Integer.hxx>

class Standard_GUID;
class TDF_Label;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


/*
  Class       : SALOMEDS_FlagsAttribute
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
*/

DEFINE_STANDARD_HANDLE( SALOMEDS_FlagsAttribute, TDataStd_Integer )

class Standard_EXPORT SALOMEDS_FlagsAttribute : public TDataStd_Integer
{
public:

static const Standard_GUID&            GetID() ;
static Handle(SALOMEDS_FlagsAttribute) Set( const TDF_Label&,
                                            const Standard_Integer );
                                                               
                                       SALOMEDS_FlagsAttribute();
virtual                                ~SALOMEDS_FlagsAttribute();

const Standard_GUID&                   ID() const;
void                                   Restore( const Handle(TDF_Attribute)& with );
Handle_TDF_Attribute                   NewEmpty() const;
void                                   Paste( const Handle(TDF_Attribute)& into,
                                              const Handle(TDF_RelocationTable)& RT ) const;
void                                   Set( const Standard_Integer );
Standard_Integer                       Get() const;

public:
  DEFINE_STANDARD_RTTI( SALOMEDS_FlagsAttribute )
};

#endif
