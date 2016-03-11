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

//  File   : provides_port.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _PROVIDES_PORT_HXX_
#define _PROVIDES_PORT_HXX_

#include "base_port.hxx"
#include "SALOME_Ports.hh"
#include "DSC_Engines.hh"

/*! \class provides_port
 *  \brief This class implements a DSC_User provides C++ port.
 *
 *  This class is base class for all DSC_User provides port.
 *  It's an abstract class.
 */
class provides_port : public base_port
{
  public :
    provides_port();
    virtual ~provides_port();

    /*!
     * This method permits to get the CORBA reference of the port.
     *
     * \return port's CORBA reference.
     */
    virtual Ports::Port_ptr get_port_ref() = 0;

    /*!
     * This method is the callback called by the component to inform
     * the provides port of a new or a removed connection.
     *
     * \param connection_nbr current connection number.
     * \param message message associated with this connection.
     */
    virtual void provides_port_changed(int connection_nbr,
                                       const Engines::DSC::Message message) {}

};

#endif

