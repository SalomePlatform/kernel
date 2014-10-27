// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

namespace SALOMESDS
{
  class PickelizedPyObjServer : public BasicDataServer, public virtual POA_SALOME::PickelizedPyObjServer
  {
  public:
    PickelizedPyObjServer(DataScopeServer *father, const std::string& varName, const SALOME::ByteVec& value);
    PickelizedPyObjServer(DataScopeServer *father, const std::string& varName, PyObject *obj);
    ~PickelizedPyObjServer();
    void setSerializedContent(const SALOME::ByteVec& newValue);
    SALOME::ByteVec *fetchSerializedContent();
  protected:
    static void FromByteSeqToCpp(const SALOME::ByteVec& bsToBeConv, std::string& ret);
    static SALOME::ByteVec *FromCppToByteSeq(const std::string& strToBeConv);
    PyObject *getPyObjFromPickled(const std::string& pickledData);
    std::string pickelize(PyObject *obj);
    void setNewPyObj(PyObject *obj);
    void setSerializedContentInternal(const SALOME::ByteVec& newValue);
    static PyObject *CreateDftObjFromType(PyObject *globals, const std::string& typeName);
  protected:
    static const char FAKE_VAR_NAME_FOR_WORK[];
    PyObject *_self;
    PortableServer::POA_var _poa;
  };
}

#endif
