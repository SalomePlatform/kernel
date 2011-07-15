// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include "ResourcesManager.hxx" 
#include <Basics_Utils.hxx>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <map>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WNT
#else
#include <unistd.h>
#endif
#include <libxml/parser.h>

#include <algorithm>

#define MAX_SIZE_FOR_HOSTNAME 256;

static LoadRateManagerFirst first;
static LoadRateManagerCycl cycl;
static LoadRateManagerAltCycl altcycl;
//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

ResourcesManager_cpp::
ResourcesManager_cpp(const char *xmlFilePath)
{
  _path_resources.push_back(xmlFilePath);
#if defined(_DEBUG_) || defined(_DEBUG)
  std::cerr << "ResourcesManager_cpp constructor" << std::endl;
#endif
  _resourceManagerMap["first"]=&first;
  _resourceManagerMap["cycl"]=&cycl;
  _resourceManagerMap["altcycl"]=&altcycl;
  _resourceManagerMap["best"]=&altcycl;
  _resourceManagerMap[""]=&altcycl;
}

//=============================================================================
/*!
 *  Standard constructor, parse resource file.
 *  - if ${APPLI} exists in environment,
 *    look for ${HOME}/${APPLI}/CatalogResources.xml
 *  - else look for default:
 *    ${KERNEL_ROOT_DIR}/share/salome/resources/kernel/CatalogResources.xml
 *  - parse XML resource file.
 */ 
//=============================================================================

ResourcesManager_cpp::ResourcesManager_cpp() throw(ResourcesException)
{
  RES_MESSAGE("ResourcesManager_cpp constructor");

  _resourceManagerMap["first"]=&first;
  _resourceManagerMap["cycl"]=&cycl;
  _resourceManagerMap["altcycl"]=&altcycl;
  _resourceManagerMap["best"]=&altcycl;
  _resourceManagerMap[""]=&altcycl;

  bool default_catalog_resource = true;
  if (getenv("USER_CATALOG_RESOURCES_FILE") != 0)
  {
    default_catalog_resource = false;
    std::string user_file("");
    user_file = getenv("USER_CATALOG_RESOURCES_FILE");
    std::ifstream ifile(user_file.c_str(), std::ifstream::in );
    if (ifile) {
      // The file exists, and is open for input
      _path_resources.push_back(user_file);
    }
    else {
      default_catalog_resource = true;
      RES_INFOS("Warning: USER_CATALOG_RESOURCES_FILE is set and file cannot be found.")
    }
  }
  if (default_catalog_resource)
  {
    std::string default_file("");
    if (getenv("APPLI") != 0)
    {
      default_file += getenv("HOME");
      default_file += "/";
      default_file += getenv("APPLI");
      default_file += "/CatalogResources.xml";
      _path_resources.push_back(default_file);
    }
    else
    {
      if(!getenv("KERNEL_ROOT_DIR"))
        throw ResourcesException("you must define KERNEL_ROOT_DIR environment variable!! -> cannot load a CatalogResources.xml");
      default_file = getenv("KERNEL_ROOT_DIR");
      default_file += "/share/salome/resources/kernel/CatalogResources.xml";
      _path_resources.push_back(default_file);
    }
  }

  _lasttime=0;

  ParseXmlFiles();
  RES_MESSAGE("ResourcesManager_cpp constructor end");
}

//=============================================================================
/*!
 *  Standard Destructor
 */ 
//=============================================================================

ResourcesManager_cpp::~ResourcesManager_cpp()
{
  RES_MESSAGE("ResourcesManager_cpp destructor");
}

//=============================================================================
//! get the list of resource names fitting constraints given by params
/*!
 * Steps:
 * 1: Restrict list with resourceList if defined
 * 2: If name is defined -> check resource list
 * 3: If not 2:, if hostname is defined -> check resource list
 * 4: If not 3:, sort resource with nb_proc, etc...
 * 5: In all cases remove resource that does not correspond with OS
 * 6: And remove resource with componentList - if list is empty ignored it...
 */ 
//=============================================================================

std::vector<std::string> 
ResourcesManager_cpp::GetFittingResources(const resourceParams& params) throw(ResourcesException)
{
  RES_MESSAGE("[GetFittingResources] on computer " << Kernel_Utils::GetHostname().c_str());
  RES_MESSAGE("[GetFittingResources] with resource name: " << params.name);
  RES_MESSAGE("[GetFittingResources] with hostname: "<< params.hostname);

  // Result
  std::vector<std::string> vec;

  // Parse Again CalatogResource File
  ParseXmlFiles();

  // Steps:
  // 1: If name is defined -> check resource list
  // 2: Restrict list with resourceList if defined
  // 3: If not 2:, if hostname is defined -> check resource list
  // 4: If not 3:, sort resource with nb_proc, etc...
  // 5: In all cases remove resource that does not correspond with OS
  // 6: And remove resource with componentList - if list is empty ignored it...

  // Step 1
  if (params.name != "")
  {
    RES_MESSAGE("[GetFittingResources] name parameter found !");
    if (_resourcesList.find(params.name) != _resourcesList.end())
    {
      vec.push_back(params.name);
      return vec;
    }
    else
      RES_MESSAGE("[GetFittingResources] resource name was not found on resource list ! name requested was " << params.name);
      std::string error("[GetFittingResources] resource name was not found on resource list ! name requested was " + params.name);
      throw ResourcesException(error);
  }

  MapOfParserResourcesType local_resourcesList = _resourcesList;
  // Step 2
  if (params.resourceList.size() > 0)
  {
    RES_MESSAGE("[GetFittingResources] Restricted resource list found !");
    local_resourcesList.clear();
    std::vector<std::string>::size_type sz = params.resourceList.size();

    for (unsigned int i=0; i < sz; i++)
    {
      if (_resourcesList.find(params.resourceList[i]) != _resourcesList.end())
        local_resourcesList[params.resourceList[i]] = _resourcesList[params.resourceList[i]];
    }
  }

  // Step 3
  if (params.hostname != "")
  {
    RES_MESSAGE("[GetFittingResources] Entering in hostname case !");

    std::string hostname = params.hostname;
    if (hostname ==  "localhost")
      hostname = Kernel_Utils::GetHostname().c_str();

    std::map<std::string, ParserResourcesType>::const_iterator iter = _resourcesList.begin();
    for (; iter != _resourcesList.end(); iter++)
    {
      if ((*iter).second.HostName == hostname)
        vec.push_back((*iter).first);
    }
  }
  // Step 4
  else
  {
    // --- Search for available resources sorted by priority
    MapOfParserResourcesType_it i = local_resourcesList.begin();
    for (; i != local_resourcesList.end(); ++i)
      vec.push_back(i->first);

    // --- set wanted parameters
    ResourceDataToSort::_nbOfProcWanted = params.nb_proc;
    ResourceDataToSort::_nbOfNodesWanted = params.nb_node;
    ResourceDataToSort::_nbOfProcPerNodeWanted = params.nb_proc_per_node;
    ResourceDataToSort::_CPUFreqMHzWanted = params.cpu_clock;
    ResourceDataToSort::_memInMBWanted = params.mem_mb;
    // --- end of set
        
    // Sort
    std::list<ResourceDataToSort> li;
    std::vector<std::string>::iterator iter = vec.begin();
    for (; iter != vec.end(); iter++)
      li.push_back(local_resourcesList[(*iter)].DataForSort);
    li.sort();

    vec.clear();
    for (std::list<ResourceDataToSort>::iterator iter2 = li.begin(); iter2 != li.end(); iter2++)
      vec.push_back((*iter2)._Name);
  }

  // Step 5
  SelectOnlyResourcesWithOS(vec, params.OS.c_str());
    
  // Step 6
  std::vector<std::string> vec_save(vec);
  KeepOnlyResourcesWithComponent(vec, params.componentList);
  if (vec.size() == 0)
    vec = vec_save;

  // End
  // Send an exception if return list is empty...
  if (vec.size() == 0)
  {
    std::string error("[GetFittingResources] ResourcesManager doesn't find any resource that fits to your parameters");
    throw ResourcesException(error);
  }

  return vec;
}

//=============================================================================
/*!
 *  add an entry in the ressources catalog xml file.
 */ 
//=============================================================================

void
ResourcesManager_cpp::AddResourceInCatalog(const ParserResourcesType & new_resource) throw(ResourcesException)
{
  // TODO - Add minimal check
  _resourcesList[new_resource.Name] = new_resource;
}

//=============================================================================
/*!
 *  Deletes a resource from the catalog
 */ 
//=============================================================================

void ResourcesManager_cpp::DeleteResourceInCatalog(const char * name)
{
  MapOfParserResourcesType_it it = _resourcesList.find(name);
  if (it != _resourcesList.end())
    _resourcesList.erase(name);
  else
    RES_INFOS("You try to delete a resource that does not exist... : " << name);
}

//=============================================================================
/*!
 *  write the current data in memory in file.
 */ 
//=============================================================================

void ResourcesManager_cpp::WriteInXmlFile(std::string xml_file)
{
  RES_MESSAGE("WriteInXmlFile : start");

  if (xml_file == "")
  {
    _path_resources_it = _path_resources.begin();
    xml_file = *_path_resources_it;
  }

  const char* aFilePath = xml_file.c_str();
  FILE* aFile = fopen(aFilePath, "w");

  if (aFile == NULL)
  {
    std::cerr << "Error opening file in WriteInXmlFile : " << xml_file << std::endl;
    return;
  }
  
  xmlDocPtr aDoc = xmlNewDoc(BAD_CAST "1.0");
  xmlNewDocComment(aDoc, BAD_CAST "ResourcesCatalog");

  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList);
  handler->PrepareDocToXmlFile(aDoc);
  delete handler;

  int isOk = xmlSaveFormatFile(aFilePath, aDoc, 1);
  if (!isOk) 
     std::cerr << "Error while XML file saving : " << xml_file << std::endl;
  
  // Free the document
  xmlFreeDoc(aDoc);
  fclose(aFile);
  RES_MESSAGE("WriteInXmlFile : WRITING DONE!");
}

//=============================================================================
/*!
 *  parse the data type catalog
 */ 
//=============================================================================

const MapOfParserResourcesType& ResourcesManager_cpp::ParseXmlFiles()
{
  // Parse file only if its modification time is greater than lasttime (last registered modification time)
  bool to_parse = false;
  for(_path_resources_it = _path_resources.begin(); _path_resources_it != _path_resources.end(); ++_path_resources_it)
  {
    struct stat statinfo;
    int result = stat((*_path_resources_it).c_str(), &statinfo);
    if (result < 0)
    {
      std::cerr << "Error in method stat for file : " << (*_path_resources_it).c_str() << " no new xml file is parsed" << std::endl;
      return _resourcesList;
    }

    if(statinfo.st_mtime > _lasttime)
    {
      to_parse = true;
      _lasttime = statinfo.st_mtime;
    }
  }

  if (to_parse)
  {
    _resourcesList.clear();
    // On parse tous les fichiers
    for(_path_resources_it = _path_resources.begin(); _path_resources_it != _path_resources.end(); ++_path_resources_it)
    {
      MapOfParserResourcesType _resourcesList_tmp;
      MapOfParserResourcesType _resourcesBatchList_tmp;
      SALOME_ResourcesCatalog_Handler* handler =
        new SALOME_ResourcesCatalog_Handler(_resourcesList_tmp);
      const char* aFilePath = (*_path_resources_it).c_str();
      FILE* aFile = fopen(aFilePath, "r");

      if (aFile != NULL)
      {
        xmlDocPtr aDoc = xmlReadFile(aFilePath, NULL, 0);
        if (aDoc != NULL)
        {
          handler->ProcessXmlDocument(aDoc);

          // adding new resources to the file
          for (MapOfParserResourcesType_it i = _resourcesList_tmp.begin(); i != _resourcesList_tmp.end(); ++i)
          {
            MapOfParserResourcesType_it j = _resourcesList.find(i->first);
            if (j == _resourcesList.end())
            {
              _resourcesList[i->first] = i->second;
            }
            else
            {
              std::cerr << "ParseXmlFiles Warning, two resources with the same name were found, taking the first declaration : " << i->first << std::endl;
            }
          }
        }
        else
          std::cerr << "ResourcesManager_cpp: could not parse file " << aFilePath << std::endl;
        // Free the document
        xmlFreeDoc(aDoc);
        fclose(aFile);
      }
      else
        std::cerr << "ResourcesManager_cpp: file " << aFilePath << " is not readable." << std::endl;

      delete handler;
    }
  }
  return _resourcesList;
}

//=============================================================================
/*!
 *   consult the content of the list
 */ 
//=============================================================================

const MapOfParserResourcesType& ResourcesManager_cpp::GetList() const
{
  return _resourcesList;
}

std::string ResourcesManager_cpp::Find(const std::string& policy, const std::vector<std::string>& listOfResources)
{
  if(_resourceManagerMap.count(policy)==0)
    return _resourceManagerMap[""]->Find(listOfResources, _resourcesList);
  return _resourceManagerMap[policy]->Find(listOfResources, _resourcesList);
}

//=============================================================================
/*!
 *  Gives a sublist of resources with matching OS.
 *  If parameter OS is empty, gives the complete list of resources
 */ 
//=============================================================================
void 
ResourcesManager_cpp::SelectOnlyResourcesWithOS(std::vector<std::string>& resources, std::string OS)
{
  if (OS != "")
  {
    // a computer list is given : take only resources with OS on those computers
    std::vector<std::string> vec_tmp = resources;
    resources.clear();
    std::vector<std::string>::iterator iter = vec_tmp.begin();
    for (; iter != vec_tmp.end(); iter++)
    {
      MapOfParserResourcesType::const_iterator it = _resourcesList.find(*iter);
      if(it != _resourcesList.end())
        if ( (*it).second.OS == OS)
          resources.push_back(*iter);
    }
  }
}


//=============================================================================
/*!
 *  Gives a sublist of machines on which the component is known.
 */ 
//=============================================================================
void 
ResourcesManager_cpp::KeepOnlyResourcesWithComponent(std::vector<std::string>& resources, 
                                                     const std::vector<std::string>& componentList)
{
  std::vector<std::string> kept_resources;

  std::vector<std::string>::iterator iter = resources.begin();
  for (; iter != resources.end(); iter++)
  {
    const std::vector<std::string>& mapOfComponentsOfCurrentHost = _resourcesList[*iter].ComponentsList;

    bool erasedHost = false;
    if( mapOfComponentsOfCurrentHost.size() > 0 )
    {
      for(unsigned int i=0; i<componentList.size(); i++)
      {
        std::vector<std::string>::const_iterator itt = find(mapOfComponentsOfCurrentHost.begin(),
                                                            mapOfComponentsOfCurrentHost.end(),
                                                            componentList[i]);
        if (itt == mapOfComponentsOfCurrentHost.end())
        {
          erasedHost = true;
          break;
        }
      }
    }
    if(!erasedHost)
      kept_resources.push_back(*iter);
  }
  resources=kept_resources;
}


ParserResourcesType 
ResourcesManager_cpp::GetResourcesDescr(const std::string & name)
{
  if (_resourcesList.find(name) != _resourcesList.end())
    return _resourcesList[name];
  else
  {
    std::string error("[GetResourcesDescr] Resource does not exist: ");
    error += name;
    throw ResourcesException(error);
  }
}
