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

//  File   : SALOMEDS_UseCaseBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECaseBuilder_H__
#define __SALOMEDS_USECaseBuilder_H__

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_UseCaseBuilder: public SALOMEDSClient_UseCaseBuilder
{

private: 
  bool _isLocal;
  SALOMEDSImpl_UseCaseBuilder*    _local_impl;
  SALOMEDS::UseCaseBuilder_var    _corba_impl;

public:
  
  SALOMEDS_UseCaseBuilder(SALOMEDSImpl_UseCaseBuilder* theBuilder);
  SALOMEDS_UseCaseBuilder(SALOMEDS::UseCaseBuilder_ptr theBuilder);
  ~SALOMEDS_UseCaseBuilder();

  virtual bool Append(const _PTR(SObject)& theObject);
  virtual bool Remove(const _PTR(SObject)& theObject);
  virtual bool AppendTo(const _PTR(SObject)& theFather, _PTR(SObject) theObject);
  virtual bool InsertBefore(const _PTR(SObject)& theFirst, _PTR(SObject) theNext);
  virtual bool  SetCurrentObject(const _PTR(SObject)& theObject);
  virtual bool SetRootCurrent();
  virtual bool  HasChildren(const _PTR(SObject)& theObject);
  virtual bool SortChildren(const _PTR(SObject)& theObject, bool theAscendingOrder);
  virtual _PTR(SObject) GetFather(const _PTR(SObject)& theObject);
  virtual bool  IsUseCase(const _PTR(SObject)& theObject);
  virtual bool  IsUseCaseNode(const _PTR(SObject)& theObject);
  virtual bool SetName(const std::string& theName);
  virtual _PTR(SObject) GetCurrentObject();
  virtual std::string GetName();
  virtual _PTR(SObject) AddUseCase(const std::string& theName);
  virtual _PTR(UseCaseIterator) GetUseCaseIterator(const _PTR(SObject)& anObject);

};
#endif
