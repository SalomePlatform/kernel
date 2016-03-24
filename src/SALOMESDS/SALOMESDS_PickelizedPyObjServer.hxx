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

#ifndef __SALOMESDS_PICKELIZEDPYOBJSERVER_HXX__
#define __SALOMESDS_PICKELIZEDPYOBJSERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include <Python.h>

#include "SALOMESDS_BasicDataServer.hxx"

#include <vector>

namespace SALOMESDS
{
  class PickelizedPyObjServer : public BasicDataServer, public virtual POA_SALOME::PickelizedPyObjServer
  {
  public:
    PickelizedPyObjServer(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value);
    PickelizedPyObjServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj);
    virtual ~PickelizedPyObjServer();
    void setSerializedContent(const SALOME::ByteVec& newValue);
    SALOME::ByteVec *fetchSerializedContent();
  public:
    bool isDict();
    void checkKeyNotAlreadyPresent(PyObject *key);
    void checkKeyPresent(PyObject *key);
    PyObject *getPyObj() const { return _self; }
  public:
    virtual void addKeyValueHard(PyObject *key, PyObject *value);
    virtual void removeKeyInVarErrorIfNotAlreadyExisting(PyObject *key);
    virtual void addKeyValueErrorIfAlreadyExisting(PyObject *key, PyObject *value) = 0;
    virtual std::string getAccessStr() const = 0;
  public:
    static void FromByteSeqToCpp(const SALOME::ByteVec& bsToBeConv, std::string& ret);
    static void FromCppToByteSeq(const std::string& strToBeConv, SALOME::ByteVec& ret);
    static SALOME::ByteVec *FromCppToByteSeq(const std::string& strToBeConv);
    static PyObject *GetPyObjFromPickled(const std::string& pickledData, DataScopeServerBase *dsb);
    static PyObject *GetPyObjFromPickled(const std::vector<unsigned char>& pickledData, DataScopeServerBase *dsb);
    static std::string Pickelize(PyObject *obj, DataScopeServerBase *dsb);
    PyObject *getPyObjFromPickled(const std::string& pickledData);
    PyObject *getPyObjFromPickled(const std::vector<unsigned char>& pickledData);
    std::string pickelize(PyObject *obj);
    void setNewPyObj(PyObject *obj);
    void setSerializedContentInternal(const SALOME::ByteVec& newValue);
    static PyObject *CreateDftObjFromType(PyObject *globals, const std::string& typeName);
  private:
    void checkKeyPresence(PyObject *key, bool presence);
  protected:
    static const char FAKE_VAR_NAME_FOR_WORK[];
    PyObject *_self;
    PortableServer::POA_var _poa;
  };

  class PickelizedPyObjServerModifiable : public PickelizedPyObjServer
  {
  public:
    PickelizedPyObjServerModifiable(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value);
    PickelizedPyObjServerModifiable(DataScopeServerBase *father, const std::string& varName, PyObject *obj);
  public:
    void addKeyValueErrorIfAlreadyExisting(PyObject *key, PyObject *value);
    void removeKeyInVarErrorIfNotAlreadyExisting(PyObject *key);
  };
}

#endif
