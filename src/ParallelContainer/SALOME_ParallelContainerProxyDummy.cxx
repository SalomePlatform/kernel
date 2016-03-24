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

//  SALOME ParallelContainerProxyDummy : Proxy of a PaCO++ object using Dummy
//  File   : SALOME_ParallelContainerProxyDummy.cxx
//  Author : André Ribes, EDF
//  Module : SALOME PARALLEL
//
#include <iostream>
#include <string>
#include <stdio.h>

#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif

// PaCO++ include
//#include "SALOME_ComponentPaCO_Engines_Container_server.h"
#include "SALOME_ParallelContainerProxy_i.hxx"
#include <paco_omni.h>
#include <paco_dummy.h>

#include "SALOME_NamingService.hxx"

#include "utilities.h"
#include "Basics_Utils.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOMETraceCollector.hxx"
#include "OpUtil.hxx"

#include "Container_init_python.hxx"

#ifdef DEBUG_PARALLEL
#include <signal.h>

void handler(int t) {
  std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  std::cerr << "SIGSEGV in :" << getpid() << std::endl;
  std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  while (1) {}
}
#endif

int main(int argc, char* argv[])
{
  INFOS("Launching a parallel proxy container");

#ifdef DEBUG_PARALLEL
  signal(SIGSEGV, handler);
#endif
  // Initialise the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  KERNEL_PYTHON::init_python(argc,argv);

  std::string containerName("");
  if(argc > 1) {
    containerName = argv[1];
  }

  int nbnodes = 1;
  if(argc > 4) 
    sscanf(argv[4],"%d",&nbnodes);

  try {  
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    ASSERT(!CORBA::is_nil(obj));
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var pman = root_poa->the_POAManager();

#ifndef WIN32
    // add this container to the kill list
    char aCommand[100];
    sprintf(aCommand, "addToKillList.py %d SALOME_ParallelContainerProxyDummy", getpid());
    system(aCommand);
#endif

    SALOME_NamingService * ns = new SALOME_NamingService(orb);

    // PaCO++ code
    paco_fabrique_manager* pfm = paco_getFabriqueManager();
    pfm->register_com("dummy", new paco_dummy_fabrique());
    pfm->register_thread("omnithread", new paco_omni_fabrique());

    Container_proxy_impl_final * proxy = 
      new Container_proxy_impl_final(orb,
                                     pfm->get_thread("omnithread"),
                                     root_poa,
                                     containerName);
    proxy->setLibCom("dummy", proxy);
    proxy->setLibThread("omnithread");
    // Topo of the parallel object
    PaCO::PacoTopology_t serveur_topo;
    serveur_topo.total = nbnodes;
    proxy->setTopology(serveur_topo);

    //PortableServer::ObjectId_var _id = root_poa->activate_object(proxy);
    //obj = root_poa->id_to_reference(_id);
    obj = proxy->_this();

    // In the NamingService
    std::string hostname = Kernel_Utils::GetHostname();
    Engines::Container_var pCont = Engines::Container::_narrow(obj);
    string _containerName = ns->BuildContainerNameForNS(containerName.c_str(),
                                                        hostname.c_str());
    std::cerr << "---------" << _containerName << "----------" << std::endl;
    ns->Register(pCont, _containerName.c_str());
    pman->activate();
    orb->run();
    PyGILState_Ensure();
    //Delete python container that destroy orb from python (pyCont._orb.destroy())
    Py_Finalize();
    delete ns;
  }
  catch (PaCO::PACO_Exception& e)
  {
    INFOS("Caught PaCO::PACO_Exception");
    std::cerr << e << std::endl;
  }
  catch(CORBA::SystemException&)
  {
    INFOS("Caught CORBA::SystemException.");
  }
  catch(PortableServer::POA::ServantAlreadyActive&)
  {
    INFOS("Caught CORBA::ServantAlreadyActiveException");
  }
  catch(CORBA::Exception&)
  {
    INFOS("Caught CORBA::Exception.");
  }
  catch(std::exception& exc)
  {
    INFOS("Caught std::exception - "<<exc.what()); 
  }
  catch(...)
  {
    INFOS("Caught unknown exception.");
  }
  return 0 ;
}

