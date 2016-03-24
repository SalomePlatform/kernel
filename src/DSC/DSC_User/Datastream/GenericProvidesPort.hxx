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

// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (Wed, 28 Feb 2007) $
// Id          : $Id$
//
#ifndef _GENERIC_PROVIDES_PORT_HXX_
#define _GENERIC_PROVIDES_PORT_HXX_

#include "provides_port.hxx"
#include "GenericPort.hxx"


template <typename DataManipulator, typename CouplingPolicy, typename ProvidesPort=provides_port> 
class GenericProvidesPort : public ProvidesPort,
                            public GenericPort< DataManipulator , CouplingPolicy > {
public :
  // Type de données manipulés 
  typedef typename DataManipulator::Type         DataType;
  typedef typename DataManipulator::CorbaInType  CorbaInDataType;

  virtual ~GenericProvidesPort() {};

};


#endif
