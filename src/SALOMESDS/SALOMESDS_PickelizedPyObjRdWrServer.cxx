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

#include "SALOMESDS_PickelizedPyObjRdWrServer.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_Exception.hxx"

#include <iostream>
#include <sstream>

using namespace SALOMESDS;

const char PickelizedPyObjRdWrServer::ACCESS_REPR[]="RdWr";

PickelizedPyObjRdWrServer::PickelizedPyObjRdWrServer(DataScopeServerBase *father, const std::string& typeName, const std::string& varName):PickelizedPyObjServerModifiable(father,varName,CreateDftObjFromType(father->getGlobals(),typeName))
{
}

PickelizedPyObjRdWrServer::PickelizedPyObjRdWrServer(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value):PickelizedPyObjServerModifiable(father,varName,value)
{
}

//! obj is consumed
PickelizedPyObjRdWrServer::PickelizedPyObjRdWrServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj):PickelizedPyObjServerModifiable(father,varName,obj)
{
}

PickelizedPyObjRdWrServer::~PickelizedPyObjRdWrServer()
{
}

/*!
 * Called remotely -> to protect against throw
 */
void PickelizedPyObjRdWrServer::setSerializedContent(const SALOME::ByteVec& newValue)
{
  setSerializedContentInternal(newValue);
}

SALOME::PickelizedPyObjRdWrServer_ptr PickelizedPyObjRdWrServer::invokePythonMethodOn(const char *method, const SALOME::ByteVec& args)
{
  if(!_self)
    throw Exception("PickelizedPyObjRdWrServer::invokePythonMethodOn : self is NULL !");
  std::string argsCpp;
  FromByteSeqToCpp(args,argsCpp);
  PyObject *argsPy(getPyObjFromPickled(argsCpp));
  //
  PyObject *selfMeth(PyObject_GetAttrString(_self,method));
  if(!selfMeth)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdWrServer::invokePythonMethodOn : Method \"" << method << "\" is not available !";
      throw Exception(oss.str());
    }
  PyObject *res(PyObject_CallObject(selfMeth,argsPy));// self can have been modified by this call !
  Py_XDECREF(selfMeth);
  Py_XDECREF(argsPy);
  if(!res)
    {
      std::ostringstream oss,oss2,oss3;
      PyObject *errTyp(0),*errValue(0),*errTB(0);
      PyErr_Fetch(&errTyp,&errValue,&errTB);
      oss2 << "(";
      if(errTyp)
        {
          PyObject *ob(PyObject_Str(errTyp));
          oss2 << " type : \"" << (const char *)PyString_AsString(ob) << "\"";
          Py_XDECREF(ob); Py_XDECREF(errTyp);
        }
      if(errValue)
        {
          PyObject *ob(PyObject_Str(errValue));
          oss2 << " value : \"" << (const char *)PyString_AsString(ob) << "\"";
          Py_XDECREF(ob); Py_XDECREF(errValue);
        }
      oss2 << " )";
      if(errTB)
        {
          PyObject *ob(PyObject_Str(errTB));
          oss2 << "( traceback : \"" << (const char *)PyString_AsString(ob) << "\"";
          Py_XDECREF(ob); Py_XDECREF(errTB);
        }
      oss2 << " )";
      PyErr_Clear();
      oss << "PickelizedPyObjRdWrServer::invokePythonMethodOn : Problem during invokation serverside of Method \"" << method << "\" ! Details are : " << oss2.str() << "\n\n" << "TraceBack is : " << oss3.str(); 
      throw Exception(oss.str());
    }
  PickelizedPyObjRdWrServer *ret(new PickelizedPyObjRdWrServer(_father,DataScopeServer::BuildTmpVarNameFrom(getVarNameCpp()),res));
  PortableServer::POA_var poa(_father->getPOA());
  PortableServer::ObjectId_var id(poa->activate_object(ret));
  CORBA::Object_var obj(poa->id_to_reference(id));
  return SALOME::PickelizedPyObjRdWrServer::_narrow(obj);
}

void PickelizedPyObjRdWrServer::addKeyValueHard(PyObject *key, PyObject *value)
{
  bool isOK(PyDict_SetItem(_self,key,value)==0);
  if(!isOK)
    throw Exception("PickelizedPyObjRdWrServer::addKeyValueHard : error when trying to add key,value to dict !");
}

std::string PickelizedPyObjRdWrServer::getAccessStr() const
{
  return std::string(ACCESS_REPR);
}
