//  SALOME NamingService : wrapping NamingService services
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_NamingService.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef SALOME_NAMINGSERVICE_H
#define SALOME_NAMINGSERVICE_H

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include CORBA_CLIENT_HEADER(SALOME_Component)

#include <vector>
#include <string>
#include "utilities.h"
#include "Utils_Mutex.hxx"
#include "ServiceUnreachable.hxx"

#ifdef WIN32
# ifdef NAMINGSERVICE_EXPORTS
#  define NAMINGSERVICE_EXPORT __declspec( dllexport )
# else
#  define NAMINGSERVICE_EXPORT __declspec( dllimport )
# endif
#else
# define NAMINGSERVICE_EXPORT
#endif

class NAMINGSERVICE_EXPORT SALOME_NamingService
{
public:
  SALOME_NamingService();
  SALOME_NamingService(CORBA::ORB_ptr orb);

  virtual ~SALOME_NamingService();

  void init_orb(CORBA::ORB_ptr orb);
  void Register(CORBA::Object_ptr ObjRef,
		const char* Path) 
    throw(ServiceUnreachable);
  CORBA::Object_ptr Resolve(const char* Path)
    throw( ServiceUnreachable); 
  CORBA::Object_ptr ResolveFirst(const char* Path)
    throw( ServiceUnreachable); 
  CORBA::Object_ptr ResolveComponent(const char* hostname,
				     const char* containerName,
				     const char* componentName,
				     const int nbproc=0)
    throw(ServiceUnreachable);
  std::string ContainerName(const char *ContainerName);
  std::string ContainerName(const Engines::MachineParameters& params);
  std::string BuildContainerNameForNS(const char *ContainerName,
				      const char *hostname);
  std::string 
  BuildContainerNameForNS(const Engines::MachineParameters& params,
			  const char *hostname);
  int Find(const char* name)
    throw(ServiceUnreachable);
  bool Create_Directory(const char* Path)
    throw(ServiceUnreachable);
  bool Change_Directory(const char* Path)
    throw(ServiceUnreachable);
  char* Current_Directory()
    throw(ServiceUnreachable);
  void list()
    throw(ServiceUnreachable);
  std::vector<std::string> list_directory()
    throw(ServiceUnreachable);
  std::vector<std::string> list_subdirs()
    throw(ServiceUnreachable);
  std::vector<std::string> list_directory_recurs()
    throw(ServiceUnreachable);
  void Destroy_Name(const char* Path)
    throw(ServiceUnreachable);
  virtual void Destroy_Directory(const char* Path)
    throw(ServiceUnreachable);
  virtual void Destroy_FullDirectory(const char* Path)
    throw(ServiceUnreachable);
  char* getIORaddr();

protected:
  Utils_Mutex _myMutex;
  CORBA::ORB_ptr _orb;
  CosNaming::NamingContext_var _root_context, _current_context;

  void _initialize_root_context();
  int _createContextNameDir(std::string path,
			    CosNaming::Name& context_name,
			    std::vector<std::string>& splitPath,
			    bool onlyDir);
  void _Find(const char* name, CORBA::Long& occurence_number);
  void _current_directory(std::vector<std::string>& splitPath,
			  int& lengthResult,
			  CosNaming::NamingContext_var contextToFind,
			  bool& notFound);
  void _list_directory_recurs(std::vector<std::string>& myList,
			      std::string relativeSubDir,
			      std::string absCurDirectory);

};

#endif // SALOME_NAMINGSERVICE_H

