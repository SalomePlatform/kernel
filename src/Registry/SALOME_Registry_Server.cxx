//  SALOME Registry : Registry server implementation
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
//  File   : SALOME_Registry_Server.cxx
//  Author : Pascale NOYRET - Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$

using namespace std;
# include <stdlib.h>
# include <iostream.h>
# include <fstream.h>

extern "C"
{
# include <stdio.h>
}

# include "utilities.h"
# include "Utils_ORB_INIT.hxx"
# include "Utils_SINGLETON.hxx"
# include "Utils_SALOME_Exception.hxx"
# include "Utils_CommException.hxx"
# include "ServiceUnreachable.hxx"
# include "SALOME_NamingService.hxx"
# include "RegistryService.hxx"

//#define CHECKTIME
#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

int main( int argc , char **argv )
{
	BEGIN_OF( argv[0] )
	INFOS_COMPILATION 
	SCRUTE(argc) 
	if( argc<3 )
	{
		MESSAGE("you must provide the Salome session name when you call SALOME_Registry_Server") ;
		throw CommException("you must provide the Salome session name when you call SALOME_Registry_Server") ;
	}
	const char *ptrSessionName=0 ;

	int k=0 ;
	for ( k=1 ; k<argc ; k++ )
	{
		if( strcmp(argv[k],"--salome_session")==0 )
		{
			ptrSessionName=argv[k+1] ;
			break ;
		}
	}
	ASSERT(ptrSessionName) ;
	ASSERT(strlen( ptrSessionName )>0) ;
	const char *registryName = "Registry" ;


	try
	{
		ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
		CORBA::ORB_var &orb = init( argc , argv ) ;


		SALOME_NamingService &naming = *SINGLETON_<SALOME_NamingService>::Instance() ;
		naming.init_orb( orb ) ;

		RegistryService *ptrRegistry = SINGLETON_<RegistryService>::Instance() ;
		ptrRegistry->SessionName( ptrSessionName ) ;
		Registry::Components_var varComponents = ptrRegistry->_this() ;

		// The RegistryService must not already exist.

		try
		{
        		CORBA::Object_var pipo = naming.Resolve( registryName ) ;
			if (CORBA::is_nil(pipo) )  throw ServiceUnreachable() ;
			MESSAGE("RegistryService servant already existing" ) ;
			exit( EXIT_FAILURE ) ;
		}
		catch( const ServiceUnreachable &ex )
		{
		}
		catch( const CORBA::Exception &exx )
		{
		}
		string absoluteName = string("/") + registryName;
		naming.Register( varComponents , absoluteName.c_str() ) ;


		CORBA::Object_var poaObj = orb->resolve_initial_references( "RootPOA" ) ;
		PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj) ;
		PortableServer::POAManager_var manager = poa->the_POAManager() ;


		MESSAGE("On attend les requetes des clients") ;
		try
		{
			// Activation du POA
			MESSAGE("Activation du POA") ;
			manager->activate() ;

			// Lancement de l'ORB
			MESSAGE("Lancement de l'ORB") ;
#ifdef CHECKTIME
			Utils_Timer timer;
			timer.Start();
			timer.Stop();
			MESSAGE("SALOME_Registry_Server.cxx - orb->run()");
			timer.ShowAbsolute();
#endif
			orb->run() ;
		}
		catch( const CORBA::Exception &ex )
		{
			MESSAGE("Erreur systeme") ;
			return EXIT_FAILURE ;
		}

	}
	catch( const SALOME_Exception &ex )
	{
		MESSAGE( "Communication Error : " << ex.what() )
		return EXIT_FAILURE ;
	}

	END_OF( argv[0] ) ;
	return 0 ;
}
