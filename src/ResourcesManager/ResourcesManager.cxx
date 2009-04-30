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
#include <unistd.h>
#include <libxml/parser.h>

#include <algorithm>

#define MAX_SIZE_FOR_HOSTNAME 256;

using namespace std;

static LoadRateManagerFirst first;
static LoadRateManagerCycl cycl;
static LoadRateManagerAltCycl altcycl;
//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

ResourcesManager_cpp::
ResourcesManager_cpp(const char *xmlFilePath) :
    _path_resources(xmlFilePath)
{
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "ResourcesManager_cpp constructor" << endl;
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
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "ResourcesManager_cpp constructor" << endl;
#endif
  _resourceManagerMap["first"]=&first;
  _resourceManagerMap["cycl"]=&cycl;
  _resourceManagerMap["altcycl"]=&altcycl;
  _resourceManagerMap["best"]=&altcycl;
  _resourceManagerMap[""]=&altcycl;

  _isAppliSalomeDefined = (getenv("APPLI") != 0);
  if(!getenv("KERNEL_ROOT_DIR"))
    throw ResourcesException("you must define KERNEL_ROOT_DIR environment variable!!");

  if (_isAppliSalomeDefined)
    {
      _path_resources = getenv("HOME");
      _path_resources += "/";
      _path_resources += getenv("APPLI");
      _path_resources += "/CatalogResources.xml";
    }

  else
    {
      _path_resources = getenv("KERNEL_ROOT_DIR");
      _path_resources += "/share/salome/resources/kernel/CatalogResources.xml";
    }

  ParseXmlFile();
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "ResourcesManager_cpp constructor end";
#endif
}

//=============================================================================
/*!
 *  Standard Destructor
 */ 
//=============================================================================

ResourcesManager_cpp::~ResourcesManager_cpp()
{
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "ResourcesManager_cpp destructor" << endl;
#endif
}

//=============================================================================
//! get the list of resource names fitting constraints given by params
/*!
 *  If hostname is specified, check if it is local or known in resources catalog.
 *
 *  Else
 *  - select first machines with corresponding OS (all machines if
 *    parameter OS empty),
 *  - then select the sublist of machines on which the component is known
 *    (if the result is empty, that probably means that the inventory of
 *    components is probably not done, so give complete list from previous step)
 */ 
//=============================================================================

std::vector<std::string> 
ResourcesManager_cpp::GetFittingResources(const machineParams& params) throw(ResourcesException)
{
  vector <std::string> vec;

  ParseXmlFile();

  const char *hostname = params.hostname.c_str();
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "GetFittingResources " << hostname << " " << Kernel_Utils::GetHostname().c_str() << endl;
#endif

  if (hostname[0] != '\0'){

    if ( strcmp(hostname, "localhost") == 0 ||
	 strcmp(hostname, Kernel_Utils::GetHostname().c_str()) == 0 )
      {
//#if defined(_DEBUG_) || defined(_DEBUG)
//	cerr << "ResourcesManager_cpp::GetFittingResources : localhost" << endl;
//#endif
	vec.push_back(Kernel_Utils::GetHostname().c_str());
//#if defined(_DEBUG_) || defined(_DEBUG)
//	cerr << "ResourcesManager_cpp::GetFittingResources : " << vec.size() << endl;
//#endif
      }
	
    else if (_resourcesList.find(hostname) != _resourcesList.end())
      {
	// --- params.hostname is in the list of resources so return it.
	vec.push_back(hostname);
      }
        
    else if (_resourcesBatchList.find(hostname) != _resourcesBatchList.end())
    {
      // --- params.hostname is in the list of resources so return it.
      vec.push_back(hostname);
    }

    else
      {
	// Cas d'un cluster: nombre de noeuds > 1
	int cpt=0;
	for (map<string, ParserResourcesType>::const_iterator iter = _resourcesList.begin(); iter != _resourcesList.end(); iter++){
	  if( (*iter).second.DataForSort._nbOfNodes > 1 ){
	    if( strncmp(hostname,(*iter).first.c_str(),strlen(hostname)) == 0 ){
	      vec.push_back((*iter).first.c_str());
	      cpt++;
	    }
	  }
	}
	if(cpt==0){
	  // --- user specified an unknown hostame so notify him.
#if defined(_DEBUG_) || defined(_DEBUG)
	  cerr << "ResourcesManager_cpp::GetFittingResources : SALOME_Exception" << endl;
#endif
	  throw ResourcesException("unknown host");
	}
      }
  }
    
  else{
    // --- Search for available resources sorted by priority
    vec=params.computerList;

    SelectOnlyResourcesWithOS(vec, params.OS.c_str());
      
    KeepOnlyResourcesWithComponent(vec, params.componentList);

    //if hosts list (vec) is empty, ignore componentList constraint and use only OS constraint
    if (vec.size() == 0)
      SelectOnlyResourcesWithOS(vec, params.OS.c_str());
    
    // --- set wanted parameters
    ResourceDataToSort::_nbOfNodesWanted = params.nb_node;
      
    ResourceDataToSort::_nbOfProcPerNodeWanted = params.nb_proc_per_node;
	
    ResourceDataToSort::_CPUFreqMHzWanted = params.cpu_clock;
	
    ResourceDataToSort::_memInMBWanted = params.mem_mb;
	
    // --- end of set
	
    list<ResourceDataToSort> li;
	
    for (vector<string>::iterator iter = vec.begin();
	 iter != vec.end();
	 iter++)
      li.push_back(_resourcesList[(*iter)].DataForSort);
	
    li.sort();
	
    unsigned int i = 0;
	
    for (list<ResourceDataToSort>::iterator iter2 = li.begin();
	 iter2 != li.end();
	 iter2++)
      vec[i++] = (*iter2)._hostName;
  }
    
  return vec;

}

//=============================================================================
/*!
 *  add an entry in the ressources catalog  xml file.
 *  Return 0 if OK (KERNEL found in new resources components) else throw exception
 */ 
//=============================================================================

int
ResourcesManager_cpp::
AddResourceInCatalog(const machineParams& paramsOfNewResources,
                     const vector<string>& componentsOnNewResources,
                     const char *alias,
                     const char *userName,
                     AccessModeType mode,
                     AccessProtocolType prot)
throw(ResourcesException)
{
  vector<string>::const_iterator iter = find(componentsOnNewResources.begin(),
					     componentsOnNewResources.end(),
					     "KERNEL");

  if (iter != componentsOnNewResources.end())
    {
      ParserResourcesType newElt;
      newElt.DataForSort._hostName = paramsOfNewResources.hostname;
      newElt.Alias = alias;
      newElt.Protocol = prot;
      newElt.Mode = mode;
      newElt.UserName = userName;
      newElt.ComponentsList = componentsOnNewResources;
      newElt.OS = paramsOfNewResources.OS;
      newElt.DataForSort._memInMB = paramsOfNewResources.mem_mb;
      newElt.DataForSort._CPUFreqMHz = paramsOfNewResources.cpu_clock;
      newElt.DataForSort._nbOfNodes = paramsOfNewResources.nb_node;
      newElt.DataForSort._nbOfProcPerNode =
        paramsOfNewResources.nb_proc_per_node;
      _resourcesList[newElt.DataForSort._hostName] = newElt;
      return 0;
    }

  else
    throw ResourcesException("KERNEL is not present in this resource");
}

//=============================================================================
/*!
 *  Deletes a resource from the catalog
 */ 
//=============================================================================

void ResourcesManager_cpp::DeleteResourceInCatalog(const char *hostname)
{
  _resourcesList.erase(hostname);
}

//=============================================================================
/*!
 *  write the current data in memory in file.
 */ 
//=============================================================================

void ResourcesManager_cpp::WriteInXmlFile()
{
  const char* aFilePath = _path_resources.c_str();
  
  FILE* aFile = fopen(aFilePath, "w");

  if (aFile == NULL)
    {
#if defined(_DEBUG_) || defined(_DEBUG)
      cerr << "Error opening file !"  << endl;
#endif
      return;
    }
  
  xmlDocPtr aDoc = xmlNewDoc(BAD_CAST "1.0");
  xmlNewDocComment(aDoc, BAD_CAST "ResourcesCatalog");

  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList, _resourcesBatchList);
  handler->PrepareDocToXmlFile(aDoc);
  delete handler;

#if defined(_DEBUG_) || defined(_DEBUG)
  int isOk = xmlSaveFile(aFilePath, aDoc);
  if (!isOk) cerr << "Error while XML file saving." << endl;
#else
  xmlSaveFile(aFilePath, aDoc);
#endif
  
  // Free the document
  xmlFreeDoc(aDoc);

  fclose(aFile);
  
#if defined(_DEBUG_) || defined(_DEBUG)
  cerr << "WRITING DONE!" << endl;
#endif
}

//=============================================================================
/*!
 *  parse the data type catalog
 */ 
//=============================================================================

const MapOfParserResourcesType& ResourcesManager_cpp::ParseXmlFile()
{
  //Parse file only if its modification time is greater than lasttime (last registered modification time) 
  static time_t lasttime=0;
  struct stat statinfo;
  int result = stat(_path_resources.c_str(), &statinfo);
  if (result < 0) return _resourcesList;
  if(statinfo.st_mtime <= lasttime)
    return _resourcesList;
  lasttime=statinfo.st_mtime;

  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList, _resourcesBatchList);

  const char* aFilePath = _path_resources.c_str();
  FILE* aFile = fopen(aFilePath, "r");
  
  if (aFile != NULL)
    {
      xmlDocPtr aDoc = xmlReadFile(aFilePath, NULL, 0);
      
      if (aDoc != NULL)
	handler->ProcessXmlDocument(aDoc);
#if defined(_DEBUG_) || defined(_DEBUG)
      else
	cerr << "ResourcesManager_cpp: could not parse file "<< aFilePath << endl;
#endif
      
      // Free the document
      xmlFreeDoc(aDoc);

      fclose(aFile);
    }
#if defined(_DEBUG_) || defined(_DEBUG)
  else
    cerr << "ResourcesManager_cpp: file "<<aFilePath<<" is not readable." << endl;
#endif
  
  delete handler;

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

string ResourcesManager_cpp::Find(const std::string& policy, const std::vector<std::string>& listOfMachines)
{
  if(_resourceManagerMap.count(policy)==0)
    return _resourceManagerMap[""]->Find(listOfMachines,_resourcesList);
  return _resourceManagerMap[policy]->Find(listOfMachines,_resourcesList);
}

//=============================================================================
/*!
 *  Gives a sublist of machines with matching OS.
 *  If parameter OS is empty, gives the complete list of machines
 */ 
//=============================================================================

// Warning need an updated parsed list : _resourcesList
void ResourcesManager_cpp::SelectOnlyResourcesWithOS( vector<string>& hosts,  const char *OS) const
throw(ResourcesException)
{
  string base(OS);

  if(hosts.size()==0)
    {
      //No constraint on computer list : take all known resources with OS
      map<string, ParserResourcesType>::const_iterator iter;
      for (iter = _resourcesList.begin(); iter != _resourcesList.end(); iter++)
        {
          if ( (*iter).second.OS == base || base.size() == 0)
            hosts.push_back((*iter).first);
        }
    }
  else
    {
      //a computer list is given : take only resources with OS on those computers
      vector<string> vec=hosts;
      hosts.clear();
      vector<string>::iterator iter;
      for (iter = vec.begin(); iter != vec.end(); iter++)
        {
          MapOfParserResourcesType::const_iterator it = _resourcesList.find(*iter);
          if(it != _resourcesList.end())
            if ( (*it).second.OS == base || base.size() == 0 )
              hosts.push_back(*iter);
        }
    }
}


//=============================================================================
/*!
 *  Gives a sublist of machines on which the component is known.
 */ 
//=============================================================================

//Warning need an updated parsed list : _resourcesList
void ResourcesManager_cpp::KeepOnlyResourcesWithComponent( vector<string>& hosts, const vector<string>& componentList) const
throw(ResourcesException)
{
  for (vector<string>::iterator iter = hosts.begin(); iter != hosts.end();)
    {
      MapOfParserResourcesType::const_iterator it = _resourcesList.find(*iter);
      const vector<string>& mapOfComponentsOfCurrentHost = (((*it).second).ComponentsList);

      bool erasedHost = false;
      if( mapOfComponentsOfCurrentHost.size() > 0 ){
	for(unsigned int i=0;i<componentList.size();i++){
          const char* compoi = componentList[i].c_str();
	  vector<string>::const_iterator itt = find(mapOfComponentsOfCurrentHost.begin(),
					      mapOfComponentsOfCurrentHost.end(),
					      compoi);
	  if (itt == mapOfComponentsOfCurrentHost.end()){
	    erasedHost = true;
	    break;
	  }
	}
      }
      if(erasedHost)
        hosts.erase(iter);
      else
        iter++;
    }
}


ParserResourcesType ResourcesManager_cpp::GetResourcesList(const std::string& machine)
{
  if (_resourcesList.find(machine) != _resourcesList.end())
    return _resourcesList[machine];
  else
    return _resourcesBatchList[machine];
}
