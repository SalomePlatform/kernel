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

#ifndef __SALOMESDS_DATASERVERMANAGER_HXX__
#define __SALOMESDS_DATASERVERMANAGER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_RequestSwitcher.hxx"

#include "SALOMESDS_Defines.hxx"

#include <string>
#include <mutex>

class SALOME_NamingService_Abstract;

namespace SALOMESDS
{
  class DataServerManager;
  
  class RequestSwitcherDSM : public RequestSwitcherBase, public virtual POA_SALOME_CMOD::RequestSwitcherDSM
  {
  public:
    RequestSwitcherDSM(CORBA::ORB_ptr orb, DataServerManager *dsm):RequestSwitcherBase(orb),_dsm(dsm) { }
    SALOME_CMOD::StringVec *listScopes();
    SALOME_CMOD::DataScopeServerTransaction_ptr giveADataScopeTransactionCalled(const char *scopeName, CORBA::Boolean& isCreated);
    void holdRequests()override;
    void activeRequests()override;
  private:
    DataServerManager *_dsm;
  };
  
  class DataScopeServer;
    
  class SALOMESDS_EXPORT DataServerManager : public virtual POA_SALOME_CMOD::DataServerManager
  {
  public:
    DataServerManager(const SALOME_CPythonHelper *pyHelper, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, SALOME_NamingService_Abstract *ns = nullptr);
    ~DataServerManager();
    SALOME_CMOD::StringVec *listScopes();
    SALOME_CMOD::StringVec *listAliveAndKickingScopes();
    SALOME_CMOD::DataScopeServer_ptr getDefaultScope();
    CORBA::Boolean isAliveAndKicking(const char *scopeName);
    SALOME_CMOD::DataScopeServerBase_ptr retriveDataScope(const char *scopeName);
    //
    SALOME_CMOD::DataScopeServer_ptr createDataScope(const char *scopeName);
    SALOME_CMOD::DataScopeServer_ptr giveADataScopeCalled(const char *scopeName, CORBA::Boolean& isCreated);
    //
    SALOME_CMOD::DataScopeServerTransaction_ptr createDataScopeTransaction(const char *scopeName);
    SALOME_CMOD::DataScopeServerTransaction_ptr giveADataScopeTransactionCalled(const char *scopeName, CORBA::Boolean& isCreated);
    //
    void removeDataScope(const char *scopeName);
    void cleanScopesInNS();
    void shutdownScopes();
    SALOME_CMOD::RequestSwitcherDSM_ptr getRequestSwitcher();
    void holdRequests();
    void activeRequests();

  public:
    CORBA::ORB_var getORB() { return _orb; }
    static std::string CreateAbsNameInNSFromScopeName(const std::string& scopeName);
    static CORBA::Boolean IsAliveAndKicking(SALOME_CMOD::DataScopeServerBase_ptr scopePtr);
    static SALOME_CMOD::DataScopeServerBase_var GetScopePtrGivenName(const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService_Abstract *ns);
    SALOME_CMOD::StringVec *listScopes_unsafe();
    SALOME_CMOD::DataScopeServerTransaction_ptr giveADataScopeTransactionCalled_unsafe(
                              const char *scopeName, CORBA::Boolean& isCreated);
  public:
    static const char NAME_IN_NS[];
    static const char DFT_SCOPE_NAME_IN_NS[];
  private:
    std::vector<std::string> listOfScopesCpp();
    SALOME_CMOD::DataScopeServerBase_var getScopePtrGivenName(const std::string& scopeName);
    CORBA::Boolean isAliveAndKicking_unsafe(const char *scopeName);
    SALOME_CMOD::DataScopeServerBase_ptr retriveDataScope_unsafe(const char *scopeName);
  private:
    //! naming service object is owned
    SALOME_NamingService_Abstract *_ns = nullptr;
    CORBA::ORB_var _orb;
    //! single thread poa
    PortableServer::POA_var _poa;
    AutoServantPtr<RequestSwitcherDSM> _rs;
    std::mutex _mutex;
  };
}

#endif
