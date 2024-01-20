// Copyright (C) 2021-2024  CEA, EDF
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

#pragma once

#include "SALOME_NamingService_defs.hxx"
#include "SALOME_NamingService_Abstract.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Embedded_NamingService)

class NAMINGSERVICE_EXPORT SALOME_Embedded_NamingService_Client : public SALOME_NamingService_Container_Abstract
{
public:
  SALOME_Embedded_NamingService_Client(Engines::EmbeddedNamingService_var remoteNSServ):_remote_ns_serv(remoteNSServ) { }
  void init_orb(CORBA::ORB_ptr orb=0) override;
  bool IsTrueNS() const override { return false; }
  SALOME_NamingService_Container_Abstract *clone() override;
  void Register(CORBA::Object_ptr ObjRef, const char* Path) override;
  void Destroy_FullDirectory(const char* Path) override;
  void Destroy_Name(const char* Path)  override;
  CORBA::Object_ptr Resolve(const char* Path) override;
  CORBA::Object_ptr ResolveFirst(const char* Path) override;
public:
  Engines::EmbeddedNamingService_var GetObject() const { return _remote_ns_serv; }
private:
  SALOME_Embedded_NamingService_Client(const SALOME_Embedded_NamingService_Client& other) = default;
private:
  Engines::EmbeddedNamingService_var _remote_ns_serv;
};
