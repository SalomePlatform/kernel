//=============================================================================
// File      : TestMPIComponentEngine.cxx
// Created   : mer jui 4 12:28:26 CEST 2003
// Author    : Bernard SECHER, 2003
// Project   : SALOME
// Copyright : CEA 2003
// $Header$
//=============================================================================

#include "utilities.h"
#include "TestMPIComponentEngine.hxx"
using namespace std;

TestMPIComponentEngine::TestMPIComponentEngine(int nbproc, int numproc,
					       CORBA::ORB_ptr orb,
					       PortableServer::POA_ptr poa,
					       PortableServer::ObjectId * contId, 
					       const char *instanceName, 
					       const char *interfaceName,
					       bool regist) :
  Engines_Component_i(orb, poa, contId, instanceName, interfaceName,true,regist), MPIObject_i(nbproc,numproc)
{
  MESSAGE("activate object")
  _thisObj = this ;
  _id = _poa->activate_object(_thisObj);
}

TestMPIComponentEngine::TestMPIComponentEngine(): Engines_Component_i(), MPIObject_i()
{
}

TestMPIComponentEngine::~TestMPIComponentEngine()
{
}

void TestMPIComponentEngine::Coucou(CORBA::Long L)
{
  BEGIN_OF("[" << _numproc << "] TestMPIComponentEngine::Coucou()");
  if(_numproc==0)
    for(int ip=1;ip<_nbproc;ip++)
      Engines::TestMPIComponent::_narrow((*_tior)[ip])->SPCoucou(L);
  MESSAGE("[" << _numproc << "] TestMPIComponentEngine : L = " << L);
  END_OF("[" << _numproc << "] TestMPIComponentEngine::Coucou()");
}

void TestMPIComponentEngine::SPCoucou(CORBA::Long L)
{
  BEGIN_OF("[" << _numproc << "] TestMPIComponentEngine::SPCoucou()");
  MESSAGE("[" << _numproc << "] TestMPIComponentEngine : L = " << L);
  END_OF("[" << _numproc << "] TestMPIComponentEngine::SPCoucou()");
}

extern "C"
{
  PortableServer::ObjectId * TestMPIComponentEngine_factory(
				 int nbproc, int numproc,
				 CORBA::ORB_ptr orb,
				 PortableServer::POA_ptr poa, 
				 PortableServer::ObjectId * contId,
				 const char *instanceName, 
				 const char *interfaceName)
  {
    bool regist;
    TestMPIComponentEngine * myTestMPIComponent;
    MESSAGE("[" << numproc << "] PortableServer::ObjectId * TestMPIComponentEngine_factory()");
    SCRUTE(interfaceName);
    if(numproc==0)
      regist = true;
    else
      regist = false;

    myTestMPIComponent 
      = new TestMPIComponentEngine(nbproc,numproc,orb, poa, contId, instanceName, interfaceName,regist);
    return myTestMPIComponent->getId() ;
  }
}

