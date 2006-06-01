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
//  File   : SALOMEDSImpl_ChildIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSImpl_CHILDITERATOR_H__
#define __SALOMEDSImpl_CHILDITERATOR_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_ChildIterator, MMgt_TShared )

#include "SALOMEDSImpl_SObject.hxx"
#include "TDF_ChildIterator.hxx"
#include "TDF_Label.hxx"

class SALOMEDSImpl_ChildIterator: public MMgt_TShared 
{
private:
  TDF_ChildIterator _it;
  Handle(SALOMEDSImpl_SObject) _so;

public:

  Standard_EXPORT SALOMEDSImpl_ChildIterator(const Handle(SALOMEDSImpl_SObject)& theSO);
  Standard_EXPORT SALOMEDSImpl_ChildIterator(const TDF_Label& theLabel);
  Standard_EXPORT ~SALOMEDSImpl_ChildIterator() {};

  Standard_EXPORT virtual void Init();
  Standard_EXPORT virtual void Init(const TDF_Label& theLabel);
  Standard_EXPORT virtual void InitEx(bool theAllLevels);
  Standard_EXPORT virtual void InitEx(const TDF_Label& theLabel, bool theAllLevels);
  Standard_EXPORT virtual bool More();
  Standard_EXPORT virtual void Next();
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) Value();
  Standard_EXPORT virtual TDF_Label Label();

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SObject )
};
#endif
