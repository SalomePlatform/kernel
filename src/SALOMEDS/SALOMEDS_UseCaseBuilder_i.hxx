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

//  File   : SALOMEDS_UseCaseBuilder_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECaseBuilder_I_H__
#define __SALOMEDS_USECaseBuilder_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include <SALOME_GenericObj_i.hh>

#include <stdio.h>

#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

class SALOMEDS_UseCaseBuilder_i: public virtual POA_SALOMEDS::UseCaseBuilder,
                                 public virtual PortableServer::ServantBase,
                                 public virtual SALOME::GenericObj_i
{
private:

  CORBA::ORB_var               _orb;
  SALOMEDSImpl_UseCaseBuilder* _impl;

public:

  //! standard constructor  
  SALOMEDS_UseCaseBuilder_i(SALOMEDSImpl_UseCaseBuilder* theDocument,
                            CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_UseCaseBuilder_i();
  
  virtual CORBA::Boolean Append(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean Remove(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean AppendTo(SALOMEDS::SObject_ptr theFather, SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean InsertBefore(SALOMEDS::SObject_ptr theFirst, SALOMEDS::SObject_ptr theNext);

  virtual CORBA::Boolean  SetCurrentObject(SALOMEDS::SObject_ptr theObject);
  
  virtual CORBA::Boolean SetRootCurrent();

  virtual CORBA::Boolean  HasChildren(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean  SortChildren(SALOMEDS::SObject_ptr theObject, CORBA::Boolean theAscendingOrder);

  virtual SALOMEDS::SObject_ptr GetFather(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean  IsUseCase(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean  IsUseCaseNode(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean SetName(const char* theName);

  virtual SALOMEDS::SObject_ptr GetCurrentObject();

  virtual char* GetName();

  virtual SALOMEDS::SObject_ptr AddUseCase(const char* theName);

  virtual SALOMEDS::UseCaseIterator_ptr GetUseCaseIterator(SALOMEDS::SObject_ptr anObject);

};
#endif
