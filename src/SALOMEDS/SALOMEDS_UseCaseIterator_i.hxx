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

//  File   : SALOMEDS_UseCaseIterator_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_USECASEITERATOR_I_H__
#define __SALOMEDS_USECASEITERATOR_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>

#include "SALOMEDSImpl_UseCaseIterator.hxx"

class SALOMEDS_UseCaseIterator_i: public virtual POA_SALOMEDS::UseCaseIterator,
                                  public virtual PortableServer::ServantBase, 
                                  public virtual SALOME::GenericObj_i
{
private:
  CORBA::ORB_var                        _orb;
  SALOMEDSImpl_UseCaseIterator*         _impl;

public:

  //! standard constructor  
  SALOMEDS_UseCaseIterator_i(const SALOMEDSImpl_UseCaseIterator& theImpl, 
                             CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_UseCaseIterator_i();
  
  virtual void Init(CORBA::Boolean);
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SObject_ptr Value();
};
#endif
