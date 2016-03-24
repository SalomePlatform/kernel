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

//  File   : PortProperties_i.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _PORTPROPERTIES_I_HXX_
#define _PORTPROPERTIES_I_HXX_

#include "DSC_Basic.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Ports)

/*! \class PortProperties_i
 *  \brief This class implements the interface Ports::PortProperties.
 */
class DSC_BASIC_EXPORT PortProperties_i:
  public virtual POA_Ports::PortProperties
{
  public:
    PortProperties_i();
    virtual ~PortProperties_i();

    /*!
     * CORBA method : set a value to a property
     * \see Ports::PortProperties::set_property
     */
    virtual void set_property(const char * name,
                              const CORBA::Any& value)
      throw (Ports::NotDefined, Ports::BadType);

    /*!
     * CORBA method : get the value of a property
     * \see Ports::PortProperties::get_property
     */
    virtual CORBA::Any* get_property(const char* name)
      throw (Ports::NotDefined);
};

#endif
