// Copyright (C) 2021-2023  CEA, EDF
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

#include "KernelLauncher.hxx"

#include "SALOME_Launcher.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_Fake_NamingService.hxx"
#include "SALOME_KernelServices.hxx"
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_ExternalServerLauncher.hxx"
#include "SALOME_LogManager.hxx"
#include "SALOME_CPythonHelper.hxx"

#include <cstring>
#include <sstream>

static Engines::LogManager_var LogManagerInstanceSingleton;

std::string RetrieveInternalInstanceOfLocalCppResourcesManager()
{
  SALOME_Launcher *launcher = KERNEL::getLauncherSA();
  SALOME_ResourcesManager *rm(launcher->getResourcesManager());
  if(rm)
  {
    std::shared_ptr<ResourcesManager_cpp> *ret1(new std::shared_ptr<ResourcesManager_cpp>(rm->GetImpl()));
    std::ostringstream oss; oss << ret1;
    return oss.str();
  }
  return std::string();
}

std::string GetContainerManagerInstance()
{
  SALOME_Launcher *launcher = KERNEL::getLauncherSA();
  SALOME_Fake_NamingService ns;
  CORBA::Object_var cm = ns.Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  CORBA::ORB_ptr orb = KERNEL::getORB();
  CORBA::String_var ior = orb->object_to_string(cm);
  return std::string(ior.in());
}

std::string GetResourcesManagerInstance()
{
  SALOME_Launcher *launcher = KERNEL::getLauncherSA();
  SALOME_Fake_NamingService ns;
  CORBA::Object_var cm = ns.Resolve(SALOME_ResourcesManager::_ResourcesManagerNameInNS);
  CORBA::ORB_ptr orb = KERNEL::getORB();
  CORBA::String_var ior = orb->object_to_string(cm);
  return std::string(ior.in());
}

std::string GetExternalServerInstance()
{
  CORBA::ORB_ptr orb = KERNEL::getORB();
  CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
  //
  PortableServer::POA_var safePOA = root_poa->find_POA("SingleThreadPOA",true);
  //
  SALOME_CPythonHelper *cPyh(SALOME_CPythonHelper::Singleton());
  SALOME_Fake_NamingService *ns = new SALOME_Fake_NamingService;
  SALOME_ExternalServerLauncher *esm(new SALOME_ExternalServerLauncher(cPyh,orb,safePOA,ns));
  esm->_remove_ref();
  //
  CORBA::Object_var esmPtr = safePOA->servant_to_reference(esm);
  SALOME::ExternalServerLauncher_var esmCPtr = SALOME::ExternalServerLauncher::_narrow(esmPtr);
  //
  CORBA::String_var ior = orb->object_to_string(esmCPtr);
  return std::string(ior.in());
}

std::string GetLogManagerInstance()
{
  CORBA::ORB_ptr orb = KERNEL::getORB();
  if( CORBA::is_nil(LogManagerInstanceSingleton) )
  {
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    //
    CORBA::PolicyList policies;
    policies.length(1);
    PortableServer::POAManager_var pman = root_poa->the_POAManager();
    PortableServer::ThreadPolicy_var threadPol(root_poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
    policies[0] = PortableServer::ThreadPolicy::_duplicate(threadPol);
    PortableServer::POA_var safePOA = root_poa->create_POA("SingleThreadPOAForLogManager",pman,policies);
    threadPol->destroy();
    //
    SALOME_CPythonHelper *cPyh(SALOME_CPythonHelper::Singleton());
    SALOME_Fake_NamingService *ns = new SALOME_Fake_NamingService;
    SALOME_LogManager *esm(new SALOME_LogManager(orb,safePOA,ns));
    esm->_remove_ref();
    //
    CORBA::Object_var esmPtr = safePOA->servant_to_reference(esm);
    LogManagerInstanceSingleton = Engines::LogManager::_narrow(esmPtr);
  }
  //
  CORBA::String_var ior = orb->object_to_string(LogManagerInstanceSingleton);
  return std::string(ior.in());
}
