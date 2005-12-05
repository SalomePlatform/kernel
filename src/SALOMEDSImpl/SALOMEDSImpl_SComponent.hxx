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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_SComponent.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SCOMPONENT_H__
#define __SALOMEDSIMPL_SCOMPONENT_H__

//SALOMEDSImpl headers
#include "SALOMEDSImpl_SObject.hxx"

//Handle definition
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_SComponent, SALOMEDSImpl_SObject )

// std C++ headers
#include <iostream>

// Cascade headers
#include <TDF_Label.hxx>
#include <SALOMEDSImpl_SObject.hxx>
#include <TCollection_AsciiString.hxx> 
#include <TDF_Tool.hxx>
#include <stdio.h>

class SALOMEDSImpl_SComponent : public SALOMEDSImpl_SObject
{
public:
  
  SALOMEDSImpl_SComponent(const TDF_Label& lab);
  
  ~SALOMEDSImpl_SComponent();
 
  virtual TCollection_AsciiString ComponentDataType();
  virtual bool ComponentIOR(TCollection_AsciiString& theID);
  static bool IsA(const TDF_Label& theLabel);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SComponent )    

};
#endif
