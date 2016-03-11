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

//  File   : SALOMEDS_SComponent_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_SCOMPONENT_I_H__
#define __SALOMEDS_SCOMPONENT_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

//SALOMEDS headers
#include "SALOMEDS_SObject_i.hxx"

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

class Standard_EXPORT SALOMEDS_SComponent_i: public POA_SALOMEDS::SComponent,
                             public SALOMEDS_SObject_i
{

public:

  static SALOMEDS::SComponent_ptr New(const SALOMEDSImpl_SComponent&, CORBA::ORB_ptr); 
  
  SALOMEDS_SComponent_i(const SALOMEDSImpl_SComponent&, CORBA::ORB_ptr);
  
  virtual ~SALOMEDS_SComponent_i();

  virtual char* ComponentDataType();
  virtual CORBA::Boolean ComponentIOR(CORBA::String_out theID);

};
#endif
