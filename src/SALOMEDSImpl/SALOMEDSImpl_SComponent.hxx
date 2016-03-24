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

//  File   : SALOMEDSImpl_SComponent.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSIMPL_SCOMPONENT_H__
#define __SALOMEDSIMPL_SCOMPONENT_H__

//SALOMEDSImpl headers
#include "SALOMEDSImpl_Defines.hxx"
#include "SALOMEDSImpl_SObject.hxx"

// std C++ headers
#include <iostream>
#include <string>

#include "DF_Label.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include <stdio.h>

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_SComponent :
  public virtual SALOMEDSImpl_SObject
{
public:

  SALOMEDSImpl_SComponent();
  SALOMEDSImpl_SComponent(const SALOMEDSImpl_SComponent& theSCO);
  SALOMEDSImpl_SComponent(const DF_Label& lab);

  ~SALOMEDSImpl_SComponent();

  virtual std::string ComponentDataType();
  virtual bool ComponentIOR(std::string& theID);

  static bool IsA(const DF_Label& theLabel);

  SALOMEDSImpl_SComponent* GetPersistentCopy() const;

};
#endif
