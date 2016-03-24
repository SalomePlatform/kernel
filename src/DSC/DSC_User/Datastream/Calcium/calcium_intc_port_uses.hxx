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

//  File   : calcium_int_port_uses.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 fév 2007) $
// Id          : $Id$
//
#ifndef _CALCIUM_INTC_PORT_USES_HXX_
#define _CALCIUM_INTC_PORT_USES_HXX_

#include <SALOMEconfig.h>

#include "Calcium_Ports.hh"
#include "CalciumGenericUsesPort.hxx"

extern char _repository_Calcium_Intc_Port_name[];

class calcium_intc_port_uses :
  public virtual CalciumGenericUsesPort< seq_u_manipulation<Ports::Calcium_Ports::seq_long,
                                                     CORBA::Long >, 
                                  Ports::Calcium_Ports::Calcium_Intc_Port, 
                                  _repository_Calcium_Intc_Port_name 
                                  >
{
  public :

  virtual ~calcium_intc_port_uses();

};

#endif
