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

//  File   : param_double_port_provides.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#include <iostream>
#include <string>
#include <sstream>

#include "Param_Double_Port_provides_i.hxx"

#include <paco_omni.h>
#include <paco_dummy.h>
#include <paco_mpi.h>
#include <paco_direct_comScheduling.h>
#include <GaBro.h>
#include <BasicBC.h>

Param_Double_Port_provides_i::Param_Double_Port_provides_i(CORBA::ORB_ptr orb, char * ior, int rank) :
  Ports::Param_Double_Port_serv(orb,ior,rank),
  Ports::Param_Double_Port_base_serv(orb,ior,rank),
  Ports::Data_Port_serv(orb,ior,rank),
  Ports::Data_Port_base_serv(orb,ior,rank),
  Ports::Port_serv(orb,ior,rank),
  Ports::Port_base_serv(orb,ior,rank),
  InterfaceParallel_impl(orb,ior,rank)
{
  _seq_data = NULL;

  seq_data_termine = false;                 
  seq_data_mutex = new pthread_mutex_t();
  pthread_mutex_init(seq_data_mutex, NULL);
  seq_data_condition = new pthread_cond_t();
  pthread_cond_init(seq_data_condition, NULL);
  seq_data_termine_cp = true;               
  seq_data_mutex_cp = new pthread_mutex_t();
  pthread_mutex_init(seq_data_mutex_cp, NULL);
  seq_data_condition_cp = new pthread_cond_t();
  pthread_cond_init(seq_data_condition_cp, NULL);

  _seq_results = NULL;

  seq_results_termine = false;              
  seq_results_mutex = new pthread_mutex_t();
  pthread_mutex_init(seq_results_mutex, NULL);
  seq_results_condition = new pthread_cond_t();
  pthread_cond_init(seq_results_condition, NULL);
  seq_results_termine_cp = true;                    
  seq_results_mutex_cp = new pthread_mutex_t();
  pthread_mutex_init(seq_results_mutex_cp, NULL);
  seq_results_condition_cp = new pthread_cond_t();
  pthread_cond_init(seq_results_condition_cp, NULL);
}

Param_Double_Port_provides_i::~Param_Double_Port_provides_i() 
{
  if (_seq_data)
    delete _seq_data;

  pthread_mutex_destroy(seq_data_mutex);
  delete seq_data_mutex;
  pthread_cond_destroy(seq_data_condition);
  delete seq_data_condition;
  pthread_mutex_destroy(seq_data_mutex_cp);
  delete seq_data_mutex_cp;
  pthread_cond_destroy(seq_data_condition_cp);
  delete seq_data_condition_cp;

  if (_seq_results)
    delete _seq_results;

  pthread_mutex_destroy(seq_results_mutex);
  delete seq_results_mutex;
  pthread_cond_destroy(seq_results_condition);
  delete seq_results_condition;
  pthread_mutex_destroy(seq_results_mutex_cp);
  delete seq_results_mutex_cp;
  pthread_cond_destroy(seq_results_condition_cp);
  delete seq_results_condition_cp;
}

Param_Double_Port_provides_i *
Param_Double_Port_provides_i::init_port(Engines_ParallelDSC_i * par_compo, 
                                        std::string port_name,
                                        CORBA::ORB_ptr orb)
{
  int rank = par_compo->getMyRank();
  int totalNode = par_compo->getTotalNode();
  paco_com * com = par_compo->getCom();

  MESSAGE("Configuration of Param_Double_Port_provides: rank = " << rank << " totalNode = " << totalNode);

  // DOIT ETRE DEJA FAIT AVANT !!!???
  paco_fabrique_manager* pfm = paco_getFabriqueManager();
  pfm->register_com("pdp_dummy", new paco_dummy_fabrique());
  pfm->register_thread("pdp_thread", new paco_omni_fabrique());
  pfm->register_comScheduling("pdp_direct", new paco_direct_fabrique());
  pfm->register_distribution("pdp_GaBro", new GaBro_fab());
  pfm->register_distribution("pdp_BasicBC", new BasicBC_fab());

  Param_Double_Port_provides_i * port = NULL; 
  Ports::Param_Double_Port_proxy_impl * proxy_node = NULL; 

  std::cerr << "Creating Proxy" << std::endl;
  if (rank == 0) {
    // On commence par créer le proxy
    // Il est enregistré dans le composant et sera détruit automatiquement
    // lorsque le composant sera détruit
    proxy_node = 
      new Ports::Param_Double_Port_proxy_impl(CORBA::ORB::_duplicate(orb),
                                              pfm->get_thread("pdp_thread"));
    proxy_node->setLibCom("pdp_dummy", proxy_node);
    proxy_node->setLibThread("pdp_thread");
    PaCO::PacoTopology_t serveur_topo;
    serveur_topo.total = totalNode;
    proxy_node->setTopology(serveur_topo);

    // Création de la propriété
    PortProperties_i * proxy_node_properties = new PortProperties_i();

    // Enregistrement du proxy
    par_compo->add_parallel_provides_proxy_port(proxy_node->_this(), 
                                                port_name.c_str(),
                                                proxy_node_properties->_this());
    proxy_node->_remove_ref();
    proxy_node_properties->_remove_ref();
  }
  else {
    par_compo->add_parallel_provides_proxy_wait(port_name.c_str());
  }

  std::cerr << "Getting proxy" << std::endl;
  char * proxy_ior = (char * ) par_compo->get_proxy(port_name.c_str());
  std::cerr << "Proxy ior is : " << proxy_ior << std::endl;

  port = new Param_Double_Port_provides_i(CORBA::ORB::_duplicate(orb), proxy_ior, rank);
  port->copyClientGlobalContext(par_compo);

  // Il faut maintenant configurer les bibliothèques
  // de redistributions de la fonction put
  ParallelMethodContext * method_ptr = port->getParallelMethodContext("put");
  method_ptr->setLibComScheduling("pdp_direct"); 
  method_ptr->setDistLibArg("param_data", "pdp_BasicBC", "in");
  BasicBC * dislib = (BasicBC *) method_ptr->getDistLibArg("param_data", "in");
  dislib->setEltSize(sizeof(CORBA::Double));

  // Il faut maintenant configurer les bibliothèques
  // de redistributions de la fonction get_results
  method_ptr = port->getParallelMethodContext("get_results");
  method_ptr->setLibComScheduling("pdp_direct"); 
  method_ptr->setDistLibArg("param_results", "pdp_GaBro", "out");
  GaBro * dislib_gabro = (GaBro *) method_ptr->getDistLibArg("param_results", "out");
  dislib_gabro->setEltSize(sizeof(CORBA::Double));

  // Enregistement du port 
  for (int i = 0; i < totalNode; i++) 
  {
    std::ostringstream node_number;
    node_number << i;
    std::string event_name("AddNode");
    event_name += node_number.str();
    std::string tag_name = proxy_ior;

    if (i == rank) {
      std::cerr << "Adding node of processor : " << i << std::endl;
      par_compo->add_parallel_provides_node_port(Ports::Port_PaCO::_narrow(port->_this()), port_name.c_str());
      port->_remove_ref();
      par_compo->InterfaceParallel_impl::_proxy->send_event(event_name.c_str(), tag_name.c_str());
    }

    par_compo->wait_event(event_name.c_str(), tag_name.c_str());
  }

  // On démarre l'objet parallèle
  std::string event_name("StartingProxy");
  std::string tag_name = proxy_ior;
  if (rank == 0) 
  {
    proxy_node->start();
    par_compo->InterfaceParallel_impl::_proxy->send_event(event_name.c_str(), tag_name.c_str());
  }

  CORBA::string_free(proxy_ior);
  return port;
}

void
Param_Double_Port_provides_i::wait_init_port(Engines_ParallelDSC_i * par_compo, 
                                             std::string port_name,
                                             CORBA::ORB_ptr orb)
{
  int rank = par_compo->getMyRank();
  int totalNode = par_compo->getTotalNode();
  // Enregistement du port 
  for (int i = 0; i < totalNode; i++) 
  {
    std::ostringstream node_number;
    node_number << i;
    std::string event_name("WaitingNode");
    event_name += node_number.str();
    char * proxy_ior = (char * ) par_compo->get_proxy(port_name.c_str());
    std::string tag_name(proxy_ior);
    CORBA::string_free(proxy_ior);
    if (i == rank) 
      par_compo->InterfaceParallel_impl::_proxy->send_event(event_name.c_str(), tag_name.c_str());
    par_compo->wait_event(event_name.c_str(), tag_name.c_str());
  }
}

void 
Param_Double_Port_provides_i::put(const Ports::Param_Double_Port::seq_double & param_data)
{

  // On attend que le get soit fait
  // Au départ seq_data_termine_cp = TRUE
  pthread_mutex_lock(seq_data_mutex_cp);
  while (seq_data_termine_cp == false)
  {
     pthread_cond_wait(seq_data_condition_cp, seq_data_mutex_cp);
  }
  seq_data_termine_cp = false;
  pthread_mutex_unlock(seq_data_mutex_cp);

  pthread_mutex_lock(seq_data_mutex);

  // Création d'une nouvelle séquence
  // Elle prend le buffer sans le copier
  Ports::Param_Double_Port::seq_double * n_param_data = (Ports::Param_Double_Port::seq_double *) &param_data;
  _seq_data = new Ports::Param_Double_Port::seq_double(n_param_data->length(), n_param_data->length(), n_param_data->get_buffer(1), 1);

  seq_data_termine = true;
  pthread_cond_signal(seq_data_condition);
  pthread_mutex_unlock(seq_data_mutex);
}
    
void 
Param_Double_Port_provides_i::get_results(Ports::Param_Double_Port::seq_double_out param_results)
{
  pthread_mutex_lock(seq_results_mutex);
  while (seq_results_termine == false)
  {
     pthread_cond_wait(seq_results_condition, seq_results_mutex);
  }

  // Création d'une nouvelle séquence
  // Elle prend le buffer sans le copier
  param_results = new Ports::Param_Double_Port::seq_double(_seq_results->length(), _seq_results->length(), _seq_results->get_buffer(1), 1);
  delete _seq_results;
  _seq_results = NULL;

  seq_results_termine = false;
  pthread_mutex_unlock(seq_results_mutex);

  // On indique que l'on a copié la valeur
  // Et donc que l'on peut recevoir une nouvelle valeur
  pthread_mutex_lock(seq_results_mutex_cp);
  seq_results_termine_cp = true;
  pthread_cond_signal(seq_results_condition_cp);
  pthread_mutex_unlock(seq_results_mutex_cp);
}

Ports::Param_Double_Port::seq_double *
Param_Double_Port_provides_i::get_data()
{
  Ports::Param_Double_Port::seq_double * result = NULL;

  pthread_mutex_lock(seq_data_mutex);
  while (seq_data_termine == false)
  {
     pthread_cond_wait(seq_data_condition, seq_data_mutex);
  }

  // Création d'une nouvelle séquence
  // Elle prend le buffer sans le copier
  result = new Ports::Param_Double_Port::seq_double(_seq_data->length(), _seq_data->length(), _seq_data->get_buffer(1), 1);
  delete _seq_data;
  _seq_data = NULL;

  seq_data_termine = false;
  pthread_mutex_unlock(seq_data_mutex);

  // On indique que l'on a copié la valeur
  // Et donc que l'on peut recevoir une nouvelle valeur
  pthread_mutex_lock(seq_data_mutex_cp);
  seq_data_termine_cp = true;
  pthread_cond_signal(seq_data_condition_cp);
  pthread_mutex_unlock(seq_data_mutex_cp);
  return result;
}

void
Param_Double_Port_provides_i::set_data(Ports::Param_Double_Port::seq_double * results)
{
  // On attend que le get soit fait
  // Au départ seq_results_termine_cp = TRUE
  pthread_mutex_lock(seq_results_mutex_cp);
  while (seq_results_termine_cp == false)
  {
     pthread_cond_wait(seq_results_condition_cp, seq_results_mutex_cp);
  }
  seq_results_termine_cp = false;
  pthread_mutex_unlock(seq_results_mutex_cp);

  pthread_mutex_lock(seq_results_mutex);

  // Création d'une nouvelle séquence
  // Elle prend le buffer sans le copier
  _seq_results = new Ports::Param_Double_Port::seq_double(results->length(), results->length(), results->get_buffer(1), 1);

  seq_results_termine = true;
  pthread_cond_signal(seq_results_condition);
  pthread_mutex_unlock(seq_results_mutex);
}

void 
Param_Double_Port_provides_i::configure_set_data(int data_length, int totalNbElt, int BeginEltPos)
{
  // Configuration de la biblothèque de redistribution
  // pour les données actuelles
  ParallelMethodContext * method_ptr = getParallelMethodContext("get_results");
  GaBro * dislib = (GaBro *) method_ptr->getDistLibArg("param_results", "out");
  dislib->setNodeNbElt(data_length);
  dislib->setTotalNbElt(totalNbElt);
  dislib->setNodePos(BeginEltPos);
}
