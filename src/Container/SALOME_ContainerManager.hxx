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
#ifndef __SALOME_CONTAINERMANAGER_HXX__
#define __SALOME_CONTAINERMANAGER_HXX__

#include "SALOME_Container.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_LoadRateManager.hxx"

#include <string>

class SALOME_NamingService;

class CONTAINER_EXPORT SALOME_ContainerManager:
  public POA_Engines::ContainerManager
{

public:
  SALOME_ContainerManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_ResourcesManager *rm, SALOME_NamingService *ns);
  ~SALOME_ContainerManager();

  // Corba Methods
  Engines::Container_ptr
  GiveContainer(const Engines::ContainerParameters& params);

  void ShutdownContainers();

  // C++ Methods
  void Shutdown();

  static const char *_ContainerManagerNameInNS;

protected:
  // C++ methods
  Engines::Container_ptr 
  StartPaCOPPContainer(const Engines::ContainerParameters& params);

  Engines::Container_ptr
  FindContainer(const Engines::ContainerParameters& params,
                const Engines::ResourceList& possibleResources);

  Engines::Container_ptr
  FindContainer(const Engines::ContainerParameters& params,
                const std::string& resource);

  std::string BuildCommandToLaunchRemoteContainer(const std::string & resource_name,
                                                  const Engines::ContainerParameters& params, 
                                                  const std::string& container_exe="SALOME_Container");

  std::string BuildCommandToLaunchLocalContainer(const Engines::ContainerParameters& params, 
                                                 const std::string& container_exe="SALOME_Container");

  std::string BuildTempFileToLaunchRemoteContainer(const std::string& resource_name,
                                                   const Engines::ContainerParameters& params) throw(SALOME_Exception);

  void RmTmpFile(std::string& tmpFile);

  void AddOmninamesParams(std::string& command) const;

  void AddOmninamesParams(std::ostringstream& oss) const;

  void AddOmninamesParams(std::ofstream& fileStream) const;

  std::string BuildTemporaryFileName() const;

  std::string GetMPIZeroNode(std::string machine);

  void machinesFile(const int nbproc);

  // For PacO++ Parallel extension
  typedef std::vector<std::string> actual_launch_machine_t;
  std::string BuildCommandToLaunchParallelContainer(const std::string& exe_name, 
                                                    const Engines::ContainerParameters& params,
                                                    SALOME_ContainerManager::actual_launch_machine_t & vect_machine, 
                                                    const std::string proxy_hostname = ""); 
  CORBA::Object_ptr 
  LaunchParallelContainer(const std::string& command, 
                          const Engines::ContainerParameters& params,
                          const std::string& name,
                          SALOME_ContainerManager::actual_launch_machine_t & vect_machine);
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;

  SALOME_ResourcesManager *_ResManager;
  SALOME_NamingService *_NS;

  //! attribute that contains current tmp files generated
  std::string _TmpFileName;

  //! contains the rsh or ssh command to access directly to machine.
  //  Only used by this->RmTmpFile in case of a remote launch.
  std::string _CommandForRemAccess;

  //! different behaviour if $APPLI exists (SALOME Application) 
  bool _isAppliSalomeDefined;

  //! attribute that contains the number of processes used in batch mode by MPI containers
  int _nbprocUsed;

  //! attributes that contains the machinefile for MPI containers
  std::string _machinesFile;

  static omni_mutex _numInstanceMutex ; // lib and instance protection

};
#endif
