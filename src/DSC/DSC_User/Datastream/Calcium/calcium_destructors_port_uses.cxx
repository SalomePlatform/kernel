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

//  File   : calcium_destructors_port_uses.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
#include "calcium_integer_port_uses.hxx"
#include "calcium_long_port_uses.hxx"
#include "calcium_intc_port_uses.hxx"
#include "calcium_real_port_uses.hxx"
#include "calcium_double_port_uses.hxx"
#include "calcium_string_port_uses.hxx"
#include "calcium_logical_port_uses.hxx"
#include "calcium_complex_port_uses.hxx"

calcium_real_port_uses::~calcium_real_port_uses(void) {};
calcium_double_port_uses::~calcium_double_port_uses(void) {};
calcium_integer_port_uses::~calcium_integer_port_uses(void) {};
calcium_long_port_uses::~calcium_long_port_uses(void) {};
calcium_intc_port_uses::~calcium_intc_port_uses(void) {};
calcium_logical_port_uses::~calcium_logical_port_uses(void) {};
calcium_complex_port_uses::~calcium_complex_port_uses(void) {};
calcium_string_port_uses::~calcium_string_port_uses(void) {};
