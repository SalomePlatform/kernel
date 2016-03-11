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

#ifndef __SALOMESDS_PICKELIZEDPYOBJRDEXTSERVER_HXX__
#define __SALOMESDS_PICKELIZEDPYOBJRDEXTSERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include <Python.h>

#include "SALOMESDS_PickelizedPyObjServer.hxx"

namespace SALOMESDS
{
  class PickelizedPyObjRdExtServer : public PickelizedPyObjServerModifiable, public virtual POA_SALOME::PickelizedPyObjRdExtServer
  {
  public:
    PickelizedPyObjRdExtServer(DataScopeServerBase *father, const std::string& varName, const SALOME::ByteVec& value);
    PickelizedPyObjRdExtServer(DataScopeServerBase *father, const std::string& varName, PyObject *obj);
    ~PickelizedPyObjRdExtServer();
    SALOME::PickelizedPyObjRdExtServer_ptr invokePythonMethodOn(const char *method, const SALOME::ByteVec& args);
  public:
    std::string getAccessStr() const;
  private:
    void checkRdExtnessOf(const std::string& methodName, PyObject *argsPy);
    void checkListRdExtnessOf(const std::string& methodName, PyObject *argsPy);
    void checkDictRdExtnessOf(const std::string& methodName, PyObject *argsPy);
    void checkDictSetitemRdExtness(PyObject *argsPy);
  public:
    static const char ACCESS_REPR[];
  };
}

#endif
