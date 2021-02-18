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

#include "SALOME_Fake_NamingService.hxx"
#include "Utils_SALOME_Exception.hxx"

std::mutex SALOME_Fake_NamingService::_mutex;
std::map<std::string,CORBA::Object_var> SALOME_Fake_NamingService::_map;

SALOME_Fake_NamingService::SALOME_Fake_NamingService(CORBA::ORB_ptr orb)
{
}

void SALOME_Fake_NamingService::init_orb(CORBA::ORB_ptr orb)
{
}

void SALOME_Fake_NamingService::Register(CORBA::Object_ptr ObjRef, const char* Path)
{
  std::lock_guard<std::mutex> g(_mutex);
  CORBA::Object_var ObjRefAuto = CORBA::Object::_duplicate(ObjRef);
  _map[Path] = ObjRefAuto;
}

void SALOME_Fake_NamingService::Destroy_Name(const char* Path)
{
}

void SALOME_Fake_NamingService::Destroy_Directory(const char* Path)
{
}

void SALOME_Fake_NamingService::Destroy_FullDirectory(const char* Path)
{
}

bool SALOME_Fake_NamingService::Change_Directory(const char* Path)
{
  return true;
}

std::vector<std::string> SALOME_Fake_NamingService::list_subdirs()
{
  return std::vector<std::string>();
}

std::vector<std::string> SALOME_Fake_NamingService::list_directory()
{
  return std::vector<std::string>();
}

CORBA::Object_ptr SALOME_Fake_NamingService::Resolve(const char* Path)
{
  std::string pathCpp(Path);
  auto it = _map.find(pathCpp);
  if( it != _map.end() )
    return CORBA::Object::_duplicate((*it).second);
  return CORBA::Object::_nil();
}

CORBA::Object_ptr SALOME_Fake_NamingService::ResolveFirst(const char* Path)
{
  return CORBA::Object::_nil();
}

SALOME_NamingService_Abstract *SALOME_Fake_NamingService::clone()
{
  return new SALOME_Fake_NamingService;
}

CORBA::Object_ptr SALOME_Fake_NamingService::ResolveComponent(const char* hostname, const char* containerName, const char* componentName, const int nbproc)
{
  THROW_SALOME_EXCEPTION("SALOME_Fake_NamingService::ResolveComponent : " << hostname << " " << containerName << " " << componentName << " " << nbproc);
}
