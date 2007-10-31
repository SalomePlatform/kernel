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


#include "SALOMEDSImpl_SObject.hxx"
#include "DF_ChildIterator.hxx"
#include "DF_Label.hxx"

class SALOMEDSImpl_ChildIterator
{
private:
  DF_ChildIterator _it;
  SALOMEDSImpl_SObject _so;

public:

  Standard_EXPORT SALOMEDSImpl_ChildIterator() {}    
  Standard_EXPORT SALOMEDSImpl_ChildIterator(const SALOMEDSImpl_SObject& theSO);
  Standard_EXPORT SALOMEDSImpl_ChildIterator(const DF_Label& theLabel);
  Standard_EXPORT ~SALOMEDSImpl_ChildIterator() {};

  Standard_EXPORT virtual void Init();
  Standard_EXPORT virtual void Init(const DF_Label& theLabel);
  Standard_EXPORT virtual void InitEx(bool theAllLevels);
  Standard_EXPORT virtual void InitEx(const DF_Label& theLabel, bool theAllLevels);
  Standard_EXPORT virtual bool More();
  Standard_EXPORT virtual void Next();
  Standard_EXPORT virtual SALOMEDSImpl_SObject Value();
  Standard_EXPORT virtual DF_Label Label();

  Standard_EXPORT SALOMEDSImpl_ChildIterator* GetPersistentCopy() const;
};
#endif
