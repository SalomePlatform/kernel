// File: SALOME_DataTypeCatalog_Parser.hxx
// Created: Mon Aug 08 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$

#ifndef SALOME_DATA_CATALOG_PARSER_H
#define SALOME_DATA_CATALOG_PARSER_H

#include <string>
#include <vector>

// Type Definitions
typedef  vector<string> ListOfParserDataTypeName;

struct ParserDataType{
  string       		        Parserdata_name ;	
  string                        Parserdata_interface_read;
  string                        Parserdata_interface_write;
  ListOfParserDataTypeName	Parserdata_parents ;
};

typedef vector<ParserDataType> ListOfParserDataType;

#ifdef WRITE_CATA_DATA_TYPE
// contains all the data types defined in the catalog
ListOfParserDataType _datatypelist; 
#else
extern ListOfParserDataType _datatypelist;
#endif

#endif // SALOME_DATA_CATALOG_PARSER_H
