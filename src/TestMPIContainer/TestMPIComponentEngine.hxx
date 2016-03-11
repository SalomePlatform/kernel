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

//=============================================================================
// File      : TestMPIComponentEngine.hxx
// Created   : mer jui 4 12:28:30 CEST 2003
// Author    : Bernard SECHER, CEA
// Project   : SALOME
// $Header$
//=============================================================================
//
#ifndef _SALOME_TESTCOMPONENT_I_HXX_
#define _SALOME_TESTCOMPONENT_I_HXX_

#include <iostream>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_TestMPIComponent)
#include "SALOME_Component_i.hxx"
#include "MPIObject_i.hxx"

class TestMPIComponentEngine: 
  public POA_Engines::TestMPIComponent,
  public Engines_Component_i, public MPIObject_i
{
public:
  TestMPIComponentEngine();
  TestMPIComponentEngine(CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         PortableServer::ObjectId * contId, 
                         const char *instanceName, 
                         const char *interfaceName,
                         bool regist);

  virtual ~TestMPIComponentEngine();

  void Coucou(CORBA::Long L);
  void SPCoucou(CORBA::Long L);
  
};

#endif
