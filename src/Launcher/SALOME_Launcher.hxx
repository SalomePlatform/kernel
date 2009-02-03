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
#ifndef __SALOME_LAUNCHER_HXX__
#define __SALOME_LAUNCHER_HXX__

#include "SALOME_Launcher_defs.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "Launcher.hxx"

#include <string>

class SALOME_NamingService;
class SALOME_ContainerManager;
class SALOME_ResourcesManager;

class SALOMELAUNCHER_EXPORT SALOME_Launcher:
  public POA_Engines::SalomeLauncher,
  public PortableServer::RefCountServantBase
{

public:
  SALOME_Launcher(CORBA::ORB_ptr orb, PortableServer::POA_var poa);
  ~SALOME_Launcher();

  CORBA::Long submitJob(const char * xmlExecuteFile,
                        const char * clusterName);

  CORBA::Long submitSalomeJob(const char * fileToExecute ,
			      const Engines::FilesList& filesToExport ,
			      const Engines::FilesList& filesToImport ,
			      const Engines::BatchParameters& batch_params,
			      const Engines::MachineParameters& params);

  char* querySalomeJob( CORBA::Long jobId, const Engines::MachineParameters& params);
  void deleteSalomeJob( CORBA::Long jobId, const Engines::MachineParameters& params);
  void getResultSalomeJob( const char * directory, CORBA::Long jobId, const Engines::MachineParameters& params );
  char* queryJob( CORBA::Long jobId, const Engines::MachineParameters& params);
  void deleteJob( CORBA::Long jobId, const Engines::MachineParameters& params);
  void getResultsJob( const char * directory, CORBA::Long jobId, const Engines::MachineParameters& params );

  CORBA::Boolean testBatch(const Engines::MachineParameters& params);

  void Shutdown();

  CORBA::Long getPID();

  static const char *_LauncherNameInNS;

protected:
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  SALOME_ContainerManager *_ContManager;
  SALOME_ResourcesManager *_ResManager;
  SALOME_NamingService *_NS;

  Launcher_cpp _l;
};

#endif
