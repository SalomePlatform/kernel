// Copyright (C) 2021-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_DataServerManager.hxx"
#include "SALOMESDS_Exception.hxx"
#include "SALOME_CPythonHelper.hxx"
#include "SALOME_NamingService_Abstract.hxx"
#include "ArgvKeeper.hxx"
#include "OpUtil.hxx"

#include <string>
#include <sstream>
#include <functional>
#include <vector>

CORBA::ORB_var GetCustomORB()
{
  CORBA::ORB_var orb;
  {
    // by disabling supportCurrent it make the POAManager::hold_requests work !
    std::vector<std::string> args = {"DTC", "-ORBsupportCurrent", "0"};
    SetArgcArgv(args);
    orb=KERNEL::GetRefToORB();
  }
  return orb;
}

int CommonDataScopeServerMain(int argc, char *argv[], CORBA::ORB_var orb, const std::string& scopeName, int isTransac, std::function<SALOME_NamingService_Container_Abstract *(CORBA::ORB_ptr)> nsGenrator)
{
  CORBA::Object_var obj(orb->resolve_initial_references("RootPOA"));
  PortableServer::POA_var poa(PortableServer::POA::_narrow(obj));
  PortableServer::POAManager_var mgr(poa->the_POAManager());
  mgr->activate();
  //
  SALOMESDS::DataScopeKiller *killer(new SALOMESDS::DataScopeKiller(orb));
  SALOME::DataScopeKiller_var killerObj(killer->_this());
  //
  SALOME_CPythonHelper cPyHelper;
  cPyHelper.initializePython(argc,argv);
  //
  SALOMESDS::DataScopeServerBase *server(nullptr);
  if(!isTransac)
    server=new SALOMESDS::DataScopeServer(&cPyHelper,orb,killerObj,scopeName,nsGenrator(orb));
  else
    server=new SALOMESDS::DataScopeServerTransaction(&cPyHelper,orb,killerObj,scopeName,nsGenrator(orb));
  //
  CORBA::PolicyList policies;
  policies.length(3);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0]=PortableServer::ThreadPolicy::_duplicate(threadPol);
  policies[1]=poa->create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
  policies[2]=poa->create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
  PortableServer::POA_var poa2(poa->create_POA("SingleThPOA4SDS",mgr,policies));
  threadPol->destroy();
  server->registerToSalomePiDict();
  //
  server->setPOA(poa2);
  obj=server->activate();
  SALOME::DataScopeServerBase_var serverPtr(SALOME::DataScopeServerBase::_narrow(obj));
  server->registerInNS(serverPtr);
  //
  orb->run();
  delete killer;
  Py_Finalize();
  return 0;
}
