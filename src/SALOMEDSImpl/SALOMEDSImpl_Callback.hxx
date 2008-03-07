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
//  File   : SALOMEDSImpl_Callback.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_CALLBACK_H__
#define __SALOMEDSIMPL_CALLBACK_H__


#include "SALOMEDSImpl_UseCaseBuilder.hxx"

class SALOMEDSImpl_Callback
{
private:
  SALOMEDSImpl_UseCaseBuilder* _builder;

public:

 SALOMEDSImpl_Callback(SALOMEDSImpl_UseCaseBuilder* builder) 
 {
   _builder = builder;
 }

 virtual void OnAddSObject(const SALOMEDSImpl_SObject& theObject) 
 {
   if(_builder != NULL && theObject) _builder->Append(theObject);
 }

 virtual void OnRemoveSObject(const SALOMEDSImpl_SObject& theObject) 
 {
   if(_builder != NULL && theObject) _builder->Remove(theObject);
 }

};

#endif
