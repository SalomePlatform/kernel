// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

BasicDataServer::BasicDataServer(DataScopeServer *father, const std::string& varName):_father(father),_var_name(varName)
{
}

SALOME::DataScopeServer_ptr BasicDataServer::getMyDataScopeServer()
{
  CORBA::Object_var obj(getPOA()->servant_to_reference(_father));
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

PortableServer::POA_var BasicDataServer::getPOA()
{
  return _father->getPOA();
}
