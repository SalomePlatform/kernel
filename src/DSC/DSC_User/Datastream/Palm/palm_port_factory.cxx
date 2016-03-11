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

//  File   : palm_port_factory.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-08 19:01:14 +0100 (lun, 08 jan 2007) $
// Id          : $Id$
//
#include "palm_port_factory.hxx"
#include "Superv_Component_i.hxx"

palm_port_factory::palm_port_factory() {
    Superv_Component_i::register_factory("PALM",this);
}

palm_port_factory::~palm_port_factory() {}

provides_port *
palm_port_factory::create_data_servant(std::string type) {
  provides_port * rtn_port = NULL;
  if (type == "short") {
    rtn_port = new palm_data_short_port_provides();
  }
  if (type == "seq_short") {
    rtn_port = new palm_data_seq_short_port_provides();
  }
  return rtn_port;
}

uses_port * 
palm_port_factory::create_data_proxy(std::string type) {
  uses_port * rtn_port = NULL;
  return rtn_port;
}

static palm_port_factory myfactory;
