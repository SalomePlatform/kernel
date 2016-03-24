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

//  File   : SALOMEDSClient_UseCaseBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSClient_USECaseBuilder_H__
#define __SALOMEDSClient_USECaseBuilder_H__

#include <string>
#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_UseCaseIterator.hxx"
#include "SALOMEDSClient_SObject.hxx"

class SALOMEDSClient_UseCaseBuilder
{
public:
  virtual ~SALOMEDSClient_UseCaseBuilder() {}
  
  virtual bool Append(const _PTR(SObject)& theObject) = 0;
  virtual bool Remove(const _PTR(SObject)& theObject) = 0;
  virtual bool AppendTo(const _PTR(SObject)& theFather, _PTR(SObject) theObject) = 0;
  virtual bool InsertBefore(const _PTR(SObject)& theFirst, _PTR(SObject) theNext) = 0;
  virtual bool SetCurrentObject(const _PTR(SObject)& theObject) = 0;
  virtual bool SetRootCurrent() = 0;
  virtual bool HasChildren(const _PTR(SObject)& theObject) = 0;
  virtual bool SortChildren(const _PTR(SObject)& theObject, bool theAscendingOrder) = 0;
  virtual _PTR(SObject) GetFather(const _PTR(SObject)& theObject) = 0;
  virtual bool IsUseCase(const _PTR(SObject)& theObject) = 0;
  virtual bool IsUseCaseNode(const _PTR(SObject)& theObject) = 0;
  virtual bool SetName(const std::string& theName) = 0;
  virtual _PTR(SObject) GetCurrentObject() = 0;
  virtual std::string GetName() = 0;
  virtual _PTR(SObject) AddUseCase(const std::string& theName) = 0;
  virtual _PTR(UseCaseIterator) GetUseCaseIterator(const _PTR(SObject)& anObject) = 0;

};


#endif
