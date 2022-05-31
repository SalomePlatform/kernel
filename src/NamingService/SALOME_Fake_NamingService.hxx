// Copyright (C) 2021-2022  CEA/DEN, EDF R&D
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

#include "omniORB4/CORBA.h"

#include "SALOME_NamingService_Abstract.hxx"

#include <mutex>
#include <utility>
#include <string>
#include <map>

class NAMINGSERVICE_EXPORT SALOME_Fake_NamingService : public SALOME_NamingService_Abstract
{
public:
  SALOME_Fake_NamingService(CORBA::ORB_ptr orb);
  SALOME_Fake_NamingService() = default;
  static void SetLogContainersFile(const std::string& logFileName);
  static std::string GetLogContainersFile();
  static void FlushLogContainersFile();
  bool IsTrueNS() const override { return false; }
  std::vector< std::string > repr() override;
  void init_orb(CORBA::ORB_ptr orb=0) override;
  void Register(CORBA::Object_ptr ObjRef, const char* Path) override;
  CORBA::Object_ptr Resolve(const char* Path) override;
  CORBA::Object_ptr ResolveFirst(const char* Path) override;
  void Destroy_Name(const char* Path) override;
  void Destroy_Directory(const char* Path) override;
  void Destroy_FullDirectory(const char* Path) override;
  bool Change_Directory(const char* Path) override;
  std::vector<std::string> list_subdirs() override;
  std::vector<std::string> list_directory() override;
  std::vector<std::string> list_directory_recurs() override;
  SALOME_NamingService_Abstract *clone() override;
  CORBA::Object_ptr ResolveComponent(const char* hostname, const char* containerName, const char* componentName, const int nbproc=0) override;
private:
  static std::string ReprOfContainersIORS_NoThreadSafe();
  static std::string ReprOfContainersIORS();
  static std::vector< std::pair< std::string, Engines::Container_var> > ListOfContainersInNS_NoThreadSafe();
  static void FlushLogContainersFile_NoThreadSafe();
  static std::vector<std::string> SplitDir(const std::string& fullPath);
private:
  static std::mutex _mutex;
  static std::map<std::string,CORBA::Object_var> _map;
  static bool _log_container_file_thread_launched;
  static std::string _log_container_file_name;
private:
  mutable std::string _current_dir;
};
