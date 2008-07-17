//  SALOME Container : implementation of container and engine for Kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_Container.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifdef HAVE_MPI2
#include <mpi.h>
#endif

#include <iostream>
#include <strstream>
#include <string>
#include <stdio.h>
#include <time.h>
#ifndef WNT
# include <sys/time.h>
# include <dlfcn.h>
#endif


#ifndef WNT
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

using namespace std;

extern "C" void HandleServerSideSignals(CORBA::ORB_ptr theORB);

#include <stdexcept>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

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
      exec << "$DEBUGGER SALOME_Container " << getpid() << "&";
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
#ifdef HAVE_MPI2
  MPI_Init(&argc,&argv);
#endif

  if(getenv ("DEBUGGER"))
    {
      setsig(SIGSEGV,&Handler);
      set_terminate(&terminateHandler);
      set_unexpected(&unexpectedHandler);
    }

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
  bool isSupervContainer = false;
  if (strcmp(argv[1],"SuperVisionContainer") == 0) isSupervContainer = true;

  if (!isSupervContainer)
    {
      // int _argc = 1;
      // char* _argv[] = {""};
      KERNEL_PYTHON::init_python(argc,argv);
    }
  else
    {
      Py_Initialize() ;
      PySys_SetArgv( argc , argv ) ;
    }
    
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
#ifndef WNT
      stringstream aCommand ;
      aCommand << "addToKillList.py " << getpid() << " SALOME_Container" << ends ;
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

      if (!isSupervContainer)
      {
        PyGILState_Ensure();
        //Delete python container that destroy orb from python (pyCont._orb.destroy())
        Py_Finalize();
      }
      else
      {
        orb->destroy();
      }
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

#ifdef HAVE_MPI2
  MPI_Finalize();
#endif

  //END_OF(argv[0]);
  //LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  //bp1->deleteInstance(bp1);
  return 0 ;
}

