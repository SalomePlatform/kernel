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

//  File   : DSC_Callbacks.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _DSC_CALLBACKS_HXX_
#define _DSC_CALLBACKS_HXX_

#include "DSC_Basic.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(DSC_Engines)

/*! \class DSC_Callbacks
 *  \brief This is an abstract class that defines methods that the component
 *  uses to notify the component user code that the state of the component has changed.
 *
 *  Currently only port's connection modifications are signaled.
 */
class DSC_BASIC_EXPORT DSC_Callbacks
{
  public:
    DSC_Callbacks() {}
    virtual ~DSC_Callbacks() {}

    /*! \brief This method is used by the component when the number of connection
     * on a provides port changes. 
     *
     * This information helps the user code to detect operation on its ports.
     *
     * \param provides_port_name the name of the provides name that has changed.
     * \param connection_nbr the new number of connection on the provides port.
     * \param message contains informations about the modification of the port.
     */
    virtual void provides_port_changed(const char* provides_port_name,
                                       int connection_nbr,
                                       const Engines::DSC::Message message) = 0;

    /*! \brief  This method is used by the component when the number of connection
     * on a uses port changes. 
     *
     * This information helps the user code to detect operation on its ports.
     *
     * \param uses_port_name the name of the uses name that has changed.
     * \param new_uses_port the new sequence representing the uses port.
     * \param message contains informations about the modification of the port.
     */
    virtual void uses_port_changed(const char* uses_port_name,
                                   Engines::DSC::uses_port * new_uses_port,
                                   const Engines::DSC::Message message) = 0;
};

#endif
