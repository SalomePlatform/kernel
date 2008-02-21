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
#ifndef __SALOME_LAUNCHER_HXX__
#define __SALOME_LAUNCHER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "SALOME_ContainerManager.hxx"
#include "BatchLight_BatchManager.hxx"

#include <string>

class SALOME_NamingService;

#if defined LAUNCHER_EXPORTS
#if defined WIN32
#define LAUNCHER_EXPORT __declspec( dllexport )
#else
#define LAUNCHER_EXPORT
#endif
#else
#if defined WNT
#define LAUNCHER_EXPORT __declspec( dllimport )
#else
#define LAUNCHER_EXPORT
#endif
#endif

class LAUNCHER_EXPORT SALOME_Launcher:
  public POA_Engines::SalomeLauncher,
  public PortableServer::RefCountServantBase
{

public:
  SALOME_Launcher(CORBA::ORB_ptr orb, PortableServer::POA_var poa);
  ~SALOME_Launcher();

  CORBA::Long submitSalomeJob(const char * fileToExecute ,
			      const Engines::FilesList& filesToExport ,
			      const Engines::FilesList& filesToImport ,
			      const Engines::BatchParameters& batch_params,
			      const Engines::MachineParameters& params);

  char* querySalomeJob( const CORBA::Long jobId, const Engines::MachineParameters& params);
  void deleteSalomeJob( const CORBA::Long jobId, const Engines::MachineParameters& params);
  void getResultSalomeJob( const char * directory, const CORBA::Long jobId, const Engines::MachineParameters& params );

  void Shutdown();

  CORBA::Long getPID();

  static const char *_LauncherNameInNS;

protected:
  BatchLight::BatchManager *FactoryBatchManager( const Engines::MachineParameters* params ) throw(SALOME_Exception);

  std::map <std::string,BatchLight::BatchManager*> _batchmap;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  SALOME_ContainerManager *_ContManager;
  SALOME_ResourcesManager *_ResManager;
  SALOME_NamingService *_NS;
};

#endif
