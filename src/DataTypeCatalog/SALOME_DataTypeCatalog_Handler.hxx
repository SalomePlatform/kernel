//  SALOME DataTypeCatalog : catalog of data types
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
//  File   : SALOME_DataTypeCatalog_Handler.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef SALOME_DATA_CATALOG_HANDLER_H
#define SALOME_DATA_CATALOG_HANDLER_H

#include "SALOME_DataTypeCatalog_Parser.hxx"
#include <qxml.h>
#include <string>
#include <vector>

#if defined DATATYPECATALOG_EXPORTS
#if defined WIN32
#define DATATYPECATALOG_EXPORT __declspec( dllexport )
#else
#define DATATYPECATALOG_EXPORT
#endif
#else
#if defined WNT
#define DATATYPECATALOG_EXPORT __declspec( dllimport )
#else
#define DATATYPECATALOG_EXPORT
#endif
#endif

class DATATYPECATALOG_EXPORT SALOME_DataTypeCatalog_Handler : public QXmlDefaultHandler 
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

  std::string content;

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
