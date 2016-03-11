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

//  File   : ConnectionManager_i.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _CONNECTION_MANAGER_I_HXX_
#define _CONNECTION_MANAGER_I_HXX_

#include "DSC_Basic.hxx"

#include <iostream>
#include <map>
#include <pthread.h>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(DSC_Engines)

/*! \class ConnectionManager_i
 *  \brief This class implements the interface Engines::ConnectionManager.
 */
class ConnectionManager_i :
  public virtual POA_Engines::ConnectionManager
{
  public :
    ConnectionManager_i(CORBA::ORB_ptr orb);
    virtual ~ConnectionManager_i();

    /*!
     * CORBA method : connect two ports of two components.
     * \see Engines::ConnectionManager::connect
     */
    Engines::ConnectionManager::connectionId connect(Engines::DSC_ptr uses_component, 
                                                     const char* uses_port_name, 
                                                     Engines::DSC_ptr provides_component, 
                                                     const char* provides_port_name);

    /*!
     * CORBA method : releases a connection performed with 
     * ConnectionManager_i::connect.
     *
     * \see Engines::ConnectionManager::disconnect
     */
    void disconnect(Engines::ConnectionManager::connectionId id,
                    Engines::DSC::Message message);

    /*!
       Shutdown the ConnectionManager process.
     */
    void ShutdownWithExit();

    /*!
       Returns the PID of the connection manager
     */
    CORBA::Long getPID();

  private :

    struct connection_infos {
      Engines::DSC_var uses_component; 
      std::string uses_port_name;
      Engines::DSC_var provides_component;
      std::string provides_port_name;
      Ports::Port_var provides_port;
    };

    typedef std::map<Engines::ConnectionManager::connectionId, 
            connection_infos *> ids_type;
    typedef std::map<Engines::ConnectionManager::connectionId, 
            connection_infos *>::iterator ids_it_type;

    ids_type ids;
    ids_it_type ids_it;

    int current_id;
    pthread_mutex_t mutex;
  protected:
    CORBA::ORB_var _orb;
};

#endif
