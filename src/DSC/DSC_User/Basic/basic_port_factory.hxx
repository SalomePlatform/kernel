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

//  File   : basic_port_factory.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _BASIC_PORT_FACTORY_HXX_
#define _BASIC_PORT_FACTORY_HXX_

#include "port_factory.hxx"
#include "data_short_port_provides.hxx"
#include "data_short_port_uses.hxx"

/*! \class basic_port_factory
 *  \brief This class is an example of factory for DSC_User ports.
 *
 *  This class implements the methods to be able
 *  to be used by Superv_Component_i.
 *  It builds basic ports.
 */
class basic_port_factory :
  public port_factory
{
  public:
    basic_port_factory();
    virtual ~basic_port_factory();

    /*!
     * This method creates a provides port of Basic ports.
     *
     * \param type the basic port's type.
     * \return a pointer of the provides port.
     */
    virtual provides_port * create_data_servant(std::string type); 

    /*!
     * This method creates a uses port of Basic ports.
     *
     * \param type the basic port's type.
     * \return a pointer of the uses port.
     */
    virtual uses_port * create_data_proxy(std::string type); 
};

#endif

