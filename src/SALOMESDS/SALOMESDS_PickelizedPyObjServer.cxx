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

#include "SALOMESDS_PickelizedPyObjServer.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_Exception.hxx"

#include <iostream>
#include <sstream>

using namespace SALOMESDS;

PickelizedPyObjServer::PickelizedPyObjServer(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value):BasicDataServer(father,varName),_self(0)
{
  setSerializedContentInternal(value);
}

//! obj is consumed
PickelizedPyObjServer::PickelizedPyObjServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj):BasicDataServer(father,varName),_self(0)
{
  setNewPyObj(obj);
}

PickelizedPyObjServer::~PickelizedPyObjServer()
{
  Py_XDECREF(_self);
}

/*!
 * Called remotely -> to protect against throw
 */
SALOME::ByteVec *PickelizedPyObjServer::fetchSerializedContent()
{
  Py_XINCREF(_self);//because pickelize consume _self
  return FromCppToByteSeq(pickelize(_self));
}

bool PickelizedPyObjServer::isDict()
{
  if(PyDict_Check(_self))
    return true;
  else
    return false;
}

void PickelizedPyObjServer::checkKeyNotAlreadyPresent(PyObject *key)
{
  checkKeyPresence(key,false);
}

void PickelizedPyObjServer::checkKeyPresent(PyObject *key)
{
  checkKeyPresence(key,true);
}

void PickelizedPyObjServer::addKeyValueHard(PyObject *key, PyObject *value)
{
  std::ostringstream oss; oss << "PickelizedPyObjServer::addKeyValueHard : var \"" << getVarNameCpp() << "\" is not permitted to alter its value !";
  throw Exception(oss.str());
}

void PickelizedPyObjServer::removeKeyInVarErrorIfNotAlreadyExisting(PyObject *key)
{
  std::ostringstream oss; oss << "PickelizedPyObjServer::removeKeyInVarErrorIfNotAlreadyExisting : var \"" << getVarNameCpp() << "\" is not permitted to alter its value !";
  throw Exception(oss.str());
}

void PickelizedPyObjServer::FromByteSeqToCpp(const SALOME::ByteVec& bsToBeConv, std::string& ret)
{
  std::size_t sz(bsToBeConv.length());
  ret.resize(sz,' ');
  char *buf(const_cast<char *>(ret.c_str()));
  for(std::size_t i=0;i<sz;i++)
    buf[i]=bsToBeConv[i];
}

void PickelizedPyObjServer::FromCppToByteSeq(const std::string& strToBeConv, SALOME::ByteVec& ret)
{
  const char *buf(strToBeConv.c_str());
  std::size_t sz(strToBeConv.size());
  ret.length(sz);
  for(std::size_t i=0;i<sz;i++)
    ret[i]=buf[i];
}

SALOME::ByteVec *PickelizedPyObjServer::FromCppToByteSeq(const std::string& strToBeConv)
{
  SALOME::ByteVec *ret(new SALOME::ByteVec);
  FromCppToByteSeq(strToBeConv,*ret);
  return ret;
}

//! New reference returned
PyObject *PickelizedPyObjServer::GetPyObjFromPickled(const std::string& pickledData, DataScopeServerBase *dsb)
{
  std::size_t sz(pickledData.size());
  PyObject *pickledDataPy(PyString_FromStringAndSize(NULL,sz));// agy : do not use PyString_FromString because std::string hides a vector of byte.
  char *buf(PyString_AsString(pickledDataPy));// this buf can be used thanks to python documentation.
  const char *inBuf(pickledData.c_str());
  std::copy(inBuf,inBuf+sz,buf);
  PyObject *selfMeth(PyObject_GetAttrString(dsb->getPickler(),"loads"));
  PyObject *args(PyTuple_New(1)); PyTuple_SetItem(args,0,pickledDataPy);
  PyObject *ret(PyObject_CallObject(selfMeth,args));
  Py_XDECREF(args);
  Py_XDECREF(selfMeth);
  return ret;
}

//! New reference returned
PyObject *PickelizedPyObjServer::getPyObjFromPickled(const std::string& pickledData)
{
  return GetPyObjFromPickled(pickledData,_father);
}

//! New reference returned
PyObject *PickelizedPyObjServer::GetPyObjFromPickled(const std::vector<unsigned char>& pickledData, DataScopeServerBase *dsb)
{
  std::size_t sz(pickledData.size());
  PyObject *pickledDataPy(PyString_FromStringAndSize(NULL,sz));// agy : do not use PyString_FromString because std::string hides a vector of byte.
  char *buf(PyString_AsString(pickledDataPy));// this buf can be used thanks to python documentation.
  const unsigned char *inBuf(&pickledData[0]);
  std::copy(inBuf,inBuf+sz,buf);
  PyObject *selfMeth(PyObject_GetAttrString(dsb->getPickler(),"loads"));
  PyObject *args(PyTuple_New(1)); PyTuple_SetItem(args,0,pickledDataPy);
  PyObject *ret(PyObject_CallObject(selfMeth,args));
  Py_XDECREF(args);
  Py_XDECREF(selfMeth);
  return ret;
}

//! New reference returned
PyObject *PickelizedPyObjServer::getPyObjFromPickled(const std::vector<unsigned char>& pickledData)
{
  return GetPyObjFromPickled(pickledData,_father);
}

//! obj is consumed by this method.
std::string PickelizedPyObjServer::Pickelize(PyObject *obj, DataScopeServerBase *dsb)
{
  PyObject *args(PyTuple_New(2));
  PyTuple_SetItem(args,0,obj);
  PyTuple_SetItem(args,1,PyInt_FromLong(2));// because "assert(cPickle.HIGHEST_PROTOCOL is 2)"
  PyObject *selfMeth(PyObject_GetAttrString(dsb->getPickler(),"dumps"));
  PyObject *retPy(PyObject_CallObject(selfMeth,args));
  Py_XDECREF(selfMeth);
  Py_XDECREF(args);
  std::size_t sz(PyString_Size(retPy));
  std::string ret(sz,'\0');
  const char *buf(PyString_AsString(retPy));
  char *inBuf(const_cast<char *>(ret.c_str()));
  for(std::size_t i=0;i<sz;i++)
    inBuf[i]=buf[i];
  Py_XDECREF(retPy);
  return ret;
}

//! obj is consumed by this method.
std::string PickelizedPyObjServer::pickelize(PyObject *obj)
{
  return Pickelize(obj,_father);
}

//! obj is consumed by this method.
void PickelizedPyObjServer::setNewPyObj(PyObject *obj)
{
  if(!obj)
    throw Exception("PickelizedPyObjServer::setNewPyObj : trying to assign a NULL pyobject in this !");
  if(obj==_self)
    return ;
  if(PyList_Check(obj)==0 && PyDict_Check(obj)==0 && PyTuple_Check(obj)==0 && PyString_Check(obj)==0 && PyInt_Check(obj)==0 && PyBool_Check(obj)==0 && PyFloat_Check(obj)==0 && obj!=Py_None)
    throw Exception("PickelizedPyObjServer::setNewPyObj : Supported python types are [list,tuple,dict,str,int,bool,float,None] !");
  if(_self)
    {
      PyObject *selfType(PyObject_Type(_self));
      if(PyObject_IsInstance(obj,selfType)!=1)
        {
          Py_XDECREF(obj);
          Py_XDECREF(selfType);
          throw Exception("PickelizedPyObjServer::setNewPyObj : type of new object is not the same than those previously set !");
        }
      else
        Py_XDECREF(selfType);
    }
  Py_XDECREF(_self);
  _self=obj;
}

void PickelizedPyObjServer::setSerializedContentInternal(const SALOME::ByteVec& newValue)
{
  std::string data;
  FromByteSeqToCpp(newValue,data);
  setNewPyObj(getPyObjFromPickled(data));
}

PyObject *PickelizedPyObjServer::CreateDftObjFromType(PyObject *globals, const std::string& typeName)
{
  PyObject *builtins(PyDict_GetItemString(globals,"__builtins__"));
  if(!builtins)
    throw Exception("PickelizedPyObjServer constructor : no __builtins__ in globals !");
  PyObject *builtins2(PyModule_GetDict(builtins));
  if(!builtins2)
    throw Exception("PickelizedPyObjServer constructor : internal error fail to invoke __dict__ on __builtins__ !");
  PyObject *tmp(PyDict_GetItemString(builtins2,typeName.c_str()));
  if(!tmp)
    {
      std::ostringstream oss; oss << "PickelizedPyObjServer::CreateDftObjFromType : Invalid type name \"" << typeName << "\" !";
      throw Exception(oss.str());
    }
  PyObject *args(PyTuple_New(0));
  PyObject *ret(PyObject_CallObject(tmp,args));
  Py_XDECREF(args);
  return ret;
}

void PickelizedPyObjServer::checkKeyPresence(PyObject *key, bool presence)
{
  if(!isDict())
    throw Exception("PickelizedPyObjServer::checkKeyPresence : not a dict !");
  PyObject *selfMeth(PyObject_GetAttrString(_self,"__contains__"));//new ref
  PyObject *args(PyTuple_New(1));
  PyTuple_SetItem(args,0,key); Py_XINCREF(key);// key is stolen by PyTuple_SetItem
  PyObject *retPy(PyObject_CallObject(selfMeth,args));
  Py_XDECREF(args);
  Py_XDECREF(selfMeth);
  //
  if(retPy!=Py_False && retPy!=Py_True)
    throw Exception("PickelizedPyObjServer::checkKeyPresence : unexpected return of dict.__contains__ !");
  if(!presence)
    {
      if(retPy==Py_True)
        throw Exception("PickelizedPyObjServer::checkKeyPresence : key is already present and it should not !");
    }
  else
    {
      if(retPy==Py_False)
        throw Exception("PickelizedPyObjServer::checkKeyPresence : key is not present and it should !");
    }
  Py_XDECREF(retPy);
}

PickelizedPyObjServerModifiable::PickelizedPyObjServerModifiable(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value):PickelizedPyObjServer(father,varName,value)
{
}

PickelizedPyObjServerModifiable::PickelizedPyObjServerModifiable(DataScopeServerBase *father, const std::string& varName, PyObject *obj):PickelizedPyObjServer(father,varName,obj)
{
}

void PickelizedPyObjServerModifiable::addKeyValueErrorIfAlreadyExisting(PyObject *key, PyObject *value)
{
  checkKeyNotAlreadyPresent(key);
  bool isOK(PyDict_SetItem(_self,key,value)==0);
  if(!isOK)
    throw Exception("PickelizedPyObjServerModifiable::addKeyValueErrorIfAlreadyExisting : error when trying to add key,value to dict !");
}

void PickelizedPyObjServerModifiable::removeKeyInVarErrorIfNotAlreadyExisting(PyObject *key)
{
  checkKeyPresent(key);
  if(PyDict_DelItem(_self,key)!=0)
    throw Exception("PickelizedPyObjServerModifiable::removeKeyInVarErrorIfNotAlreadyExisting : error during deletion of key in dict !");
}
