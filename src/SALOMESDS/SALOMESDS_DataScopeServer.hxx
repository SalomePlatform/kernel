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

#ifndef __SALOMESDS_DATASCOPEERVER_HXX__
#define __SALOMESDS_DATASCOPEERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_RequestSwitcher.hxx"
#include "SALOMESDS_RefCountServ.hxx"
#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_BasicDataServer.hxx"
#include "SALOMESDS_Auto.hxx"
#include "SALOMESDS_Defines.hxx"
#include "SALOME_CPythonHelper.hxx"

#include <Python.h>

#include <string>
#include <vector>
#include <list>

class SALOME_NamingService_Container_Abstract;

namespace SALOMESDS
{
  class SALOMESDS_EXPORT DataScopeKiller : public POA_SALOME_CMOD::DataScopeKiller
  {
  public:
    DataScopeKiller(CORBA::ORB_ptr orb):_orb(CORBA::ORB::_duplicate(orb)) { }
    void shutdown();
  private:
    CORBA::ORB_var _orb;
  };

  class DataScopeServerTransaction;
  
  /*!
   * Servant activated by a specific POA (single thread) having itself its specific POA_manager.
   * This class is able to hold/active the default POA_manager shared by other POA than this.
   */
  class SALOMESDS_EXPORT RequestSwitcher : public RequestSwitcherBase, public virtual POA_SALOME_CMOD::RequestSwitcher
  {
  public:
    RequestSwitcher(CORBA::ORB_ptr orb, DataScopeServerTransaction *ds);
    SALOME_CMOD::StringVec *listVars();
    SALOME_CMOD::ByteVec *fetchSerializedContent(const char *varName);
    void fetchAndGetAccessOfVar(const char *varName, CORBA::String_out access, SALOME_CMOD::ByteVec_out data);
  private:
    //! handle on its creator to give access to services when _poa_manager_under_control is in hold mode.
    DataScopeServerTransaction *_ds;
  };

  class KeyWaiter;
  class PickelizedPyObjServer;

  class SALOMESDS_EXPORT DataScopeServerBase : public virtual POA_SALOME_CMOD::DataScopeServerBase, public POAHolder
  {
  public:
    DataScopeServerBase(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, SALOME_CMOD::DataScopeKiller_var killer, const std::string& scopeName, SALOME_NamingService_Container_Abstract *ns);
    DataScopeServerBase(const DataScopeServerBase& other);
  public: // remote access methods
    void ping();
    char *getScopeName();
    SALOME_CMOD::StringVec *listVars();
    CORBA::Boolean existVar(const char *varName);
    SALOME_CMOD::BasicDataServer_ptr retrieveVarInternal(const char *varName);
    void deleteVar(const char *varName);
    CORBA::Boolean shutdownIfNotHostedByDSM(SALOME_CMOD::DataScopeKiller_out killer);
    SALOME_CMOD::ByteVec *fetchSerializedContent(const char *varName);
    SALOME_CMOD::SeqOfByteVec *getAllKeysOfVarWithTypeDict(const char *varName);
    SALOME_CMOD::ByteVec *getValueOfVarWithTypeDict(const char *varName, const SALOME_CMOD::ByteVec& constKey);
    void takeANap(CORBA::Double napDurationInSec);
  public:
    ~DataScopeServerBase();
    BasicDataServer *retrieveVarInternal2(const std::string& varName);
    void registerToSalomePiDict() const;
    void setPOA(PortableServer::POA_var poa);
    void registerInNS(SALOME_CMOD::DataScopeServerBase_ptr ptr);
    PyObject *getGlobals() const { return _pyHelper->getGlobals(); }
    PyObject *getLocals() const { return _pyHelper->getLocals(); }
    PyObject *getPickler() const { return _pyHelper->getPickler(); }
    PortableServer::POA_var getPOA() const { return _poa; }
    CORBA::ORB_var getORB() { return _orb; }
    std::string getScopeNameCpp() const { return _name; }
    static std::string BuildTmpVarNameFrom(const std::string& varName);
  public:
    std::vector< std::string> getAllVarNames() const;
    bool isExistingVar(const std::string& varName) const;
    void checkNotAlreadyExistingVar(const std::string& varName) const;
    void checkExistingVar(const std::string& varName) const;
    PickelizedPyObjServer *checkVarExistingAndDict(const std::string& varName);
  public:
    void moveStatusOfVarFromRdWrToRdOnly(const std::string& varName);
    void moveStatusOfVarFromRdOnlyToRdWr(const std::string& varName);
    void moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit(const std::string& varName);
    void moveStatusOfVarFromRdExtOrRdExtInitToRdExt(const std::string& varName);
  protected:
    std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::const_iterator retrieveVarInternal3(const std::string& varName) const;
    std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > >::iterator retrieveVarInternal4(const std::string& varName);
  protected:
    SALOME_NamingService_Container_Abstract *_ns = nullptr;
    const SALOME_CPythonHelper *_pyHelper = nullptr;
    PortableServer::POA_var _poa;
    CORBA::ORB_var _orb;
    std::string _name;
    std::list< std::pair< SALOME_CMOD::BasicDataServer_var, BasicDataServer * > > _vars;
    SALOME_CMOD::DataScopeKiller_var _killer;
    static std::size_t COUNTER;
  };
  
  class SALOMESDS_EXPORT DataScopeServer : public DataScopeServerBase, public virtual POA_SALOME_CMOD::DataScopeServer
  {
  public:
    DataScopeServer(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, SALOME_CMOD::DataScopeKiller_var killer, const std::string& scopeName, SALOME_NamingService_Container_Abstract *ns);
    DataScopeServer(const DataScopeServer& other);
    SALOME_CMOD::BasicDataServer_ptr retrieveVar(const char *varName) { return retrieveVarInternal(varName); }
    SALOME_CMOD::PickelizedPyObjRdOnlyServer_ptr createRdOnlyVar(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::PickelizedPyObjRdExtServer_ptr createRdExtVar(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::PickelizedPyObjRdWrServer_ptr createRdWrVar(const char *typeName, const char *varName);
    ~DataScopeServer();
  };

  class SALOMESDS_EXPORT DataScopeServerTransaction : public DataScopeServerBase, public virtual POA_SALOME_CMOD::DataScopeServerTransaction
  {
  public://not remotely callable
    DataScopeServerTransaction(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, SALOME_CMOD::DataScopeKiller_var killer, const std::string& scopeName, SALOME_NamingService_Container_Abstract *ns);
    DataScopeServerTransaction(const DataScopeServerTransaction& other);
    ~DataScopeServerTransaction();
    void createRdOnlyVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue);
    void createRdExtVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue);
    void createRdExtVarFreeStyleInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue, std::string&& compareFuncContent, SALOME::AutoPyRef&& compareFunc);
    void createRdExtInitVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue);
    void createRdWrVarInternal(const std::string& varName, const SALOME_CMOD::ByteVec& constValue);
    PortableServer::POA_var getPOA4KeyWaiter() const { return _poa_for_key_waiter; }
    void addWaitKey(KeyWaiter *kw);
    void pingKey(PyObject *keyObj);
    void notifyKey(const std::string& varName, PyObject *keyObj, PyObject *valueObj);
    SALOME_CMOD::ByteVec *waitForMonoThrRev(SALOME_CMOD::KeyWaiter_ptr kw);
    SALOME_CMOD::ByteVec *waitForAndKill(SALOME_CMOD::KeyWaiter_ptr kw);
  public://remotely callable
    char *getAccessOfVar(const char *varName);
    void fetchAndGetAccessOfVar(const char *varName, CORBA::String_out access, SALOME_CMOD::ByteVec_out data);
    SALOME_CMOD::Transaction_ptr createRdOnlyVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::Transaction_ptr createRdExtVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::Transaction_ptr createRdExtVarFreeStyleTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue, const char *compareFuncContent);
    SALOME_CMOD::Transaction_ptr createRdExtInitVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::Transaction_ptr createRdWrVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::Transaction_ptr addKeyValueInVarHard(const char *varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value);
    SALOME_CMOD::Transaction_ptr addKeyValueInVarErrorIfAlreadyExisting(const char *varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value);
    SALOME_CMOD::TransactionMultiKeyAddSession_ptr addMultiKeyValueSession(const char *varName);
    SALOME_CMOD::Transaction_ptr removeKeyInVarErrorIfNotAlreadyExisting(const char *varName, const SALOME_CMOD::ByteVec& key);
    SALOME_CMOD::TransactionRdWrAccess_ptr createWorkingVarTransac(const char *varName, const SALOME_CMOD::ByteVec& constValue);
    SALOME_CMOD::Transaction_ptr killVarTransac(const char *varName);
    SALOME_CMOD::KeyWaiter_ptr waitForKeyInVar(const char *varName, const SALOME_CMOD::ByteVec& keyVal);
    SALOME_CMOD::KeyWaiter_ptr waitForKeyInVarAndKillIt(const char *varName, const SALOME_CMOD::ByteVec& keyVal, SALOME_CMOD::Transaction_out transac);
    void atomicApply(const SALOME_CMOD::ListOfTransaction& transactions);
    SALOME_CMOD::RequestSwitcher_ptr getRequestSwitcher();
  private:
    PortableServer::POA_var _poa_for_key_waiter;
    std::list< KeyWaiter * > _waiting_keys;
    AutoServantPtr<RequestSwitcher> _rs;
  };
}

#endif
