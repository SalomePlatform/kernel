//=============================================================================
// File      : TestMPIComponentEngine.hxx
// Created   : mer jui 4 12:28:30 CEST 2003
// Author    : Bernard SECHER, CEA
// Project   : SALOME
// Copyright : CEA 2003
// $Header$
//=============================================================================

#ifndef _SALOME_TESTCOMPONENT_I_HXX_
#define _SALOME_TESTCOMPONENT_I_HXX_

#include <iostream.h>
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
  TestMPIComponentEngine(int nbproc, int numproc,
			 CORBA::ORB_ptr orb,
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
