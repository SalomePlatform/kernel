// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
// Author : Anthony GEAY (EDF R&D)

#include "SALOMESDS_BasicDataServer.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_Exception.hxx"

#include <sstream>

using namespace SALOMESDS;

BasicDataServer::BasicDataServer(DataScopeServerBase *father, const std::string& varName):_father(father),_var_name(varName)
{
  /*PortableServer::POA_var poa(_father->getPOA());
  PortableServer::POAManager_var pman(poa->the_POAManager());
  CORBA::Object_var obj(_father->getORB()->resolve_initial_references("RootPOA"));
  PortableServer::POA_var rootPOA(PortableServer::POA::_narrow(obj));
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(rootPOA->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0]=PortableServer::ThreadPolicy::_duplicate(threadPol);
  std::ostringstream poaName; poaName << "POA_" <<  POA_CNT++ << "_" << _var_name;
  _poa=rootPOA->create_POA(poaName.str().c_str(),pman,policies);
  threadPol->destroy();*/
}

SALOME::DataScopeServer_ptr BasicDataServer::getMyDataScopeServer()
{
  CORBA::Object_var obj(_father->getPOA()->servant_to_reference(_father));
  return SALOME::DataScopeServer::_narrow(obj);
}

/*!
 * Called remotely -> to protect against throw
 */
char *BasicDataServer::getVarName()
{
  return CORBA::string_dup(_var_name.c_str());
}

char *BasicDataServer::getScopeName()
{
  return _father->getScopeName();
}

void BasicDataServer::Register()
{
  incrRef();
}

void BasicDataServer::UnRegister()
{
  decrRef();
}

void BasicDataServer::Destroy()
{
  enforcedRelease();
}

CORBA::Long BasicDataServer::getRCValue()
{
  return getCounter();
}

PortableServer::POA_var BasicDataServer::getPOA() const
{
  return _father->getPOA();
}
