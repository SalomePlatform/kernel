//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
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
//  File   : SALOME_ResourcesCatalog_Handler.hxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#ifndef SALOME_RESOURCES_CATALOG_HANDLER
#define SALOME_RESOURCES_CATALOG_HANDLER

#include "SALOME_ResourcesCatalog_Parser.hxx"

#include <qxml.h>
#include <string>
#include <vector>

class QDomElement;
class QDomDocument;

class SALOME_ResourcesCatalog_Handler : public QXmlDefaultHandler
{
public :
  //! standard constructor
  SALOME_ResourcesCatalog_Handler(MapOfParserResourcesType& listOfResources);

  const MapOfParserResourcesType& GetResourcesAfterParsing() const;

  //! standard destructor
  virtual ~SALOME_ResourcesCatalog_Handler();

  //! method to overload handler function startDocument
  /*! is called before a xml file is parsed
    \return true if no error was detected
  */
  virtual bool startDocument();

  //! method to overload handler function startElement
  /*!
    \param QString argument by value
    \param QString argument by value
    \param QString qName argument by value
    \param QXmlAttributes atts argument by value
    \return true if no error was detected
  */
  virtual bool startElement(const QString& , const QString& ,
			    const QString& name, const QXmlAttributes& attrs);

 //! method to overload handler function endElement
  /*!
    \param QString argument by value
    \param QString argument by value
    \param QString qName argument by value
    \return true if no error was detected
  */
  virtual bool endElement(const QString&, const QString&,
			  const QString& qName);

 //! method to overload handler function characters
  /*!
    \param QString chars argument by value
    \return true if no error was detected
  */
  virtual bool characters(const QString& chars);

 //! method to overload handler function endDocument
  /*! is called at the end of the parsing
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
    \param QXmlParseException exception argument by value
    \return true if no error was detected
  */
  virtual bool fatalError(const QXmlParseException& exception);

//! method to fill the document to be writen in a file
  void PrepareDocToXmlFile(QDomDocument& doc);

private :
  QString errorProt;
  std::string content;
  std::string previous_module_name;
  std::string previous_module_path;

  ParserResourcesType _resource;
  MapOfParserResourcesType& _resources_list;

  const char *test_machine;
  const char *test_resources;

  const char *test_hostname;
  const char *test_alias;
  const char *test_protocol;
  const char *test_mode;
  const char *test_user_name;
  const char *test_modules;
  const char *test_module_name;
  const char *test_module_path;
  const char *test_pre_req_file_path;
  const char *test_os;
  const char *test_mem_in_mb;
  const char *test_cpu_freq_mhz;
  const char *test_nb_of_nodes;
  const char *test_nb_of_proc_per_node;

};

#endif // SALOME_RESOURCES_CATALOG_HANDLER
