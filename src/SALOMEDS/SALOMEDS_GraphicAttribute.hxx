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
//  File   : SALOMEDS_GraphicAttribute.hxx
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef SALOMEDS_GraphicAttribute_HeaderFile
#define SALOMEDS_GraphicAttribute_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Attribute.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>

class Standard_GUID;
class TDF_Label;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


/*
  Class       : SALOMEDS_GraphicAttribute
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

DEFINE_STANDARD_HANDLE( SALOMEDS_GraphicAttribute, TDF_Attribute )

class Standard_EXPORT SALOMEDS_GraphicAttribute : public TDF_Attribute
{

public:
                                  SALOMEDS_GraphicAttribute();
                                  ~SALOMEDS_GraphicAttribute();

  static const                    Standard_GUID& GetID() ;

  void                            SetVisibility( const Standard_Integer,
                                                 const Standard_Boolean );
  Standard_Boolean                GetVisibility( const Standard_Integer ) const;
  const Standard_GUID&            ID() const;
  void                            Restore( const Handle(TDF_Attribute)& theWith );
  Handle(TDF_Attribute)           NewEmpty() const;
  void                            Paste( const Handle(TDF_Attribute)& theInto,
                                         const Handle(TDF_RelocationTable)& ) const;
  virtual Standard_OStream&       Dump( Standard_OStream& anOS ) const;

  void                            SetVisibility( const TColStd_DataMapOfIntegerInteger& );

  const TColStd_DataMapOfIntegerInteger& GetVisibility();
  
private:
  TColStd_DataMapOfIntegerInteger myVisibility;

public:
  DEFINE_STANDARD_RTTI( SALOMEDS_GraphicAttribute )
};

#endif
