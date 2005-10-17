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
//  File   : SALOME_DataTypeCatalog_impl.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef DATATYPECATALOG_IMPL_H
#define DATATYPECATALOG_IMPL_H

#include "utilities.h"
#include "SALOME_DataTypeCatalog_Handler.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_DataTypeCatalog)

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

class DATATYPECATALOG_EXPORT SALOME_DataTypeCatalogImpl: public POA_SALOME_DataTypeCatalog::DataCatalog,
				  public PortableServer::RefCountServantBase 
{
public:
  //! standard constructor
  SALOME_DataTypeCatalogImpl(int argc, char** argv);

  //! standard destructor
  virtual ~SALOME_DataTypeCatalogImpl();

  //! method to get a data type list
  /*!
    \return a data type list 
  */
  virtual SALOME_DataTypeCatalog::ListOfDataTypeName* GetDataTypeList();

  //! method to know if type_out is derived from type_in
  /*!
    \param type_in const char* arguments
    \param type_out const char* arguments
    \return a boolean (true if type_out is derived from type_in)
  */
  virtual CORBA::Boolean isDerivedFrom(const char* type_in, const char* type_out) throw(SALOME_DataTypeCatalog::NotFound);

  //! method to get the name of the interface to acces the data in read mode
  /*!
    \param type const char* arguments
    \return the read interface name
  */
  virtual char* GetDataInterfaceRead(const char* type) throw(SALOME_DataTypeCatalog::NotFound);


  //! method to get the name of the interface to acces the data in write mode
  /*!
    \param type const char* arguments
    \return the write interface name
  */
  virtual char* GetDataInterfaceWrite(const char* type) throw(SALOME_DataTypeCatalog::NotFound);

 //! method to get parents data type list
  /*!
    \param type const char* arguments
    \return a data type list 
  */
  virtual SALOME_DataTypeCatalog::ListOfDataTypeName* 
  GetDataTypeParents(const char* type) throw(SALOME_DataTypeCatalog::NotFound);


private:
 //! method to know if parents type of data_out derived from type_in
  /*!
    \param type_in const char* arguments
    \param data_out const ParserDataType arguments (by value)
    \return a boolean (true if type_out is derived from type_in)
  */
  virtual CORBA::Boolean _explore_parents(const char* type_in,
					  ParserDataType& data_out);


  //! method to parse the data type catalog
  /*! 
    \param file const char* arguments
    \param datatypelist ListOfParserDataType arguments
  */
  virtual void _parse_xml_file(const char* file, 
			       ListOfParserDataType& datatypelist);


 //! method to verify data type content
  /*!
    \param datatypelist ListOfParserDataType arguments
    \return true if verfication is OK
  */
  virtual bool _verify_data_type(ListOfParserDataType datatypelist);


 //! method to parse arguments to get general and personal catalog files
  /*!
    \param argc int arguments
    \param argv char** arguments
    \param _path_data char** arguments
    \return true if parsing is OK
  */
  virtual bool _parseArguments(int argc, char **argv, char **_path_data);


  // will contain the path to the data type catalog
  char* _path_datatype;


  //will contain the informations on the data type catalog(after parsing)
  ListOfParserDataType _datatype_list ;
};

#endif // DATATYPECATALOG_IMPL_H
