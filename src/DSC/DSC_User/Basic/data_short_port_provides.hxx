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

//  File   : data_short_port_provides.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _DATA_SHORT_PORT_PROVIDES_HXX_
#define _DATA_SHORT_PORT_PROVIDES_HXX_

#include <SALOMEconfig.h>

#include <iostream>
#include "SALOME_Ports.hh"
#include "provides_port.hxx"
#include <pthread.h>

/*! \class data_short_port_provides
 *  \brief This class a port that sends a CORBA short with
 *  the basic port policy.
 */
class data_short_port_provides :
  public virtual POA_Ports::Data_Short_Port,
  public virtual provides_port
{
  public :
    data_short_port_provides();
    virtual ~data_short_port_provides();

    /*!
     * This method implements the CORBA method of the interface.
     * \see Ports::Data_Short_Port::put
     */
    virtual void put(CORBA::Short data);

    /*!
     * This method is used by the component to get
     * the last value received.
     *
     * \return the last value received (default 0).
     */
    virtual CORBA::Short get();

    /*!
     * This method gives the port CORBA reference.
     *
     * \return port's CORBA reference.
     */
    virtual Ports::Port_ptr get_port_ref();

  private :
    CORBA::Short _val;
    pthread_mutex_t * short_mutex;
    pthread_cond_t * short_condition;
    bool short_termine;
    pthread_mutex_t * short_mutex_cp;
    pthread_cond_t * short_condition_cp;
    bool short_termine_cp;

};

#endif
