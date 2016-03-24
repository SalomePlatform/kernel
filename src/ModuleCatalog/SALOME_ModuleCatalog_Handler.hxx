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
//  File   : SALOME_ModuleCatalog_Handler.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#ifndef SALOME_CATALOG_HANDLER_H
#define SALOME_CATALOG_HANDLER_H

#include "SALOME_ModuleCatalog.hxx"

#include "SALOME_ModuleCatalog_Parser.hxx"

#include <string>
#include <stack>
#include <map>

#include <libxml/parser.h>

#ifdef WIN32
#pragma warning(disable:4251) // Warning DLL Interface ...
#endif

class MODULECATALOG_EXPORT SALOME_ModuleCatalog_Handler
{
public:
  //! standard constructor
  SALOME_ModuleCatalog_Handler(ParserPathPrefixes& pathList,ParserComponents& moduleList,ParserTypes& typeMap,TypeList& typeList);

  //! standard destructor
  virtual ~SALOME_ModuleCatalog_Handler();

  void ProcessXmlDocument(xmlDocPtr theDoc);
 
private:

  const char *test_path_prefix_name ;
  const char *test_computer_name ;
  const char *test_computer_list ;
  const char *test_path_prefix ;
  const char *test_path_prefix_list;

  const char *test_component_name;
  const char *test_component_username;
  const char *test_component_type ;
  const char *test_component_multistudy ;
  const char *test_component_icon ;
  const char *test_component_impltype;
  const char *test_component_implname;
  const char *test_component_version;
  const char *test_component_comment;
  
  const char *test_interface_name;
  
  const char *test_service_name;
  const char *test_defaultservice;
  const char *test_typeofnode;
  
  const char *test_inParameter_type;
  const char *test_inParameter_name;
  const char *test_inParameter;
  const char *test_inParameter_list;
  
  const char *test_outParameter_type;
  const char *test_outParameter_name;
  const char *test_outParameter;
  const char *test_outParameter_list;
  
  const char *test_inDataStreamParameter_type;
  const char *test_inDataStreamParameter_name;
  const char *test_inDataStreamParameter_dependency;
  const char *test_inDataStreamParameter;
  const char *test_inDataStreamParameter_list;
  
  const char *test_outDataStreamParameter_type;
  const char *test_outDataStreamParameter_name;
  const char *test_outDataStreamParameter_dependency;
  const char *test_outDataStreamParameter;
  const char *test_outDataStreamParameter_list;
  
  const char *test_service;
  const char *test_service_list;
  const char *test_interface_list;
  const char *test_constraint;
  
  const char *test_component_list;
  const char *test_component;
  
  ParserPathPrefix _pathPrefix;
  
  ParserComponent _aModule;
  
  ParserPathPrefixes&  _pathList;
  ParserComponents&    _moduleList;
  
  ParserInterfaces _interfaceList;
  ParserInterface  _aInterface;
  
  ParserServices   _serviceList;
  ParserService    _aService;

  ParserParameters _inParamList;
  ParserParameter  _inParam;
  
  ParserParameters _outParamList;
  ParserParameter  _outParam;
  
  ParserDataStreamParameters _inDataStreamParamList;
  ParserDataStreamParameter  _inDataStreamParam;
  
  ParserDataStreamParameters _outDataStreamParamList;
  ParserDataStreamParameter  _outDataStreamParam;

  ParserTypes& _typeMap;
  TypeList& _typeList;

  ParserSequences _sequenceMap;
  ParserObjrefs _objrefMap;
  ParserStructs _structMap;
};

#endif // SALOME_CATALOG_HANDLER_H
