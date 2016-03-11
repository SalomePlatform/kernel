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

#ifndef __SALOMESDS_PICKELIZEDPYOBJRDONLYSERVER_HXX__
#define __SALOMESDS_PICKELIZEDPYOBJRDONLYSERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include <Python.h>

#include "SALOMESDS_PickelizedPyObjServer.hxx"

namespace SALOMESDS
{
  class PickelizedPyObjRdOnlyServer : public PickelizedPyObjServer, public virtual POA_SALOME::PickelizedPyObjRdOnlyServer
  {
  public:
    PickelizedPyObjRdOnlyServer(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value);
    PickelizedPyObjRdOnlyServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj);
    ~PickelizedPyObjRdOnlyServer();
  public:
    void addKeyValueErrorIfAlreadyExisting(PyObject *key, PyObject *value);
    std::string getAccessStr() const;
  public:
    static const char ACCESS_REPR[];
  };
}

#endif
