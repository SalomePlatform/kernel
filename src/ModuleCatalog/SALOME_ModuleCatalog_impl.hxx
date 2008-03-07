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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_ModuleCatalog_impl.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$


#ifndef MODULECATALOG_IMPL_H
#define MODULECATALOG_IMPL_H

#include <SALOME_ModuleCatalog.hxx>

#include <string>
#include <map>

#include "SALOME_ModuleCatalog_Handler.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_ModuleCatalog)

class MODULECATALOG_EXPORT SALOME_ModuleCatalogImpl: public POA_SALOME_ModuleCatalog::ModuleCatalog
{
public:
  //! standard constructor
  SALOME_ModuleCatalogImpl(int artgc, char** argv, CORBA::ORB_ptr orb = NULL);

  //! standard destructor
  virtual ~SALOME_ModuleCatalogImpl();

  //! method to get a component list
  /*!
    \return a component list 
  */
  virtual SALOME_ModuleCatalog::ListOfComputers* GetComputerList();

  //! method to get the list of all types of the catalog
  /*!
   * \return the types list
   */
  virtual SALOME_ModuleCatalog::ListOfTypeDefinition* GetTypes();

  //! method to get the PathPrefix of a computer
 /*! If the wanted computer doesn't exist, the Notfound exception is thrown
   \param machinename const char* arguments 
   \return the prefix path
 */
  virtual char* GetPathPrefix(const char* machinename);

  //! method to read a XML file and import new components into the component list
 /*! If the XML file doesn't exist or is not readable, the Notfound exception is thrown
   \param  const char* xmlFileName 
 */
  virtual void ImportXmlCatalogFile(const char* xmlFileName);

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
  
  //! method to get a component description
  /*!
    \param componentname const char* arguments 
    \return the wanted component description
  */
  virtual SALOME_ModuleCatalog::ComponentDef *
    GetComponentInfo(const char *name);

  void ping(){};
  CORBA::Long getPID();
  void ShutdownWithExit();

  void shutdown() { if(!CORBA::is_nil(_orb)) _orb->shutdown(0); };

private:
  //! method to parse one module catalog
  /*! 
    \param file const char* arguments
    \param modulelist ParserComponents arguments
    \param pathlist ParserPathPrefixes arguments
    \param typeMap ParserTypes arguments
  */
  virtual void _parse_xml_file(const char* file, 
			  ParserComponents & modulelist, 
			  ParserPathPrefixes & pathlist,
        ParserTypes& typeMap,
        TypeList& typeList);

  //! method to find component in the parser list
  /*!
    \param name  string argument
    \return pointer on a component, NULL if not found
  */
  ParserComponent *findComponent(const std::string & name);

  //! method to create a CORBA component description from parser
  /*!
    \param C_corba  Component argument
    \param C_parser const ParserComponent argument
  */
  void duplicate(SALOME_ModuleCatalog::ComponentDef & C_corba,
		 const ParserComponent & C_parser);
    
  //! method to create a CORBA interface description from parser
  /*!
    \param I_corba  DefinitionInterface argument
    \param I_parser const ParserInterface argument
  */
  void duplicate(SALOME_ModuleCatalog::DefinitionInterface & I_corba,
		 const ParserInterface & I_parser);
  
  //! method to create a CORBA service description from parser
  /*!
    \param S_corba  Service argument
    \param S_parser const ParserService argument
  */
  void duplicate(SALOME_ModuleCatalog::Service & S_corba,
		 const ParserService & service);
  
  //! method to create a CORBA parameter description from parser
  /*!
    \param P_corba  ServicesParameter argument
    \param P_parser const ParserParameter argument
  */
  void duplicate(SALOME_ModuleCatalog::ServicesParameter & P_corba,
		 const ParserParameter & P_parser);
  
  //! method to create a CORBA datastream parameter description from parser
  /*!
    \param P_corba  ServicesDataStreamParameter argument
    \param P_parser const ParserDataStreamParameter argument
  */
  void duplicate(SALOME_ModuleCatalog::ServicesDataStreamParameter & P_corba,
		 const ParserDataStreamParameter & P_parser);
  
  //! method to create the path prefix structures from the catalog parsing
  /*!
    \param pathes ParserPathPrefixes arguments
    \return the pathes
  */
  void duplicate(ParserPathPrefixes & p_out, const ParserPathPrefixes & P_in);

 //! method to verify path prefix content
  /*!
    \param pathlist ListOfParserPathPrefix arguments
    \return true if verfication is OK
  */
  virtual bool _verify_path_prefix(ParserPathPrefixes & pathlist);


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
  ParserComponents    _general_module_list ;
  ParserPathPrefixes  _general_path_list ;
  ParserTypes _typeMap;
  TypeList _typeList;

  // These variables will contain the informations on the personal catalog
  ParserComponents    _personal_module_list ;
  ParserPathPrefixes  _personal_path_list ; 

  std::map <std::string, SALOME_ModuleCatalog::DataStreamDependency> 
  DataStreamDepConvert;

  std::map <ParserComponentType, SALOME_ModuleCatalog::ComponentType> 
  ComponentTypeConvert;

  CORBA::ORB_ptr _orb;
};

#endif // MODULECATALOG_IMPL_H
