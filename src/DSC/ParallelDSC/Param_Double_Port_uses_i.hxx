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

//  File   : param_double_port_uses.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _PARAM_DOUBLE_PORT_USES_HXX_
#define _PARAM_DOUBLE_PORT_USES_HXX_

#include "SALOME_ParamPortsPaCO_Ports_Param_Double_Port_client.hxx"

#include "ParallelDSC_i.hxx"
#include "PortProperties_i.hxx"

#include <paco_direct_comScheduling.h>
#include <GaBro.h>
#include <BasicBC.h>

class Param_Double_Port_uses_i
{
  public :
    Param_Double_Port_uses_i(Engines_ParallelDSC_i * par_compo, 
                             std::string port_name,
                             CORBA::ORB_ptr orb);
    virtual ~Param_Double_Port_uses_i();

    // Port local init methods
    virtual void add_port_to_component();
    virtual void start_port();
    void configure_port_method_put(int totalNbElt); 

    // Port methods
    void put(const Ports::Param_Double_Port::seq_double & param_data);
    void get_results(Ports::Param_Double_Port::seq_double_out param_results);

  private :
    CORBA::ORB_var _orb;
    std::string _port_name;
    Engines_ParallelDSC_i * _par_compo;
    Ports::Param_Double_Port_var _proxy_port;
    PortProperties_i *  _fake_properties;
    Ports::PortProperties_var _fake_prop_ref;
    Ports::PaCO_Param_Double_Port * _provides_port;
};
#endif

