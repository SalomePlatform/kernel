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
//
#ifndef SALOME_CATALOG_PARSER_H
#define SALOME_CATALOG_PARSER_H

#include <string>
#include <vector>

// Type Definitions
struct ParserPathPrefix
{
  std::string         path;
  std::vector<std::string> listOfComputer ;
};

typedef std::vector<ParserPathPrefix> ParserPathPrefixes ;

enum ParserComponentType {GEOM, MESH, Med, SOLVER, DATA, VISU, SUPERV, OTHER} ;

struct ParserParameter
{
  std::string type;
  std::string name;
} ;


struct ParserDataStreamParameter
{
  std::string type;
  std::string name;
  std::string dependency;
} ;

typedef std::vector<ParserParameter> ParserParameters;
typedef std::vector<ParserDataStreamParameter> ParserDataStreamParameters;

struct ParserService
{
  ParserService() {
    typeOfNode = 1;
    byDefault = 1;
  }
  std::string                name;
  ParserParameters           inParameters;
  ParserParameters           outParameters;
  ParserDataStreamParameters inDataStreamParameters;
  ParserDataStreamParameters outDataStreamParameters;
  bool                       byDefault;
  bool                       typeOfNode;
} ;

typedef std::vector<ParserService> ParserServices ;

struct ParserInterface
{
  std::string    name ;
  ParserServices services ;
} ;

typedef std::vector<ParserInterface> ParserInterfaces;

struct ParserComponent
{
  std::string         name;
  std::string         username;
  ParserComponentType type;
  bool                multistudy;
  std::string         icon;
  std::string         constraint;
  ParserInterfaces    interfaces;
  ParserPathPrefixes  prefixes;
  bool                implementationType;
  std::string         version;
  std::string         comment;
};

typedef std::vector<ParserComponent> ParserComponents ;

#ifdef WRITE_CATA_COMPONENT
// contains all the paths and the computers defined in the catalog 
       ParserPathPrefixes  _pathList;
  
// contains all the modules defined in the catalog
       ParserComponents    _moduleList; 
#else
extern ParserPathPrefixes _pathList;
extern ParserComponents   _moduleList; 
#endif


#endif // SALOME_CATALOG_PARSER_H
