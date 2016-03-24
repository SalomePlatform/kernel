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
// File      : TestMPIComponentEngine.cxx
// Created   : mer jui 4 12:28:26 CEST 2003
// Author    : Bernard SECHER, 2003
// Project   : SALOME
// $Header$
//=============================================================================
//
#include <mpi.h>
#include "utilities.h"
#include "TestMPIComponentEngine.hxx"

TestMPIComponentEngine::TestMPIComponentEngine(CORBA::ORB_ptr orb,
                                               PortableServer::POA_ptr poa,
                                               PortableServer::ObjectId * contId, 
                                               const char *instanceName, 
                                               const char *interfaceName,
                                               bool regist) :
  Engines_Component_i(orb, poa, contId, instanceName, interfaceName,true,regist)
{
  MESSAGE("activate object")
  _thisObj = this ;
  _id = _poa->reference_to_id(_thisObj->_this());
}

TestMPIComponentEngine::TestMPIComponentEngine()
{
}

TestMPIComponentEngine::~TestMPIComponentEngine()
{
}

void TestMPIComponentEngine::Coucou(CORBA::Long L)
{
  if(_numproc==0)
    for(int ip=1;ip<_nbproc;ip++)
      Engines::TestMPIComponent::_narrow((*_tior)[ip])->SPCoucou(L);
  SPCoucou(L);
}

void TestMPIComponentEngine::SPCoucou(CORBA::Long L)
{
  BEGIN_OF("[" << _numproc << "] TestMPIComponentEngine::Coucou()");
  MESSAGE("[" << _numproc << "] TestMPIComponentEngine : L = " << L);
  END_OF("[" << _numproc << "] TestMPIComponentEngine::Coucou()");
  MPI_Barrier(MPI_COMM_WORLD);
}

extern "C"
{
  PortableServer::ObjectId * TestMPIComponentEngine_factory(
                                 CORBA::ORB_ptr orb,
                                 PortableServer::POA_ptr poa, 
                                 PortableServer::ObjectId * contId,
                                 const char *instanceName, 
                                 const char *interfaceName)
  {
    int numproc;
    bool regist;
    TestMPIComponentEngine * myTestMPIComponent;

    MESSAGE("[" << numproc << "] PortableServer::ObjectId * TestMPIComponentEngine_factory()");
    SCRUTE(interfaceName);
    MPI_Comm_rank( MPI_COMM_WORLD, &numproc );
    if(numproc==0)
      regist = true;
    else
      regist = false;

    myTestMPIComponent 
      = new TestMPIComponentEngine(orb, poa, contId, instanceName, interfaceName,regist);
    return myTestMPIComponent->getId() ;
  }
}

