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

//  File   : DSC_interface.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _DSC_INTERFACE_HXX_
#define _DSC_INTERFACE_HXX_

#include <iostream>
#include <map>
#include <string.h>
#include <assert.h>

#include "DSC_Callbacks.hxx"

/*! \class Engines_DSC_interface
 *  \brief This class implements the interface Engines::DSC
 *
 *  This class is used by the sequential DSC implementation
 *  and the parallel DSC implementation.
 */
class Engines_DSC_interface: 
  public DSC_Callbacks
{
public:
  Engines_DSC_interface();
  virtual ~Engines_DSC_interface();

  /*!
   * \see Engines::DSC::add_provides_port
   */
  virtual void add_provides_port(Ports::Port_ptr ref, 
                                 const char* provides_port_name,
                                 Ports::PortProperties_ptr port_prop) 
    throw (Engines::DSC::PortAlreadyDefined,
           Engines::DSC::NilPort,
           Engines::DSC::BadProperty);

  /*!
   * \see Engines::DSC::add_uses_port
   */
  virtual void add_uses_port(const char* repository_id, 
                             const char* uses_port_name,
                             Ports::PortProperties_ptr port_prop)
    throw (Engines::DSC::PortAlreadyDefined,
           Engines::DSC::BadProperty);

  /*!
   * \see Engines::DSC::get_provides_port
   */
  virtual Ports::Port_ptr get_provides_port(const char* provides_port_name,
                                            const CORBA::Boolean connection_error) 
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected,
           Engines::DSC::BadPortType);

  /*!
   * \see Engines::DSC::get_uses_port
   */
  virtual Engines::DSC::uses_port * get_uses_port(const char* uses_port_name)
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected,
           Engines::DSC::BadPortType);

  /*!
   * \see Engines::DSC::connect_provides_port
   *
   * \note This method uses Callbacks mechanism to inform the provides
   * port how much uses ports are connected with. Currently, the provides
   * port doesn't know its uses ports references. It's framework or application role
   * to manage connections between ports.
   */
  virtual void connect_provides_port(const char* provides_port_name)
    throw (Engines::DSC::PortNotDefined);

  /*!
   * \see Engines::DSC::connect_uses_port
   *
   * \note This method uses Callbacks mechanism to inform the uses
   * port how much provides ports are connected with.
   */
  virtual void connect_uses_port(const char* uses_port_name,
                                 Ports::Port_ptr provides_port_ref) 
  throw (Engines::DSC::PortNotDefined,
         Engines::DSC::BadPortType,
         Engines::DSC::NilPort);

  /*!
   * \see Engines::DSC::is_connected
   */
  virtual CORBA::Boolean is_connected(const char* port_name)
    throw (Engines::DSC::PortNotDefined);

   /*!
   * \see Engines::DSC::disconnect_provides_port
   *
   * \note This method uses Callbacks mechanism to inform the provides
   * port how much uses ports are connected with. Currently, the provides
   * port doesn't know its uses ports references. It's framework or application role
   * to manage connections between ports.
   */
  virtual void disconnect_provides_port(const char* provides_port_name,
                                        const Engines::DSC::Message message)
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected);

   /*!
   * \see Engines::DSC::disconnect_uses_port
   *
   *
   * \note This method uses Callbacks mechanism to inform the uses
   * port how much provides ports are connected with. 
   *
   * \warning The new sequence of the uses port is sended by the callback. 
   * The old sequence is not destoyed. Is uses port user's role to destroy 
   * the sequence.
   */
  virtual void disconnect_uses_port(const char* uses_port_name,
                                    Ports::Port_ptr provides_port_ref,
                                    const Engines::DSC::Message message)
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected,
           Engines::DSC::BadPortReference);

  /*!
   * \see Engines::DSC::get_port_properties
   */
  virtual Ports::PortProperties_ptr get_port_properties(const char* port_name)
    throw (Engines::DSC::PortNotDefined);

  static void writeEvent(const char* request,const std::string& containerName, const char* instance_name,
                         const char* port_name, const char* error, const char* message);

protected:

  /*-------------------------------------------------*/
  /* Definition des types pour le stockage des ports */
  
  enum port_type {uses, provides, none};

  struct port_t {
    port_type type;
    int connection_nbr;
    
    // Specifique aux uses port
    Engines::DSC::uses_port uses_port_refs;
    std::string repository_id;

    // Specifique aux provides port;
    Ports::Port_var provides_port_ref;

    Ports::PortProperties_var port_prop;
  };

  typedef std::map<std::string, port_t *> ports;

  /*-------------------------------------------------*/
  /*-------------------------------------------------*/
 
  ports my_ports;
  ports::iterator my_ports_it;
};

#endif
