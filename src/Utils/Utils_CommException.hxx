//=============================================================================
// File      : Utils_CommException.hxx
// Created   : lun nov  5 16:04:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

# if  !defined ( __Utils_CommException_H__ )
# define __Utils_CommException_H__ )

# include "Utils_SALOME_Exception.hxx"

class CommException : public SALOME_Exception
{
public :
	CommException( void );
	CommException( const char *texte );
	CommException( const CommException &ex );
	~CommException() throw ();
} ;

# endif	/* # if ( !defined __Utils_CommException_H__ ) */
