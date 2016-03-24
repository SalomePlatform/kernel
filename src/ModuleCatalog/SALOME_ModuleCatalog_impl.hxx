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
//  File   : SALOME_ModuleCatalog_impl.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#ifndef MODULECATALOG_IMPL_H
#define MODULECATALOG_IMPL_H

#include "SALOME_ModuleCatalog.hxx"

#include <string>
#include <map>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_ModuleCatalog)

#ifdef WIN32
#pragma warning(disable:4275) // Disable warning interface non dll
#pragma warning(disable:4290) // Warning Exception ...
#endif

class MODULECATALOG_EXPORT SALOME_ModuleCatalogImpl: public POA_SALOME_ModuleCatalog::ModuleCatalog
{
  class Private;

public:
  //! standard constructor
  SALOME_ModuleCatalogImpl(int artgc, char** argv, CORBA::ORB_ptr orb = NULL);

  //! standard destructor
  virtual ~SALOME_ModuleCatalogImpl();

  //! method to get a component list
  /*!
   * \return a component list 
   */
  virtual SALOME_ModuleCatalog::ListOfComputers* GetComputerList();

  //! method to get the list of all types of the catalog
  /*!
   * \return the types list
   */
  virtual SALOME_ModuleCatalog::ListOfTypeDefinition* GetTypes();

  //! method to get the PathPrefix of a computer
  /*! If the wanted computer doesn't exist, the Notfound exception is thrown
   * \param machinename const char* arguments 
   * \return the prefix path
  */
  virtual char* GetPathPrefix(const char* machinename);

  //! method to read a XML file and import new components into the component list
  /*! If the XML file doesn't exist or is not readable, the Notfound exception is thrown
   * \param  const char* xmlFileName 
   */
  virtual void ImportXmlCatalogFile(const char* xmlFileName);

  //! method to get a component list
  /*!
   * \return a component list 
   */
  virtual SALOME_ModuleCatalog::ListOfComponents* GetComponentList();

  //! method to get a component list of component name and component icone
  /*!
   * \return a list of couple (component name, component icone) 
   */
  virtual SALOME_ModuleCatalog::ListOfIAPP_Affich* GetComponentIconeList();

  //! method to get a component list of a wanted type
  /*!
   * \return a component list 
   */
  virtual SALOME_ModuleCatalog::ListOfComponents* 
    GetTypedComponentList(SALOME_ModuleCatalog::ComponentType component_type);

  //! method to get a component
  /*!
   * \param componentname const char* arguments 
   * \return the wanted component
   */
  virtual SALOME_ModuleCatalog::Acomponent_ptr 
    GetComponent(const char* componentname);
  
  //! method to get a component description
  /*!
   * \param componentname const char* arguments 
   * \return the wanted component description
   */
  virtual SALOME_ModuleCatalog::ComponentDef*
    GetComponentInfo(const char *name);

  //! method to check the server is alive
  void ping();

  //! get PID of the server
  CORBA::Long getPID();

  //! shutdown server and exit
  void ShutdownWithExit();

  //! shutdown server
  void shutdown();

private:
  //! method to parse arguments to get general and personal catalog files
  /*!
   * \param argc int arguments
   * \param argv char** arguments
   * \param _general char** arguments
   * \return true if parsing is OK
   */
  virtual bool _parseArguments(int argc, char **argv, 
                               char **_general, char** _personal);


  CORBA::ORB_ptr _orb;
  Private* myPrivate;
};

#endif // MODULECATALOG_IMPL_H
