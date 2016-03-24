// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  File   : data_short_port_uses.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#include "data_short_port_uses.hxx"
#include <iostream>

data_short_port_uses::data_short_port_uses() {
  _my_ports = NULL;
}

data_short_port_uses::~data_short_port_uses() {}

const char *
data_short_port_uses::get_repository_id() {
  return "IDL:Ports/Data_Short_Port:1.0";
}

void
data_short_port_uses::put(CORBA::Short data) {
//  if (!CORBA::is_nil(_my_port))
//    _my_port->put(data);
  if (!_my_ports)
    std::cerr << "data_short_port_uses::put is NULL" << std::endl;
  else
  {
    for(int i = 0; i < _my_ports->length(); i++)
    {
      Ports::Data_Short_Port_ptr port = Ports::Data_Short_Port::_narrow((*_my_ports)[i]);
      port->put(data);
    }
  }
}

void 
data_short_port_uses::uses_port_changed(Engines::DSC::uses_port * new_uses_port,
                       const Engines::DSC::Message message)
{
  if (_my_ports)
    delete _my_ports;

  std::cerr << "data_short_port_uses::uses_port_changed" << std::endl;
  _my_ports = new Engines::DSC::uses_port(*new_uses_port);
}
