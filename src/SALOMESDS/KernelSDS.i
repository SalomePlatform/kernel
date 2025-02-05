// Copyright (C) 2021-2025  CEA, EDF
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

%module KernelSDS

%include "std_string.i"

%{
#include "KernelSDS.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "SALOME_CPythonHelper.hxx"
%}


class SALOME_Exception
{
  public:
  SALOME_Exception(const std::string& text);
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
    SWIG_Python_Raise(SWIG_NewPointerObj((new SALOME_Exception(static_cast< const SALOME_Exception& >(_e.what()))),SWIGTYPE_p_SALOME_Exception,SWIG_POINTER_OWN), "SALOME_Exception", SWIGTYPE_p_SALOME_Exception);
    SWIG_fail;
  }
}

%inline
{
  void KillCPythonHelper()
  {
    SALOME_CPythonHelper::KillSingleton();
  }
  std::string GetDSMInstanceInternal(PyObject *argv)
  {
    if(!PyList_Check(argv))
      THROW_SALOME_EXCEPTION("Not a pylist");
    Py_ssize_t sz=PyList_Size(argv);
    std::vector<std::string> argvCpp(sz);
    for(Py_ssize_t i = 0 ; i < sz ; ++i)
    {
      PyObject *obj = PyList_GetItem(argv,i);
      if(!PyUnicode_Check(obj))
        THROW_SALOME_EXCEPTION("Not a pylist of strings");
      {
        Py_ssize_t dummy;
        argvCpp[i] = PyUnicode_AsUTF8AndSize(obj,&dummy);
      }
    }
    return GetDSMInstanceInternal(argvCpp);
  }
}

%pythoncode %{
def GetDSMInstance(argv):
  import SALOME
  import CORBA
  orb=CORBA.ORB_init([''])
  return orb.string_to_object(GetDSMInstanceInternal(argv))
%}
