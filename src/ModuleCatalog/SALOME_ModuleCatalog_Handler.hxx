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
//  File   : SALOME_ModuleCatalog_Handler.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef SALOME_CATALOG_HANDLER_H
#define SALOME_CATALOG_HANDLER_H

#include <SALOME_ModuleCatalog.hxx>

#include "SALOME_ModuleCatalog_Parser.hxx"
#include <qxml.h>
#include <string>
#include <stack>
#include <map>

class MODULECATALOG_EXPORT SALOME_ModuleCatalog_Handler : public QXmlDefaultHandler 
{
public:
  //! standard constructor
  SALOME_ModuleCatalog_Handler();

  //! standard destructor
  virtual ~SALOME_ModuleCatalog_Handler();


  //! method to overload handler function startDocument
  /*! Is called before a xml file is parsed
   \return true if no error was detected
  */ 
  virtual bool startDocument();


  //! method to overload handler function startElement
  /*! 
   \param QString argument by value
   \param QString argument by value
   \param QString argument by value
   \param QXmlAttributes argument by value
   \return true if no error was detected
  */
  virtual bool startElement(const QString&, const QString &,
			    const QString& qName, const QXmlAttributes& atts); 

  //! method to overload handler function endElement
  /*!
   \param QString argument by value
   \param QString argument by value
   \param QString argument by value
   \return true if no error was detected
   \sa _Find
  */   
  virtual bool endElement(const QString&, const QString &,
			  const QString& qName);
  

  //! method to overload handler function characters
  /*!
   \param QString argument by value
   \return true if no error was detected
  */
  virtual bool characters(const QString& chars);
  
  //! method to overload handler function endDocument
  /*! Is called at the end of the parsing
   \return true if no error was detected
  */
  virtual bool endDocument();

  //! method to overload handler function errorProtocol
 /*!
   \return the error message
  */
  virtual QString errorProtocol();

  //! method to overload handler function fatalError
  /*!
    \param QXmlParseExecption argument by value
    \return true if no error was detected
  */
  virtual bool fatalError(const QXmlParseException& exception);
 
private:
  QString errorProt ;

  std::string content;

  QString test_path_prefix_name ;
  QString test_computer_name ;
  QString test_path_prefix ;

  QString test_component_name;
  QString test_component_username;
  QString test_component_type ;
  QString test_component_multistudy ;
  QString test_component_icon ;
  QString test_component_impltype;
  QString test_component_version;
  QString test_component_comment;

  QString test_interface_name;

  QString test_service_name;
  QString test_defaultservice;
  QString test_typeofnode;

  QString test_inParameter_type;
  QString test_inParameter_name;
  QString test_inParameter;
  QString test_inParameter_list;

  QString test_outParameter_type;
  QString test_outParameter_name;
  QString test_outParameter;
  QString test_outParameter_list;

  QString test_inDataStreamParameter_type;
  QString test_inDataStreamParameter_name;
  QString test_inDataStreamParameter_dependency;
  QString test_inDataStreamParameter;
  QString test_inDataStreamParameter_list;

  QString test_outDataStreamParameter_type;
  QString test_outDataStreamParameter_name;
  QString test_outDataStreamParameter_dependency;
  QString test_outDataStreamParameter;
  QString test_outDataStreamParameter_list;

  QString test_service;
  QString test_service_list;
  QString test_interface_list;
  QString test_constraint;
  
  QString test_component;

  ParserPathPrefix _pathPrefix;

  ParserComponent _aModule;


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

  std::stack<QString> _xml_pile;
};

#endif // SALOME_CATALOG_HANDLER_H
