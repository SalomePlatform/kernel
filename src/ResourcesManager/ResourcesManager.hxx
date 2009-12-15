//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#ifndef __RESOURCESMANAGER_HXX__
#define __RESOURCESMANAGER_HXX__

#include "ResourcesManager_Defs.hxx"

#include <string>
#include <fstream>
#include <vector>
#include <list>
#include "SALOME_ResourcesCatalog_Parser.hxx"
#include "SALOME_ResourcesCatalog_Handler.hxx"
#include "SALOME_LoadRateManager.hxx"
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WNT
#else
#include <unistd.h>
#endif

// --- WARNING ---
// The call of BuildTempFileToLaunchRemoteContainer and RmTmpFile must be done
// in a critical section to be sure to be clean.
// Only one thread should use the SALOME_ResourcesManager class in a SALOME
// session.
struct resourceParams
{
  std::string name;
  std::string hostname;
  std::string OS;
  unsigned int nb_proc;
  unsigned int nb_node;
  unsigned int nb_proc_per_node;
  unsigned int cpu_clock;
  unsigned int mem_mb;
  std::vector<std::string> componentList;
  std::vector<std::string> resourceList;
};

class RESOURCESMANAGER_EXPORT ResourcesException
{
public:
  const std::string msg;

  ResourcesException(const std::string m) : msg(m) {}
};

class RESOURCESMANAGER_EXPORT ResourcesManager_cpp
  {

  public:

    ResourcesManager_cpp(const char *xmlFilePath);
    ResourcesManager_cpp() throw(ResourcesException);

    ~ResourcesManager_cpp();

    std::vector<std::string> 
    GetFittingResources(const resourceParams& params) throw(ResourcesException);

    std::string Find(const std::string& policy, 
		     const std::vector<std::string>& listOfResources);

    int AddResourceInCatalog (const resourceParams& paramsOfNewResources,
			      const std::vector<std::string>& componentsOnNewResources,
			      const char *userName,
			      AccessModeType mode,
			      AccessProtocolType prot,
			      AccessProtocolType iprot) throw(ResourcesException);

    void DeleteResourceInCatalog(const char * name);

    void WriteInXmlFile(std::string & xml_file);

    const MapOfParserResourcesType& ParseXmlFiles();

    const MapOfParserResourcesType& GetList() const;

    ParserResourcesType GetResourcesDescr(const std::string & name);

  protected:
    
    void SelectOnlyResourcesWithOS(std::vector<std::string>& resources, std::string OS);

    void KeepOnlyResourcesWithComponent(std::vector<std::string>& resources, 
					const std::vector<std::string>& componentList);

    //! will contain the path to the ressources catalog
    std::list<std::string> _path_resources;
    std::list<std::string>::iterator _path_resources_it;

    //! will contain the informations on the data type catalog(after parsing)
    MapOfParserResourcesType _resourcesList;

    //! a map that contains all the available load rate managers (the key is the name)
    std::map<std::string , LoadRateManager*> _resourceManagerMap;

    //! contain the time where resourcesList was created
    time_t _lasttime;
  };

#endif // __RESOURCESMANAGER_HXX__
