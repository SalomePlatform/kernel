//=============================================================================
// File      : SALOME_TestComponent_i.hxx
// Created   : jeu jui 12 12:28:30 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#ifndef _SALOME_TESTCOMPONENT_I_HXX_
#define _SALOME_TESTCOMPONENT_I_HXX_

#include <iostream.h>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_TestComponent)
#include "SALOME_Component_i.hxx"

class Engines_TestComponent_i: 
  public POA_Engines::TestComponent,
  public Engines_Component_i 
{
public:
  Engines_TestComponent_i();
  Engines_TestComponent_i(CORBA::ORB_ptr orb,
			  PortableServer::POA_ptr poa,
			  PortableServer::ObjectId * contId, 
			  const char *instanceName, 
			  const char *interfaceName);

  virtual ~Engines_TestComponent_i();

  char* Coucou(CORBA::Long L);
  
private:

};

#endif
