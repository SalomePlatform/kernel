//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
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
//  File   : SALOME_ModuleCatalog_Parser.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

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
