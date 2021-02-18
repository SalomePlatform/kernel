// Copyright (C) 2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#pragma once

#include "SALOME_NamingService_defs.hxx"

#include "omniORB4/CORBA.h"

#include <vector>
#include <string>

class NAMINGSERVICE_EXPORT SALOME_NamingService_Abstract
{
public:
  virtual void init_orb(CORBA::ORB_ptr orb=0) = 0;
  virtual void Register(CORBA::Object_ptr ObjRef, const char* Path) = 0;
  virtual CORBA::Object_ptr Resolve(const char* Path) = 0;
  virtual CORBA::Object_ptr ResolveFirst(const char* Path) = 0; 
  virtual void Destroy_Name(const char* Path) = 0;
  virtual void Destroy_Directory(const char* Path) = 0;
  virtual void Destroy_FullDirectory(const char* Path) = 0;
  virtual bool Change_Directory(const char* Path) = 0;
  virtual std::vector<std::string> list_subdirs() = 0;
  virtual std::vector<std::string> list_directory() = 0;
  virtual SALOME_NamingService_Abstract *clone() = 0;
  virtual CORBA::Object_ptr ResolveComponent(const char* hostname, const char* containerName, const char* componentName, const int nbproc=0) = 0;
  virtual ~SALOME_NamingService_Abstract() { }
  static std::string ContainerName(const char *ContainerName);
  static std::string BuildContainerNameForNS(const char *ContainerName, const char *hostname);
};
