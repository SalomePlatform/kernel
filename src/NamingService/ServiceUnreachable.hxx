//=============================================================================
// File      : ServiceUnreachable.hxx
// Created   : mer jun 27 16:20:39 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#if !defined ( SERVICEUNREACHABLE_H )
#define SERVICEUNREACHABLE_H

#include "Utils_SALOME_Exception.hxx"

//! exception class for NamingService : ServiceUnreachable

class ServiceUnreachable : public SALOME_Exception
{
public :
	ServiceUnreachable( void );
	ServiceUnreachable( const ServiceUnreachable &ex  );
} ;

#endif	/* #if ( !defined  SERVICEUNREACHABLE_H ) */
