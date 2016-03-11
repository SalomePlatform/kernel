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

#ifndef __SALOMESDS_DATASCOPEERVER_HXX__
#define __SALOMESDS_DATASCOPEERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_RefCountServ.hxx"
#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_BasicDataServer.hxx"
#include "SALOMESDS_Defines.hxx"

#include <Python.h>

#include <string>
#include <vector>
#include <list>

namespace SALOMESDS
{
  class SALOMESDS_EXPORT DataScopeKiller : public POA_SALOME::DataScopeKiller
  {
  public:
    DataScopeKiller(CORBA::ORB_ptr orb):_orb(CORBA::ORB::_duplicate(orb)) { }
    void shutdown();
  private:
    CORBA::ORB_var _orb;
  };
  
  class KeyWaiter;
  class PickelizedPyObjServer;

  class SALOMESDS_EXPORT DataScopeServerBase : public virtual POA_SALOME::DataScopeServerBase, public POAHolder
  {
  public:
    DataScopeServerBase(CORBA::ORB_ptr orb, SALOME::DataScopeKiller_var killer, const std::string& scopeName);
    DataScopeServerBase(const DataScopeServerBase& other);
    void ping();
    char *getScopeName();
    SALOME::StringVec *listVars();
    CORBA::Boolean existVar(const char *varName);
    SALOME::BasicDataServer_ptr retrieveVarInternal(const char *varName);
    void deleteVar(const char *varName);
    CORBA::Boolean shutdownIfNotHostedByDSM(SALOME::DataScopeKiller_out killer);
    SALOME::ByteVec *fetchSerializedContent(const char *varName);
    SALOME::SeqOfByteVec *getAllKeysOfVarWithTypeDict(const char *varName);
    ~DataScopeServerBase();
  public:
    BasicDataServer *retrieveVarInternal2(const std::string& varName);
    void initializePython(int argc, char *argv[]);
    void registerToSalomePiDict() const;
    void setPOA(PortableServer::POA_var poa);
    void registerInNS(SALOME::DataScopeServerBase_ptr ptr);
    PyObject *getGlobals() const { return _globals; }
    PyObject *getLocals() const { return _locals; }
    PyObject *getPickler() const { return _pickler; }
    PortableServer::POA_var getPOA() const { return _poa; }
    CORBA::ORB_var getORB() { return _orb; }
    std::string getScopeNameCpp() const { return _name; }
    static std::string BuildTmpVarNameFrom(const std::string& varName);
  public:
    std::vector< std::string> getAllVarNames() const;
    void checkNotAlreadyExistingVar(const std::string& varName) const;
    void checkExistingVar(const std::string& varName) const;
    PickelizedPyObjServer *checkVarExistingAndDict(const std::string& varName);
  public:
    void moveStatusOfVarFromRdWrToRdOnly(const std::string& varName);
    void moveStatusOfVarFromRdOnlyToRdWr(const std::string& varName);
    void moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit(const std::string& varName);
    void moveStatusOfVarFromRdExtOrRdExtInitToRdExt(const std::string& varName);
  protected:
    std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::const_iterator retrieveVarInternal3(const std::string& varName) const;
    std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > >::iterator retrieveVarInternal4(const std::string& varName);
  protected:
    PyObject *_globals;
    PyObject *_locals;
    PyObject *_pickler;
    PortableServer::POA_var _poa;
    CORBA::ORB_var _orb;
    std::string _name;
    std::list< std::pair< SALOME::BasicDataServer_var, BasicDataServer * > > _vars;
    SALOME::DataScopeKiller_var _killer;
    static std::size_t COUNTER;
  };
  
  class SALOMESDS_EXPORT DataScopeServer : public DataScopeServerBase, public virtual POA_SALOME::DataScopeServer
  {
  public:
    DataScopeServer(CORBA::ORB_ptr orb, SALOME::DataScopeKiller_var killer, const std::string& scopeName);
    DataScopeServer(const DataScopeServer& other);
    SALOME::BasicDataServer_ptr retrieveVar(const char *varName) { return retrieveVarInternal(varName); }
    SALOME::PickelizedPyObjRdOnlyServer_ptr createRdOnlyVar(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::PickelizedPyObjRdExtServer_ptr createRdExtVar(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::PickelizedPyObjRdWrServer_ptr createRdWrVar(const char *typeName, const char *varName);
    ~DataScopeServer();
  };

  class SALOMESDS_EXPORT DataScopeServerTransaction : public DataScopeServerBase, public virtual POA_SALOME::DataScopeServerTransaction
  {
  public://not remotely callable
    DataScopeServerTransaction(CORBA::ORB_ptr orb, SALOME::DataScopeKiller_var killer, const std::string& scopeName);
    DataScopeServerTransaction(const DataScopeServerTransaction& other);
    ~DataScopeServerTransaction();
    void createRdOnlyVarInternal(const std::string& varName, const SALOME::ByteVec& constValue);
    void createRdExtVarInternal(const std::string& varName, const SALOME::ByteVec& constValue);
    void createRdExtInitVarInternal(const std::string& varName, const SALOME::ByteVec& constValue);
    void createRdWrVarInternal(const std::string& varName, const SALOME::ByteVec& constValue);
    PortableServer::POA_var getPOA4KeyWaiter() const { return _poa_for_key_waiter; }
    void addWaitKey(KeyWaiter *kw);
    void pingKey(PyObject *keyObj);
    void notifyKey(const std::string& varName, PyObject *keyObj, PyObject *valueObj);
    SALOME::ByteVec *waitForMonoThrRev(SALOME::KeyWaiter_ptr kw);
  public://remotely callable
    char *getAccessOfVar(const char *varName);
    void fetchAndGetAccessOfVar(const char *varName, CORBA::String_out access, SALOME::ByteVec_out data);
    SALOME::Transaction_ptr createRdOnlyVarTransac(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::Transaction_ptr createRdExtVarTransac(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::Transaction_ptr createRdExtInitVarTransac(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::Transaction_ptr createRdWrVarTransac(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::Transaction_ptr addKeyValueInVarHard(const char *varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value);
    SALOME::Transaction_ptr addKeyValueInVarErrorIfAlreadyExisting(const char *varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value);
    SALOME::TransactionMultiKeyAddSession_ptr addMultiKeyValueSession(const char *varName);
    SALOME::Transaction_ptr removeKeyInVarErrorIfNotAlreadyExisting(const char *varName, const SALOME::ByteVec& key);
    SALOME::TransactionRdWrAccess_ptr createWorkingVarTransac(const char *varName, const SALOME::ByteVec& constValue);
    SALOME::Transaction_ptr killVarTransac(const char *varName);
    SALOME::KeyWaiter_ptr waitForKeyInVar(const char *varName, const SALOME::ByteVec& keyVal);
    SALOME::KeyWaiter_ptr waitForKeyInVarAndKillIt(const char *varName, const SALOME::ByteVec& keyVal, SALOME::Transaction_out transac);
    void atomicApply(const SALOME::ListOfTransaction& transactions);
  private:
    PyObject *getPyCmpFunc();
  private:
    PortableServer::POA_var _poa_for_key_waiter;
    std::list< KeyWaiter * > _waiting_keys;
  };
}

#endif
