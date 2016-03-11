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

//  File   : data_short_port_uses.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _DATA_SHORT_PORT_USES_HXX_
#define _DATA_SHORT_PORT_USES_HXX_

#include "uses_port.hxx"
#include "SALOME_Ports.hh"

/*! \class data_short_port_uses
 *  \brief This class a port that sends a CORBA short with
 *  the basic port policy.
 */
class data_short_port_uses :
  public virtual uses_port
{
  public :
    data_short_port_uses();
    virtual ~data_short_port_uses();

    /*!
     * This method is used by the component to get
     * port's CORBA repository id
     *
     * \return port's CORBA repository id
     */
    virtual const char * get_repository_id();

    /*!
     * This method is used by the component to send
     * a short value to all the provides ports connected.
     *
     * \param data the short sended.
     */
    virtual void put(CORBA::Short data);

    /*!
     * This method is a callback for be aware of modification 
     * of the port's connections.
     *
     * \param new_uses_port the new uses port's sequence.
     * \param message message associated to the modification.
     */
    virtual void uses_port_changed(Engines::DSC::uses_port * new_uses_port,
                                   const Engines::DSC::Message message);

  private :
    Engines::DSC::uses_port * _my_ports;
};

#endif

