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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#ifndef __SALOME_LOADRATEMANAGER_HXX__
#define __SALOME_LOADRATEMANAGER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include <string>
#include "SALOME_ResourcesCatalog_Parser.hxx"
#include "SALOME_NamingService.hxx"

#if defined RESOURCESMANAGER_EXPORTS
#if defined WIN32
#define RESOURCESMANAGER_EXPORT __declspec( dllexport )
#else
#define RESOURCESMANAGER_EXPORT
#endif
#else
#if defined WNT
#define RESOURCESMANAGER_EXPORT __declspec( dllimport )
#else
#define RESOURCESMANAGER_EXPORT
#endif
#endif

class RESOURCESMANAGER_EXPORT SALOME_LoadRateManager
  {

  public:
    std::string FindFirst(const Engines::MachineList& hosts);
    std::string FindNext(const Engines::MachineList& hosts,MapOfParserResourcesType& resList,SALOME_NamingService *ns);
    std::string FindBest(const Engines::MachineList& hosts) throw (SALOME_Exception);
  };

#endif
