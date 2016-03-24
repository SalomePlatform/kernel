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

//  File   : SALOMEDS_SComponentIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_SCOMPONENTITERATOR_H__
#define __SALOMEDS_SCOMPONENTITERATOR_H__

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_SComponentIterator: public SALOMEDSClient_SComponentIterator
{
public:
  SALOMEDS_SComponentIterator(const SALOMEDSImpl_SComponentIterator& theIterator);
  SALOMEDS_SComponentIterator(SALOMEDS::SComponentIterator_ptr theIterator);
  ~SALOMEDS_SComponentIterator();

private:
  bool                                    _isLocal;
  SALOMEDSImpl_SComponentIterator*        _local_impl;
  SALOMEDS::SComponentIterator_var        _corba_impl;

public:
  virtual void Init();
  virtual bool More();
  virtual void Next();
  virtual _PTR(SComponent) Value();  
};
#endif
