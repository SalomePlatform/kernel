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

//  SALOME ParallelContainerNodeMpi : Launch mpi PaCO++ object nodes
//  File   : SALOME_ParallelContainerNodeMpi.cxx
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

#include "SALOME_ParallelContainer_i.hxx"

// PaCO++ include
#include <paco_omni.h>
#include <paco_mpi.h>

#include <mpi.h>

#include "SALOME_NamingService.hxx"

#include "utilities.h"
#include "Basics_Utils.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOMETraceCollector.hxx"
#include "OpUtil.hxx"

#include "Container_init_python.hxx"


#ifdef _DEBUG_
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
    exec << "$DEBUGGER SALOME_ParallelContainerNodeMpi " << getpid() << "&";
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
  INFOS("Launching a parallel Mpi container node");

  // MPI Init
  int provided;
  int myid;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE ,&provided);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

#ifdef _DEBUG_
  if(getenv ("DEBUGGER"))
  {
    std::cerr << "Unexpected: unexpected exception !"  << std::endl;
    setsig(SIGSEGV,&Handler);
    set_terminate(&terminateHandler);
    //set_terminate(__gnu_cxx::__verbose_terminate_handler);
    set_unexpected(&unexpectedHandler);
  }
#endif

  std::cerr << "Level MPI_THREAD_SINGLE : " << MPI_THREAD_SINGLE << std::endl;
  std::cerr << "Level MPI_THREAD_SERIALIZED : " << MPI_THREAD_SERIALIZED << std::endl;
  std::cerr << "Level MPI_THREAD_FUNNELED : " << MPI_THREAD_FUNNELED << std::endl;
  std::cerr << "Level MPI_THREAD_MULTIPLE : " << MPI_THREAD_MULTIPLE << std::endl;
  std::cerr << "Level provided : " << provided << std::endl;
  // Initialise the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  KERNEL_PYTHON::init_python(argc,argv);

  // Code pour choisir le reseau infiniband .....
  /*    std::string hostname_temp = GetHostname();
        hostent * t = gethostbyname(hostname_temp.c_str());
        std::cerr << " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA " << t->h_addr << " " << hostname_temp << std::endl;
        std::cerr << t->h_addr << std::endl;
        in_addr * address=(in_addr * ) t->h_addr;
        std::cerr << inet_ntoa(* address) << std::endl;
        std::string ip = inet_ntoa(* address);
        std::cerr << " AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA " << std::endl;
        std::string com = "giop:tcp:" + ip + ":";
        const char* options[][2] = { { "endPoint", com.c_str() }, { 0, 0 } };
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "omniORB4", options);
        */
  std::string containerName("");
  containerName = argv[1];

  std::string proxy_hostname("");
  if(argc > 3) {
    proxy_hostname = argv[3];
  }

  try {  
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var pman = root_poa->the_POAManager();

#ifndef WIN32
    // add this container to the kill list
    char aCommand[100];
    sprintf(aCommand, "addToKillList.py %d SALOME_ParallelContainerNodeMpi", getpid());
    system(aCommand);
#endif

    SALOME_NamingService * ns = new SALOME_NamingService(CORBA::ORB::_duplicate(orb));
    // On récupère le proxy 
    std::string proxyNameInNS = ns->BuildContainerNameForNS(containerName.c_str(), 
                                                       proxy_hostname.c_str());
    obj = ns->Resolve(proxyNameInNS.c_str());
    char * proxy_ior = orb->object_to_string(obj);

    // Node creation
    std::string node_name = containerName + "Node";
    Engines_Parallel_Container_i * servant =  new Engines_Parallel_Container_i(CORBA::ORB::_duplicate(orb), 
                                                                               proxy_ior,
                                                                               myid,
                                                                               root_poa,
                                                                               node_name);
    // PaCO++ init
    paco_fabrique_manager * pfm = paco_getFabriqueManager();
    pfm->register_com("mpi", new paco_mpi_fabrique());
    pfm->register_thread("omni", new paco_omni_fabrique());
    servant->setLibCom("mpi", MPI_COMM_WORLD);
    servant->setLibThread("omni");

    // Activation
    obj = servant->_this();

    // In the NamingService
    std::string hostname = Kernel_Utils::GetHostname();

    int myid;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    char buffer [5];
    snprintf(buffer, 5, "%d", myid);
    node_name = node_name + buffer;

    // We register nodes in two different parts
    // In the real machine name and in the proxy machine
    std::string _containerName = ns->BuildContainerNameForNS(node_name.c_str(),
                                                        hostname.c_str());
    std::string _proxymachine_containerName = ns->BuildContainerNameForNS(node_name.c_str(),
                                                                     proxy_hostname.c_str());
    std::cerr << "Register container node : " << _containerName << std::endl;
    std::cerr << "Register container node : " << _proxymachine_containerName << std::endl;
    ns->Register(obj, _containerName.c_str());
    ns->Register(obj, _proxymachine_containerName.c_str());
    pman->activate();
    orb->run();
    PyGILState_Ensure();
    //Delete python container that destroy orb from python (pyCont._orb.destroy())
    Py_Finalize();
    MPI_Finalize();
    CORBA::string_free(proxy_ior);
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

