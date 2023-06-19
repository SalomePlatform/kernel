// Copyright (C) 2021-2023  CEA, EDF
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

%module KernelServices

%include "std_string.i"

%{
#include "KernelServices.hxx"
#include "Utils_SALOME_Exception.hxx"
%}

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

%inline
{
    void RegisterCompoInternal(const std::string& compoName, const std::string& compoIOR);
    std::string RetrieveCompoInternal(const std::string& compoName);
    void GenerateViolentMemoryFaultForTestPurpose();
}

%pythoncode %{
def RegisterCompo(compoName,compoRef):
  import CORBA
  orb=CORBA.ORB_init([''])
  RegisterCompoInternal(compoName,orb.object_to_string(compoRef))

def RetrieveCompo(compoName):
  import CORBA
  orb=CORBA.ORB_init([''])
  return orb.string_to_object(RetrieveCompoInternal(compoName))
%}
