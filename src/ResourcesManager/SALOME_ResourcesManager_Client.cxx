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

#include "SALOME_ResourcesManager_Client.hxx"
#include "SALOME_ResourcesManager.hxx"

using namespace std;

SALOME_ResourcesManager_Client::SALOME_ResourcesManager_Client(SALOME_NamingService *ns)
  : _rm(Engines::ResourcesManager::_nil())
{
  CORBA::Object_var objRM = ns->Resolve(SALOME_ResourcesManager::_ResourcesManagerNameInNS);
  if (!CORBA::is_nil(objRM))
    _rm = Engines::ResourcesManager::_narrow(objRM);
  if (CORBA::is_nil(_rm))
    throw SALOME_Exception("Cannot get SALOME ResourcesManager");
}

SALOME_ResourcesManager_Client::~SALOME_ResourcesManager_Client()
{
  
}

vector<string> SALOME_ResourcesManager_Client::GetFittingResources(const resourceParams& params)
{
  Engines::ResourceParameters_var corba_params = resourceParameters_CPPtoCORBA(params);
  Engines::ResourceList_var corba_rl = _rm->GetFittingResources(corba_params);
  vector<string> res = resourceList_CORBAtoCPP(corba_rl);
  return res;
}

string SALOME_ResourcesManager_Client::Find(const string & policy, const vector<string> & listOfResources)
{
  Engines::ResourceList_var corba_rl = resourceList_CPPtoCORBA(listOfResources);
  CORBA::String_var corba_res = _rm->Find(policy.c_str(), corba_rl);
  string res = corba_res.in();
  return res;
}

ParserResourcesType SALOME_ResourcesManager_Client::GetResourceDefinition(const std::string & name)
{
  Engines::ResourceDefinition_var corba_res = _rm->GetResourceDefinition(name.c_str());
  ParserResourcesType res = resourceDefinition_CORBAtoCPP(corba_res);
  return res;
}

string SALOME_ResourcesManager_Client::getMachineFile(const string & resource_name,
                                                      long nb_procs,
                                                      const string & parallelLib)
{
  CORBA::String_var corba_res = _rm->getMachineFile(resource_name.c_str(),
                                                    nb_procs,
                                                    parallelLib.c_str());
  string res = corba_res.in();
  return res;
}
