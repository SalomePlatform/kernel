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
//  File   : SALOME_ModuleCatalog_impl.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef MODULECATALOG_IMPL_H
#define MODULECATALOG_IMPL_H

#include "utilities.h"
#include "SALOME_ModuleCatalog_Handler.hxx"
#include "PathPrefix.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_ModuleCatalog)

class SALOME_ModuleCatalogImpl: public POA_SALOME_ModuleCatalog::ModuleCatalog,
				public PortableServer::RefCountServantBase 
{
public:
  //! standard constructor
  SALOME_ModuleCatalogImpl(int artgc, char** argv);

  //! standard destructor
  virtual ~SALOME_ModuleCatalogImpl();

  //! method to get a component list
  /*!
    \return a component list 
  */
  virtual SALOME_ModuleCatalog::ListOfComputers* GetComputerList();

  //! method to get the PathPrefix of a computer
 /*! If the wanted computer doesn't exist, the Notfound exception is thrown
   \param machinename const char* arguments 
   \return the prefix path
 */
  virtual char* GetPathPrefix(const char* machinename);

  //! method to get a component list
  /*!
    \return a component list 
  */
  virtual SALOME_ModuleCatalog::ListOfComponents* GetComponentList();

  //! method to get a component list of component name and component icone
  /*!
    \return a list of couple (component name, component icone) 
  */
  virtual SALOME_ModuleCatalog::ListOfIAPP_Affich* GetComponentIconeList();

  //! method to get a component list of a wanted type
  /*!
    \return a component list 
  */
  virtual SALOME_ModuleCatalog::ListOfComponents* 
    GetTypedComponentList(SALOME_ModuleCatalog::ComponentType component_type);

  //! method to get a component
  /*!
    \param componentname const char* arguments 
    \return the wanted component
  */
  virtual SALOME_ModuleCatalog::Acomponent_ptr 
    GetComponent(const char* componentname);

  void ping(){};

private:
  //! method to parse one module catalog
  /*! 
    \param file const char* arguments
    \param modulelist ListOfParserComponent arguments
    \param pathlist ListOfParserPathPrefix arguments
  */
  virtual void _parse_xml_file(const char* file, 
			  ListOfParserComponent& modulelist, 
			  ListOfParserPathPrefix& pathlist);

 //! method to create a list of interfaces from the parsing of the catalog
  /*!
    \param list_interface ListOfDefinitionInterface arguments
    \return the interfaces list
  */
  virtual SALOME_ModuleCatalog::ListOfDefInterface 
          duplicate_interfaces(ListOfDefinitionInterface list_interface);
  
 //! method to create the path prefix structures from the catalog parsing
  /*!
    \param pathes ListOfParserPathPrefix arguments
    \return the pathes
  */
  virtual ListOfPathPrefix duplicate_pathes(ListOfParserPathPrefix pathes);

 //! method to verify path prefix content
  /*!
    \param pathlist ListOfParserPathPrefix arguments
    \return true if verfication is OK
  */
  virtual bool _verify_path_prefix(ListOfParserPathPrefix pathlist);


 //! method to parse arguments to get general and personal catalog files
  /*!
    \param argc int arguments
    \param argv char** arguments
    \param _general char** arguments
    \return true if parsing is OK
  */
  virtual bool _parseArguments(int argc, char **argv, 
			       char **_general, char** _personal);


  // Theses variables will contain the path to the general and personal catalogs
  char* _general_path;
  char* _personal_path;


  // These variables will contain the informations on the general common catalog
  ListOfParserComponent _general_module_list ;
  ListOfParserPathPrefix _general_path_list ;

  // These variables will contain the informations on the personal catalog
  ListOfParserComponent _personal_module_list ;
  ListOfParserPathPrefix _personal_path_list ; 

};

#endif // MODULECATALOG_IMPL_H
