//  SALOME LifeCycleCORBA : implementation of containers and engines life cycle both in Python and C++
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_LifeCycleCORBA.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

#include "OpUtil.hxx"
#include "utilities.h"

#include <ServiceUnreachable.hxx>

#include "SALOME_LifeCycleCORBA.hxx"
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include "SALOME_NamingService.hxx"

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA()
{
  _NS = NULL;
  _FactoryServer = NULL ;
}

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA(SALOME_NamingService *ns)
{
  _NS = ns;
  _FactoryServer = NULL ;
}

SALOME_LifeCycleCORBA::~SALOME_LifeCycleCORBA()
{
}

string SALOME_LifeCycleCORBA::ContainerName(
                                         const char * aComputerContainer ,
                                         string * theComputer ,
                                         string * theContainer ) {
  char * ContainerName = new char [ strlen( aComputerContainer ) + 1 ] ;
  strcpy( ContainerName , aComputerContainer ) ;
  string theComputerContainer("/Containers/");
  char * slash = strchr( ContainerName , '/' ) ;
  if ( !slash ) {
    *theComputer = GetHostname() ;
    theComputerContainer += *theComputer ;
    theComputerContainer += "/" ;
    *theContainer = ContainerName ;
    theComputerContainer += *theContainer ;
  }
  else {
    slash[ 0 ] = '\0' ;
    slash += 1 ;
    *theContainer = slash ;
    if ( !strcmp( ContainerName , "localhost" ) ) {
      *theComputer = GetHostname() ;
    }
    else {
      *theComputer = ContainerName ;
    }
    theComputerContainer += *theComputer ;
    theComputerContainer += "/" ;
    theComputerContainer += *theContainer ;
  }
  return theComputerContainer ;
}

string SALOME_LifeCycleCORBA::ComputerPath(
                                         const char * theComputer ) {
//  CORBA::String_var path;
//   CORBA::Object_var obj = _NS->Resolve("/Kernel/ModulCatalog");
//   SALOME_ModuleCatalog::ModuleCatalog_var Catalog = 
// 	             SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
//   try {
//     path = Catalog->GetPathPrefix( theComputer );
//   }
//   catch (SALOME_ModuleCatalog::NotFound&) {
//     MESSAGE("GetPathPrefix(" << theComputer << ") not found!");
//     path = "" ;
//   }
// Modification provisoire B. Secher en attendant le gestionnaire de ressources
// 21/10/2003
// Le KERNEL_ROOT_DIR sera a lire dans le catalogue de machines
// en attendant on suppose qu'il est identique au KERNEL_ROOT_DIR local
  string path = string(getenv("KERNEL_ROOT_DIR")) + "/bin/salome/";
  SCRUTE( path ) ;
  return path;
}

Engines::Container_var SALOME_LifeCycleCORBA::FindContainer(const char *containerName ) {
  ASSERT(_NS != NULL);
  string cont ;
  if ( strncmp( containerName , "/Containers/" , 12 ) ) { // Compatibility ...
    string theComputer ;
    string theContainer ;
    cont = ContainerName( containerName , &theComputer , &theContainer ) ;
  }
  else {
    cont = containerName ;
  }
  try {

    SCRUTE( cont );

    CORBA::Object_var obj = _NS->Resolve( cont.c_str() );
    if( !CORBA::is_nil( obj ) ) {
      return Engines::Container::_narrow( obj ) ;
    }
  }
  catch (ServiceUnreachable&) {
    INFOS("Caught exception: Naming Service Unreachable");
  }
  catch (...) {
    INFOS("Caught unknown exception.");
  }
  return Engines::Container::_nil();
}

Engines::Container_var SALOME_LifeCycleCORBA::FindOrStartContainer(
                                              const string aComputerContainer ,
                                              const string theComputer ,
                                              const string theContainer ) {
  Engines::Container_var aContainer = FindContainer( aComputerContainer.c_str() ) ;
  Engines::Container_var aFactoryServer ;
  SCRUTE( aComputerContainer ) ;
  SCRUTE( theComputer ) ;
  SCRUTE( theContainer ) ;
  bool pyCont = false ;
  int len = theContainer.length() ;
  if ( !strcmp( &theContainer.c_str()[len-2] , "Py" ) ) {
    pyCont = true ;
  }
  if ( !CORBA::is_nil( aContainer ) ) {
    return aContainer ;
  }
  else {
    string FactoryServer = theComputer ;
    if ( pyCont ) {
      FactoryServer += "/FactoryServerPy" ;
    }
    else {
      FactoryServer += "/FactoryServer" ;
    }
    aFactoryServer = FindContainer( FactoryServer.c_str() ) ;
    if ( CORBA::is_nil( aFactoryServer ) ) {
// rsh -n machine_name ${KERNEL_ROOT_DIR}/bin/salome SALOME_Container -ORBInitRef NameService=corbaname::localhost:1515 &
      string rsh( "" ) ;
      if ( theComputer!= GetHostname() ) {
        rsh += "rsh -n " ;
        rsh += theComputer ;
        rsh += " " ;
      }
      string path = ComputerPath( theComputer.c_str() ) ;
      SCRUTE( path ) ;
      rsh += path;
//       rsh += "runSession " ;
      if ( pyCont ) {
        rsh += "SALOME_ContainerPy.py " ;
        rsh += "FactoryServerPy -" ;
      }
      else {
        rsh += "SALOME_Container " ;
        rsh += "FactoryServer -" ;
      }
      string omniORBcfg( getenv( "OMNIORB_CONFIG" ) ) ;
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
        rsh += " > /tmp/FactoryServerPy_" ;
      }
      else {
        rsh += " > /tmp/FactoryServer_" ;
      }
      rsh += theComputer ;
      rsh += ".log 2>&1 &" ;
      SCRUTE( rsh );
      int status = system( rsh.c_str() ) ;
      if (status == -1) {
        INFOS("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed (system command status -1)") ;
      }
      else if (status == 217) {
        INFOS("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed (system command status 217)") ;
      }
      else {
        int count = 21 ;
        while ( CORBA::is_nil( aFactoryServer ) && count ) {
          sleep( 1 ) ;
          count-- ;
          if ( count != 10 )
            MESSAGE( count << ". Waiting for FactoryServer on " << theComputer)
          aFactoryServer = FindContainer( FactoryServer.c_str() ) ;
	}
        if ( CORBA::is_nil( aFactoryServer ) ) {
          INFOS("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed") ;
	}
        else if ( strcmp( theComputer.c_str() , GetHostname().c_str() ) ) {
          _FactoryServer = aFactoryServer ;
	}
      }
    }
    if ( !CORBA::is_nil( aFactoryServer ) ) {
      if ( strcmp( theContainer.c_str() , "FactoryServer" ) ||
           strcmp( theContainer.c_str() , "FactoryServerPy" ) ) {
        MESSAGE("Container not found ! trying to start " << aComputerContainer);
        Engines::Container_var myContainer = aFactoryServer->start_impl( theContainer.c_str() ) ;
        if ( !CORBA::is_nil( myContainer ) ) {
          MESSAGE("Container " << aComputerContainer << " started");
          return myContainer ;
        }
        else {
          MESSAGE("Container " << aComputerContainer << " NOT started");
        }
      }
      else {
        MESSAGE("Container " << aComputerContainer << " started");
        return aFactoryServer ;
      }
    }
  }
  return Engines::Container::_nil();
}

Engines::Component_var SALOME_LifeCycleCORBA::FindOrLoad_Component
                                   (const char *containerName,
				    const char *componentName,
				    const char *implementation)
{
  BEGIN_OF("FindOrLoad_Component(1)");
  ASSERT(_NS != NULL);
  string theComputer ;
  string theContainer ;
  string theComputerContainer = ContainerName( containerName ,
                                               &theComputer ,
                                               &theContainer ) ;
  Engines::Container_var cont = FindOrStartContainer( theComputerContainer ,
                                                      theComputer ,
                                                      theContainer ) ;
//  ASSERT(!CORBA::is_nil(cont));

  string path( theComputerContainer );
  path = path + "/";
  path = path + componentName;
  SCRUTE(path);
  try
    {
      CORBA::Object_var obj = _NS->Resolve(path.c_str());
      if (CORBA::is_nil(obj))
	{
	  MESSAGE("Component not found ! trying to load " << path);
	  Engines::Component_var compo 
	    = cont->load_impl(componentName, implementation);
//	  ASSERT(!CORBA::is_nil(compo));
	  MESSAGE("Component launched !" << path);
	  return compo;
	}
      else
	{
	  MESSAGE("Component found !" << path);
	  Engines::Component_var compo = Engines::Component::_narrow(obj);
//	  ASSERT(!CORBA::is_nil(compo));
	  try
	    {
	      compo->ping(); 
	    }
	  catch (CORBA::COMM_FAILURE&)
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

Engines::Component_var SALOME_LifeCycleCORBA::FindOrLoad_Component
                                  (const char *containerName,
				   const char *componentName)
{
//  BEGIN_OF("FindOrLoad_Component(2)");
  ASSERT(_NS != NULL);
  string theComputer ;
  string theContainer ;
  string theComputerContainer = ContainerName( containerName ,
                                               &theComputer ,
                                               &theContainer ) ;
  Engines::Container_var cont = FindOrStartContainer( theComputerContainer ,
                                                      theComputer ,
                                                      theContainer ) ;

  if ( CORBA::is_nil( cont ) ) {
    MESSAGE("Container not found ! " << theComputerContainer );
    return Engines::Component::_nil();
  }

//  char * machine = cont->machineName() ;
  const char * machine = theComputer.c_str() ;

  string path( theComputerContainer );
  path += "/";
  path += componentName;
  SCRUTE(path);

  try {
    CORBA::Object_var obj = _NS->Resolve(path.c_str());
    if ( CORBA::is_nil( obj ) ) {
      MESSAGE("Component not found ! trying to load " << path);
	  CORBA::Object_var obj2 = _NS->Resolve("/Kernel/ModulCatalog");
	  SALOME_ModuleCatalog::ModuleCatalog_var Catalog = 
	    SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj2);

	  SALOME_ModuleCatalog::Acomponent_ptr compoInfo = 
	    Catalog->GetComponent(componentName);
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
	  implementation += componentName;
	  implementation += "Engine.so";
	  
	  Engines::Component_var compo 
	    = cont->load_impl(componentName, implementation.c_str());

//	  ASSERT(!CORBA::is_nil(compo));
//	  MESSAGE("Component launched !" << path);
	  return compo;
    }
    else
	{
	  MESSAGE("Component found !" << path);
	  Engines::Component_var compo = Engines::Component::_narrow(obj);
//	  ASSERT(!CORBA::is_nil(compo));
	  try
	    {
	      string instanceName = compo->instanceName(); 
	    }
	  catch (CORBA::COMM_FAILURE&)
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
