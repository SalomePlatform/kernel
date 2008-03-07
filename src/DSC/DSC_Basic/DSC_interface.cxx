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
//  File   : DSC_interface.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL

#include <string>
#include "DSC_interface.hxx"

Engines_DSC_interface::Engines_DSC_interface() {}

Engines_DSC_interface::~Engines_DSC_interface() 
{
  my_ports_it = my_ports.begin();
  for(;my_ports_it != my_ports.end();my_ports_it++)
    delete my_ports_it->second;

}

void
Engines_DSC_interface::add_provides_port(Ports::Port_ptr ref, 
				 const char* provides_port_name,
				 Ports::PortProperties_ptr port_prop) 
throw (Engines::DSC::PortAlreadyDefined,
       Engines::DSC::NilPort,
       Engines::DSC::BadProperty) 
{
  // Method args test
  assert(provides_port_name);
  if (CORBA::is_nil(ref))
    throw Engines::DSC::NilPort();
  if (CORBA::is_nil(port_prop))
    throw Engines::DSC::BadProperty();

  my_ports_it = my_ports.find(provides_port_name);
  if (my_ports_it ==  my_ports.end()) {
    // Creating a new port provides
    port_t * new_port = new port_t();
    new_port->type = provides;
    new_port->connection_nbr = 0;
    new_port->provides_port_ref = Ports::Port::_duplicate(ref);
    new_port->port_prop = Ports::PortProperties::_duplicate(port_prop);

    // Port into the port's map
    my_ports[provides_port_name] = new_port;
  }
  else
    throw Engines::DSC::PortAlreadyDefined();
}

void
Engines_DSC_interface::add_uses_port(const char* repository_id, 
			     const char* uses_port_name,
			     Ports::PortProperties_ptr port_prop) 
throw (Engines::DSC::PortAlreadyDefined,
       Engines::DSC::BadProperty) 
{
  // Method args test
  // Note : We can't be shure that repository id
  // is a correct CORBA id.
  assert(repository_id);
  assert(uses_port_name);
  if (CORBA::is_nil(port_prop))
    throw Engines::DSC::BadProperty();

  my_ports_it = my_ports.find(uses_port_name);
  if (my_ports_it ==  my_ports.end()) {
    // Creating a new uses port
    port_t * new_port = new port_t();
    new_port->type = uses;
    new_port->connection_nbr = 0;
    new_port->uses_port_refs.length(0);
    new_port->repository_id = repository_id;
    new_port->port_prop = Ports::PortProperties::_duplicate(port_prop);

    // Port into port's map
    my_ports[uses_port_name] = new_port;
  }
  else
    throw Engines::DSC::PortAlreadyDefined();
}

Ports::Port_ptr
Engines_DSC_interface::get_provides_port(const char* provides_port_name,
				 const CORBA::Boolean connection_error) 
  throw (Engines::DSC::PortNotDefined,
	 Engines::DSC::PortNotConnected, 
	 Engines::DSC::BadPortType) 
{
  // Method arg test
  assert(provides_port_name);

  Ports::Port_ptr rtn_port = Ports::Port::_nil();
//   std::cout << "---- DSC_Interface : MARK 1 ---- Recherche de : " << provides_port_name << "----" << std::endl;
//   ports::iterator it;
//   std::cout << "----> ";
//   for(it=my_ports.begin();it!=my_ports.end();++it) 
//     std::cout << "|"<<(*it).first<<"|, ";
//   std::cout << std::endl;
 
  // Searching the port
  my_ports_it = my_ports.find(provides_port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();
  if (my_ports[provides_port_name]->type != provides) {
    Engines::DSC::BadPortType BPT;
    BPT.expected = CORBA::string_dup("Expected a provides port");
    BPT.received = CORBA::string_dup((std::string("Received a uses/none port : ")+provides_port_name).c_str());
    throw BPT;
  }

  if (my_ports[provides_port_name]->connection_nbr == 0 && connection_error)
    throw Engines::DSC::PortNotConnected();

  rtn_port = Ports::Port::_duplicate(my_ports[provides_port_name]->provides_port_ref);
  return rtn_port;
}

Engines::DSC::uses_port * 
Engines_DSC_interface::get_uses_port(const char* uses_port_name) 
  throw (Engines::DSC::PortNotDefined,
	 Engines::DSC::PortNotConnected,
	 Engines::DSC::BadPortType) 
{
  // Method arg test
  assert(uses_port_name);

  Engines::DSC::uses_port * rtn_port = NULL;  

  // Searching the uses port
  my_ports_it = my_ports.find(uses_port_name);
  if (my_ports_it == my_ports.end())
    throw Engines::DSC::PortNotDefined();
  if (my_ports[uses_port_name]->type != uses){
    Engines::DSC::BadPortType BPT;
    BPT.expected = CORBA::string_dup("Expected a uses port");
    BPT.received = CORBA::string_dup((std::string("Received a provides/none port : ")+uses_port_name).c_str());
   std::cout << "---- DSC_Interface : MARK 1 ---- exception : " << uses_port_name << "----" << std::endl;
    throw BPT;
  }

  // Is the port connected ?
  if (my_ports[uses_port_name]->connection_nbr > 0) {
    rtn_port = new Engines::DSC::uses_port(my_ports[uses_port_name]->uses_port_refs);
  }
  else
    {
   std::cout << "---- DSC_Interface : MARK 2 ---- exception : " << uses_port_name << "----" << std::endl;
    throw Engines::DSC::PortNotConnected();
    }
  
  return rtn_port;
}

void
Engines_DSC_interface::connect_provides_port(const char* provides_port_name)
    throw (Engines::DSC::PortNotDefined)
{
  // Method arg test
  assert(provides_port_name);

  // Searching the provides port
  my_ports_it = my_ports.find(provides_port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();
  if (my_ports[provides_port_name]->type != provides)
    throw Engines::DSC::PortNotDefined();


  // Adding a new connection
  my_ports[provides_port_name]->connection_nbr += 1;
  // User code is informed
  provides_port_changed(provides_port_name, 
			my_ports[provides_port_name]->connection_nbr,
			Engines::DSC::AddingConnection
		       );
}

void
Engines_DSC_interface::connect_uses_port(const char* uses_port_name,
					 Ports::Port_ptr provides_port_ref) 
  throw (Engines::DSC::PortNotDefined,
	 Engines::DSC::BadPortType,
	 Engines::DSC::NilPort)
{
  // Method arg test
  assert(uses_port_name);

  if (CORBA::is_nil(provides_port_ref))
    throw Engines::DSC::NilPort();

  // Searching the uses port
  my_ports_it = my_ports.find(uses_port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();
  if (my_ports[uses_port_name]->type != uses) {
    Engines::DSC::BadPortType BPT;
    BPT.expected = CORBA::string_dup("Expected a uses port");
    BPT.received = CORBA::string_dup((std::string("Received a provides/none port : ")+uses_port_name).c_str());
    throw BPT;
  }

  // repository_id test
  const char * repository_id = my_ports[uses_port_name]->repository_id.c_str();
  if (provides_port_ref->_is_a(repository_id)) 
  {
    // Adding provides port into the uses port sequence
    CORBA::ULong lgth = my_ports[uses_port_name]->uses_port_refs.length();
    my_ports[uses_port_name]->
      uses_port_refs.length(lgth + 1);
    my_ports[uses_port_name]->uses_port_refs[lgth] = 
      Ports::Port::_duplicate(provides_port_ref);

    // Adding a new connection
    my_ports[uses_port_name]->connection_nbr += 1;
    // User code is informed
    uses_port_changed(uses_port_name,
		      new Engines::DSC::uses_port(my_ports[uses_port_name]->uses_port_refs),
		      Engines::DSC::AddingConnection);
  }
  else {
    Engines::DSC::BadPortType BPT;
    BPT.expected = CORBA::string_dup("Expected ...");
    BPT.received = CORBA::string_dup((std::string("Received an incorrect repository id type ")+
				      repository_id).c_str());
    throw BPT;
  }

}

CORBA::Boolean
Engines_DSC_interface::is_connected(const char* port_name) 
  throw (Engines::DSC::PortNotDefined) 
{
  CORBA::Boolean rtn = false;

  // Method arg test
  assert(port_name);

  my_ports_it = my_ports.find(port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();

  // Is it connected ?
  if (my_ports[port_name]->connection_nbr > 0)
    rtn = true;

  return rtn;
}

void
Engines_DSC_interface::disconnect_provides_port(const char* provides_port_name,
					const Engines::DSC::Message message)
throw (Engines::DSC::PortNotDefined,
       Engines::DSC::PortNotConnected)
{
  // Method args test
  assert(provides_port_name);

  my_ports_it = my_ports.find(provides_port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();
  if (my_ports[provides_port_name]->type != provides)
    throw Engines::DSC::PortNotDefined();

  // Is it connected ?
  if (my_ports[provides_port_name]->connection_nbr > 0) 
  {
    my_ports[provides_port_name]->connection_nbr -= 1;
    provides_port_changed(provides_port_name,
			  my_ports[provides_port_name]->connection_nbr,
			  message);
  }
  else
    throw Engines::DSC::PortNotConnected();
}

void
Engines_DSC_interface::disconnect_uses_port(const char* uses_port_name,
				    Ports::Port_ptr provides_port_ref,
				    const Engines::DSC::Message message)
throw (Engines::DSC::PortNotDefined,
       Engines::DSC::PortNotConnected,
       Engines::DSC::BadPortReference) 
{
  // Method args test
  assert(uses_port_name);

  my_ports_it = my_ports.find(uses_port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();
  if (my_ports[uses_port_name]->type != uses)
    throw Engines::DSC::PortNotDefined();

  if (CORBA::is_nil(provides_port_ref))
    throw Engines::DSC::BadPortReference();

  // Is it connected ?
  if (my_ports[uses_port_name]->connection_nbr > 0) {
    CORBA::Long port_index = -1;
    CORBA::ULong seq_length = my_ports[uses_port_name]->uses_port_refs.length(); 
    for(int i = 0; i < seq_length; i++)
    {
      if (my_ports[uses_port_name]->uses_port_refs[i]->_is_equivalent(provides_port_ref))
      {
	port_index = i;
	break;
      }
    }
    if (port_index == -1)
      throw Engines::DSC::BadPortReference();

    my_ports[uses_port_name]->connection_nbr -= 1;
    Engines::DSC::uses_port * new_uses_port = 
      new Engines::DSC::uses_port();
    new_uses_port->length(seq_length - 1);

    int index_ancien = 0;
    int index_nouveau = 0;
    for(;index_ancien < seq_length;) {
      if (index_ancien == port_index) 
      {
	// Rien a faire !
	// On ne change pas le index du nouveau tableau
	index_ancien += 1;
      }
      else 
      {
	(*new_uses_port)[index_nouveau] = my_ports[uses_port_name]->uses_port_refs[index_ancien];
	index_ancien += 1;
	index_nouveau += 1;
      }
    }

    // New uses port's sequence
    my_ports[uses_port_name]->uses_port_refs = *new_uses_port;

    // The user code is informed
    uses_port_changed(uses_port_name,
		      new_uses_port,
		      message);
  }
  else
    throw Engines::DSC::PortNotConnected();
}

Ports::PortProperties_ptr
Engines_DSC_interface::get_port_properties(const char* port_name) 
  throw (Engines::DSC::PortNotDefined) 
{
  Ports::PortProperties_ptr rtn_properties = Ports::PortProperties::_nil();

  // Method arg test
  assert(port_name);

  my_ports_it = my_ports.find(port_name);
  if (my_ports_it ==  my_ports.end())
    throw Engines::DSC::PortNotDefined();

  rtn_properties = Ports::PortProperties::_duplicate(my_ports[port_name]->port_prop);
  return rtn_properties;
}
