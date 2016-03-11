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

//  File   : SALOMEDS_AttributeTarget.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeTarget_HeaderFile
#define SALOMEDS_AttributeTarget_HeaderFile

#include <vector>

#include "SALOMEDSClient.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTarget.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeTarget: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTarget
{
public:  
  SALOMEDS_AttributeTarget(SALOMEDSImpl_AttributeTarget* theAttr);
  SALOMEDS_AttributeTarget(SALOMEDS::AttributeTarget_ptr theAttr);
  ~SALOMEDS_AttributeTarget();

  virtual void Add(const _PTR(SObject)& theObject);
  virtual std::vector<_PTR(SObject)> Get();
  virtual void Remove(const _PTR(SObject)& theObject); 
};

#endif
