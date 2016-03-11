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

//  File   : ParallelDSC_i.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#include "ParallelDSC_i.hxx"

Engines_ParallelDSC_i::Engines_ParallelDSC_i(CORBA::ORB_ptr orb, 
                                             char * ior,
                                             int rank,
                                             PortableServer::POA_ptr poa,
                                             PortableServer::ObjectId * contId,
                                             const char *instanceName,
                                             const char *interfaceName,
                                             bool notif) :
  Engines_Parallel_Component_i(orb, ior, rank, poa, contId, instanceName, interfaceName, notif),
  Engines::Parallel_DSC_serv(orb, ior, rank),
  Engines::Parallel_DSC_base_serv(orb, ior, rank),
  Engines::DSC_serv(orb, ior, rank),
  Engines::DSC_base_serv(orb, ior, rank),
  Engines::Superv_Component_serv(orb, ior, rank),
  Engines::Superv_Component_base_serv(orb, ior, rank),
  Engines::EngineComponent_serv(orb, ior, rank),
  Engines::EngineComponent_base_serv(orb, ior, rank),
  Engines::Parallel_Component_serv(orb, ior, rank),
  Engines::Parallel_Component_base_serv(orb, ior, rank),
  InterfaceParallel_impl(orb, ior, rank)
{
}


Engines_ParallelDSC_i::~Engines_ParallelDSC_i() {}

// WARNING !!!
// CURRENTLY ERRORS ARE NOT SUPPORTED !!!!!!!
// WARNING !!

void
Engines_ParallelDSC_i::set_paco_proxy(const CORBA::Object_ptr ref, 
                                      const char* provides_port_name,
                                      Ports::PortProperties_ptr port_prop) {
  assert(provides_port_name);
  Engines_DSC_interface::add_provides_port(Ports::Port::_narrow(ref), 
                                           provides_port_name,
                                           port_prop);
  // Waiting that all the nodes have the proxy
  _my_com->paco_barrier(); 
  cerr << "set_paco_proxy node fin" << endl;
}

const char *
Engines_ParallelDSC_i::get_proxy(const char* provides_port_name) {
  char * rtn_char = NULL;
  Ports::Port_ptr proxy = Engines_DSC_interface::get_provides_port(provides_port_name, 
                                                                   0);
  if (!CORBA::is_nil(proxy))
    rtn_char = (char *) _orb->object_to_string(proxy);
  return rtn_char;
}

CORBA::Boolean 
Engines_ParallelDSC_i::add_parallel_provides_proxy_port(const CORBA::Object_ptr ref, 
                                                        const char * provides_port_name,
                                                        Ports::PortProperties_ptr port_prop) 
{
  assert(provides_port_name);
  CORBA::Boolean rtn_bool = false;
  Engines::Parallel_DSC_var real_comp_proxy = 
    Engines::Parallel_DSC::_narrow(InterfaceParallel_impl::_proxy);
  real_comp_proxy->set_paco_proxy(ref, provides_port_name, port_prop);
  rtn_bool = true;
  return rtn_bool;
}

CORBA::Boolean 
Engines_ParallelDSC_i::add_parallel_provides_proxy_wait(const char * provides_port_name) 
{
  assert(provides_port_name);
  char * proxy = NULL;
  try {
    proxy = (char *) get_proxy(provides_port_name);
  }
  catch(...) {
    while(proxy == NULL)
    {
      sleep(1);
      try {
        proxy = (char *) get_proxy(provides_port_name);
      }
      catch(...) {}
    }
  }
  return true;
}

CORBA::Boolean
Engines_ParallelDSC_i::add_parallel_provides_node_port(Ports::Port_PaCO_ptr ref, 
                                                       const char* provides_port_name) 
{
  CORBA::Boolean rtn_bool = false;
  PaCO::InterfaceParallel_var node = PaCO::InterfaceParallel::_narrow(ref);
  node->deploy();
  rtn_bool = true;
  return rtn_bool;
}
