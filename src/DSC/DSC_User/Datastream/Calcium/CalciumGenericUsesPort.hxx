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

//  File   : CalciumGenericUsesPort.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Id          : $Id$
//
#ifndef _CALCIUM_GENERIC_USES_PORT_HXX_
#define _CALCIUM_GENERIC_USES_PORT_HXX_

#include "GenericUsesPort.hxx"
#include "calcium_uses_port.hxx"

template <typename DataManipulator, typename CorbaPortType, char * repositoryName > 
class CalciumGenericUsesPort : public GenericUsesPort<DataManipulator,CorbaPortType, repositoryName,
                                               calcium_uses_port >
{
public :
  virtual ~CalciumGenericUsesPort() {};
  void disconnect(bool provideLastGivenValue);

};


template <typename DataManipulator,typename CorbaPortType, char * repositoryName > 
void
CalciumGenericUsesPort< DataManipulator,CorbaPortType, repositoryName >::disconnect(bool provideLastGivenValue ) {

  typedef typename CorbaPortType::_ptr_type CorbaPortTypePtr;

  if (!this->_my_ports)
    throw DSC_Exception(LOC("There is no connected provides port to communicate with."));

  for(int i = 0; i < this->_my_ports->length(); i++) {
    CorbaPortTypePtr port = CorbaPortType::_narrow((*this->_my_ports)[i]);
    try {
#ifdef MYDEBUG
      std::cerr << "-------- CalciumGenericUsesPort<>::disconnect: "<< i << std::endl;
#endif

      port->disconnect(provideLastGivenValue);
    } catch(const CORBA::SystemException& ex){
      std::cerr << "Can't call disconnect on provides port " << i << std::endl;
    }
  }
  
}


#endif
