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

#ifndef __SALOME_RESOURCESMANAGER_CLIENT_HXX__
#define __SALOME_RESOURCESMANAGER_CLIENT_HXX__

#include "SALOME_ResourcesManager_Common.hxx"

class SALOME_NamingService;

// This class is fully thread-safe because the ResourcesManager CORBA object
// runs in a single-thread POA.
class SALOMERESOURCESMANAGER_EXPORT SALOME_ResourcesManager_Client
{
public:

  SALOME_ResourcesManager_Client(SALOME_NamingService *ns);
  ~SALOME_ResourcesManager_Client();

  std::vector<std::string> GetFittingResources(const resourceParams& params);
  //std::string FindFirst(const std::vector<std::string>& listOfResources) const;
  std::string Find(const std::string & policy, const std::vector<std::string> & listOfResources);
  ParserResourcesType GetResourceDefinition(const std::string & name);
  //void AddResource(const ParserResourcesType & new_resource, bool write, const std::string & xml_file);
  //void RemoveResource(const std::string & name, bool write, const std::string & xml_file);
  std::string getMachineFile(const std::string & resource_name,
                             long nb_procs,
                             const std::string & parallelLib);

protected:

  Engines::ResourcesManager_var _rm;

};

#endif
