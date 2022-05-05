// Copyright (C) 2021-2022  CEA/DEN, EDF R&D
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

#include "SALOME_Embedded_NamingService_Client.hxx"
#include "SALOME_Embedded_NamingService_Common.hxx"

#include <memory>

void SALOME_Embedded_NamingService_Client::init_orb(CORBA::ORB_ptr orb)
{
}

SALOME_NamingService_Container_Abstract *SALOME_Embedded_NamingService_Client::clone()
{
  return new SALOME_Embedded_NamingService_Client(*this);
}

void SALOME_Embedded_NamingService_Client::Register(CORBA::Object_ptr ObjRef, const char* Path)
{
  std::unique_ptr<Engines::IORType> ior( ObjectToIOR(ObjRef) );
  this->_remote_ns_serv->Register( *(ior.get()), Path);
}

void SALOME_Embedded_NamingService_Client::Destroy_FullDirectory(const char* Path)
{
  this->_remote_ns_serv->Destroy_FullDirectory(Path);
}

void SALOME_Embedded_NamingService_Client::Destroy_Name(const char* Path)
{
  this->_remote_ns_serv->Destroy_Name(Path);
}

CORBA::Object_ptr SALOME_Embedded_NamingService_Client::Resolve(const char* Path)
{
  std::unique_ptr<Engines::IORType> ior( this->_remote_ns_serv->Resolve(Path) );
  CORBA::Object_var ret( IORToObject( *(ior.get()) ) );
  return CORBA::Object::_duplicate(ret);
}

CORBA::Object_ptr SALOME_Embedded_NamingService_Client::ResolveFirst(const char* Path)
{
  std::unique_ptr<Engines::IORType> ior( this->_remote_ns_serv->ResolveFirst(Path) );
  CORBA::Object_var ret( IORToObject( *(ior.get()) ) );
  return CORBA::Object::_duplicate(ret);
}
