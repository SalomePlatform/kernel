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

//  File   : SALOMEDS_SComponentIterator_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_SCOMPONENTITERATOR_I_H__
#define __SALOMEDS_SCOMPONENTITERATOR_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>

//SALOMEDS headers
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"

class SALOMEDS_SComponentIterator_i:public virtual POA_SALOMEDS::SComponentIterator,
                                    public virtual PortableServer::ServantBase,
                                    public virtual SALOME::GenericObj_i 
{

private:

  CORBA::ORB_var                   _orb;
  SALOMEDSImpl_SComponentIterator* _impl;

public:
  
  SALOMEDS_SComponentIterator_i(const SALOMEDSImpl_SComponentIterator& theImpl, CORBA::ORB_ptr);
  
  ~SALOMEDS_SComponentIterator_i();
  
  virtual void Init();
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SComponent_ptr Value();  
};
#endif
