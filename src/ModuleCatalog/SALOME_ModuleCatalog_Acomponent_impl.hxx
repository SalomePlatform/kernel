//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_ModuleCatalog_Acomponent_impl.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef ACOMPONENT_IMPL_H
#define ACOMPONENT_IMPL_H

#include "utilities.h"
#include "PathPrefix.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_ModuleCatalog)

class SALOME_ModuleCatalog_AcomponentImpl: public POA_SALOME_ModuleCatalog::Acomponent,
                      public PortableServer::RefCountServantBase 
{
public:
  //! standard constructor
  SALOME_ModuleCatalog_AcomponentImpl(const char* name, 
				      const char* username, 
				      const char* constraint, 
				      SALOME_ModuleCatalog::ComponentType componenttype, 
				      CORBA::Boolean componentmultistudy,
				      const char* icone,
				      SALOME_ModuleCatalog::ListOfDefInterface list_interfaces,
				      ListOfPathPrefix pathes);

  //! standard destructor
  virtual ~SALOME_ModuleCatalog_AcomponentImpl();

  //! method to get a list of the interfaces name of a component
  /*!
    \return a list of the interfaces of a component 
  */
  virtual SALOME_ModuleCatalog::ListOfInterfaces* GetInterfaceList() ;

  //! method to get one interface of a component
  /*! If the specified interface doesn't exist, the Notfound exception is thrown
    \param interface name const car* arguments
    \return the wanted interface
  */
  virtual SALOME_ModuleCatalog::DefinitionInterface* 
             GetInterface(const char* interfacename)
             throw(SALOME_ModuleCatalog::NotFound);

  //! method to get a list of the services name of an interface of a component
 /*!If the specified interface doesn't exist, the Notfound exception is thrown
   \param interfacename const char* arguments 
   \return a list of the services of the wanted interface
 */
  virtual SALOME_ModuleCatalog::ListOfServices* 
             GetServiceList(const char* interfacename)
             throw(SALOME_ModuleCatalog::NotFound);

    
  //! method to get one service of an interface of a component
 /*! If the wanted service or the specified interface don't exist, 
  *  the Notfound exception is thrown
   \param interfacename const char* arguments 
   \param servicename const char* arguments
   \return the wanted service
 */
  virtual SALOME_ModuleCatalog::Service* 
             GetService(const char* interfacename, 
			const char* servicename) 
             throw(SALOME_ModuleCatalog::NotFound);

  //! method to get the default service of an interface of a component
  /*! If the specified interface doesn't exist, the Notfound exception is thrown
    \param interfacename const char* arguments 
    \return the default service of the interface
  */
  virtual SALOME_ModuleCatalog::Service* 
             GetDefaultService(const char* interfacename) 
             throw(SALOME_ModuleCatalog::NotFound);

  //! method to get the PathPrefix of a computer
 /*! If the wanted computer doesn't exist, the Notfound exception is thrown
   \param machinename const char* arguments 
   \return the prefix path
 */
  virtual char* GetPathPrefix(const char* machinename) 
                throw(SALOME_ModuleCatalog::NotFound);

  //! method to obtain the constraint affected to a component
  /*! to be resolved by LifeCycle for the computer choice
    \return the constraint
  */
  virtual char* constraint() ;

  //! method to obtain the componentname
  /*!
    \return the component name
  */
  virtual char* componentname();

  //! method to obtain the componentusername
  /*!
    \return the component user name
  */
  virtual char* componentusername();

  //! method to define if a component can be multistudy or not
  /*!
    \return true if the component supports multistudy
  */
  virtual CORBA::Boolean multistudy();

  //! method to define the type of the component
  /*!
    \return the component type
  */
  virtual SALOME_ModuleCatalog::ComponentType component_type() ;

  //! method to obtain the component icone (for IAPP)
  /*!
    \return the component icone
  */
  virtual char* component_icone();

private :
  char* _component_name ;
  char* _component_user_name ;
  char* _constraint;
  char* _icone;
  SALOME_ModuleCatalog::ComponentType _componenttype;
  CORBA::Boolean _componentmultistudy;
  SALOME_ModuleCatalog::ListOfDefInterface _list_interfaces;
  ListOfPathPrefix _pathes ;
  

 //! method to duplicate a service
  /*!
    \param service SALOME_ModuleCatalog::Service arguments
    \return the service
  */
  virtual SALOME_ModuleCatalog::Service_var _duplicate_service(SALOME_ModuleCatalog::Service service);
};

#endif // ACOMPONENT_IMPL_H
