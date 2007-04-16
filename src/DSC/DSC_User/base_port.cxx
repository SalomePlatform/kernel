#include "base_port.hxx"


base_port::base_port() 
{
  default_properties = new PortProperties_i();
}

base_port::~base_port() 
{
  delete default_properties;
}

Ports::PortProperties_ptr 
base_port::get_port_properties() 
{
  return default_properties->_this();
}
