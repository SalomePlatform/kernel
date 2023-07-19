// Copyright (C) 2007-2023  CEA, EDF, OPEN CASCADE
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

#include "SALOME_DataScopeServer_Common.cxx"

#include "SALOME_Embedded_NamingService_Client.hxx"
#include "Utils_SALOME_Exception.hxx"

int main(int argc, char *argv[])
{
  CORBA::ORB_var orb = GetCustomORB();
  std::string scopeName;
  if(argc<=2)
    throw SALOMESDS::Exception("In the main of SALOME_DataScopeServer.cxx !");
  scopeName=argv[1];
  std::istringstream isTransacSS(argv[2]);
  int isTransac(0);
  isTransacSS >> isTransac;
  std::string IORFakeNS(argv[3]);
  CORBA::Object_var ns_serv_obj_base = orb->string_to_object(IORFakeNS.c_str());
  if( CORBA::is_nil(ns_serv_obj_base) )
    THROW_SALOME_EXCEPTION( "SALOME_DataScopeServer_SSL : argument 3 is NOT a valid IOR" );
  Engines::EmbeddedNamingService_var ns_serv_obj = Engines::EmbeddedNamingService::_narrow(ns_serv_obj_base);
  if( CORBA::is_nil(ns_serv_obj) )
    THROW_SALOME_EXCEPTION( "SALOME_DataScopeServer_SSL : argument 3 is NOT a valid IOR of Engines::EmbeddedNamingService" );
  return CommonDataScopeServerMain(argc,argv,orb,scopeName,isTransac,[ns_serv_obj](CORBA::ORB_ptr orb) { return new SALOME_Embedded_NamingService_Client(ns_serv_obj); });
}
