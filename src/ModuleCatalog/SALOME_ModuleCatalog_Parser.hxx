// File: SALOME_ModuleCatalog_Parser.hxx
// Created: Tue June 25 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$

#ifndef SALOME_CATALOG_PARSER_H
#define SALOME_CATALOG_PARSER_H

#include <string>
#include <vector>

// Type Definitions
struct ParserPathPrefix
{
  string         path;
  vector<string> ListOfComputer ;
};

typedef vector<ParserPathPrefix> ListOfParserPathPrefix ;

enum ParserComponentType {GEOM, MESH, Med, SOLVER, DATA, VISU, SUPERV, OTHER} ;

struct ParserServParam
{
  string ParserParamtype;
  string ParserParamname;
} ;

typedef vector<ParserServParam> ListOfParserServicesParameter;

struct ParserService
{
  string                        ParserServiceName;
  ListOfParserServicesParameter ParserServiceinParameter;
  ListOfParserServicesParameter ParserServiceoutParameter;
  bool                          ParserServicebydefault;
} ;

typedef vector<ParserService> ListOfParserServices ;

struct ParserDefInterface
{
  string               Parserinterfacename ;
  ListOfParserServices Parserinterfaceservicelist ;
} ;

typedef vector<ParserDefInterface> ListOfDefinitionInterface;

struct ParserComponent
{
  string Parsercomponentname;
  string Parsercomponentusername;
  ParserComponentType Parsercomponenttype;
  bool Parsercomponentmultistudy ;
  string Parsercomponenticone;
  ListOfDefinitionInterface ParserListInterface;
  string Parserconstraint ;
};

typedef vector<ParserComponent> ListOfParserComponent ;

#ifdef WRITE_CATA_COMPONENT
// contains all the paths and the computers defined in the catalog
ListOfParserPathPrefix _pathlist;
  
// contains all the modules defined in the catalog
ListOfParserComponent _modulelist; 
#else
extern ListOfParserPathPrefix _pathlist;
extern ListOfParserComponent _modulelist; 
#endif



#endif // SALOME_CATALOG_PARSER_H
