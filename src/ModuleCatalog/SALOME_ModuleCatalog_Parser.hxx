// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
//  File   : SALOME_ModuleCatalog_Parser.hxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#ifndef SALOME_CATALOG_PARSER_H
#define SALOME_CATALOG_PARSER_H

#include <string>
#include <vector>
#include <map>

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
  std::string         implementationType;
  std::string         implementationName;
  std::string         version;
  std::string         comment;
};

typedef std::vector<ParserComponent> ParserComponents ;

struct ParserType
{
  std::string         name;
  std::string         kind;
  std::string         id;
  std::string         content;
  std::vector<std::string> bases;
  std::vector< std::pair<std::string,std::string> > members;
};
typedef std::map<std::string,ParserType> ParserTypes ;
typedef std::map<std::string,ParserType&> RefTypes ;
typedef std::vector<ParserType> TypeList ;

struct ParserSequence:public ParserType
{
  ParserSequence(){kind="sequence";}
};
typedef std::map<std::string,ParserSequence> ParserSequences ;

struct ParserObjref:public ParserType
{
  ParserObjref(){kind="objref";}
};
typedef std::map<std::string,ParserObjref> ParserObjrefs ;

struct ParserStruct:public ParserType
{
  ParserStruct(){kind="struct";}
};
typedef std::map<std::string,ParserStruct> ParserStructs ;

#endif // SALOME_CATALOG_PARSER_H
