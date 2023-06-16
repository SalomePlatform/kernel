// Copyright (C) 2018-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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
// Author : Anthony GEAY (EDF R&D)

#include "SALOMESDS_Sha1Keeper.hxx"
#include "SALOMESDS_Exception.hxx"

#include <sstream>

void SALOMESDS::Sha1Keeper::checkSame(const std::string& /*varName*/,const std::string& compareFuncContent, PyObject *oldObj, PyObject *newObj)
{
  if(compareFuncContent!=_cmp_func_content)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtFreeStyleServer::checkSame : content of compare func are not exactly the same ! It should !";
      throw NotSameException(oss.str());
    }
  SALOME::AutoPyRef resu(PyObject_CallFunctionObjArgs(_cmp_func,oldObj,newObj,nullptr));
  if(resu.isNull())
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtFreeStyleServer::checkSame : evaluation of function failed !";
      throw NotSameException(oss.str());
    }
  if(resu.get()!=Py_False && resu.get()!=Py_True)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtFreeStyleServer::checkSame : evaluation of function is OK but a boolean is expected !";
      throw NotSameException(oss.str());
    }
  if(resu.get()==Py_False)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtFreeStyleServer::checkSame : comparison function returns False. 2 pybjects are considered different -> createRdExtVarFreeStyleTransac fails !";
      throw NotSameException(oss.str());
    }
}
