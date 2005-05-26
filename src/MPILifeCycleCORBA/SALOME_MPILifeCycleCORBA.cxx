//=============================================================================
// File      : SALOME_MPILifeCycleCORBA.cxx
// Created   : mar jui 03 14:55:50 CEST 2003
// Author    : Bernard SECHER CEA
// Project   : SALOME
// Copyright : CEA 2003
// $Header$
//=============================================================================

#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>

#include "OpUtil.hxx"
#include "utilities.h"

#include <ServiceUnreachable.hxx>

#include "SALOME_MPILifeCycleCORBA.hxx"
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include "SALOME_NamingService.hxx"
using namespace std;

SALOME_MPILifeCycleCORBA::SALOME_MPILifeCycleCORBA() : 
  SALOME_LifeCycleCORBA()
{
  _MPIFactoryServer = NULL;
}

SALOME_MPILifeCycleCORBA::SALOME_MPILifeCycleCORBA(SALOME_NamingService *ns) :
  SALOME_LifeCycleCORBA(ns)
{
  _MPIFactoryServer = NULL;
}

SALOME_MPILifeCycleCORBA::~SALOME_MPILifeCycleCORBA()
{
}

Engines::MPIContainer_var SALOME_MPILifeCycleCORBA::FindOrStartMPIContainer(
                                              const std::string theComputer ,
                                              const std::string theMPIContainerRoot,
					      const int nbproc)
{
  char nbp[1024];

  sprintf(nbp,"_%d",nbproc);
  std::string theMPIContainer = theMPIContainerRoot + nbp; 
  std::string aComputerContainer = theComputer + "/" + theMPIContainer;

  SCRUTE( aComputerContainer ) ;
  SCRUTE( theComputer ) ;
  SCRUTE( theMPIContainer ) ;

  // On recherche si le containe rest deja lance
  Engines::MPIContainer_var aMPIContainer = Engines::MPIContainer::_narrow(FindContainer(aComputerContainer.c_str()));

  //On a trouve le container: on renvoie une poigne dessus
  if ( !CORBA::is_nil( aMPIContainer ) ) {
    MESSAGE("MPIContainer " << aComputerContainer << " found!!!");
    return aMPIContainer ;
  }
  // On a pas trouve le container
  else {
    MESSAGE("MPIContainer " << aComputerContainer << " not found!!!");
    // On recherche un container generique
    bool pyCont = false ;
    int len = theMPIContainer.length() ;
    if ( !strcmp( &theMPIContainerRoot.c_str()[len-2] , "Py" ) ) {
      pyCont = true ;
    }
    std::string MPIFactoryServer = theComputer ;
    if ( pyCont ) {
      MPIFactoryServer += "/MPIFactoryServerPy" ;
    }
    else {
      MPIFactoryServer += "/MPIFactoryServer" ;
    }
    MPIFactoryServer += nbp;
    Engines::MPIContainer_var aMPIFactoryServer = Engines::MPIContainer::_narrow(FindContainer( MPIFactoryServer.c_str()));

    // On n'a pas trouve le container generique: on lance le container demande
    if ( CORBA::is_nil( aMPIFactoryServer ) ) {
// rsh -n ikkyo /export/home/rahuel/SALOME_ROOT/bin/runSession SALOME_Container -ORBInitRef NameService=corbaname::dm2s0017:1515 &
      std::string rsh( "" ) ;
      if ( theComputer!= GetHostname() ) {
        rsh += "rsh -n " ;
        rsh += theComputer ;
        rsh += " " ;
      }
      std::string path = ComputerPath( theComputer.c_str() ) ;
      SCRUTE( path ) ;
      //      rsh += "runSession " ;
      if ( pyCont ) {
	MESSAGE("MPI python container not implemented");
	return Engines::MPIContainer::_nil();
//         rsh += "SALOME_MPIContainerPy.py " ;
//         rsh += "MPIFactoryServerPy -" ;
      }
      else {
	sprintf(nbp,"mpirun -np %d %sSALOME_MPIContainer ",nbproc,path.c_str());
        rsh += nbp;
        rsh += theMPIContainer +" -" ;
      }
      std::string omniORBcfg( getenv( "OMNIORB_CONFIG" ) ) ;
      ifstream omniORBfile( omniORBcfg.c_str() ) ;
      char ORBInitRef[12] ;
      char nameservice[132] ;
      omniORBfile >> ORBInitRef ;
      rsh += ORBInitRef ;
      rsh += " " ;
      omniORBfile >> nameservice ;
      omniORBfile.close() ;
      char * bsn = strchr( nameservice , '\n' ) ;
      if ( bsn ) {
        bsn[ 0 ] = '\0' ;
      }
      rsh += nameservice ;
      if ( pyCont ) {
        rsh += " > /tmp/MPIFactoryServerPy_" ;
      }
      else {
        rsh += " > /tmp/MPIFactoryServer_" ;
      }
      sprintf(nbp,"%d_",nbproc);
      rsh += nbp;
      rsh += theComputer ;
      rsh += ".log 2>&1 &" ;
      SCRUTE( rsh );
      int status = system( rsh.c_str() ) ;
      if (status == -1) {
        INFOS("SALOME_MPILifeCycleCORBA::FindOrStartMPIContainer rsh failed (system command status -1)") ;
      }
      else if (status == 217) {
        INFOS("SALOME_MPILifeCycleCORBA::FindOrStartContainer rsh failed (system command status 217)") ;
      }
      else {
        int count = 21 ;
        while ( CORBA::is_nil( aMPIFactoryServer ) && count ) {
          sleep( 1 ) ;
          count-- ;
          if ( count != 10 )
            MESSAGE( count << ". Waiting for FactoryServer on " << theComputer)
          aMPIFactoryServer = Engines::MPIContainer::_narrow(FindContainer( MPIFactoryServer.c_str()));
	}
        if ( CORBA::is_nil( aMPIFactoryServer ) ) {
          INFOS("SALOME_MPILifeCycleCORBA::FindOrStartMPIContainer rsh failed") ;
	}
        else if ( strcmp( theComputer.c_str() , GetHostname().c_str() ) ) {
          _MPIFactoryServer = aMPIFactoryServer ;
	}
      }
    }
    // On a trouve le container generique distant: on se sert de lui
    // pour lancer un nouveau container MPI
    // a revoir...
    if ( !CORBA::is_nil( aMPIFactoryServer ) ) {
      if ( strcmp( theMPIContainer.c_str() , "MPIFactoryServer" ) ||
           strcmp( theMPIContainer.c_str() , "MPIFactoryServerPy" ) ) {
        MESSAGE("MPI Container not found ! trying to start " << aComputerContainer);
        Engines::MPIContainer_var myMPIContainer = aMPIFactoryServer->start_MPIimpl( theMPIContainer.c_str(), nbproc ) ;
        if ( !CORBA::is_nil( myMPIContainer ) ) {
          MESSAGE("MPIContainer " << aComputerContainer << " started");
          return myMPIContainer ;
        }
        else {
          MESSAGE("MPIContainer " << aComputerContainer << " NOT started");
        }
      }
      else {
        MESSAGE("MPIContainer " << aComputerContainer << " started");
        return aMPIFactoryServer ;
      }
    }
  }
  return Engines::MPIContainer::_nil();
}

// Engines::Component_var SALOME_MPILifeCycleCORBA::FindOrLoad_MPIComponent
//                                    (const char *MPIcontainerName,
// 				    const char *MPIcomponentName,
// 				    const char *implementation,
// 				    const int nbproc)
// {
//   BEGIN_OF("FindOrLoad_MPIComponent(1)");
//   ASSERT(_NS != NULL);
//   string theComputer ;
//   string theMPIContainer ;
//   string theComputerContainer = ContainerName( MPIcontainerName ,
//                                                &theComputer ,
//                                                &theMPIContainer ) ;
//   Engines::MPIContainer_var cont = FindOrStartMPIContainer( theComputerContainer ,
// 							    theComputer ,
// 							    theMPIContainer,
// 							    nbproc) ;
// //  ASSERT(!CORBA::is_nil(cont));

//   string path( theComputerContainer );
//   path = path + "/";
//   path = path + MPIcomponentName;
//   SCRUTE(path);
//   try
//     {
//       CORBA::Object_var obj = _NS->Resolve(path.c_str());
//       if (CORBA::is_nil(obj))
// 	{
// 	  MESSAGE("MPIComponent not found ! trying to load " << path);
// 	  Engines::Component_var compo 
// 	    = cont->load_impl(MPIcomponentName, implementation);
// //	  ASSERT(!CORBA::is_nil(compo));
// 	  MESSAGE("MPIComponent launched !" << path);
// 	  return compo;
// 	}
//       else
// 	{
// 	  MESSAGE("MPIComponent found !" << path);
// 	  Engines::Component_var compo = Engines::Component::_narrow(obj);
// //	  ASSERT(!CORBA::is_nil(compo));
// 	  try
// 	    {
// 	      compo->ping(); 
// 	    }
// 	  catch (CORBA::COMM_FAILURE&)
// 	    {
// 	      INFOS("Caught CORBA::SystemException CommFailure. Engine "
// 		    << path << "does not respond" );
// 	    }
// 	  return compo;
// 	}
//     }
//   catch (ServiceUnreachable&)
//     {
//       INFOS("Caught exception: Naming Service Unreachable");
//     }
//   catch (...)
//     {
//       INFOS("Caught unknown exception.");
//     }
//   return Engines::Component::_nil();
// }

Engines::Component_var SALOME_MPILifeCycleCORBA::FindOrLoad_MPIComponent
                                  (const char *MPIcontainerName,
				   const char *MPIcomponentName,
				   const int nbproc)
{

  char nbp[1024];

  sprintf(nbp,"_%d",nbproc);
//  BEGIN_OF("FindOrLoad_Component(2)");
  ASSERT(_NS != NULL);
  string theComputer ;
  string theMPIContainerRoot ;
  string theMPIContainer;
  string theComputerContainer = ContainerName( MPIcontainerName ,
                                               &theComputer ,
                                               &theMPIContainerRoot ) ;
  theMPIContainer = theMPIContainerRoot + nbp;
  Engines::MPIContainer_var cont = FindOrStartMPIContainer( theComputer ,
							    theMPIContainerRoot,
							    nbproc ) ;

  if ( CORBA::is_nil( cont ) ) {
    MESSAGE("MPIContainer not found ! " << theComputerContainer );
    return Engines::Component::_nil();
  }

//  char * machine = cont->machineName() ;
  const char * machine = theComputer.c_str() ;

  string path( theComputerContainer );
  path += nbp;
  path += "/";
  path += MPIcomponentName;
  SCRUTE(path);

  try {
    CORBA::Object_var obj = _NS->Resolve(path.c_str());
    if ( CORBA::is_nil( obj ) ) {
      MESSAGE("MPIComponent not found ! trying to load " << path);
      CORBA::Object_var obj2 = _NS->Resolve("/Kernel/ModulCatalog");
      SALOME_ModuleCatalog::ModuleCatalog_var Catalog = 
	SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj2);

      SALOME_ModuleCatalog::Acomponent_ptr compoInfo = 
	Catalog->GetComponent(MPIcomponentName);
      if (CORBA::is_nil (compoInfo)) 
	{
	  INFOS("Catalog Error : Component not found in the catalog")
	    return Engines::Component::_nil();
//		exit (-1);
	}
      
      string  path;
      try
	{
	  path = compoInfo->GetPathPrefix( machine ) ;
	  path += "/" ;
	}
      catch (SALOME_ModuleCatalog::NotFound&)
	{
	  MESSAGE("GetPathPrefix(" << machine << ") not found!"
		  << "trying localhost");
	  try {
	    path = compoInfo->GetPathPrefix("localhost") ;
	    path += "/" ;
	  }
	  catch (SALOME_ModuleCatalog::NotFound&) {
	    MESSAGE("GetPathPrefix(localhost) not found!") ;
	    path = "" ;
	  }
	}
      
      SCRUTE(path); 
      string implementation(path);
      implementation += "lib";
      implementation += MPIcomponentName;
      implementation += "Engine.so";
      
      Engines::Component_var compo 
	= cont->load_impl(MPIcomponentName, implementation.c_str());
      
//	  ASSERT(!CORBA::is_nil(compo));
//	  MESSAGE("Component launched !" << path);
      return compo;
    }
    else
      {
	MESSAGE("MPIComponent found !" << path);
	Engines::Component_var compo = Engines::Component::_narrow(obj);
	//	  ASSERT(!CORBA::is_nil(compo));
	try
	  {
	    string instanceName = compo->instanceName(); 
	  }
	catch (CORBA::SystemException&)
	  {
	    INFOS("Caught CORBA::SystemException CommFailure. Engine "
		  << path << "does not respond" );
	  }
	return compo;
      }
  }
  catch (ServiceUnreachable&)
    {
      INFOS("Caught exception: Naming Service Unreachable");
    }
  catch (...)
    {
      INFOS("Caught unknown exception.");
    }
  return Engines::Component::_nil();
}
