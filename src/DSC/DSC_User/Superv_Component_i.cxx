//  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : Superv_Component_i.cxx
//  Author : André RIBES (EDF), Eric Fayolle (EDF)
//  Module : KERNEL

#include "Superv_Component_i.hxx"

DSC_EXCEPTION_CXX(Superv_Component_i,BadFabType);
DSC_EXCEPTION_CXX(Superv_Component_i,BadType);
DSC_EXCEPTION_CXX(Superv_Component_i,BadCast);
DSC_EXCEPTION_CXX(Superv_Component_i,UnexpectedState);
DSC_EXCEPTION_CXX(Superv_Component_i,PortAlreadyDefined);
DSC_EXCEPTION_CXX(Superv_Component_i,PortNotDefined);
DSC_EXCEPTION_CXX(Superv_Component_i,PortNotConnected);
DSC_EXCEPTION_CXX(Superv_Component_i,NilPort);
DSC_EXCEPTION_CXX(Superv_Component_i,BadProperty);

Superv_Component_i::Superv_Component_i(CORBA::ORB_ptr orb,
				       PortableServer::POA_ptr poa,
				       PortableServer::ObjectId * contId,
				       const char *instanceName,
				       const char *interfaceName,
				       bool notif) : 
  Engines_DSC_i(orb, poa, contId, instanceName, interfaceName) 
{
#ifdef _DEBUG_
  std::cerr << "--Superv_Component_i : MARK 1 ----  " << instanceName << "----" << std::endl;
#endif
  register_factory("BASIC", new basic_port_factory());
  register_factory("PALM", new palm_port_factory());
  register_factory("CALCIUM", new calcium_port_factory());
}
Superv_Component_i::Superv_Component_i(CORBA::ORB_ptr orb,
				       PortableServer::POA_ptr poa,
				       Engines::Container_ptr container, 
				       const char *instanceName,
				       const char *interfaceName,
				       bool notif) : Engines_DSC_i(orb, poa, container, instanceName, interfaceName) 
{
#ifdef _DEBUG_
  std::cerr << "--Superv_Component_i : MARK 1 ----  " << instanceName << "----" << std::endl;
#endif
  register_factory("BASIC", new basic_port_factory());
  register_factory("PALM", new palm_port_factory());
  register_factory("CALCIUM", new calcium_port_factory());
}

  
Superv_Component_i::~Superv_Component_i() 
{
  factory_map_t::iterator begin = _factory_map.begin();
  factory_map_t::iterator end = _factory_map.end();
  for(;begin!=end;begin++) 
  {
    delete begin->second;
  }

  my_superv_ports_it = my_superv_ports.begin();
  for(;my_superv_ports_it != my_superv_ports.end();my_superv_ports_it++) 
    delete my_superv_ports_it->second;

}

void 
Superv_Component_i::register_factory(const std::string & factory_name,
				     port_factory * factory_ptr) 
{
  factory_map_t::iterator it = _factory_map.find(factory_name);

  if (it == _factory_map.end() )
  {
    _factory_map[factory_name] = factory_ptr;
  }
}

port_factory *
Superv_Component_i::get_factory(const std::string & factory_name) 
{
  port_factory * rtn_factory = NULL;
  factory_map_t::iterator it = _factory_map.find(factory_name);

  if (it != _factory_map.end() )
  {
    rtn_factory = _factory_map[factory_name];
  }

  return rtn_factory;
}

provides_port *
Superv_Component_i::create_provides_data_port(const std::string& port_fab_type)
  throw (BadFabType)
{ 
  provides_port * rtn_port = NULL;
  std::string factory_name;
  std::string type_name;
  int search_result;

  search_result = port_fab_type.find("_");
  factory_name = port_fab_type.substr(0,search_result);
  type_name = port_fab_type.substr(search_result+1, port_fab_type.length());

  port_factory * factory = get_factory(factory_name);
  if (factory) {
    rtn_port = factory->create_data_servant(type_name);
  }

  if (rtn_port == NULL)
    throw  BadFabType( LOC(OSS()<< "Impossible d'accéder à la fabrique "
			   <<port_fab_type));

  return rtn_port;
}

uses_port *
Superv_Component_i::create_uses_data_port(const std::string& port_fab_type) 
throw (BadFabType)
{
  uses_port * rtn_proxy = NULL;
  std::string factory_name;
  std::string type_name;
  int search_result;

  search_result = port_fab_type.find("_");
  factory_name = port_fab_type.substr(0,search_result);
  type_name = port_fab_type.substr(search_result+1, port_fab_type.length());

  port_factory * factory = get_factory(factory_name);
  if (factory) {
    rtn_proxy = factory->create_data_proxy(type_name);
  }
  
  if (rtn_proxy == NULL)
   throw BadFabType( LOC(OSS()<< "Impossible d'accéder à la fabrique "
			  <<port_fab_type));

  return rtn_proxy;
}

void
Superv_Component_i::add_port(const char * port_fab_type,
			     const char * port_type,
			     const char * port_name)
  throw (PortAlreadyDefined, BadFabType, BadType, BadProperty)
{
  assert(port_fab_type);
  assert(port_type);
  assert(port_name);

  std::string s_port_type(port_type);
  if (s_port_type == "provides") {
    provides_port * port = create_provides_data_port(port_fab_type);
    add_port(port, port_name);
  }
  else if (s_port_type == "uses") {
#ifdef _DEBUG_
    std::cerr << "---- Superv_Component_i::add_port : MARK 1 ---- "  << std::endl;
#endif
    uses_port * port = create_uses_data_port(port_fab_type);
#ifdef _DEBUG_
    std::cerr << "---- Superv_Component_i::add_port : MARK 2 ---- "  << std::endl;
#endif
    add_port(port, port_name);
  }
  else
    throw BadType( LOC(OSS()<< "Le port_type doit être soit 'provides' soit 'uses' not "
		       << port_type));

}

void 
Superv_Component_i::add_port(provides_port * port, 
			     const char* provides_port_name) 
  throw (PortAlreadyDefined, NilPort, BadProperty)
{
  assert(port);
  assert(provides_port_name);

  try {

    Ports::PortProperties_var portproperties=port->get_port_properties();
    Ports::Port_var portref=port->get_port_ref();
    Engines_DSC_interface::add_provides_port(portref, 
					     provides_port_name,
					     portproperties);

    superv_port_t * new_superv_port = new superv_port_t();
    new_superv_port->p_ref = port;
    my_superv_ports[provides_port_name] = new_superv_port;

  } 
  catch (const Engines::DSC::PortAlreadyDefined&) {
    throw PortAlreadyDefined( LOC(OSS()<< "Le port provides "
				  << provides_port_name <<" existe déjà."));
  } 
  catch (const Engines::DSC::NilPort&) {
    throw NilPort( LOC(OSS()<< "Le pointeur sur port provides est nul."));
  }
  catch (const Engines::DSC::BadProperty&) {
    throw BadProperty( LOC(OSS()<< "La propriété est mal définie"));
  }
}

void
Superv_Component_i::add_port(uses_port * port, 
			     const char* uses_port_name) 
  throw (PortAlreadyDefined, NilPort, BadProperty)
{
  assert(port);
  assert(uses_port_name);

  try {
    Ports::PortProperties_var portproperties=port->get_port_properties();
    Engines_DSC_interface::add_uses_port(port->get_repository_id(), 
					 uses_port_name,
					 portproperties);
    superv_port_t * new_superv_port = new superv_port_t();
    new_superv_port->u_ref = port;
    my_superv_ports[uses_port_name] = new_superv_port;
  } 
  catch (const Engines::DSC::PortAlreadyDefined&) {
    throw PortAlreadyDefined( LOC(OSS()<< "Le port uses " 
				  << uses_port_name <<" existe déjà."));
  } 
  catch (const Engines::DSC::NilPort&) {
    throw NilPort( LOC(OSS()<< "Le pointeur sur port uses est nul."));
  }
  catch (const Engines::DSC::BadProperty&) {
    throw BadProperty( LOC(OSS()<< "La propriété est mal définie"));
  }
}

void
Superv_Component_i::get_port(provides_port *& port,
			     const char * provides_port_name)
  throw (PortNotDefined,PortNotConnected)
{
  assert(provides_port_name);

  try {
    Ports::Port_var portref=Engines_DSC_interface::get_provides_port(provides_port_name, false);
    port = my_superv_ports[provides_port_name]->p_ref;
  } catch (const Engines::DSC::PortNotDefined&) {
    throw PortNotDefined( LOC(OSS()<< "Le port provides  "
			      << provides_port_name <<" n'existe pas."));
  } catch (const Engines::DSC::PortNotConnected&) {
    throw PortNotConnected( LOC(OSS()<< "Le port provides " << provides_port_name 
				<< " n'est pas connecté."));
  }
}

void
Superv_Component_i::get_port(uses_port *& port,
			     const char * uses_port_name)
  throw (PortNotDefined, PortNotConnected)
{
  assert(uses_port_name);

  try {
    Engines::DSC::uses_port * portseq=Engines_DSC_i::get_uses_port(uses_port_name);
    delete portseq;
    port = my_superv_ports[uses_port_name]->u_ref;
  } catch (const Engines::DSC::PortNotDefined&) {    
    throw PortNotDefined( LOC(OSS()<< "Le port uses  "
			      << uses_port_name <<" n'existe pas."));
  } catch (const Engines::DSC::PortNotConnected&) {
    throw PortNotConnected( LOC(OSS()<< "Le port uses " << uses_port_name 
				<< " n'est pas connecté."));
  }
}



void
Superv_Component_i::provides_port_changed(const char* provides_port_name,
					  int connection_nbr,
					  const Engines::DSC::Message message)
{
  my_superv_ports_it = my_superv_ports.find(provides_port_name);
  if (my_superv_ports_it !=  my_superv_ports.end())
    my_superv_ports[provides_port_name]->p_ref->provides_port_changed(connection_nbr,
								      message);
}

void
Superv_Component_i::uses_port_changed(const char* uses_port_name,
				      Engines::DSC::uses_port * new_uses_port,
				      const Engines::DSC::Message message)
{
  my_superv_ports_it = my_superv_ports.find(uses_port_name);
  if (my_superv_ports_it !=  my_superv_ports.end())
    my_superv_ports[uses_port_name]->u_ref->uses_port_changed(new Engines::DSC::uses_port(*new_uses_port),
							      message);
  //delete the copy made by the caller
  delete new_uses_port;
}



void
Superv_Component_i::get_uses_port_names(std::vector<std::string> & port_names,
					const std::string servicename) const {

  port_names.reserve(my_superv_ports.size());

  superv_ports::const_iterator it;

  for (it=my_superv_ports.begin(); it!=my_superv_ports.end();++it)
    if( (*it).second->p_ref == NULL ) port_names.push_back((*it).first);
}
