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

#ifndef __SALOMESDS_DATASCOPEERVER_HXX__
#define __SALOMESDS_DATASCOPEERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_RefCountServ.hxx"
#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_BasicDataServer.hxx"

#include <Python.h>

#include <string>
#include <vector>
#include <list>

namespace SALOMESDS
{
  class DataScopeServer : public virtual POA_SALOME::DataScopeServer
  {
  public:
    DataScopeServer(CORBA::ORB_ptr orb, const std::string& scopeName);
    DataScopeServer(const DataScopeServer& other);
    void ping();
    char *getScopeName();
    SALOME::StringVec *listVars();
    SALOME::BasicDataServer_ptr retrieveVar(const char *varName);
    void deleteVar(const char *varName);
    SALOME::PickelizedPyObjRdOnlyServer_ptr createRdOnlyVar(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::PickelizedPyObjRdExtServer_ptr createRdExtVar(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::PickelizedPyObjRdWrServer_ptr createRdWrVar(const char *typeName, const char *varName);
    void shutdownIfNotHostedByDSM();
    ~DataScopeServer();
  public:
    void initializePython(int argc, char *argv[]);
    void registerToSalomePiDict() const;
    void setPOAAndRegister(PortableServer::POA_var poa, SALOME::DataScopeServer_ptr ptr);
    PyObject *getGlobals() const { return _globals; }
    PyObject *getLocals() const { return _locals; }
    PyObject *getPickler() const { return _pickler; }
    PortableServer::POA_var getPOA() { return _poa; }
    static std::string BuildTmpVarNameFrom(const std::string& varName);
  private:
    std::vector< std::string> getAllVarNames() const;
    CORBA::Object_var activateWithDedicatedPOA(BasicDataServer *ds);
    void checkNotAlreadyExistingVar(const std::string& varName);
  private:
    PyObject *_globals;
    PyObject *_locals;
    PyObject *_pickler;
    PortableServer::POA_var _poa;
    CORBA::ORB_var _orb;
    std::string _name;
    std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > > _vars;
    static std::size_t COUNTER;
  };
}

#endif
