//=============================================================================
// File      : Utils_SALOME_Exception.hxx
// Created   : Mon Nov  5 17:01:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#if !defined( __Utils_SALOME_Exception_hxx__ )
#define __Utils_SALOME_Exception_hxx__

using namespace std;
# include <exception>
# include <iostream>

# define LOCALIZED(message) #message , __FILE__ , __LINE__

class SALOME_Exception : public exception
{

private :
	SALOME_Exception( void );

protected :
	const char* _text ;	// pointeur constant et zone pointee constante !!!!

public :
	SALOME_Exception( const char *text, const char *fileName=0, const unsigned int lineNumber=0 );
	SALOME_Exception( const SALOME_Exception &ex );
	~SALOME_Exception() throw ();
	friend ostream & operator<<( ostream &os , const SALOME_Exception &ex );
	virtual const char *what( void ) const throw () ;
} ;


#endif		/* #if !defined( __Utils_SALOME_Exception_hxx__ ) */
