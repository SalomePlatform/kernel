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

//  File   : palm_data_seq_short_port_provides.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-08 19:01:14 +0100 (lun, 08 jan 2007) $
// Id          : $Id$
//
#ifndef _PALM_DATA_SEQ_SHORT_PORT_PROVIDES_HXX_
#define _PALM_DATA_SEQ_SHORT_PORT_PROVIDES_HXX_

#include "Palm_Ports.hh"
#include "provides_port.hxx"
#include "GenericPort.hxx"
#include "PalmCouplingPolicy.hxx"

class palm_data_seq_short_port_provides :
  public virtual POA_Ports::Palm_Ports::Palm_Data_Seq_Short_Port,
  public virtual provides_port,
  public virtual GenericPort< seq_u_manipulation<Ports::Palm_Ports::seq_short, CORBA::Short>,
                              PalmCouplingPolicy >
{
  typedef Ports::Palm_Ports::seq_short                       CorbaDataType;
  typedef seq_u_manipulation<CorbaDataType, CORBA::Short>    DataManipulator;
  typedef GenericPort< DataManipulator, PalmCouplingPolicy > Port;
  
  public :
    palm_data_seq_short_port_provides() {}
    virtual ~palm_data_seq_short_port_provides() {}

  void put(DataManipulator::CorbaInType data, CORBA::Long time, CORBA::Long tag) {
    Port::put(data, time, tag);
  }

  Ports::Port_ptr get_port_ref() {
    return _this();
  }
};

#endif
