// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : port_factory.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _PORT_FACTORY_HXX
#define _PORT_FACTORY_HXX

#include "base_port.hxx"
#include "uses_port.hxx"
#include "provides_port.hxx"

#include <string>

/*! \class port_factory
 *  \brief This class is an abstract for all the DSC ports factories that have to be 
 *  registered into the component.
 */
class port_factory {
 public:
   virtual ~port_factory() {}

    /*!
     * This method creates a provides port.
     *
     * \param type port's type.
     * \return a pointer of the provides port.
     */
   virtual provides_port * create_data_servant(std::string type) = 0;

    /*!
     * This method creates a uses port.
     *
     * \param type port's type.
     * \return a pointer of the uses port.
     */
   virtual uses_port * create_data_proxy(std::string type) = 0;
};

#endif

