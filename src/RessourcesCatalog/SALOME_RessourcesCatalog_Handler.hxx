//File SALOME_RessourcesCatalog_Handler.hxx
//Created: Fri Sept 07 2001
//Author: Estelle Deville
//Project: SALOME
//Copyright: CEA/DEN/DM2S/LGLS
//$Header$

#ifndef SALOME_RESSOURCES_CATALOG_HANDLER
#define SALOME_RESSOURCES_CATALOG_HANDLER

#include "utilities.h"
#include "SALOME_RessourcesCatalog_Parser.hxx"
#include <qxml.h>
#include <string>
#include <vector>

class SALOME_RessourcesCatalog_Handler : public QXmlDefaultHandler
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

  string content;

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
