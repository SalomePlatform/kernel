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
//  File   : SALOME_DataTypeCatalog_Parser.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#ifndef SALOME_DATA_CATALOG_PARSER_H
#define SALOME_DATA_CATALOG_PARSER_H

#include <string>
#include <vector>

// Type Definitions
typedef  std::vector<std::string> ListOfParserDataTypeName;

struct ParserDataType{
  std::string  		        Parserdata_name ;	
  std::string                   Parserdata_interface_read;
  std::string                   Parserdata_interface_write;
  ListOfParserDataTypeName	Parserdata_parents ;
};

typedef std::vector<ParserDataType> ListOfParserDataType;

// contains all the data types defined in the catalog
extern ListOfParserDataType _datatypelist;


#endif // SALOME_DATA_CATALOG_PARSER_H
