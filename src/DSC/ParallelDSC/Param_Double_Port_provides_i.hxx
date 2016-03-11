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

//  File   : param_double_port_provides.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _PARAM_DOUBLE_PORT_PROVIDES_HXX_
#define _PARAM_DOUBLE_PORT_PROVIDES_HXX_

#include "SALOME_ParamPortsPaCO_Ports_Param_Double_Port_server.hxx"

#include "ParallelDSC_i.hxx"
#include "PortProperties_i.hxx"

class Param_Double_Port_provides_i :
  public virtual Ports::Param_Double_Port_serv
{
  public :
    Param_Double_Port_provides_i(CORBA::ORB_ptr orb, char * ior, int rank);
    virtual ~Param_Double_Port_provides_i();

    void put(const Ports::Param_Double_Port::seq_double & param_data);
    void get_results(Ports::Param_Double_Port::seq_double_out param_results);

    // local methods
    Ports::Param_Double_Port::seq_double * get_data();
    void set_data(Ports::Param_Double_Port::seq_double * results);
    void configure_set_data(int data_length, 
                            int totalNbElt, 
                            int BeginEltPos);

    // Aide à la création du port
    static Param_Double_Port_provides_i * init_port(Engines_ParallelDSC_i * par_compo, 
                                                    std::string port_name,
                                                    CORBA::ORB_ptr orb);

    // Méthode temporaire en attendant d'avoir des méthodes parallèles bien synchronisé
    static void wait_init_port(Engines_ParallelDSC_i * par_compo, 
                               std::string port_name,
                               CORBA::ORB_ptr orb);
  private:
    // Buffers pour la réception et l'envoi
    Ports::Param_Double_Port::seq_double * _seq_data;
    Ports::Param_Double_Port::seq_double * _seq_results;

    // Variable pour la gestion du buffer de réception
    pthread_mutex_t * seq_data_mutex;
    pthread_cond_t * seq_data_condition;
    bool seq_data_termine;
    pthread_mutex_t * seq_data_mutex_cp;
    pthread_cond_t * seq_data_condition_cp;
    bool seq_data_termine_cp;

    // Variable pour la gestion du buffer d'envoi
    pthread_mutex_t * seq_results_mutex;
    pthread_cond_t * seq_results_condition;
    bool seq_results_termine;
    pthread_mutex_t * seq_results_mutex_cp;
    pthread_cond_t * seq_results_condition_cp;
    bool seq_results_termine_cp;
};
#endif
