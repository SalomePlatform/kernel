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

#ifndef __SALOMESDS_PICKELIZEDPYOBJRDWRSERVER_HXX__
#define __SALOMESDS_PICKELIZEDPYOBJRDWRSERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include <Python.h>

#include "SALOMESDS_PickelizedPyObjServer.hxx"

namespace SALOMESDS
{
  class PickelizedPyObjRdWrServer : public PickelizedPyObjServerModifiable, public virtual POA_SALOME_CMOD::PickelizedPyObjRdWrServer
  {
  public:
    PickelizedPyObjRdWrServer(DataScopeServerBase *father, const std::string& typeName, const std::string& varName);
    PickelizedPyObjRdWrServer(DataScopeServerBase *father, const std::string& varName, const SALOME_CMOD::ByteVec& value);
    PickelizedPyObjRdWrServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj);
    ~PickelizedPyObjRdWrServer();
    void setSerializedContent(const SALOME_CMOD::ByteVec& newValue);
    SALOME_CMOD::PickelizedPyObjRdWrServer_ptr invokePythonMethodOn(const char *method, const SALOME_CMOD::ByteVec& args);
  public:
    std::string getAccessStr() const;
    void addKeyValueHard(PyObject *key, PyObject *value);
  public:
    static const char ACCESS_REPR[];
  };
}

#endif
