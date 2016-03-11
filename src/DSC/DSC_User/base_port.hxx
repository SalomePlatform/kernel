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

//  File   : base_port.hxx
//  Author : Eric Fayolle (EDF), André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _PORT_HXX_
#define _PORT_HXX_

#include "PortProperties_i.hxx"
/*
 *  This class is base class for all DSC_User provides and uses port.
 *  It provides a default property object for the port.
 */
class base_port
{
public :
  base_port();
  virtual ~base_port();

  /*!
   * This is used to get the property object of the port.
   *
   * \return property's CORBA reference.
   */
  virtual Ports::PortProperties_ptr get_port_properties();

protected :
  PortProperties_i * default_properties;
};

#endif

