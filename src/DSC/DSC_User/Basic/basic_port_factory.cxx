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
//  File   : basic_port_factory.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL

#include "basic_port_factory.hxx"

using namespace std;

basic_port_factory::basic_port_factory() {}

basic_port_factory::~basic_port_factory() {}

provides_port *
basic_port_factory::create_data_servant(string type) {
  provides_port * rtn_port = NULL;
  if (type == "short") {
    rtn_port = new data_short_port_provides();  
  }
  return rtn_port;
}

uses_port * 
basic_port_factory::create_data_proxy(string type) {
  uses_port * rtn_port = NULL;
  if (type == "short")
    rtn_port = new data_short_port_uses();
  return rtn_port;
}
