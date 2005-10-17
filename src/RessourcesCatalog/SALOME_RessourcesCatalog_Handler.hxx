//  SALOME RessourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
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
//  File   : SALOME_RessourcesCatalog_Handler.hxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#ifndef SALOME_RESSOURCES_CATALOG_HANDLER
#define SALOME_RESSOURCES_CATALOG_HANDLER

#include "SALOME_RessourcesCatalog_Parser.hxx"

#include <qxml.h>
#include <string>
#include <vector>

#if defined RESSOURCESCatalog_EXPORTS
#if defined WIN32
#define RESSOURCESCatalog_EXPORT __declspec( dllexport )
#else
#define RESSOURCESCatalog_EXPORT
#endif
#else
#if defined WNT
#define RESSOURCESCatalog_EXPORT __declspec( dllimport )
#else
#define RESSOURCESCatalog_EXPORT
#endif
#endif

class RESSOURCESCatalog_EXPORT SALOME_RessourcesCatalog_Handler : public QXmlDefaultHandler
{
public :
  //! standard constructor
  SALOME_RessourcesCatalog_Handler();

  //! standard destructor
  virtual ~SALOME_RessourcesCatalog_Handler();

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
			    const QString& qName, const QXmlAttributes& atts);

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


private :
  QString errorProt;

  std::string content;

  const char* test_computer;
  const char* test_computer_name;
  const char* test_computer_OS;
  const char* test_computer_OS_version;
  const char* test_proc;
  const char* test_proc_number;
  const char* test_proc_model;
  const char* test_proc_cpu;
  const char* test_proc_cache;
  const char* test_proc_list;
  const char* test_container_type;
  const char* test_container_type_list;

  Parserressources _a_ressource;
  
  ListOfParserProc _procs_list;
  ParserProc _a_proc;
  ListOfParserContainerType _container_list;

};

#endif // SALOME_RESSOURCES_CATALOG_HANDLER
