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

//  File   : SALOMEDSImpl_Callback.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSIMPL_CALLBACK_H__
#define __SALOMEDSIMPL_CALLBACK_H__

#include "SALOMEDSImpl_Defines.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_Callback
{
private:
  SALOMEDSImpl_UseCaseBuilder* _builder;

public:

  SALOMEDSImpl_Callback(SALOMEDSImpl_UseCaseBuilder* builder) 
  {
    _builder = builder;
  }

  virtual void OnAddSObject(const SALOMEDSImpl_SObject& theObject) 
  {
    if(_builder != NULL && theObject) _builder->Append(theObject);
  }

  virtual void OnRemoveSObject(const SALOMEDSImpl_SObject& theObject) 
  {
    if(_builder != NULL && theObject) _builder->Remove(theObject);
  }

};

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AbstractCallback
{
public:
  virtual bool addSO_Notification(const SALOMEDSImpl_SObject& theSObject){return false;};
  virtual bool removeSO_Notification(const SALOMEDSImpl_SObject& theSObject){return false;};
  virtual bool modifySO_Notification(const SALOMEDSImpl_SObject& theSObject, int reason ){return false;};
  virtual bool modifyNB_Notification(const char* theVarName){return false;};
  virtual void RegisterGenObj  (const std::string& theIOR) {}
  virtual void UnRegisterGenObj(const std::string& theIOR) {}
};
#endif
