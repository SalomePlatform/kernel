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

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include CORBA_CLIENT_HEADER(SALOME_Component)

#include <vector>
#include <string>

class NAMINGSERVICE_EXPORT SALOME_NamingService_Container_Abstract
{
public:
  virtual void init_orb(CORBA::ORB_ptr orb=0) = 0;
  virtual SALOME_NamingService_Container_Abstract *clone() = 0;
  virtual void Register(CORBA::Object_ptr ObjRef, const char* Path) = 0;
  virtual void Destroy_FullDirectory(const char* Path) = 0;
  virtual void Destroy_Name(const char* Path) = 0;
  virtual CORBA::Object_ptr Resolve(const char* Path) = 0;
  virtual CORBA::Object_ptr ResolveFirst(const char* Path) = 0;
  virtual bool IsTrueNS() const = 0;
  static constexpr char SEP = '/';
};

class NAMINGSERVICE_EXPORT SALOME_NamingService_Abstract : public SALOME_NamingService_Container_Abstract
{
public:
  SALOME_NamingService_Abstract *cloneCoVar();
  virtual std::vector< std::string > repr() = 0;
  virtual void Destroy_Directory(const char* Path) = 0;
  virtual bool Change_Directory(const char* Path) = 0;
  virtual std::vector<std::string> list_subdirs() = 0;
  virtual std::vector<std::string> list_directory() = 0;
  virtual std::vector<std::string> list_directory_recurs() = 0;
  virtual CORBA::Object_ptr ResolveComponent(const char* hostname, const char* containerName, const char* componentName, const int nbproc=0) = 0;
  virtual ~SALOME_NamingService_Abstract() { }
  static std::string ContainerName(const Engines::ContainerParameters& params);
  static std::string ContainerName(const char *ContainerName);
  static std::string BuildContainerNameForNS(const char *ContainerName, const char *hostname);
  static std::string BuildContainerNameForNS(const Engines::ContainerParameters& params, const char *hostname);
  static std::string BuildComponentName(const char* hostname, const char* containerName, const char* componentName, const int nbproc);
};

class SALOME_NamingService_Abstract_Decorator : public SALOME_NamingService_Abstract
{
public:
  SALOME_NamingService_Abstract_Decorator(SALOME_NamingService_Container_Abstract *ns_cont):_ns_cont(ns_cont) { }
  void init_orb(CORBA::ORB_ptr orb=0) override { _ns_cont->init_orb(orb); }
  SALOME_NamingService_Container_Abstract *clone() override { return new SALOME_NamingService_Abstract_Decorator(_ns_cont); }
  void Register(CORBA::Object_ptr ObjRef, const char* Path) override { _ns_cont->Register(ObjRef,Path); }
  void Destroy_FullDirectory(const char* Path) override { _ns_cont->Destroy_FullDirectory(Path); }
  void Destroy_Name(const char* Path) override { _ns_cont->Destroy_Name(Path); }
  CORBA::Object_ptr Resolve(const char* Path) override { return _ns_cont->Resolve(Path); }
  CORBA::Object_ptr ResolveFirst(const char* Path) override { return _ns_cont->ResolveFirst(Path); }
  bool IsTrueNS() const override { return _ns_cont->IsTrueNS(); }
  //
  std::vector< std::string > repr() override;
  void Destroy_Directory(const char* Path) override;
  bool Change_Directory(const char* Path) override;
  std::vector<std::string> list_subdirs() override;
  std::vector<std::string> list_directory() override;
  std::vector<std::string> list_directory_recurs() override;
  CORBA::Object_ptr ResolveComponent(const char* hostname, const char* containerName, const char* componentName, const int nbproc=0) override;
private:
  //! take no ownership of decoree
  SALOME_NamingService_Container_Abstract *_ns_cont = nullptr;
};

