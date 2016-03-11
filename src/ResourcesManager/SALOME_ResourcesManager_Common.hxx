// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

#ifndef __SALOME_RESOURCESMANAGER_COMMON_HXX__
#define __SALOME_RESOURCESMANAGER_COMMON_HXX__

#ifdef WIN32
# if defined SALOMERESOURCESMANAGER_EXPORTS || defined SalomeResourcesManager_EXPORTS
#  define SALOMERESOURCESMANAGER_EXPORT __declspec( dllexport )
# else
#  define SALOMERESOURCESMANAGER_EXPORT __declspec( dllimport )
# endif
#else
# define SALOMERESOURCESMANAGER_EXPORT
#endif

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ResourcesManager)

#include "ResourcesManager.hxx"

// Functions for CPP <-> CORBA conversions
// All those functions are thread-safe
SALOMERESOURCESMANAGER_EXPORT resourceParams resourceParameters_CORBAtoCPP(const Engines::ResourceParameters & params);
SALOMERESOURCESMANAGER_EXPORT Engines::ResourceParameters_var resourceParameters_CPPtoCORBA(const resourceParams & params);

SALOMERESOURCESMANAGER_EXPORT std::vector<std::string> resourceList_CORBAtoCPP(const Engines::ResourceList & resList);
SALOMERESOURCESMANAGER_EXPORT Engines::ResourceList_var resourceList_CPPtoCORBA(const std::vector<std::string> & resList);

SALOMERESOURCESMANAGER_EXPORT ParserResourcesType resourceDefinition_CORBAtoCPP(const Engines::ResourceDefinition & resDef);
SALOMERESOURCESMANAGER_EXPORT Engines::ResourceDefinition_var resourceDefinition_CPPtoCORBA(const ParserResourcesType & resource);
#endif
