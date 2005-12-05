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
//  File   : SALOMEDSImpl_Callback.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_CALLBACK_H__
#define __SALOMEDSIMPL_CALLBACK_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_Callback, MMgt_TShared )

#include "SALOMEDSImpl_UseCaseBuilder.hxx"

class SALOMEDSImpl_Callback : public MMgt_TShared  
{
private:
  Handle(SALOMEDSImpl_UseCaseBuilder) _builder;

public:

 SALOMEDSImpl_Callback(const Handle(SALOMEDSImpl_UseCaseBuilder)& builder) 
 {
   _builder = builder;
 }

 virtual void OnAddSObject(Handle(SALOMEDSImpl_SObject) theObject) 
 {
   if(_builder != NULL && theObject != NULL) _builder->Append(theObject);
 }

 virtual void OnRemoveSObject(Handle(SALOMEDSImpl_SObject) theObject) 
 {
   if(_builder != NULL && theObject != NULL) _builder->Remove(theObject);
 }

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_Callback )

};

#endif
