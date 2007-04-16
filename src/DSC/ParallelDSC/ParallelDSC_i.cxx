// André Ribes EDF R&D - 2006
// 

#include "ParallelDSC_i.hxx"

Engines_ParallelDSC_i::Engines_ParallelDSC_i(CORBA::ORB_ptr orb, char * ior,
					     PortableServer::POA_ptr poa,
					     PortableServer::ObjectId * contId,
					     const char *instanceName,
					     const char *interfaceName,
					     bool notif) :
  Engines_Parallel_Component_i(orb, ior, poa, contId, instanceName, interfaceName, notif),
  Engines::Parallel_DSC_serv(orb, ior),
  Engines::DSC_serv(orb, ior),
//  Engines::Superv_Component_serv(orb, ior),
  Engines::Component_serv(orb, ior),
  InterfaceParallel_impl(orb,ior)
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
  PaCO_operation * global_ptr = getContext("global_paco_context");
  cerr << " my_comm : " << global_ptr->my_com << endl;
  // Waiting that all the nodes have the proxy
  global_ptr->my_com->paco_barrier(); 
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
							const char * provides_port_name) 
{
  assert(provides_port_name);
  CORBA::Boolean rtn_bool = false;
  CORBA::Object_ptr _comp_proxy = _orb->string_to_object(_ior.c_str());
  Engines::Parallel_DSC_var real_comp_proxy = Engines::Parallel_DSC::_narrow(_comp_proxy);
  real_comp_proxy->set_paco_proxy(ref, provides_port_name);
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
  node->deploy(getMyRank());
  rtn_bool = true;
  return rtn_bool;
}
