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

//  File   : DSC_i.hxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#ifndef _DSC_I_HXX_
#define _DSC_I_HXX_

#include "DSC_Basic.hxx"

#include <iostream>
#include <map>
#include <string.h>
#include <assert.h>

#include "SALOME_Component_i.hxx"
#include "DSC_interface.hxx"

/*! \class Engines_DSC_i
 *  \brief This class implements the interface Engines::DSC
 *
 *  The implementation of methods are performed by the class Engines_DSC_interface.
 */
class DSC_BASIC_EXPORT Engines_DSC_i: 
  public Engines_Component_i, // keyword virtual is not use to be able to
                              // call the right Engines_Component_i constructor.
                              // If virtual is used, a derived class of Engines_DSC_i
                              // will have to call the right Engines_Component_i constructor.
  public virtual POA_Engines::DSC,
  public Engines_DSC_interface
{
public:
  Engines_DSC_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          PortableServer::ObjectId * contId,
                          const char *instanceName,
                          const char *interfaceName,
                          bool notif = false);
  Engines_DSC_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          Engines::Container_ptr container, 
                          const char *instanceName,
                          const char *interfaceName,
        bool notif = false,
        bool regist = true );

  virtual ~Engines_DSC_i();

  /*!
   * \see Engines::DSC::add_provides_port
   */
  virtual void add_provides_port(Ports::Port_ptr ref, 
                                 const char* provides_port_name,
                                 Ports::PortProperties_ptr port_prop) 
    throw (Engines::DSC::PortAlreadyDefined,
           Engines::DSC::NilPort,
           Engines::DSC::BadProperty) {
      Engines_DSC_interface::add_provides_port(ref, 
                                               provides_port_name,
                                               port_prop);
    }

  /*!
   * \see Engines::DSC::add_uses_port
   */
  virtual void add_uses_port(const char* repository_id, 
                             const char* uses_port_name,
                             Ports::PortProperties_ptr port_prop)
    throw (Engines::DSC::PortAlreadyDefined,
           Engines::DSC::BadProperty) {
      Engines_DSC_interface::add_uses_port(repository_id, 
                                           uses_port_name,
                                           port_prop);
    }

  /*!
   * \see Engines::DSC::get_provides_port
   */
  virtual Ports::Port_ptr get_provides_port(const char* provides_port_name,
                                            const CORBA::Boolean connection_error) 
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected) {
      return Engines_DSC_interface::get_provides_port(provides_port_name,
                                                      connection_error);
    }

  /*!
   * \see Engines::DSC::get_uses_port
   */
  virtual Engines::DSC::uses_port * get_uses_port(const char* uses_port_name)
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected) {
      return Engines_DSC_interface::get_uses_port(uses_port_name);
    }

  /*!
   * \see Engines::DSC::connect_provides_port
   */
  virtual void connect_provides_port(const char* provides_port_name)
    throw (Engines::DSC::PortNotDefined) {
      Engines_DSC_interface::connect_provides_port(provides_port_name);
    }

  /*!
   * \see Engines::DSC::connect_uses_port
   */
  virtual void connect_uses_port(const char* uses_port_name,
                                 Ports::Port_ptr provides_port_ref) 
  throw (Engines::DSC::PortNotDefined,
         Engines::DSC::BadPortType,
         Engines::DSC::NilPort) {
    Engines_DSC_interface::connect_uses_port(uses_port_name,
                                             provides_port_ref);
  }

  /*!
   * \see Engines::DSC::is_connected
   */
  virtual CORBA::Boolean is_connected(const char* port_name)
    throw (Engines::DSC::PortNotDefined) {
      return Engines_DSC_interface::is_connected(port_name);
    }

   /*!
   * \see Engines::DSC::disconnect_provides_port
   */
  virtual void disconnect_provides_port(const char* provides_port_name,
                                        const Engines::DSC::Message message)
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected) {
      Engines_DSC_interface::disconnect_provides_port(provides_port_name,
                                                      message);
    }

   /*!
   * \see Engines::DSC::disconnect_uses_port
   */
  virtual void disconnect_uses_port(const char* uses_port_name,
                                    Ports::Port_ptr provides_port_ref,
                                    const Engines::DSC::Message message)
    throw (Engines::DSC::PortNotDefined,
           Engines::DSC::PortNotConnected,
           Engines::DSC::BadPortReference) {
      Engines_DSC_interface::disconnect_uses_port(uses_port_name,
                                                  provides_port_ref,
                                                  message);
    }

  /*!
   * \see Engines::DSC::get_port_properties
   */
  virtual Ports::PortProperties_ptr get_port_properties(const char* port_name)
    throw (Engines::DSC::PortNotDefined) {
      return Engines_DSC_interface::get_port_properties(port_name);
    }
};

#endif
