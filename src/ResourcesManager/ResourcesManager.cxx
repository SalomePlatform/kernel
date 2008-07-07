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
#include "ResourcesManager.hxx" 
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

#include "utilities.h"

#define MAX_SIZE_FOR_HOSTNAME 256;

using namespace std;

//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

ResourcesManager_cpp::
ResourcesManager_cpp(const char *xmlFilePath) :
    _path_resources(xmlFilePath)
{
  MESSAGE ( "ResourcesManager_cpp constructor" );
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

ResourcesManager_cpp::ResourcesManager_cpp()
{
  MESSAGE ( "ResourcesManager_cpp constructor" );
  _isAppliSalomeDefined = (getenv("APPLI") != 0);

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
  MESSAGE ( "ResourcesManager_cpp constructor end" );
}

//=============================================================================
/*!
 *  Standard Destructor
 */ 
//=============================================================================

ResourcesManager_cpp::~ResourcesManager_cpp()
{
  MESSAGE ( "ResourcesManager_cpp destructor" );
}

//=============================================================================
/*!
 *  get the list of name of ressources fitting for the specified module.
 *  If hostname specified, check it is local or known in resources catalog.
 *
 *  Else
 *  - select first machines with corresponding OS (all machines if
 *    parameter OS empty),
 *  - then select the sublist of machines on witch the module is known
 *    (if the result is empty, that probably means that the inventory of
 *    modules is probably not done, so give complete list from previous step)
 */ 
//=============================================================================

std::vector<std::string> 
ResourcesManager_cpp::GetFittingResources(const machineParams& params,
				      const std::vector<std::string>& componentList) throw(ResourcesException)
{
//   cerr << "ResourcesManager_cpp::GetFittingResources" << endl;
  vector <std::string> vec;

  ParseXmlFile();

  const char *hostname = params.hostname.c_str();
  INFOS ( "GetFittingResources " << hostname << " " << GetHostname().c_str() );

  if (hostname[0] != '\0'){
    //       cerr << "ResourcesManager_cpp::GetFittingResources : hostname specified" << endl;

    if ( strcmp(hostname, "localhost") == 0 ||
	 strcmp(hostname, GetHostname().c_str()) == 0 )
      {
	//           cerr << "ResourcesManager_cpp::GetFittingResources : localhost" << endl;
	vec.push_back(GetHostname().c_str());
	// 	  cerr << "ResourcesManager_cpp::GetFittingResources : " << vec.size() << endl;
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
	      //cerr << "SALOME_ResourcesManager_cpp::GetFittingResources vector["
	      //     << cpt << "] = " << (*iter).first.c_str() << endl ;
	      cpt++;
	    }
	  }
	}
	if(cpt==0){
	  // --- user specified an unknown hostame so notify him.
	  MESSAGE ( "ResourcesManager_cpp::GetFittingResources : SALOME_Exception" );
	  throw ResourcesException("unknown host");
	}
      }
  }
    
  else{
    // --- Search for available resources sorted by priority
    SelectOnlyResourcesWithOS(vec, params.OS.c_str());
      
    KeepOnlyResourcesWithModule(vec, componentList);
	
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
 *  Return 0 if OK (KERNEL found in new resources modules) else throw exception
 */ 
//=============================================================================

int
ResourcesManager_cpp::
AddResourceInCatalog(const machineParams& paramsOfNewResources,
                     const vector<string>& modulesOnNewResources,
                     const char *alias,
                     const char *userName,
                     AccessModeType mode,
                     AccessProtocolType prot)
throw(ResourcesException)
{
  vector<string>::const_iterator iter = find(modulesOnNewResources.begin(),
					     modulesOnNewResources.end(),
					     "KERNEL");

  if (iter != modulesOnNewResources.end())
    {
      ParserResourcesType newElt;
      newElt.DataForSort._hostName = paramsOfNewResources.hostname;
      newElt.Alias = alias;
      newElt.Protocol = prot;
      newElt.Mode = mode;
      newElt.UserName = userName;
      newElt.ModulesList = modulesOnNewResources;
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
      INFOS ( "Error opening file !" );
      return;
    }
  
  xmlDocPtr aDoc = xmlNewDoc(BAD_CAST "1.0");
  xmlNewDocComment(aDoc, BAD_CAST "ResourcesCatalog");

  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList, _resourcesBatchList);
  handler->PrepareDocToXmlFile(aDoc);
  delete handler;

  int isOk = xmlSaveFile(aFilePath, aDoc);
  
  if (!isOk)
    INFOS ( "Error while XML file saving." );
  
  // Free the document
  xmlFreeDoc(aDoc);

  fclose(aFile);
  
  INFOS ( "WRITING DONE!" );
}

//=============================================================================
/*!
 *  parse the data type catalog
 */ 
//=============================================================================

const MapOfParserResourcesType& ResourcesManager_cpp::ParseXmlFile()
{
  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList, _resourcesBatchList);

  const char* aFilePath = _path_resources.c_str();
  FILE* aFile = fopen(aFilePath, "r");
  
  if (aFile != NULL)
    {
      xmlDocPtr aDoc = xmlReadFile(aFilePath, NULL, 0);
      
      if (aDoc != NULL)
	handler->ProcessXmlDocument(aDoc);
      else
	INFOS ( "ResourcesManager_cpp: could not parse file "<< aFilePath );
      
      // Free the document
      xmlFreeDoc(aDoc);

      fclose(aFile);
    }
  else
    INFOS ( "ResourcesManager_cpp: file "<<aFilePath<<" is not readable." );
  
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


//=============================================================================
/*!
 *  dynamically obtains the first machines
 */ 
//=============================================================================

string ResourcesManager_cpp::FindFirst(const std::vector<std::string>& listOfMachines)
{
  return _dynamicResourcesSelecter.FindFirst(listOfMachines);
}

//=============================================================================
/*!
 *  dynamically obtains the best machines
 */ 
//=============================================================================

string ResourcesManager_cpp::FindNext(const std::vector<std::string>& listOfMachines)
{
  return _dynamicResourcesSelecter.FindNext(listOfMachines,_resourcesList);
}
//=============================================================================
/*!
 *  dynamically obtains the best machines
 */ 
//=============================================================================

string ResourcesManager_cpp::FindBest(const std::vector<std::string>& listOfMachines)
{
  return _dynamicResourcesSelecter.FindBest(listOfMachines);
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

  for (map<string, ParserResourcesType>::const_iterator iter =
         _resourcesList.begin();
       iter != _resourcesList.end();
       iter++)
    {
      if ( (*iter).second.OS == base || base.size() == 0)
        hosts.push_back((*iter).first);
    }
}


//=============================================================================
/*!
 *  Gives a sublist of machines on which the module is known.
 */ 
//=============================================================================

//Warning need an updated parsed list : _resourcesList
void ResourcesManager_cpp::KeepOnlyResourcesWithModule( vector<string>& hosts, const vector<string>& componentList) const
throw(ResourcesException)
{
  for (vector<string>::iterator iter = hosts.begin(); iter != hosts.end();)
    {
      MapOfParserResourcesType::const_iterator it = _resourcesList.find(*iter);
      const vector<string>& mapOfModulesOfCurrentHost = (((*it).second).ModulesList);

      bool erasedHost = false;
      if( mapOfModulesOfCurrentHost.size() > 0 ){
	for(int i=0;i<componentList.size();i++){
          const char* compoi = componentList[i].c_str();
	  vector<string>::const_iterator itt = find(mapOfModulesOfCurrentHost.begin(),
					      mapOfModulesOfCurrentHost.end(),
					      compoi);
// 					      componentList[i]);
	  if (itt == mapOfModulesOfCurrentHost.end()){
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

std::string ResourcesManager_cpp::GetHostname()
{
  int ls = 100, r = 1;
  char *s;

  while (ls < 10000 && r) {
    ls *= 2;
    s = new char[ls];
    r = gethostname(s, ls-1);
    switch (r) 
      {
      case 0:
	  break;
      default:
#ifdef EINVAL
      case EINVAL:
#endif
#ifdef ENAMETOOLONG
      case ENAMETOOLONG:
#endif
        delete [] s;
	continue;
      }
  }

  if (r != 0) {
    s = new char[50];
    strcpy(s, "localhost");
  }

  // remove all after '.'
  char *aDot = (strchr(s,'.'));
  if (aDot) aDot[0] = '\0';

  string p = s;
  delete [] s;
  return p;
}

