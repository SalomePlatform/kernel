#ifndef _SALOME_PCONTAINER_
#define _SALOME_PCONTAINER_ 

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(MPIContainer)
#include "SALOME_Container_i.hxx"
#include "SALOME_NamingService.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"
#include "MPIObject_i.hxx"

class MPIContainer_i : public POA_Engines::MPIContainer,
		     public Engines_Container_i,
		     public MPIObject_i
{

 public:
  // Constructor
  MPIContainer_i( int nbproc, int numproc,
		  CORBA::ORB_ptr orb,
		  PortableServer::POA_ptr poa,
		  char * containerName);
  // Destructor
  ~MPIContainer_i();

  // Load a component
  Engines::Component_ptr load_impl(const char* nameToRegister,
				   const char* componentName);

  // Unload a component
  void remove_impl(Engines::Component_ptr component_i);
  void finalize_removal();

};
#endif
