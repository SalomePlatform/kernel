// Copyright (C) 2021  CEA/DEN, EDF R&D
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

#include "KernelSDS.hxx"
#include "SALOMESDS_DataServerManager.hxx"
#include "SALOME_Fake_NamingService.hxx"
#include "SALOME_KernelORB.hxx"

static SALOME::DataServerManager_var _dsm_singleton;

std::string GetDSMInstanceInternal()
{
  CORBA::ORB_ptr orb = KERNEL::getORB();
  if( CORBA::is_nil(_dsm_singleton) )
  {
    PortableServer::POA_var root_poa;
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    if(!CORBA::is_nil(obj))
      root_poa = PortableServer::POA::_narrow(obj);
    SALOME_CPythonHelper cPyh;
    {
      int argc(1);
      char *argv[2] = {"oops",nullptr};
      cPyh.initializePython(argc,argv);
    }
    SALOME_Fake_NamingService *ns(new SALOME_Fake_NamingService);
    SALOMESDS::DataServerManager *dsm(new SALOMESDS::DataServerManager(&cPyh,orb,root_poa,ns));
    dsm->_remove_ref();
    CORBA::Object_var objRef = ns->Resolve(SALOMESDS::DataServerManager::NAME_IN_NS);
    _dsm_singleton = SALOME::DataServerManager::_narrow(objRef);
  }
  CORBA::String_var ior = orb->object_to_string(_dsm_singleton);
  return std::string(ior.in());
}
