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

#include "SALOME_Container_Common.cxx"
#include "SALOME_Container_i.hxx"
#include "SALOME_Embedded_NamingService_Client.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "SALOME_KernelORB.hxx"
#include "KernelBasis.hxx"

int main(int argc, char* argv[])
{
  if(argc<3)
    THROW_SALOME_EXCEPTION( "SALOME_Container_No_NS_Serv : requires 2 input arguments <containerName> <IOR of Engines::EmbeddedNamingService>" );
  CORBA::ORB_ptr orb(KERNEL::getORB());
  std::string IOROfEmbeddedNamingService(argv[2]);
  setIOROfEmbeddedNS(IOROfEmbeddedNamingService);
  CORBA::Object_var ns_serv_obj_base = orb->string_to_object(IOROfEmbeddedNamingService.c_str());
  if( CORBA::is_nil(ns_serv_obj_base) )
    THROW_SALOME_EXCEPTION( "SALOME_Container_No_NS_Serv : argument 2 is NOT a valid IOR" );
  Engines::EmbeddedNamingService_var ns_serv_obj = Engines::EmbeddedNamingService::_narrow(ns_serv_obj_base);
  if( CORBA::is_nil(ns_serv_obj) )
    THROW_SALOME_EXCEPTION( "SALOME_Container_No_NS_Serv : argument 2 is NOT a valid IOR of Engines::EmbeddedNamingService" );
  std::unique_ptr<SALOME_NamingService_Container_Abstract> ns( new SALOME_Embedded_NamingService_Client(ns_serv_obj) );
  return container_common_main<Engines_Container_SSL_i>(argc,argv,std::move(ns));
}
