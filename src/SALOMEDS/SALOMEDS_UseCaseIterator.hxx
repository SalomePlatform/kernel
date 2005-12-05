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
//  File   : SALOMEDS_UseCaseIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECASEITERATOR_H__
#define __SALOMEDS_USECASEITERATOR_H__

#include "SALOMEDSClient_UseCaseIterator.hxx"
#include "SALOMEDSClient_SObject.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_UseCaseIterator: public SALOMEDSClient_UseCaseIterator
{
private:
  bool _isLocal;
  Handle(SALOMEDSImpl_UseCaseIterator) _local_impl;
  SALOMEDS::UseCaseIterator_var        _corba_impl;

public:
  
  SALOMEDS_UseCaseIterator(const Handle(SALOMEDSImpl_UseCaseIterator)& theIterator);
  SALOMEDS_UseCaseIterator(SALOMEDS::UseCaseIterator_ptr theIterator);
  ~SALOMEDS_UseCaseIterator();

  virtual void Init(bool theAllLevels);
  virtual bool More();
  virtual void Next();
  virtual _PTR(SObject) Value();
};

#endif
