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
//  File   : SALOME_RessourcesCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#include "SALOME_RessourcesCatalog_Handler.hxx"

#include "utilities.h"

using namespace std;

ListOfParserressources _ressources_list;

//----------------------------------------------------------------------
//Function : SALOME_RessourcesCatalog_Handler
//Purpose: Constructor
//----------------------------------------------------------------------
SALOME_RessourcesCatalog_Handler::SALOME_RessourcesCatalog_Handler()
{
  MESSAGE("SALOME_RessourcesCatalog_Handler creation");
  //XML tags initialisation
  test_computer = "computer";
  test_computer_name="name";
  test_computer_OS="OS";
  test_computer_OS_version = "OS-version";
  test_proc = "proc";
  test_proc_number ="number" ;
  test_proc_model= "model" ;
  test_proc_cpu= "CPU-MHz" ;
  test_proc_cache = "cache" ;
  test_proc_list = "proc-list" ;
  test_container_type = "containertype" ;
  test_container_type_list = "containertype-list" ;
}

//----------------------------------------------------------------------
//Function : ~SALOME_RessourcesCatalog_Handler
//Purpose: Destructor
//----------------------------------------------------------------------
SALOME_RessourcesCatalog_Handler::~SALOME_RessourcesCatalog_Handler()
{
  MESSAGE("SALOME_RessourcesCatalog_Handler destruction");
}

//----------------------------------------------------------------------
//Function : startDocument
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool 
SALOME_RessourcesCatalog_Handler::startDocument()
{
  MESSAGE("Begin parse document");
  // Empty private elements
  _procs_list.resize(0);
  _ressources_list.resize(0);
  _container_list.resize(0);
  return true;
}

//----------------------------------------------------------------------
//Function : startElement
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool 
SALOME_RessourcesCatalog_Handler::startElement(const QString&, 
					       const QString&,
					       const QString& qName, 
					       const QXmlAttributes& atts)
{
  return true;
}

//----------------------------------------------------------------------
//Function : endElement
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool 
SALOME_RessourcesCatalog_Handler::endElement(const QString&, const QString&,
					     const QString& qName)
{
  // Ressources

  // tag test_computer_name
  if((qName.compare(QString(test_computer_name))==0))
    _a_ressource.Parsername = content;

  // tag test_computer_OS
  if((qName.compare(QString(test_computer_OS))==0))
    _a_ressource.ParserOS = content;

 // tag test_computer_OS_version
  if((qName.compare(QString(test_computer_OS_version))==0))
    _a_ressource.ParserOS_version = content;

  //tag test_proc_number
 if((qName.compare(QString(test_proc_number))==0))
    _a_proc.Parsernumber = atol(content.c_str());

  //tag test_proc_model
 if((qName.compare(QString(test_proc_model))==0))
    _a_proc.Parsermodel_name = content;

  //tag test_proc_cpu
 if((qName.compare(QString(test_proc_cpu))==0))
    _a_proc.Parsercpu_mhz = atof(content.c_str());

  //tag test_proc_cache
 if((qName.compare(QString(test_proc_cache))==0))
    _a_proc.Parsercache_size = atof(content.c_str());

  //tag test_proc
 if((qName.compare(QString(test_proc))==0))
   {
     _procs_list.push_back(_a_proc);

     // Empty temporary structures
     _a_proc.Parsernumber = 0;
     _a_proc.Parsermodel_name = "";
     _a_proc.Parsercpu_mhz = 0;
     _a_proc.Parsercache_size = 0;
   }

 //tag test_proc_list
 if((qName.compare(QString(test_proc_list))==0))
   {
     _a_ressource.Parserprocs = _procs_list;

     // Empty temporary structures
     _procs_list.resize(0);
   }

  //tag test_container_type
 if((qName.compare(QString(test_container_type))==0))
   {
     // We just have to compare the first character of content
     // If C => Cpp
     // If p => python
     // If N => NP
     switch(content[0]) {
     case 'C':
       _container_list.push_back(Cpp) ;
       break;
     case 'p':
        _container_list.push_back(python) ;
       break;
     case 'N':
        _container_list.push_back(NP) ;
       break;
       
     default:
       // If it'not in all theses cases, the type is affected to Cpp
       _container_list.push_back(Cpp) ;
       break;
       }
   }

 //tag test container_type_list
 if((qName.compare(QString(test_container_type_list))==0))
   {
     _a_ressource.Parsercontainertype = _container_list;

     // Empty temporary structures
     _container_list.resize(0);
   }

 // tag test_computer
  if((qName.compare(QString(test_computer))==0))
   {
     _ressources_list.push_back(_a_ressource);

     // Empty temporary structures
     _a_ressource.Parsername = "";
     _a_ressource.ParserOS="";
     _a_ressource.ParserOS_version="";
     _a_ressource.Parserprocs.resize(0);
     _a_ressource.Parsercontainertype.resize(0);
   }

  return true;
}

//----------------------------------------------------------------------
//Function : characters
//Purpose: overload handler function
//----------------------------------------------------------------------
bool SALOME_RessourcesCatalog_Handler::characters(const QString& chars)
{
  content = (const char *)chars ;
  return true;
}

//----------------------------------------------------------------------
//Function : endDocument
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool SALOME_RessourcesCatalog_Handler::endDocument()
{
  //_ressources_list
  for (unsigned int ind = 0; ind < _ressources_list.size(); ind++)
    {
      MESSAGE("Ressources name :"<<_ressources_list[ind].Parsername);
      MESSAGE("OS :"<<_ressources_list[ind].ParserOS); 
      MESSAGE("OS version :"<<_ressources_list[ind].ParserOS_version);
      for (unsigned int i = 0; i < _ressources_list[ind].Parserprocs.size(); i++)
	{
	  MESSAGE("Proc number :" << _ressources_list[ind].Parserprocs[i].Parsernumber);
	  MESSAGE("Model name :" << _ressources_list[ind].Parserprocs[i].Parsermodel_name);
	  MESSAGE("CPU(MHz) :" << _ressources_list[ind].Parserprocs[i].Parsercpu_mhz);
	  MESSAGE("Cache :" << _ressources_list[ind].Parserprocs[i].Parsercache_size);
	}
      for (unsigned int j = 0; j < _ressources_list[ind].Parsercontainertype.size(); j++)
	MESSAGE("Container Type :" << _ressources_list[ind].Parsercontainertype[j]);
    }

 return true;
}

//----------------------------------------------------------------------
//Function : errorProtocol
//Purpose: overload handler function
//----------------------------------------------------------------------
QString SALOME_RessourcesCatalog_Handler::errorProtocol()
{
  return errorProt;
}

//----------------------------------------------------------------------
//Function : fatalError
//Purpose: overload handler function
//----------------------------------------------------------------------
bool 
SALOME_RessourcesCatalog_Handler::fatalError(const QXmlParseException& exception)
{
    errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
    .arg( exception.message() )
    .arg( exception.lineNumber() )
    .arg( exception.columnNumber() );

  return QXmlDefaultHandler::fatalError( exception );
}

