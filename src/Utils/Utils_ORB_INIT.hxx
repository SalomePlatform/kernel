//=============================================================================
// File      : Utils_ORB_INIT.hxx
// Created   : lun nov  5 16:04:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================


# if ! defined( __ORB_INIT_HXX__ )
# define __ORB_INIT_HXX__

# include "CORBA.h" 

#include "Utils_CommException.hxx"


/*!
 * Ce composant prend en charge la connexion et la deconnexion a l'orb
 * Il est souhaitable de l'utiliser dans un SINGLETON.
 */

class ORB_INIT
{

private :
	CORBA::ORB_var _orb ;

public :
	ORB_INIT( void );
	~ORB_INIT();
	CORBA::ORB_var & operator() ( int argc , char **argv ) throw( CommException ) ;

	inline CORBA::ORB_var &orb( void );
} ;

inline CORBA::ORB_var &ORB_INIT::orb( void )
{
	return _orb ;
}

# endif
