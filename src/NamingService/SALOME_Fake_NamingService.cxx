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
#include "SALOME_KernelORB.hxx"

#include CORBA_CLIENT_HEADER(SALOME_Component)

#include <sstream>
#include <fstream>

std::mutex SALOME_Fake_NamingService::_mutex;
std::map<std::string,CORBA::Object_var> SALOME_Fake_NamingService::_map;
bool SALOME_Fake_NamingService::_log_container_file_thread_launched = false;
std::string SALOME_Fake_NamingService::_log_container_file_name;

SALOME_Fake_NamingService::SALOME_Fake_NamingService(CORBA::ORB_ptr orb)
{
}

std::vector< std::string > SALOME_Fake_NamingService::repr()
{
  std::lock_guard<std::mutex> g(_mutex);
  std::vector< std::string > ret;
  for(auto it : _map)
  {
    ret.push_back( it.first );
  }
  return ret;
}

void SALOME_Fake_NamingService::init_orb(CORBA::ORB_ptr orb)
{
}

void SALOME_Fake_NamingService::Register(CORBA::Object_ptr ObjRef, const char* Path)
{
  std::lock_guard<std::mutex> g(_mutex);
  CORBA::Object_var ObjRefAuto = CORBA::Object::_duplicate(ObjRef);
  _map[Path] = ObjRefAuto;
  FlushLogContainersFile_NoThreadSafe();
}

void SALOME_Fake_NamingService::Destroy_Name(const char* Path)
{
  std::lock_guard<std::mutex> g(_mutex);
  std::string pathCpp(Path);
  auto it = _map.find(pathCpp);
  if(it!=_map.end())
    _map.erase(it);
}

void SALOME_Fake_NamingService::Destroy_Directory(const char* Path)
{
}

void SALOME_Fake_NamingService::Destroy_FullDirectory(const char* Path)
{
}

bool SALOME_Fake_NamingService::Change_Directory(const char* Path)
{
  std::lock_guard<std::mutex> g(_mutex);
  _current_dir = Path;
  return true;
}

std::vector<std::string> SALOME_Fake_NamingService::SplitDir(const std::string& fullPath)
{
  constexpr char SEP = '/';
  std::vector<std::string> ret;
  if(fullPath.empty())
    THROW_SALOME_EXCEPTION("Empty input string");
  if(fullPath.at(0) != SEP)
    THROW_SALOME_EXCEPTION("input string is expected to start with " << SEP);
  auto len = fullPath.length();
  if( len == 1 )
    THROW_SALOME_EXCEPTION("input string is expected to be different from " << SEP);
  std::size_t pos = 1;
  while(pos < len)
  {
    std::size_t endPos = fullPath.find_first_of(SEP,pos);
    std::string elt(fullPath.substr(pos,endPos==std::string::npos?std::string::npos:endPos-pos));
    ret.push_back(elt);
    pos = endPos==std::string::npos?std::string::npos:endPos+1;
  }
  return ret;
}

std::vector<std::string> SALOME_Fake_NamingService::list_subdirs()
{
  return std::vector<std::string>();
}

std::vector<std::string> SALOME_Fake_NamingService::list_directory()
{
  std::lock_guard<std::mutex> g(_mutex);
  std::vector<std::string> ret;
  std::vector<std::string> splitCWD(SplitDir(_current_dir));
  for(auto it : _map)
  {
    std::vector<std::string> splitIt(SplitDir(it.first));
    if(splitIt.size()<=splitCWD.size())
      continue;
    std::vector<std::string> partSplitIt(splitIt.cbegin(),splitIt.cbegin()+splitCWD.size());
    if(partSplitIt!=splitCWD)
      continue;
    ret.push_back(splitIt.at(splitCWD.size()));
  }
  return ret;
}

std::vector<std::string> SALOME_Fake_NamingService::list_directory_recurs()
{
  std::vector<std::string> result;
  for(const std::pair< std::string,CORBA::Object_var>& it : _map)
    if( it.first.length() >= _current_dir.length() &&
        it.first.compare(0, _current_dir.length(), _current_dir) == 0)
      result.push_back(it.first);
  return result;
}

CORBA::Object_ptr SALOME_Fake_NamingService::Resolve(const char* Path)
{
  std::lock_guard<std::mutex> g(_mutex);
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
  std::ostringstream oss;
  oss << SEP << "Containers" << SEP << hostname << SEP << containerName << SEP << componentName;
  std::string entryToFind(oss.str());
  return Resolve(entryToFind.c_str());
}

std::vector< std::pair< std::string, Engines::Container_var> > SALOME_Fake_NamingService::ListOfContainersInNS_NoThreadSafe()
{
  std::vector< std::pair< std::string, Engines::Container_var> > ret;
  for(auto it : _map)
  {
    Engines::Container_var elt = Engines::Container::_narrow(it.second);
    if(!CORBA::is_nil(elt))
      ret.push_back({it.first,elt});
  }
  return ret;
}

std::string SALOME_Fake_NamingService::ReprOfContainersIORS_NoThreadSafe()
{
  std::vector< std::pair< std::string, Engines::Container_var> > conts( ListOfContainersInNS_NoThreadSafe() );
  std::ostringstream oss;
  CORBA::ORB_ptr orb = KERNEL::getORB();
  char SEP[2] = { '\0', '\0' };
  constexpr char SEP2[] = " : ";
  for(auto it : conts)
  {
    CORBA::String_var ior(orb->object_to_string(it.second));
    oss << SEP << it.first << SEP2 << ior;
    SEP[0] = '\n';
  }
  return oss.str();
}

std::string SALOME_Fake_NamingService::ReprOfContainersIORS()
{
  std::lock_guard<std::mutex> g(_mutex);
  return ReprOfContainersIORS_NoThreadSafe();
}

std::string SALOME_Fake_NamingService::GetLogContainersFile()
{
  return _log_container_file_name;
}

void SALOME_Fake_NamingService::FlushLogContainersFile()
{
  std::lock_guard<std::mutex> g(_mutex);
  FlushLogContainersFile_NoThreadSafe();
}

void SALOME_Fake_NamingService::FlushLogContainersFile_NoThreadSafe()
{
  if(!_log_container_file_name.empty())
  {
    std::string content( ReprOfContainersIORS_NoThreadSafe() );
    std::ofstream ofs(_log_container_file_name);
    ofs.write(content.c_str(),content.length());
  }
}

void SALOME_Fake_NamingService::SetLogContainersFile(const std::string& logFileName)
{
  if(logFileName.empty())
    THROW_SALOME_EXCEPTION("SALOME_Fake_NamingService::SetLogContainersFile : empty log name !");
  constexpr char EXPT_CONTENT[] = "SALOME_Fake_NamingService::SetLogContainersFile : input logFileName write access failed ! no log file set !";
  {
    std::ofstream ofs(logFileName);
    if(!ofs)
      THROW_SALOME_EXCEPTION(EXPT_CONTENT);
  }
  _log_container_file_name = logFileName;
}
