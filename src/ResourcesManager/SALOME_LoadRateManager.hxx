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

#include "ResourcesManager_Defs.hxx"
#include <string>
#include "SALOME_ResourcesCatalog_Parser.hxx"

class RESOURCESMANAGER_EXPORT SALOME_LoadRateManager
  {

  public:
    std::string FindFirst(const std::vector<std::string>& hosts);
    std::string FindNext(const std::vector<std::string>& hosts,MapOfParserResourcesType& resList);
    std::string FindBest(const std::vector<std::string>& hosts);
  };

#endif
