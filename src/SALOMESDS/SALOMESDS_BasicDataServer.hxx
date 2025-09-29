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

#ifndef __SALOMESDS_BASICDATASERVER_HXX__
#define __SALOMESDS_BASICDATASERVER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_RefCountServ.hxx"

#include <string>

namespace SALOMESDS
{
  class DataScopeServerBase;
  
  class BasicDataServer : public RefCountServ, public virtual POA_SALOME_CMOD::BasicDataServer
  {
  public:
    BasicDataServer(DataScopeServerBase *father, const std::string& varName);
    SALOME_CMOD::DataScopeServer_ptr getMyDataScopeServer();
    char *getVarName();
    char *getScopeName();
  public:
    void Register();
    void UnRegister();
    void Destroy();
    CORBA::Long getRCValue();
  public:
    DataScopeServerBase *getFather() const { return _father; }
    std::string getVarNameCpp() const { return _var_name; }
  protected:
    PortableServer::POA_var getPOA() const;
  protected:
    DataScopeServerBase *_father;
  private:
    std::string _var_name;
  };
}

#endif
