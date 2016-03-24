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

#include "base_port.hxx"
#include <omniORB4/CORBA.h>


base_port::base_port() 
{
  default_properties = new PortProperties_i();
}

base_port::~base_port() 
{
  //do not call delete on corba servant: deactivate it and then call _remove_ref or delete
  PortableServer::POA_var poa =default_properties->_default_POA();
  PortableServer::ObjectId_var oid = poa->servant_to_id(default_properties);
  poa->deactivate_object(oid);
  default_properties->_remove_ref();
}

Ports::PortProperties_ptr 
base_port::get_port_properties() 
{
  return default_properties->_this();
}
