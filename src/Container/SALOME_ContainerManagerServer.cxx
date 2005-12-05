// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
#include "SALOME_ContainerManager.hxx"
#include "SALOMETraceCollector.hxx"
#include "utilities.h"

int main(int argc, char* argv[])
{
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
	  pman = root_poa->the_POAManager();
      }
  catch(CORBA::COMM_FAILURE&){
    MESSAGE( "Container: CORBA::COMM_FAILURE: Unable to contact the Naming Service" );
  }
  try{
    SALOME_ContainerManager *cmServ=new SALOME_ContainerManager(orb);
    pman->activate();
    orb->run();
  }catch(CORBA::SystemException&){
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

