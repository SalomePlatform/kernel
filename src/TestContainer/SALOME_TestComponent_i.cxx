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

//  SALOME TestContainer : test of container creation and its life cycle
//  File   : SALOME_TestComponent_i.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$
//

#include "utilities.h"
#include "SALOME_TestComponent_i.hxx"
#include <stdio.h>
#include <cstdlib>
#include <map>

Engines_TestComponent_i::Engines_TestComponent_i(CORBA::ORB_ptr orb,
                                                 PortableServer::POA_ptr poa,
                                                 PortableServer::ObjectId * contId, 
                                                 const char *instanceName, 
                                                 const char *interfaceName) :
  Engines_Component_i(orb, poa, contId, instanceName, interfaceName)
{
  MESSAGE("activate object");
  _thisObj = this ;
  _id = _poa->activate_object(_thisObj);
  //does not work with 4.0.6 (too bad)
  //SCRUTE(_pd_refCount);
  //SCRUTE(_refcount_value());
}

Engines_TestComponent_i::Engines_TestComponent_i()
{
}

Engines_TestComponent_i::~Engines_TestComponent_i()
{
  MESSAGE("~Engines_TestComponent_i()");
}

char* Engines_TestComponent_i::Coucou(CORBA::Long L)
{
  char s[100];
  sprintf(s, "TestComponent_i : L = %ld", (long) L);
  //does not work with 4.0.6 (too bad)
  //SCRUTE(_pd_refCount);
  //SCRUTE(_refcount_value());

  return CORBA::string_dup(s);
}

void Engines_TestComponent_i::Setenv()
{
  // bool overwrite = true;
  std::map<std::string,CORBA::Any>::iterator it;
  MESSAGE("set environment associated with keys in map _fieldsDict");
  for (it = _fieldsDict.begin(); it != _fieldsDict.end(); it++)
    {
      std::string cle((*it).first);
      if ((*it).second.type()->kind() == CORBA::tk_string)
        {
          const char* value;
          (*it).second >>= value;
          //CCRT porting : setenv not defined in stdlib.h
          std::string s(cle);
          s+='=';
          s+=value;
          //char* cast because 1st arg of linux putenv function is not a const char* !!!
          putenv((char *)s.c_str());
          //End of CCRT porting
          //int ret = setenv(cle.c_str(), value, overwrite);
          MESSAGE("--- setenv: "<<cle<<" = "<< value);
        }
    }
  MESSAGE("read environment associated with keys in map _fieldsDict");
  for (it = _fieldsDict.begin(); it != _fieldsDict.end(); it++)
    {
      std::string cle((*it).first);
      char* valenv= getenv(cle.c_str());
      if (valenv)
        MESSAGE("--- getenv: "<<cle<<" = "<< valenv);
    }
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

