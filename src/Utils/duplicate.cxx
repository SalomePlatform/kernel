using namespace std;
//=============================================================================
// File      : duplicate.cxx
// Created   : lun nov  5 16:04:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

/*!
 *	This function can be changed by strdup() if strdup() is ANSI.
 *	It is strongly (and only) used in the Registry environment
 *	(RegistryService, RegistryConnexion, Identity, ...)
 */
extern "C"
{
#include <stdlib.h>
#include <string.h>
}
#include  "utilities.h"

const char* duplicate( const char *const str )
{
	ASSERT(str!=NULL) ;
	const size_t length = strlen( str ) ;
	ASSERT(length>0) ;
	char *new_str = new char[ 1+length ] ;
	ASSERT(new_str) ;
	strcpy( new_str , str ) ;
	return new_str ;
}
