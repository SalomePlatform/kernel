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
//  File   : SALOME_DataTypeCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#define WRITE_CATA_DATA_TYPE
#include "SALOME_DataTypeCatalog_Handler.hxx"
using namespace std;

//----------------------------------------------------------------------
// Function : SALOME_DataTypeCatalog_Handler
// Purpose  : Constructor
//----------------------------------------------------------------------
SALOME_DataTypeCatalog_Handler::SALOME_DataTypeCatalog_Handler()
{
  MESSAGE("SALOME_DataTypeCatalog_Handler creation");
  // XML Tags initialisation
  // Used in the function endElement
  test_data_type_name = "DataType-name";
  test_data_type_interf_read = "DataType-interface-read";
  test_data_type_interf_write = "DataType-interface-write";
  test_data_type_parent_name = "Parent-name" ;
  test_data_type_parents = "DataType-Parents-list";
  test_data_type = "DataType";
}

//----------------------------------------------------------------------
// Function : ~SALOME_DataTypeCatalog_Handler
// Purpose  : Destructor
//----------------------------------------------------------------------
SALOME_DataTypeCatalog_Handler::~SALOME_DataTypeCatalog_Handler()
{
  MESSAGE("SALOME_DataTypeCatalog_Handler destruction");
}

//----------------------------------------------------------------------
// Function : startDocument
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_DataTypeCatalog_Handler::startDocument()
{
  MESSAGE("Begin parse document")
  // Empty the private elements
  _datatypelist.resize(0);
  _a_data_parents_list.resize(0);
  return true;
}

//----------------------------------------------------------------------
// Function : startElement
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_DataTypeCatalog_Handler::startElement(const QString&, 
						  const QString &,
						  const QString& qName, 
						  const QXmlAttributes& atts)
{
  return true;
} 

//----------------------------------------------------------------------
// Function : endElement
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_DataTypeCatalog_Handler::endElement(const QString&, 
						const QString &,
						const QString& qName)
{
  // DataType

  // tag test_data_type_name
  if((qName.compare(QString(test_data_type_name))==0))
    _a_data_type.Parserdata_name = content;
  // tag test_data_type_interf_read
  if((qName.compare(QString(test_data_type_interf_read))==0)) 
    _a_data_type.Parserdata_interface_read = content;
  // tag test_data_type_interf_write
  if((qName.compare(QString(test_data_type_interf_write))==0)) 
    _a_data_type.Parserdata_interface_write = content;

  // tag test_data_type_parent_name
  if((qName.compare(QString(test_data_type_parent_name))==0)) 
    _a_data_parents_list.push_back(content);

  // tag test_data_type_parents
  if((qName.compare(QString(test_data_type_parents))==0)) 
    {
      _a_data_type.Parserdata_parents = _a_data_parents_list;
      _a_data_parents_list.resize(0);;
    }

  // tag test_data_type
  if((qName.compare(QString(test_data_type))==0))
    {
      _datatypelist.push_back(_a_data_type);

      // Empty temporary structures
      _a_data_type.Parserdata_name = "";
      _a_data_type.Parserdata_interface_read = "";
      _a_data_type.Parserdata_interface_write = "";
      _a_data_type.Parserdata_parents.resize(0);
    }



  return true;
}
  
//----------------------------------------------------------------------
// Function : characters
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_DataTypeCatalog_Handler::characters(const QString& chars)
{
  content = (const char*)chars ;
  return true;
}

//----------------------------------------------------------------------
// Function : endDocument
// Purpose  : overload handler function
//            Print all informations find in the catalog 
//            (only in DEBUG mode!!)
//----------------------------------------------------------------------  
bool SALOME_DataTypeCatalog_Handler::endDocument()
{
  //_datatypelist
  for (unsigned int ind = 0; ind < _datatypelist.size(); ind++)
    {
      MESSAGE("DataType Name :"<<_datatypelist[ind].Parserdata_name);
      MESSAGE("DataType interface read :"<<_datatypelist[ind].Parserdata_interface_read);
      MESSAGE("DataType interface write :"<<_datatypelist[ind].Parserdata_interface_write);

      for (unsigned int i = 0; i < _datatypelist[ind].Parserdata_parents.size(); i++)
	MESSAGE("Parent name :" << _datatypelist[ind].Parserdata_parents[i]);
    }

  MESSAGE("Document parsed");
  return true;
}
 
//----------------------------------------------------------------------
// Function : errorProtocol
// Purpose  : overload handler function
//----------------------------------------------------------------------  
QString SALOME_DataTypeCatalog_Handler::errorProtocol()
{
  return errorProt ;
}


//----------------------------------------------------------------------
// Function : fatalError
// Purpose  : overload handler function
//----------------------------------------------------------------------  
bool SALOME_DataTypeCatalog_Handler::fatalError(const QXmlParseException& exception)
{
    errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
    .arg( exception.message() )
    .arg( exception.lineNumber() )
    .arg( exception.columnNumber() );

  return QXmlDefaultHandler::fatalError( exception );
}
