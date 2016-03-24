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

//  SALOME Container : implementation of container and engine for Kernel
//  File   : SALOME_Container.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$
//
#ifdef _MPI_SEQ_CONTAINER_
  #ifdef HAVE_MPI2
#include <mpi.h>
  #endif
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>
#ifndef WIN32
# include <sys/time.h>
# include <dlfcn.h>
#endif


#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif
#include "SALOME_Container_i.hxx"
#include "utilities.h"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"

#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

#include "Container_init_python.hxx"

extern "C" void HandleServerSideSignals(CORBA::ORB_ptr theORB);

#include <stdexcept>
#include <signal.h>
#include <sys/types.h>
#ifndef WIN32
# include <sys/wait.h>
#endif

void AttachDebugger();
void Handler(int);
void terminateHandler();
void unexpectedHandler();

#ifndef WIN32
void (* setsig(int, void (*)(int)))(int);

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
#endif //WIN32

void AttachDebugger()
{
#ifndef WIN32
  if(getenv ("DEBUGGER"))
    {
      std::stringstream exec;
      exec << "$DEBUGGER SALOME_Container " << getpid() << "&";
      std::cerr << exec.str() << std::endl;
      system(exec.str().c_str());
      while(1);
    }
#endif
}

void Handler(int theSigId)
{
  std::cerr << "Signal= "<< theSigId  << std::endl;
  AttachDebugger();
  //to exit or not to exit
  _exit(1);
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
#ifdef _MPI_SEQ_CONTAINER_
  #ifdef HAVE_MPI2
  MPI_Init(&argc,&argv);
  #endif
#endif  

#ifndef WIN32
  if(getenv ("DEBUGGER"))
    {
      setsig(SIGSEGV,&Handler);
      setsig(SIGFPE,&Handler);
      std::set_terminate(&terminateHandler);
      std::set_unexpected(&unexpectedHandler);
    }
#endif

  // Initialise the ORB.
  //SRN: BugID: IPAL9541, it's necessary to set a size of one message to be at least 100Mb
  //CORBA::ORB_var orb = CORBA::ORB_init( argc , argv ) ;
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  CORBA::ORB_ptr orb = init(argc , argv ) ;

  //  LocalTraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
  INFOS_COMPILATION;
  BEGIN_OF(argv[0]);

  ASSERT(argc > 1);
  SCRUTE(argv[1]);

  KERNEL_PYTHON::init_python(argc,argv);
    
  char *containerName = (char *)"";
  if(argc > 1)
    {
      containerName = argv[1] ;
    }

  try
    {  
      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      ASSERT(!CORBA::is_nil(obj));
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);

      PortableServer::POAManager_var pman = root_poa->the_POAManager();

      // add new container to the kill list
#ifndef WIN32
      std::stringstream aCommand ;
      aCommand << "addToKillList.py " << getpid() << " SALOME_Container" << std::ends ;
      system(aCommand.str().c_str());
#endif
      
      new Engines_Container_i(orb, root_poa, containerName , argc , argv );
      
      pman->activate();
      
#ifdef CHECKTIME
      Utils_Timer timer;
      timer.Start();
      timer.Stop();
      timer.ShowAbsolute();
#endif

      HandleServerSideSignals(orb);

//#define MEMORYLEAKS
#ifdef MEMORYLEAKS
        PyGILState_Ensure();
        //Destroy orb from python (for chasing memory leaks)
        PyRun_SimpleString("from omniORB import CORBA");
        PyRun_SimpleString("orb=CORBA.ORB_init([''], CORBA.ORB_ID)");
        PyRun_SimpleString("orb.destroy()");
        Py_Finalize();
#endif
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

#ifdef _MPI_SEQ_CONTAINER_
  #ifdef HAVE_MPI2
  MPI_Finalize();
  #endif
#endif  

  return 0 ;
}

