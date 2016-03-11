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

#ifndef __SALOME_RESOURCESMANAGER_HXX__
#define __SALOME_RESOURCESMANAGER_HXX__

#include "Utils_SALOME_Exception.hxx"
#include "utilities.h"
#include <SALOMEconfig.h>
#include "SALOME_NamingService.hxx"
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include <string>
#include <fstream>
#include <vector>
#include "ResourcesManager.hxx"

#include "SALOME_ResourcesManager_Common.hxx"

#ifdef WIN32
#pragma warning(disable:4275) // Disable warning interface non dll
#endif

// --- WARNING ---
// The call of BuildTempFileToLaunchRemoteContainer and RmTmpFile must be done
// in a critical section to be sure to be clean.
// Only one thread should use the SALOME_ResourcesManager class in a SALOME
// session.

class SALOMERESOURCESMANAGER_EXPORT SALOME_ResourcesManager:
  public POA_Engines::ResourcesManager,
  public PortableServer::RefCountServantBase
  {

  public:

    SALOME_ResourcesManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_NamingService *ns, const char *xmlFilePath);
    SALOME_ResourcesManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_NamingService *ns);

    ~SALOME_ResourcesManager();

    // CORBA Methods
    Engines::ResourceList * GetFittingResources(const Engines::ResourceParameters& params);
    char* FindFirst(const Engines::ResourceList& listOfResources);
    char* Find(const char *policy, const Engines::ResourceList& listOfResources);
    Engines::ResourceDefinition * GetResourceDefinition(const char * name);
    void AddResource(const Engines::ResourceDefinition& new_resource,
                     CORBA::Boolean write,
                     const char * xml_file);

    void RemoveResource(const char * resource_name,
                        CORBA::Boolean write,
                        const char * xml_file);
    char* getMachineFile(const char * resource_name,
                         CORBA::Long nb_procs,
                         const char * parallelLib);

    // Cpp Methods
    void Shutdown();
    ResourcesManager_cpp *GetImpl() { return &_rm; }


    static const char *_ResourcesManagerNameInNS;
  protected:

    SALOME_NamingService *_NS;
    CORBA::ORB_var _orb;
    PortableServer::POA_var _poa;

    //
    //! will contain the informations on the data type catalog(after parsing)
    MapOfParserResourcesType _resourcesBatchList;

    ResourcesManager_cpp _rm;
  };

#endif // RESSOURCESCATALOG_IMPL_H
