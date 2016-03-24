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

//  File   : uses_port.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _USES_PORT_HXX_
#define _USES_PORT_HXX_

#include "base_port.hxx"
#include "SALOME_Ports.hh"
#include "DSC_Engines.hh"

/*! \class uses_port
 *  \brief This class implements a DSC_User uses C++ port.
 *
 *  This class is base class for all DSC_User uses port.
 *  It's an abstract class.
 *
 *  Contrary to DSC_Basic layer, a uses port as an implementation
 *  provided by this class which permits to manipulate the uses port.
 */
class uses_port : public base_port
{
  public :
    uses_port();
    virtual ~uses_port();

    /*!
     * This is used by the component to get the CORBA repository id
     * of the uses port. It's an abstract method.
     *
     * \return port's CORBA repository id.
     */
    virtual const char * get_repository_id() = 0;

    /*!
     * This method is the uses port's callback to be aware of
     * connections states.
     * It's an abstract method. The uses port uses this method
     * to manage the sequence of the DSC_Basic uses port.
     *
     * \param new_uses_port new uses port sequence.
     * \param message message associated with this modification.
     */
    virtual void uses_port_changed(Engines::DSC::uses_port * new_uses_port,
                                   const Engines::DSC::Message message) = 0;

};

#endif

