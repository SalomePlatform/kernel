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
//  File   : SALOME_RessourcesCatalog_Parser.hxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#ifndef SALOME_RESSOURCES_CATALOG_PARSER
#define SALOME_RESSOURCES_CATALOG_PARSER

#include <string>
#include <vector>

struct ParserProc{
    long Parsernumber;
    string Parsermodel_name;
    float Parsercpu_mhz;
    float Parsercache_size;
};

typedef vector<ParserProc> ListOfParserProc;

enum Parsercontainertype {Cpp, python, NP};
typedef vector<Parsercontainertype> ListOfParserContainerType;

struct Parserressources{
  string Parsername;
  string ParserOS;
  string ParserOS_version;
  ListOfParserProc Parserprocs;
  ListOfParserContainerType Parsercontainertype;
};

typedef vector<Parserressources> ListOfParserressources;

#ifdef WRITE_RESSOURCES_TYPE
ListOfParserressources _ressources_list;
#else
extern ListOfParserressources _ressources_list;
#endif

#endif //SALOME_RESSOURCES_CATALOG_PARSER
