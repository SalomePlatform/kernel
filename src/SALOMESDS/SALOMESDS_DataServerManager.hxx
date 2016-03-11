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

#ifndef __SALOMESDS_DATASERVERMANAGER_HXX__
#define __SALOMESDS_DATASERVERMANAGER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_DataScopeServer.hxx"

#include "SALOMESDS_Defines.hxx"

#include <string>

class SALOME_NamingService;

namespace SALOMESDS
{
  class DataScopeServer;
    
  class SALOMESDS_EXPORT DataServerManager : public virtual POA_SALOME::DataServerManager
  {
  public:
    DataServerManager(int argc, char *argv[], CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);
    SALOME::StringVec *listScopes();
    SALOME::StringVec *listAliveAndKickingScopes();
    SALOME::DataScopeServer_ptr getDefaultScope();
    CORBA::Boolean isAliveAndKicking(const char *scopeName);
    SALOME::DataScopeServerBase_ptr retriveDataScope(const char *scopeName);
    //
    SALOME::DataScopeServer_ptr createDataScope(const char *scopeName);
    SALOME::DataScopeServer_ptr giveADataScopeCalled(const char *scopeName, CORBA::Boolean& isCreated);
    //
    SALOME::DataScopeServerTransaction_ptr createDataScopeTransaction(const char *scopeName);
    SALOME::DataScopeServerTransaction_ptr giveADataScopeTransactionCalled(const char *scopeName, CORBA::Boolean& isCreated);
    //
    void removeDataScope(const char *scopeName);
    void cleanScopesInNS();
    void shutdownScopes();
  public:
    CORBA::ORB_var getORB() { return _orb; }
    static std::string CreateAbsNameInNSFromScopeName(const std::string& scopeName);
    static CORBA::Boolean IsAliveAndKicking(SALOME::DataScopeServerBase_ptr scopePtr);
    static SALOME::DataScopeServerBase_var GetScopePtrGivenName(const std::string& scopeName, const std::vector<std::string>& scopes, SALOME_NamingService& ns);
  public:
    static const char NAME_IN_NS[];
    static const char DFT_SCOPE_NAME_IN_NS[];
  private:
    std::vector<std::string> listOfScopesCpp();
    SALOME::DataScopeServerBase_var getScopePtrGivenName(const std::string& scopeName);
  private:
    CORBA::ORB_var _orb;
    //! single thread poa
    PortableServer::POA_var _poa;
  };
}

#endif
