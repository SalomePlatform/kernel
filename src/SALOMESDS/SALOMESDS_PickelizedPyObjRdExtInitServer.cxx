// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

#include "SALOMESDS_PickelizedPyObjRdExtInitServer.hxx"
#include "SALOMESDS_PickelizedPyObjRdExtServer.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_Exception.hxx"

#include <iostream>
#include <sstream>

using namespace SALOMESDS;

const char PickelizedPyObjRdExtInitServer::ACCESS_REPR[]="RdExtInit";

PickelizedPyObjRdExtInitServer::PickelizedPyObjRdExtInitServer(DataScopeServerBase *father, const std::string& varName, const SALOME_CMOD::ByteVec& value):PickelizedPyObjServerModifiable(father,varName,value),_nb_of_clients(0)
{
  _self_deep_copy=DeepCopyPyObj(_self);
}

//! obj is consumed
PickelizedPyObjRdExtInitServer::PickelizedPyObjRdExtInitServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj):PickelizedPyObjServerModifiable(father,varName,obj),_self_deep_copy(0),_nb_of_clients(0)
{
  _self_deep_copy=DeepCopyPyObj(obj);
}

PickelizedPyObjRdExtInitServer::~PickelizedPyObjRdExtInitServer()
{
  Py_XDECREF(_self_deep_copy);
}

PickelizedPyObjRdExtServer *PickelizedPyObjRdExtInitServer::buildStdInstanceFrom(const std::string& varName)
{
  PyObject *pyobj(this->getPyObj()); Py_XINCREF(pyobj);
  return new PickelizedPyObjRdExtServer(getFather(),varName,pyobj);
}

std::string PickelizedPyObjRdExtInitServer::getAccessStr() const
{
  return std::string(ACCESS_REPR);
}

SALOME_CMOD::ByteVec *PickelizedPyObjRdExtInitServer::fetchSerializedContent()
{
  Py_XINCREF(_self_deep_copy);//because pickelize consume _self_deep_copy
  return FromCppToByteSeq(pickelize(_self_deep_copy));
}

PyObject *PickelizedPyObjRdExtInitServer::DeepCopyPyObj(PyObject *pyobj)
{
  PyObject *mod(PyImport_ImportModule("copy"));//new value
  PyObject *meth(PyObject_GetAttrString(mod,"deepcopy"));//new value
  PyObject *args(PyTuple_New(1));
  Py_XINCREF(pyobj); PyTuple_SetItem(args,0,pyobj);
  PyObject *ret(PyObject_CallObject(meth,args));
  Py_XDECREF(args);
  Py_XDECREF(meth);
  Py_XDECREF(mod);
  return ret;
}

PickelizedPyObjRdExtServer *PickelizedPyObjRdExtInitFreeStyleServer::buildStdInstanceFrom(const std::string& varName)
{
  PyObject *pyobj(this->getPyObj()); Py_XINCREF(pyobj);
  return new PickelizedPyObjRdExtFreeStyleServer(getFather(),varName,pyobj,std::move(_cmp_func_content),std::move(_cmp_func));
}
