//  SALOME ParallelContainerNodeDummy : launcher of a PaCO++ object
//
//  Copyright (C) 2007 OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : SALOME_ParallelContainerNodeDummy.cxx
//  Author : André Ribes, EDF
//  Module : SALOME PARALLEL

#include <iostream>
#include <string>
#include <stdio.h>

#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif

#include "SALOME_ParallelContainer_i.hxx"

#include <paco_omni.h>
#include <paco_dummy.h>

#include "SALOME_NamingService.hxx"

#include "utilities.h"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOMETraceCollector.hxx"
#include "OpUtil.hxx"

using namespace std;

#ifdef _DEBUG_
#include <signal.h>

void handler(int t) {
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cerr << "SIGSEGV in :" << getpid() << endl;
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  while (1) {}
}
#endif

typedef void (*sighandler_t)(int);
sighandler_t setsig(int sig, sighandler_t handler)
{
  struct sigaction context, ocontext;
  context.sa_handler = handler;
  sigemptyset(&context.sa_mask);
  context.sa_flags = 0;
  if (sigaction(sig, &context, &ocontext) == -1)
    return SIG_ERR;
  return ocontext.sa_handler;
}

void AttachDebugger()
{
  if(getenv ("DEBUGGER"))
  {
    std::stringstream exec;
    exec << "$DEBUGGER SALOME_ParallelContainerNodeDummy " << getpid() << "&";
    std::cerr << exec.str() << std::endl;
    system(exec.str().c_str());
    while(1);
  }
}

void Handler(int theSigId)
{
  std::cerr << "SIGSEGV: "  << std::endl;
  AttachDebugger();
  //to exit or not to exit
  exit(1);
}

void terminateHandler(void)
{
  std::cerr << "Terminate: not managed exception !"  << std::endl;
  AttachDebugger();
}

void unexpectedHandler(void)
{
  std::cerr << "Unexpected: unexpected exception !"  << std::endl;
  AttachDebugger();
}

int main(int argc, char* argv[])
{
  INFOS("Launching a parallel container node");

  if(getenv ("DEBUGGER"))
  {
    setsig(SIGSEGV,&Handler);
    set_terminate(&terminateHandler);
    set_unexpected(&unexpectedHandler);
  }

  // Initialise the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

  std::string containerName("");
  if(argc > 1) {
    containerName = argv[1];
  }
  std::string hostname("");
  if(argc > 3) {
    hostname = argv[3];
  }

  try {  
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var pman = root_poa->the_POAManager();

#ifndef WIN32
    // add this container to the kill list
    char aCommand[100];
    sprintf(aCommand, "addToKillList.py %d SALOME_ParallelContainerNode", getpid());
    system(aCommand);
#endif

    SALOME_NamingService * ns = new SALOME_NamingService(CORBA::ORB::_duplicate(orb));
    // Get the proxy
    string proxyNameInNS = ns->BuildContainerNameForNS(containerName.c_str(), 
						       hostname.c_str());
    obj = ns->Resolve(proxyNameInNS.c_str());
    char * proxy_ior = orb->object_to_string(obj);

    // Creating a node
    string node_name = containerName + "Node";
    Engines_Parallel_Container_i * servant = new Engines_Parallel_Container_i(CORBA::ORB::_duplicate(orb), 
									      proxy_ior,
									      0,
									      root_poa,
									      (char*) node_name.c_str(),
									      argc, argv);
    // PaCO++ init
    paco_fabrique_manager * pfm = paco_getFabriqueManager();
    pfm->register_com("dummy", new paco_dummy_fabrique());
    pfm->register_thread("omni", new paco_omni_fabrique());
    servant->setLibCom("dummy", servant);
    servant->setLibThread("omni");

    // Activation
    PortableServer::ObjectId * _id = root_poa->activate_object(servant);
    servant->set_id(_id);
    obj = root_poa->id_to_reference(*_id);

    // In the NamingService
    string hostname = Kernel_Utils::GetHostname();
    int myid = 0;
    char buffer [5];
    snprintf(buffer, 5, "%d", myid);
    node_name = node_name + buffer;
    string _containerName = ns->BuildContainerNameForNS((char*) node_name.c_str(),
							hostname.c_str());
    cerr << "---------" << _containerName << "----------" << endl;
    ns->Register(obj, _containerName.c_str());
    pman->activate();
    orb->run();
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

