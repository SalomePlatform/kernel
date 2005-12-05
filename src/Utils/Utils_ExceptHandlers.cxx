// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
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

