using namespace std;
//=============================================================================
// File      : Utils_ORB_INIT.cxx
// Created   : lun nov  5 16:04:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

using namespace std;
# include "Utils_ORB_INIT.hxx" 
# include "utilities.h" 

ORB_INIT::ORB_INIT( void ): _orb( CORBA::ORB::_nil() )
{
	;
}


ORB_INIT::~ORB_INIT()
{
	if ( ! CORBA::is_nil( _orb ) )
	{
		MESSAGE("appel _orb->destroy()") ;
		_orb->destroy() ;
		MESSAGE("retour _orb->destroy()") ;
	}
}


CORBA::ORB_var &ORB_INIT::operator() ( int argc , char **argv ) throw( CommException )
{
	if ( CORBA::is_nil( _orb ) )
	{
		try
		{
			_orb = CORBA::ORB_init( argc , argv ) ;
			//set GIOP message size equal to 50Mb for transferring brep shapes as 
			//sequence of bytes using C++ streams
			omniORB::MaxMessageSize(100*1024*1024);
		}
		catch( const CORBA::Exception &ex )
		{
			throw CommException( "Unable to create an ORB connexion" ) ;
		}
	}
	return _orb ;
}
