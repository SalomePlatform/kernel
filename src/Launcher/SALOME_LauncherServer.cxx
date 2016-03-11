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

#include "SALOME_Launcher.hxx"
#include "SALOMESDS_DataServerManager.hxx"
#include "utilities.h"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <libxml/parser.h>

void AttachDebugger();
void terminateHandler(void);
void unexpectedHandler(void);

void AttachDebugger()
{
#ifndef WIN32
  if(getenv ("DEBUGGER"))
    {
      std::stringstream exec;
      exec << "$DEBUGGER SALOME_LauncherServer " << getpid() << "&";
      MESSAGE ( exec.str() );
      system(exec.str().c_str());
      while(1);
    }
#endif
}

void terminateHandler(void)
{
  MESSAGE ( "Terminate: not managed exception !"  );
  AttachDebugger();
}

void unexpectedHandler(void)
{
  MESSAGE ( "Unexpected: unexpected exception !"  );
  AttachDebugger();
}


int main(int argc, char* argv[])
{
  if(getenv ("DEBUGGER"))
    {
//       setsig(SIGSEGV,&Handler);
      std::set_terminate(&terminateHandler);
      std::set_unexpected(&unexpectedHandler);
    }
  /* Init libxml 
   * To avoid memory leak, need to call xmlInitParser in the main thread
   * and not call xmlCleanupParser later (cause implicit reinitialization in thread)
   */
  xmlInitParser();

  PortableServer::POA_var root_poa;
  PortableServer::POAManager_var pman;
  CORBA::Object_var obj;

  CORBA::ORB_var orb = CORBA::ORB_init( argc , argv ) ;
  //  LocalTraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
  INFOS_COMPILATION;
  BEGIN_OF(argv[0]);
  try{ 
    obj = orb->resolve_initial_references("RootPOA");
    if(!CORBA::is_nil(obj))
      root_poa = PortableServer::POA::_narrow(obj);
    if(!CORBA::is_nil(root_poa))
      {
        pman = root_poa->the_POAManager();
        pman->activate();
      }
  }
  catch(CORBA::COMM_FAILURE&){
    MESSAGE( "Container: CORBA::COMM_FAILURE: Unable to contact the Naming Service" );
  }
  try
    {
      SALOME_Launcher *lServ(new SALOME_Launcher(orb,root_poa));
      lServ->_remove_ref();
      //
      SALOMESDS::DataServerManager *dsm(new SALOMESDS::DataServerManager(argc,argv,orb,root_poa));
      dsm->_remove_ref();
      //
      orb->run();
      orb->destroy();
    }
  catch(CORBA::SystemException&){
    MESSAGE("Caught CORBA::SystemException.");
  }catch(PortableServer::POA::WrongPolicy&){
    MESSAGE("Caught CORBA::WrongPolicyException.");
  }catch(PortableServer::POA::ServantAlreadyActive&){
    MESSAGE("Caught CORBA::ServantAlreadyActiveException");
  }catch(CORBA::Exception&){
    MESSAGE("Caught CORBA::Exception.");
  }catch(std::exception& exc){
    MESSAGE("Caught std::exception - "<<exc.what()); 
  }catch(...){
    MESSAGE("Caught unknown exception.");
  }
  END_OF(argv[0]);
  //  delete myThreadTrace;
}

