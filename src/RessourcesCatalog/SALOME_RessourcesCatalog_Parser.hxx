//File SALOME_RessourcesCatalog_Parser.hxx
//Created: Fri Sept 07 2001
//Author: Estelle Deville
//Project: SALOME
//Copyright: CEA/DEN/DM2S/LGLS
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
