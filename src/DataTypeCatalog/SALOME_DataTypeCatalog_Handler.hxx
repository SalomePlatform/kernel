// File: SALOME_DataTypeCatalog_Handler.hxx
// Created: Mon Aug 08 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$

#ifndef SALOME_DATA_CATALOG_HANDLER_H
#define SALOME_DATA_CATALOG_HANDLER_H

#include "utilities.h"
#include "SALOME_DataTypeCatalog_Parser.hxx"
#include <qxml.h>
#include <string>
#include <vector>

class SALOME_DataTypeCatalog_Handler : public QXmlDefaultHandler 
{
public:
  //! standard constructor
  SALOME_DataTypeCatalog_Handler();

  //! standard destructor
  virtual ~SALOME_DataTypeCatalog_Handler();


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

  const char* test_data_type_name ;
  const char* test_data_type_interf_read ;
  const char* test_data_type_interf_write ;
  const char* test_data_type_parent_name ;
  const char* test_data_type_parents ;
  const char* test_data_type;

  ParserDataType _a_data_type;

  ListOfParserDataTypeName _a_data_parents_list ;
};

#endif // SALOME_DATA_CATALOG_HANDLER_H
