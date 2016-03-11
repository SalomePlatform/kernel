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

//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
//  File   : SALOME_ModuleCatalog_Acomponent_impl.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#ifndef ACOMPONENT_IMPL_H
#define ACOMPONENT_IMPL_H

#include "SALOME_ModuleCatalog.hxx"

#include <SALOMEconfig.h>

#include CORBA_SERVER_HEADER(SALOME_ModuleCatalog)

#ifdef WIN32
#pragma warning(disable:4275) // Disable warning interface non dll
#pragma warning(disable:4290) // Warning Exception ...
#endif

class MODULECATALOG_EXPORT SALOME_ModuleCatalog_AcomponentImpl: public POA_SALOME_ModuleCatalog::Acomponent
{
public:
  //! standard constructor
  SALOME_ModuleCatalog_AcomponentImpl(SALOME_ModuleCatalog::ComponentDef &C);

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
    \return the component icon
  */
  virtual char* component_icone();

  //! method to define if a component is implemented in a dyn lib a python module or an executable
  /*!
    \return an enum SO or PY or EXE or CEXE
  */
  virtual SALOME_ModuleCatalog::ImplType implementation_type();

  //! method to obtain the implementation name of the component if the default one is not convenient
  /*!
    \return the implementation name to exec
  */
  virtual char* implementation_name();

private :

  SALOME_ModuleCatalog::ComponentDef _Component;

 //! method to duplicate an interface
  /*!
    \param service SALOME_ModuleCatalog::DefinitionInterface arguments
    \return the interface
  */
  void duplicate(SALOME_ModuleCatalog::DefinitionInterface &I_out,
                 const SALOME_ModuleCatalog::DefinitionInterface &I_in);

 //! method to duplicate a service
  /*!
    \param service SALOME_ModuleCatalog::Service arguments
    \return the service
  */
  void duplicate(SALOME_ModuleCatalog::Service & S_out,
                 const SALOME_ModuleCatalog::Service &S_in);

 //! method to duplicate a parameter
  /*!
    \param parameter SALOME_ModuleCatalog::ServicesParameter arguments
    \return the parameter
  */
  void duplicate(SALOME_ModuleCatalog::ServicesParameter & P_out,
                 const SALOME_ModuleCatalog::ServicesParameter & P_in);

 //! method to duplicate a parameter
  /*!
    \param parameter SALOME_ModuleCatalog::ServicesDataStreamParameter arguments
    \return the parameter
  */
  void duplicate(SALOME_ModuleCatalog::ServicesDataStreamParameter & P_out,
                 const SALOME_ModuleCatalog::ServicesDataStreamParameter &P_in);

};


#endif // ACOMPONENT_IMPL_H
