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

//  File   : Superv_Component_i.hxx
//  Author : André RIBES (EDF), Eric Fayolle (EDF)
//  Module : KERNEL
//
#ifndef _SUPERV_COMPONENT_I_HXX_
#define _SUPERV_COMPONENT_I_HXX_

#include "DSC_i.hxx"
#include "base_port.hxx"
#include "uses_port.hxx"
#include "provides_port.hxx"
#include "port_factory.hxx"

#include "DSC_Exception.hxx"
#include <vector>

//#define MYDEBUG

/*! \class Superv_Component_i
 *  \brief This class implements DSC_User component.
 *
 *  This class allows a higher programming level than DSC_Basic. It enables
 *  a programming level for service's developpers who want to use DSC ports.
 *
 *  This class has two level for using and declare ports. The higher level proposes
 *  operations to add ports that are provided by default by SALOME like Calcium ports.
 *  It provides too some methods to add their own DSC_User ports.
 *
 *  \note This class doesn't implement the init_service CORBA operation.
 */
class Superv_Component_i :
  public Engines_DSC_i,
  virtual public POA_Engines::Superv_Component
{
public:
  Superv_Component_i(CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr poa,
                     PortableServer::ObjectId * contId,
                     const char *instanceName,
                     const char *interfaceName,
                     bool notif = false);
  Superv_Component_i(CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr poa,
                     Engines::Container_ptr container, 
                     const char *instanceName,
                     const char *interfaceName,
                     bool notif = false,
         bool regist = true );
  virtual ~Superv_Component_i();

  // Exceptions declarations.
  // There are defined on the Superv_Component_i.cxx to avoid problems
  // from dlopen.
  DSC_EXCEPTION(BadFabType);
  DSC_EXCEPTION(BadType);
  DSC_EXCEPTION(BadCast);
  DSC_EXCEPTION(UnexpectedState);
  DSC_EXCEPTION(PortAlreadyDefined);
  DSC_EXCEPTION(PortNotDefined);
  DSC_EXCEPTION(PortNotConnected);
  DSC_EXCEPTION(NilPort);
  DSC_EXCEPTION(BadProperty);
  
  /*!
   * \warning currently disabled.
   */
  virtual provides_port * create_provides_control_port() 
  {return NULL;}

  /*!
   * \warning currently disabled.
   */
  virtual provides_port * create_provides_data_and_control_port(const char* port_type) 
  {return NULL;}

  /*!
   * \warning currently disabled.
   */
  virtual uses_port * create_uses_control_port()
  {return NULL;}

  /*!
   * \warning currently disabled.
   */
  virtual uses_port * create_uses_data_and_control_port(const char* port_type)
  {return NULL;}

  /*!
   * This method permits to create a provides port provided by the platform.
   * (See documentation of DSC for knoing these ports).
   *   
   *
   * \param port_fab_type type provides port.
   * \return the provides port.
   *
   * \note It's user repsonsability to destroy the provides port.
   */
  virtual provides_port * create_provides_data_port(const std::string& port_fab_type)
    throw (BadFabType);


  /*!
   * This method permits to create a uses port provided by the platform.
   * (See documentation of DSC for knoing these ports).
   *   
   *
   * \param port_fab_type type uses port.
   * \return the uses port.
   *
   * \note It's user repsonsability to destroy the uses port.
   */
  virtual uses_port * create_uses_data_port(const std::string& port_fab_type)
    throw (BadFabType); 

  /*!
   * Adds a port to the component. With this method only Salomé's provided DSC ports
   * can be added.
   *
   * \param port_fab_type type of the port.
   * \param port_type uses or provides.
   * \param port_name the name of the port in the component.
   */
  virtual void add_port(const char * port_fab_type,
                        const char * port_type,
                        const char * port_name)
    throw (PortAlreadyDefined, BadFabType, BadType, BadProperty);

  /*!
   * Adds a port to the component. With this method only Salomé's provided DSC ports
   * can be added.
   *
   * \param port_fab_type type of the port.
   * \param port_type uses or provides.
   * \param port_name the name of the port in the component.
   * \return the created port.   
   */
  template < typename SpecificPortType >  
  SpecificPortType * add_port(const char * port_fab_type,
                              const char * port_type,
                              const char * port_name)
    throw (PortAlreadyDefined, BadFabType, BadType, BadCast, BadProperty);

  /*!
   * Adds a created provides port to the component.
   *
   * \param port the provides port.
   * \param provides_port_name the name of the port in the component.
   */
  virtual void add_port(provides_port * port, 
                        const char* provides_port_name)
    throw (PortAlreadyDefined, NilPort, BadProperty);

  /*!
   * Adds a created uses port to the component.
   *
   * \param port the uses port.
   * \param uses_port_name the name of the port in the component.
   */
  virtual void add_port(uses_port * port, 
                        const char* uses_port_name)
    throw (PortAlreadyDefined, NilPort, BadProperty);

  /*!
   * Gets the provides port already added in the component.
   *
   * \param port the provides port pointer.
   * \param provides_port_name the name of the port.
   */
  virtual void get_port(provides_port *& port, 
                        const char* provides_port_name)
    throw (PortNotDefined, PortNotConnected);
  
  /*!
   * Gets the uses port already added in the component.
   *
   * \param port the uses port pointer.
   * \param uses_port_name the name of the port.
   */
  virtual void get_port(uses_port *& port, 
                        const char* uses_port_name)
    throw (PortNotDefined, PortNotConnected);

  /*!
   * Gets the list of the ports of a service.
   * If servicename is not set, all the ports of the component are 
   * returned.
   *
   * \param port_names the ports's list.
   * \param servicename service's name.
   */
  virtual void get_uses_port_names(std::vector<std::string> & port_names,
                                   const std::string servicename="") const;

  /*!
   * Gets a port already added in the component.
   *
   * \param port_name the name of the port.
   * \return a port's pointer.
   */
  template <typename SpecificPortType > 
  SpecificPortType * get_port( const char * port_name)
    throw (PortNotDefined, PortNotConnected, BadCast, UnexpectedState);
 
  /*!
   * \see DSC_Callbacks::provides_port_changed
   */
  virtual void provides_port_changed(const char* provides_port_name,
                                     int connection_nbr,
                                     const Engines::DSC::Message message);

  /*!
   * \see DSC_Callbacks::uses_port_changed
   */
  virtual void uses_port_changed(const char* uses_port_name,
                                 Engines::DSC::uses_port * new_uses_port,
                                 const Engines::DSC::Message message);


  /*!
   * Add a factory the component. If the factory_name is already
   * used, the new library is not added.
   *
   * \param factory_name name of the factory (used by Superv_Component_i::create_provides_data_port
   * and Superv_Component_i::create_uses_data_port)
   * \param factory_ptr factory pointer (destroyed by the component)
   */
  static void register_factory(const std::string & factory_name,
                                port_factory * factory_ptr);

  /*!
   * Get a factory from the component. 
   *
   * \param factory_name name of the factory.
   * \return factory pointer, NULL if the factory doesn't exist.
   */
  virtual port_factory * get_factory(const std::string & factory_name);

  /*!
   */
  static long dscTimeOut;
  static void setTimeOut();
  void beginService(const char *serviceName);


  // This method is implemented by default since it is a very specific usage.
  // It also permits to not break compatibility with older components.
  virtual CORBA::Boolean init_service_with_multiple(const char* service_name,
                                                    const Engines::Superv_Component::seq_multiple_param & params)
  {
    return true;
  }

private:
  // Factory map
  typedef std::map<std::string, port_factory*> factory_map_t;
  static factory_map_t _factory_map;

  /*-------------------------------------------------*/
  // A Superv_Component port.
  struct superv_port_t {
    superv_port_t():u_ref(NULL),p_ref(NULL){};
    ~superv_port_t()
    {
      if(u_ref)delete u_ref;
      if(p_ref)
        {
          // do not delete CORBA servant : deactivate it and then call _remove_ref or delete
          PortableServer::ServantBase* servant=dynamic_cast<PortableServer::ServantBase*>(p_ref);
          if(servant)
            {
              PortableServer::POA_var poa =servant->_default_POA();
              PortableServer::ObjectId_var oid = poa->servant_to_id(servant);
              poa->deactivate_object(oid);
              servant->_remove_ref();
            }
        }
    };
    // For uses ports.
    uses_port * u_ref;
    // For provides ports.
    provides_port * p_ref;
  };

  typedef std::map<std::string, superv_port_t *> superv_ports;

  /*-------------------------------------------------*/
  /*-------------------------------------------------*/

  superv_ports my_superv_ports;
  superv_ports::iterator my_superv_ports_it;
};



template < typename SpecificPortType >  SpecificPortType * 
Superv_Component_i::add_port(const char * port_fab_type,
                             const char * port_type,
                             const char * port_name)
  throw (PortAlreadyDefined, BadFabType, BadType, BadCast, BadProperty)
{
  assert(port_fab_type);
  assert(port_type);
  assert(port_name);
  SpecificPortType * retPort; 

#ifdef MYDEBUG
  std::cout << "---- Superv_Component_i::add_port :  Mark 0 ----  " << port_name << "----" << std::endl;
#endif
    
  std::string s_port_type(port_type);
  if (s_port_type == "provides") {
    provides_port * port = create_provides_data_port(port_fab_type);
    add_port(port, port_name);
    retPort = dynamic_cast<SpecificPortType *>(port);
    if ( retPort == NULL ) { delete port;  
      throw BadCast( LOC("Can't cast to asked port type " ));
    }
  }
  else if (s_port_type == "uses") {
    uses_port * port = create_uses_data_port(port_fab_type);
    add_port(port, port_name);
#ifdef MYDEBUG
    std::cout << "---- Superv_Component_i::add_port :  Mark 1 ----  " << port << "----" << std::endl;
    std::cout << "---- Superv_Component_i::add_port :  Mark 1 ----   get_repository_id()" << port->get_repository_id() << std::endl;
#endif
    retPort = dynamic_cast<SpecificPortType *>(port);
#ifdef MYDEBUG
    std::cout << "---- Superv_Component_i::add_port :  Mark 2 ----  " << retPort << "----" << std::endl;
#endif
    if ( retPort == NULL ) { delete port;  
      throw BadCast( LOC("Can't cast to asked port type " ));
    }
  }
  else
    throw BadType(LOC(OSS()<< "port_type must be either 'provides' either 'uses' not "
                      << port_type));
  
  return retPort;
};


template <typename SpecificPortType > SpecificPortType * 
Superv_Component_i::get_port( const char * port_name)
  throw (PortNotDefined, PortNotConnected, BadCast, UnexpectedState)
{
  assert(port_name);
    
  SpecificPortType * retPort;
  base_port        * port;

  my_superv_ports_it = my_superv_ports.find(port_name);
  if (my_superv_ports_it == my_superv_ports.end())
  {
    throw PortNotDefined( LOC(OSS()<< "Port " << port_name <<" does not exist"));
  }

  superv_port_t * superv_port =  my_superv_ports[port_name];
  try {
    if ( superv_port->p_ref != NULL ) {
      port = superv_port->p_ref;
      Ports::Port_var portref=Engines_DSC_interface::get_provides_port(port_name, false); 
    } 
    else if ( superv_port->u_ref != NULL ) {
      port = superv_port->u_ref;
      Engines::DSC::uses_port * portseq=Engines_DSC_i::get_uses_port(port_name);
      delete portseq;
    } else {
      throw UnexpectedState( LOC(OSS()<< "Internal Error superv_port struct is inconsistent "));
    
    }
  } catch (const Engines::DSC::PortNotDefined&) {
    throw PortNotDefined( LOC(OSS()<< "port "
                              << port_name <<" does not exist."));
  } catch (const Engines::DSC::PortNotConnected&) {
    throw PortNotConnected( LOC(OSS()<< "port " << port_name 
                                << " is not connected."));
  }
  
  retPort = dynamic_cast<SpecificPortType *>(port);
  if ( retPort == NULL ) {
    throw BadCast( LOC("Can't cast to required port type " ));
  }

  return retPort;
};


#endif
