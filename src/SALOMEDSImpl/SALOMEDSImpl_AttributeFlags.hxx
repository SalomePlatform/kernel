// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
//  File   : SALOMEDSImpl_AttributeFlags.hxx
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef SALOMEDSImpl_AttributeFlags_HeaderFile
#define SALOMEDSImpl_AttributeFlags_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


/*
  Class       : SALOMEDSImpl_AttributeFlags
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
*/

DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeFlags, SALOMEDSImpl_GenericAttribute )

#ifndef WNT
class Standard_EXPORT SALOMEDSImpl_AttributeFlags :  public SALOMEDSImpl_GenericAttribute
#else
class SALOMEDSImpl_AttributeFlags :  public SALOMEDSImpl_GenericAttribute
#endif
{
private:
Standard_Integer myValue;

public:
Standard_EXPORT  static const Standard_GUID&            GetID() ;
Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeFlags) Set(const TDF_Label&, const Standard_Integer );
                                                               
                                       SALOMEDSImpl_AttributeFlags();
Standard_EXPORT  virtual                                ~SALOMEDSImpl_AttributeFlags();

Standard_EXPORT  const Standard_GUID&                   ID() const;
Standard_EXPORT  void                                   Restore( const Handle(TDF_Attribute)& with );
Standard_EXPORT  Handle_TDF_Attribute                   NewEmpty() const;
Standard_EXPORT  void                                   Paste( const Handle(TDF_Attribute)& into,
                                              const Handle(TDF_RelocationTable)& RT ) const;
Standard_EXPORT  void                                   Set( const Standard_Integer );
Standard_EXPORT  Standard_Integer                       Get() const;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeFlags )
};

#endif
