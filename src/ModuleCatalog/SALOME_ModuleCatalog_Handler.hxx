// File: SALOME_ModuleCatalog_Handler.hxx
// Created: Tue June 27 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$

#ifndef SALOME_CATALOG_HANDLER_H
#define SALOME_CATALOG_HANDLER_H

#include "utilities.h"
#include "SALOME_ModuleCatalog_Parser.hxx"
#include <qxml.h>
#include <string>
#include <vector>

class SALOME_ModuleCatalog_Handler : public QXmlDefaultHandler 
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
   \param Qstring argument by value
   \param Qstring argument by value
   \param Qstring argument by value
   \param QXmlAttributes argument by value
   \return true if no error was detected
  */
  virtual bool startElement(const QString&, const QString &,
			    const QString& qName, const QXmlAttributes& atts); 

  //! method to overload handler function endElement
  /*!
   \param Qstring argument by value
   \param Qstring argument by value
   \param Qstring argument by value
   \return true if no error was detected
   \sa _Find
  */   
  virtual bool endElement(const QString&, const QString &,
			  const QString& qName);
  

  //! method to overload handler function characters
  /*!
   \param Qstring argument by value
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

  string content;

  const char* test_path_prefix_name ;
  const char* test_computer_name ;
  const char* test_path_prefix ;

  const char* test_component_name;
  const char* test_component_username;
  const char* test_component_type ;
  const char* test_component_multistudy ;
  const char* test_component_icone ;

  const char* test_interface_name;

  const char* test_service_name;
  const char* test_defaultservice;

  const char* test_inParameter_type;
  const char* test_inParameter_name;
  const char* test_inParameter;
  const char* test_inParameter_list;
  const char* test_outParameter_type;
  const char* test_outParameter_name;
  const char* test_outParameter;
  const char* test_outParameter_list;

  const char* test_service;
  const char* test_service_list;
  const char* test_interface_list;
  const char* test_constraint;
  
  const char* test_component;

  ParserPathPrefix _path_prefix;

  ParserComponent _amodule;


  ListOfDefinitionInterface _interfacelist;
  ParserDefInterface _aInterface;
  ListOfParserServices _servicelist;
  ParserService _aService;
  ParserServParam _inparam;
  ParserServParam _outparam;
  ListOfParserServicesParameter _inparamlist;
  ListOfParserServicesParameter _outparamlist;
};

#endif // SALOME_CATALOG_HANDLER_H
