// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOMESDS_PickelizedPyObjRdExtServer.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_Exception.hxx"

#include <iostream>
#include <sstream>

using namespace SALOMESDS;

const char PickelizedPyObjRdExtServer::ACCESS_REPR[]="RdExt";

PickelizedPyObjRdExtServer::PickelizedPyObjRdExtServer(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value):PickelizedPyObjServerModifiable(father,varName,value)
{
}

//! obj is consumed
PickelizedPyObjRdExtServer::PickelizedPyObjRdExtServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj):PickelizedPyObjServerModifiable(father,varName,obj)
{
}

PickelizedPyObjRdExtServer::~PickelizedPyObjRdExtServer()
{
}

/*!
 * Called remotely -> to protect against throw
 */
SALOME::PickelizedPyObjRdExtServer_ptr PickelizedPyObjRdExtServer::invokePythonMethodOn(const char *method, const SALOME::ByteVec& args)
{
  if(!_self)
    throw Exception("PickelizedPyObjRdExtServer::invokePythonMethodOn : self is NULL !");
  std::string argsCpp;
  FromByteSeqToCpp(args,argsCpp);
  PyObject *argsPy(getPyObjFromPickled(argsCpp));
  checkRdExtnessOf(method,argsPy);
  //
  PyObject *selfMeth(PyObject_GetAttrString(_self,method));
  if(!selfMeth)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtServer::invokePythonMethodOn : Method \"" << method << "\" is not available !";
      throw Exception(oss.str());
    }
  PyObject *res(PyObject_CallObject(selfMeth,argsPy));// self can have been modified by this call !
  Py_XDECREF(selfMeth);
  Py_XDECREF(argsPy);
  if(!res)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtServer::invokePythonMethodOn : Problem during invokation serverside of Method \"" << method << "\" !";
      throw Exception(oss.str());
    }
  PickelizedPyObjRdExtServer *ret(new PickelizedPyObjRdExtServer(_father,DataScopeServer::BuildTmpVarNameFrom(getVarNameCpp()),res));
  PortableServer::POA_var poa(_father->getPOA());
  PortableServer::ObjectId_var id(poa->activate_object(ret));
  CORBA::Object_var obj(poa->id_to_reference(id));
  return SALOME::PickelizedPyObjRdExtServer::_narrow(obj);
}

void PickelizedPyObjRdExtServer::checkRdExtnessOf(const std::string& methodName, PyObject *argsPy)
{
  if(!_self)
    {
      Py_XDECREF(argsPy);
      throw Exception("PickelizedPyObjRdExtServer::checkRdExtnessOf : self is NULL !");
    }
  if(PyTuple_Check(_self)==1 || PyString_Check(_self)==1 || PyInt_Check(_self)==1 || PyBool_Check(_self)==1 || PyFloat_Check(_self)==1)
    return ;//_self is tuple, str, int or float -> immutable in python. So no method can break the RdExtness of _self.
  if(PyList_Check(_self)==1)
    checkListRdExtnessOf(methodName,argsPy);
  else if(PyDict_Check(_self)==1)
    checkDictRdExtnessOf(methodName,argsPy);
  else
    throw Exception("PickelizedPyObjRdExtServer::checkRdExtnessOf : Supported python types are [list,tuple,dict,str,int,float] !");
}

void PickelizedPyObjRdExtServer::checkListRdExtnessOf(const std::string& methodName, PyObject *argsPy)
{
  static const char *THE_RDEXT_METH_OF_LIST[]={"__getitem__","append","extend","insert","reverse","sort"};
  for(std::size_t i=0;i<sizeof(THE_RDEXT_METH_OF_LIST)/sizeof(const char *);i++)
    if(methodName==THE_RDEXT_METH_OF_LIST[i])
      return ;
  Py_XDECREF(argsPy);
  std::ostringstream oss; oss << "PickelizedPyObjRdExtServer::checkListRdExtnessOf : The method \"" << methodName << "\" is a method that can lead to a loss of data ! Supported method of \"list \"without loss of data are : __getitem__, append, extend, insert, reverse, sort !";
  throw Exception(oss.str());
}

void PickelizedPyObjRdExtServer::checkDictRdExtnessOf(const std::string& methodName, PyObject *argsPy)
{
  static const char *THE_RDEXT_METH_OF_DICT[]={"__getitem__","get","items","keys","setdefault","update","values"};
  for(std::size_t i=0;i<sizeof(THE_RDEXT_METH_OF_DICT)/sizeof(const char *);i++)
    if(methodName==THE_RDEXT_METH_OF_DICT[i])
      return ;
  if(methodName=="__setitem__")
    {
      checkDictSetitemRdExtness(argsPy);
      return ;
    }
  Py_XDECREF(argsPy);
  std::ostringstream oss; oss << "PickelizedPyObjRdExtServer::checkDictRdExtnessOf : The method \"" << methodName << "\" is a method that can lead to a loss of data ! Supported method of \"list \"without loss of data are : __getitem__, __setitem__(with conditions), get, items, keys, setdefault, update, values !";
  throw Exception(oss.str());
}

void PickelizedPyObjRdExtServer::checkDictSetitemRdExtness(PyObject *argsPy)
{
  if(PyTuple_Check(argsPy)==0)
    {
      Py_XDECREF(argsPy);
      throw Exception("PickelizedPyObjRdExtServer::checkDictSetitemRdExtness : args of dic(self).__setitem__ is not a tuple !");
    }
  if(PyTuple_Size(argsPy)!=2)
    {
      Py_XDECREF(argsPy);
      throw Exception("PickelizedPyObjRdExtServer::checkDictSetitemRdExtness : tuple of dic(self).__setitem__ has not a size equal to 2 !");
    }
  if(PyDict_GetItem(_self,PyTuple_GetItem(argsPy,0))!=0)
    {// argsPy[0] is already a key of _self -> __setitem__ is NOT RdExt !
      Py_XDECREF(argsPy);
      throw Exception("PickelizedPyObjRdExtServer::checkDictSetitemRdExtness : specified key of __setitem__ already exists ! RdExt property is not applied !");
    }
}

std::string PickelizedPyObjRdExtServer::getAccessStr() const
{
  return std::string(ACCESS_REPR);
}
