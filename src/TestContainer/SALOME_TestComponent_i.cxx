//  SALOME TestContainer : test of container creation and its life cycle
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_TestComponent_i.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#include "utilities.h"
#include "SALOME_TestComponent_i.hxx"
#include <stdio.h>
using namespace std;

Engines_TestComponent_i::Engines_TestComponent_i(CORBA::ORB_ptr orb,
						 PortableServer::POA_ptr poa,
						 PortableServer::ObjectId * contId, 
						 const char *instanceName, 
						 const char *interfaceName) :
  Engines_Component_i(orb, poa, contId, instanceName, interfaceName)
{
  MESSAGE("activate object")
  _thisObj = this ;
  _id = _poa->activate_object(_thisObj);
  //SCRUTE(this)
}

Engines_TestComponent_i::Engines_TestComponent_i()
{
}

Engines_TestComponent_i::~Engines_TestComponent_i()
{
}

char* Engines_TestComponent_i::Coucou(CORBA::Long L)
{
  char s[100];
  sprintf(s, "TestComponent_i : L = %ld", (long) L);

  return CORBA::string_dup(s);
}

extern "C"
{
  PortableServer::ObjectId * SalomeTestComponentEngine_factory(
				 CORBA::ORB_ptr orb,
				 PortableServer::POA_ptr poa, 
				 PortableServer::ObjectId * contId,
				 const char *instanceName, 
				 const char *interfaceName)
  {
    MESSAGE("PortableServer::ObjectId * TestComponent_factory()");
    SCRUTE(interfaceName);
    Engines_TestComponent_i * myTestComponent 
      = new Engines_TestComponent_i(orb, poa, contId, instanceName, interfaceName);
    return myTestComponent->getId() ;
  }
}

