// File:	Utils_ExceptHandler.cxx
// Created:	Mon Mar 15 10:23:41 2004
// Author:	Oksana TCHEBANOVA
//		<ota@localhost.localdomain>


#include "Utils_ExceptHandlers.hxx"
#include "Utils_CorbaException.hxx"
#include "Utils_SALOME_Exception.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Exception)

using namespace std;

void SalomeException ()
{
  throw SALOME_Exception("Salome Exception");
}

void SALOME_SalomeException() {
  THROW_SALOME_CORBA_EXCEPTION("INTERNAL ERROR", SALOME::INTERNAL_ERROR);
}

