using namespace std;
//=============================================================================
// File      : Utils_CommException.cxx
// Created   : lun nov  5 16:04:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

using namespace std;
# include "Utils_CommException.hxx"

CommException::CommException( void ): SALOME_Exception( "CommException" )
{
}


CommException::CommException( const char *texte ): SALOME_Exception( texte )
{
}


CommException::CommException( const CommException &ex ): SALOME_Exception( ex )
{
}


CommException::~CommException() throw ()
{
}
