using namespace std;
//=============================================================================
// File      : Utils_SALOME_Exception.cxx
// Created   : Mon Nov  5 17:01:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

using namespace std;
#include <iostream>
#include "Utils_SALOME_Exception.hxx"
#include "utilities.h"
extern "C"
{
#include <math.h>
#include <stdio.h>
#include <string.h>
}


const char* duplicate( const char *const str ) ;

SALOME_Exception::SALOME_Exception( void ): exception() , _text(0)
{
	MESSAGE( "You must user the standard builder : SALOME_Exception::SALOME_Exception( const char *text )" ) ;
	INTERRUPTION(1) ;
}



const char *makeText( const char *text, const char *fileName, const unsigned int lineNumber )
{
	char *newText = 0 ;

	ASSERT(text) ;
	const size_t l1 = 1+strlen(text) ;
	ASSERT(l1>1) ;

	const char* prefix = "Salome Exception" ;
	const size_t l0 = 2+strlen(prefix) ;

	if ( fileName )
	{
		const size_t l2 = 4+strlen(fileName) ;
		ASSERT(l2>4) ;

		ASSERT(lineNumber>=1) ;
		const size_t l3 = 4+int(log10(float(lineNumber))) ;
		
		newText = new char [ 1+l0+l1+l2+l3 ] ;
		sprintf( newText , "%s in %s [%u] : %s" , prefix, fileName, lineNumber, text ) ;
	}
	else
	{
		newText = new char [ 1+l0+l1 ] ;
		sprintf( newText , "%s : %s" , prefix, text ) ;
	}
	ASSERT(newText) ;
	return newText ;
}


SALOME_Exception::SALOME_Exception( const char *text, const char *fileName, const unsigned int lineNumber ) : exception(), _text( makeText( text , fileName , lineNumber ) )
{
}


SALOME_Exception::~SALOME_Exception() throw ()
{
	if ( _text )
	{
		delete [] _text ;
		char *& txt = (char*)_text ;
		txt = 0 ;
	}
	ASSERT(_text==NULL) ;
}



SALOME_Exception::SALOME_Exception( const SALOME_Exception &ex ): _text(duplicate(ex._text))
{
	;
}


ostream & operator<<( ostream &os , const SALOME_Exception &ex )
{
	os << ex._text ;
	return os ;
}



const char* SALOME_Exception::what( void ) const throw ()
{
	return _text ;
}
