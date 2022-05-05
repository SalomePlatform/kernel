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

%module NamingService

%{
#include "SALOME_Fake_NamingService.hxx"
#include "SALOME_Embedded_NamingService.hxx"
#include "SALOME_KernelORB.hxx"
#include "Utils_SALOME_Exception.hxx"
%}

%include "std_string.i"
%include "std_vector.i"

%rename(NamingService) SALOME_Fake_NamingService ;

%template(svec) std::vector<std::string>;

%exceptionclass SALOME_Exception;

class SALOME_Exception
{
public:
  SALOME_Exception(const std::string& text);
  ~SALOME_Exception() noexcept;
  const char *what() const noexcept;
  %extend
  {
    std::string __str__() const
    {
      return std::string(self->what());
    }
  }
};

%exception {
  try {
    $action
  }
  catch (SALOME_Exception& _e) {
    // Reraise with SWIG_Python_Raise
    SWIG_Python_Raise(SWIG_NewPointerObj((new SALOME_Exception(static_cast< const SALOME_Exception& >(_e))),SWIGTYPE_p_SALOME_Exception,SWIG_POINTER_OWN), "SALOME_Exception", SWIGTYPE_p_SALOME_Exception);
    SWIG_fail;
  }
}

class SALOME_Fake_NamingService
{
public:
  SALOME_Fake_NamingService();
  std::vector< std::string > repr();
  static std::string GetLogContainersFile();
  static void FlushLogContainersFile();
  void Destroy_Name(const char* Path);
  void Destroy_FullDirectory(const char* Path);
  %extend {
    static void SetLogContainersFileInternal(const std::string& logFileName)
    {
      SALOME_Fake_NamingService::SetLogContainersFile(logFileName);
    }
    std::string _ResolveInternal(const char *Path)
    {
      CORBA::Object_var obj = self->Resolve(Path);
      CORBA::ORB_ptr orb = KERNEL::getORB();
      CORBA::String_var ior = orb->object_to_string(obj);
      return std::string(ior);
    }
    void _RegisterInternal(const char *ior, const char* Path)
    {
      CORBA::ORB_ptr orb = KERNEL::getORB();
      CORBA::Object_var obj = orb->string_to_object(ior);
      self->Register(obj,Path);
    }
    std::string _Resolve_DirInternal(const char *Path)
    {
      CORBA::ORB_ptr orb = KERNEL::getORB();
      CORBA::Object_var ret = self->Resolve(Path);
      CORBA::String_var ior = orb->object_to_string(ret);
      return std::string(ior.in());
    }
    static std::string IOROfNS()
    {
      CORBA::ORB_ptr orb = KERNEL::getORB();
      Engines::EmbeddedNamingService_var ns = GetEmbeddedNamingService();
      CORBA::String_var ior = orb->object_to_string(ns);
      return std::string(ior);
    }
  }
};

%pythoncode %{
def NamingService_Resolve(self,Path):
  ret = self._ResolveInternal(Path)
  import CORBA
  orb=CORBA.ORB_init([''])
  return orb.string_to_object(ret)
def NamingService_Register(self,obj,Path):
  import CORBA
  orb=CORBA.ORB_init([''])
  self._RegisterInternal( orb.object_to_string(obj) , Path)
def NamingService_Resolve_Dir(self,Path):
  ret = self._Resolve_DirInternal( Path )
  import CORBA
  orb=CORBA.ORB_init([''])
  return orb.string_to_object(ret)
NamingService.Resolve = NamingService_Resolve
NamingService.Register = NamingService_Register
NamingService.Resolve_Dir = NamingService_Resolve_Dir
def NamingService_SetLogContainersFile(cls,logFileName = None):
  if logFileName is None:
    import tempfile
    with tempfile.NamedTemporaryFile() as f:
      logFileName = f.name
  cls.SetLogContainersFileInternal(logFileName)
NamingService.SetLogContainersFile = classmethod(NamingService_SetLogContainersFile)
def NamingService_RefOfNS(cls):
  ret = cls.IOROfNS()
  import Engines
  import CORBA
  orb=CORBA.ORB_init([''])
  return orb.string_to_object(ret)
NamingService.RefOfNS = classmethod(NamingService_RefOfNS)
def NamingService_KillContainersInFile(cls,logFileName):
  import Engines
  import CORBA
  orb=CORBA.ORB_init([''])
  with open(logFileName) as f:
    cont_to_kill = [elt.split(" : ") for elt in f]
    for name,ior in cont_to_kill:
      try:
        ref = orb.string_to_object(ior)
        ref.Shutdown()
      except Exception as e:
        print("Failed to kill container remotely \"{}\"".format(name))
NamingService.KillContainersInFile = classmethod(NamingService_KillContainersInFile)
%}
