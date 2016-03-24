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

//  File   : calcium_port_factory.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:36:05 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$
//
#ifndef _CALCIUM_PORT_FACTORY_HXX_
#define _CALCIUM_PORT_FACTORY_HXX_

#include "port_factory.hxx"
#include "uses_port.hxx"
#include "provides_port.hxx"
#include <string>

#include "CalciumProvidesPort.hxx"

#include "calcium_integer_port_uses.hxx"
#include "calcium_long_port_uses.hxx"
#include "calcium_intc_port_uses.hxx"
#include "calcium_real_port_uses.hxx"
#include "calcium_double_port_uses.hxx"
#include "calcium_string_port_uses.hxx"
#include "calcium_logical_port_uses.hxx"
#include "calcium_complex_port_uses.hxx"

class calcium_port_factory :
  public port_factory
{
  public:
    calcium_port_factory();
    virtual ~calcium_port_factory();

    virtual provides_port * create_data_servant(std::string type); 
    virtual uses_port * create_data_proxy(std::string type); 
};

#endif

