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
//  File   : SALOMEDS_Study_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//
#ifndef __SALOMEDS_CALLBACK_I_H__
#define __SALOMEDS_CALLBACK_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_Callback_i: public virtual POA_SALOMEDS::Callback,
                           public virtual PortableServer::ServantBase 
{
private:
  SALOMEDS::UseCaseBuilder_var _builder;

public:

 SALOMEDS_Callback_i(SALOMEDS::UseCaseBuilder_ptr theBuilder) {
   _builder = SALOMEDS::UseCaseBuilder::_duplicate(theBuilder);
 }

 virtual void OnAddSObject(SALOMEDS::SObject_ptr theObject) {
   if(!_builder->_is_nil() && !theObject->_is_nil()) _builder->Append(theObject);
 }

 virtual void OnRemoveSObject(SALOMEDS::SObject_ptr theObject) {
   if(!_builder->_is_nil() && !theObject->_is_nil()) _builder->Remove(theObject);
 }

};

#endif
