#ifndef __SALOME_RESOURCESMANAGER_HXX__
#define __SALOME_RESOURCESMANAGER_HXX__

#include "Utils_SALOME_Exception.hxx"
#include "utilities.h"
#include <SALOMEconfig.h>
#include "SALOME_ResourcesCatalog_Handler.hxx"
#include "SALOME_LoadRateManager.hxx"
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include <string>
#include <fstream>
#include <vector>

//WARNING the call of BuildTempFileToLaunchRemoteContainer and RmTmpFile must be done in a critical section to be sure to be clean.
//Only one thread should use the SALOME_ResourcesManager class in a SALOME session.

class SALOME_ResourcesManager
{
public:
  //! standard constructor
  SALOME_ResourcesManager();

  //!just for test
  SALOME_ResourcesManager(const char *xmlFilePath);
  
  //! standard destructor
  ~SALOME_ResourcesManager();

  //! method to get the list of name of ressources fitting for the specified module.
  std::vector<std::string> GetFittingResources(const Engines::MachineParameters& params,const char *moduleName) throw(SALOME_Exception);

  //! method to dynamically obtain the best machines
  std::string FindBest(const Engines::MachineList& listOfMachines);

  //! method that builds in a temporary file the script to be launched
  std::string BuildTempFileToLaunchRemoteContainer(const std::string& machine,const char *containerName);

  //! method that builds the command to be launched.
  std::string BuildCommandToLaunchLocalContainer(const char *containerName);

  //! method that remove the generated temporary file in case of a remote launch.
  void RmTmpFile();

  //! method that builds the script to be launched
  std::string BuildCommand(const std::string& machine,const char *containerName);

  //! add an entry in the ressources catalog  xml file. Return 1 if OK. Return 0 if the ressource with the same hostname already exists.
  int AddResourceInCatalog(const Engines::MachineParameters& paramsOfNewResources, const std::map<std::string,std::string>& modulesOnNewResources,
			   const char *environPathOfPrerequired,
			   const char *alias, const char *userName, AccessModeType mode, AccessProtocolType prot) throw(SALOME_Exception);
  
  //! delete a ressource from the Catalog.
  void DeleteResourceInCatalog(const char *hostname);

  //! write the current data in memory in file.
  void WriteInXmlFile();

  //! method to parse the data type catalog
  const MapOfParserResourcesType& ParseXmlFile();

  //! to consult the content of the list
  const MapOfParserResourcesType& GetList() const;

private:

  //! method to verify ressources catalog content - return true if verfication is OK
  bool _verify_resources(MapOfParserResourcesType resourceslist);

  //! method that fill hosts with only resources in xml files that are on the specified OS
  void SelectOnlyResourcesWithOS(std::vector<std::string>& hosts,const char *OS) const throw(SALOME_Exception);

  //! method that keep from hosts only those having component of name moduleName.
  void KeepOnlyResourcesWithModule(std::vector<std::string>& hosts,const char *moduleName) const throw(SALOME_Exception);

  //! methode that add to command all options relative to naming service.
  void AddOmninamesParams(std::string& command) const;

  //! method that add to command all options relative to naming service.
  void AddOmninamesParams(std::ofstream& fileStream) const;

  //! method that generate a file name in /tmp directory
  std::string BuildTemporaryFileName() const;

  // will contain the path to the ressources catalog
  QString _path_resources;

  //! attribute that contains current tmp files generated
  std::string _TmpFileName;

  //! attribute that contains the rsh or ssh command to access directly to machine. Only used by this->RmTmpFile in case of a remote launch.
  std::string _CommandForRemAccess;
  
  //will contain the informations on the data type catalog(after parsing)
  MapOfParserResourcesType _resourcesList;

  SALOME_LoadRateManager _dynamicResourcesSelecter;
};

#endif // RESSOURCESCATALOG_IMPL_H
