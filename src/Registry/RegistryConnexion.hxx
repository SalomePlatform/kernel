//=============================================================================
// File      : RegistryConnexion.hxx
// Created   : Mon Nov  5 17:26:23 CET 2001
// Author    : Pascale NOYRET - Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

/*
	RegistryConnexion should be used by an engine to add or to remove a component.

*/
# if !defined( __RegistryConnexion_HXX__ )
# define __RegistryConnexion_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Registry)
#include <string>

class RegistryConnexion
{
protected :
	const char*			_Ior		; // engine ior
	Registry::Components_var	_VarComponents	; // RegistryService reference
	string				_SessionName	;
	string				_Name		; // code name
	int   				_Id   		; // code identity returned by RegistryService
	void add( const char *aName );
	void remove( void );
	RegistryConnexion( void );

public :
	RegistryConnexion( int argc , char **argv , const char *ior , const char *ptrSessionName, const char *componentName );
	~RegistryConnexion();
} ;

# endif		/* # if !defined( __RegistryConnexion_H__ ) */
