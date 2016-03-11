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

//  File   : param_double_port_uses.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#include "Param_Double_Port_uses_i.hxx"

Param_Double_Port_uses_i::Param_Double_Port_uses_i(Engines_ParallelDSC_i * par_compo, 
                                                   std::string port_name,
                                                   CORBA::ORB_ptr orb)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _fake_properties = new PortProperties_i();
  _fake_prop_ref = _fake_properties->_this();
  _fake_properties->_remove_ref();

  _par_compo = par_compo;
  _port_name = port_name;
  _provides_port = NULL;

  paco_fabrique_manager * pfm = paco_getFabriqueManager();
  pfm->register_comScheduling("Param_Double_Port_uses_i_direct", new paco_direct_fabrique());
  pfm->register_distribution("Param_Double_Port_uses_i_GaBro", new GaBro_fab());
  pfm->register_distribution("Param_Double_Port_uses_i_BasicBC", new BasicBC_fab());
}

Param_Double_Port_uses_i::~Param_Double_Port_uses_i()
{
  if (_provides_port)
  {
    _provides_port->stop();
    delete _provides_port;
  }
}

void 
Param_Double_Port_uses_i::add_port_to_component()
{
    _par_compo->add_uses_port("IDL:Ports/Param_Double_Port:1.0", 
                              _port_name.c_str(),
                              _fake_prop_ref);
}

void
Param_Double_Port_uses_i::start_port()
{
  Engines::DSC::uses_port * uport = _par_compo->get_uses_port(_port_name.c_str());  
  _proxy_port =  Ports::Param_Double_Port::_narrow((*uport)[0]);
  _provides_port = Ports::PaCO_Param_Double_Port::PaCO_narrow(_proxy_port, _orb);
  _provides_port->copyClientGlobalContext(_par_compo);
  _provides_port->init(_par_compo->getMyRank(), _par_compo->getTotalNode());
  
  // Il faut maintenant configurer les bibliothèques
  // de redistributions de la fonction put
  ParallelMethodContext * method_ptr;
  method_ptr = _provides_port->getParallelMethodContext("put");
  method_ptr->setLibComScheduling("Param_Double_Port_uses_i_direct"); 
  method_ptr->setDistLibArg("param_data", "Param_Double_Port_uses_i_BasicBC", "in");
  BasicBC * dislib = (BasicBC *) method_ptr->getDistLibArg("param_data", "in");
  dislib->setEltSize(sizeof(CORBA::Double));
  dislib->setBlocSize(0); // BLOC
  dislib->setNodeRank(_par_compo->getMyRank());

  // Il faut maintenant configurer les bibliothèques
  // de redistributions de la fonction get_results
  method_ptr = _provides_port->getParallelMethodContext("get_results");
  method_ptr->setLibComScheduling("Param_Double_Port_uses_i_direct"); 
  method_ptr->setDistLibArg("param_results", "Param_Double_Port_uses_i_GaBro", "out");
  GaBro * dislib_gabro = (GaBro *) method_ptr->getDistLibArg("param_results", "out");
  dislib_gabro->setEltSize(sizeof(CORBA::Double));

  _provides_port->start();
  delete uport;
}

void 
Param_Double_Port_uses_i::configure_port_method_put(int totalNbElt)
{
  ParallelMethodContext * method_ptr;
  method_ptr = _provides_port->getParallelMethodContext("put");
  BasicBC * dislib = (BasicBC *) method_ptr->getDistLibArg("param_data", "in");
  dislib->setTotalNbElt(totalNbElt);
}

void 
Param_Double_Port_uses_i::put(const Ports::Param_Double_Port::seq_double & param_data)
{
  _provides_port->put(param_data);
}

void 
Param_Double_Port_uses_i::get_results(Ports::Param_Double_Port::seq_double_out param_results)
{
  _provides_port->get_results(param_results);
}
