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
//  File   : SALOME_RessourcesCatalog_impl.cxx
//  Author : Estelle Deville
//  Module : SALOME

#include "SALOME_RessourcesCatalog_impl.hxx"
#include <fstream>
using namespace std;

//----------------------------------------------------------------------
// Function : SALOME_RessourcesCatalogImpl
// Purpose  : Constructor 
//----------------------------------------------------------------------
SALOME_RessourcesCatalogImpl::SALOME_RessourcesCatalogImpl(int argc, char** argv)
{
  MESSAGE("Ressources Catalog creation");

  // Empty used variables
  _ressourceslist.resize(0);

  // Parse the arguments given at server run
  if (!_parseArguments(argc, argv,&_path_ressources))
    MESSAGE( "Error while argument parsing" )

  // Test existency of files
  if (_path_ressources == NULL)
    MESSAGE( "Error the ressources catalog should be indicated" )
  else
    {
      MESSAGE("Parse ressources catalog");
      // Affect the variable _ressourceslist 
      _parse_xml_file(_path_ressources,_ressourceslist);

      // Verification of _datatype_list content
      if(!_verify_ressources(_ressourceslist))
	MESSAGE( "Error while parsing the ressources catalog" )
      else MESSAGE("Ressources Catalog OK");
    }
}

//----------------------------------------------------------------------
// Function : ~SALOME_RessourcesCatalogImpl
// Purpose  : Destructor 
//----------------------------------------------------------------------
SALOME_RessourcesCatalogImpl::~SALOME_RessourcesCatalogImpl()
{
  MESSAGE("Ressources Catalog Destruction");
}

//----------------------------------------------------------------------
// Function : GetComputerList
// Purpose  : get ressources list
//----------------------------------------------------------------------
SALOME_RessourcesCatalog::ListOfComputer* 
SALOME_RessourcesCatalogImpl::GetComputerList()
{
  MESSAGE("Begin of GetComputerList");
  SALOME_RessourcesCatalog::ListOfComputer_var _list = new SALOME_RessourcesCatalog::ListOfComputer;
  _list->length(_ressourceslist.size());
 for (unsigned int ind=0; ind < _ressourceslist.size();ind++)
   {
     _list[ind]=CORBA::string_dup(_ressourceslist[ind].Parsername.c_str());
   }
  return _list._retn();
}


//----------------------------------------------------------------------
// Function : GetRessourcesList
// Purpose  : get a data type name list
//----------------------------------------------------------------------
SALOME_RessourcesCatalog::ListOfContainerType* 
SALOME_RessourcesCatalogImpl::GetContainerTypeList(const char* computer) 
                              throw(SALOME_RessourcesCatalog::NotFound)
{
  MESSAGE("Begin of GetContainerTypeList");
  SALOME_RessourcesCatalog::ListOfContainerType_var _list = 
    new SALOME_RessourcesCatalog::ListOfContainerType;

  bool find = false ;

  // Looking for ressources named "computer" 
  // If found, get container type list
  // If not found, exception is thrown

  for (unsigned int ind=0; ind < _ressourceslist.size();ind++)
    {
     if (strcmp((_ressourceslist[ind].Parsername).c_str(),computer) == 0)
        {
          MESSAGE("Ressources named " << computer << " found in the ressources catalog");
            find = true;
	    
	    _list->length(_ressourceslist[ind].Parsercontainertype.size());
	    for (unsigned int ind1=0; ind1 < _ressourceslist[ind].Parsercontainertype.size();ind1++)
	      {
             // get container type list
		switch(_ressourceslist[ind].Parsercontainertype[ind1]){
		case Cpp:
		  _list[ind1] = SALOME_RessourcesCatalog::Cpp;
		  break;
		case python:
		  _list[ind1] = SALOME_RessourcesCatalog::python;
		  break;
		case NP:
		  _list[ind1] = SALOME_RessourcesCatalog::NP;
		  break;
		}
	      }
	}
    }

  if (!find)
    {
      // The ressources was not found, the exception should be thrown
      MESSAGE( "The ressource " << computer <<  " was not found in the ressources catalog" )
      throw SALOME_RessourcesCatalog::NotFound() ;
    }
  return _list._retn();
}

//----------------------------------------------------------------------
// Function : GetComputerInfo
// Purpose  : get ressources information
//----------------------------------------------------------------------
SALOME_RessourcesCatalog::computer_info*
SALOME_RessourcesCatalogImpl::GetComputerInfo(const char* computer)
                            throw(SALOME_RessourcesCatalog::NotFound)
{
  MESSAGE("Begin of GetComputerInfo");
  SALOME_RessourcesCatalog::computer_info_var _computer_info = 
    new SALOME_RessourcesCatalog::computer_info;

  bool find = false ;

  // Looking for ressources named "computer" 
  // If found, get computer info
  // If not found, exception is thrown
   for (unsigned int ind=0; ind < _ressourceslist.size();ind++)
    {
     if (strcmp((_ressourceslist[ind].Parsername).c_str(),computer) == 0)
        {
          MESSAGE("Ressources named " << computer << " found in the ressources catalog");
            find = true;

	    _computer_info->name = CORBA::string_dup(_ressourceslist[ind].Parsername.c_str());
	    _computer_info->OS = CORBA::string_dup(_ressourceslist[ind].ParserOS.c_str());
	    _computer_info->OS_version = CORBA::string_dup(_ressourceslist[ind].ParserOS_version.c_str());
	    _computer_info->procs = _duplicate_procs(_ressourceslist[ind].Parserprocs);    
	}
    }
 
  if (!find)
    {
      // The ressources was not found, the exception should be thrown
      MESSAGE( "The ressource " << computer <<  " was not found in the ressources catalog" )
      throw SALOME_RessourcesCatalog::NotFound() ;
    }
  return _computer_info._retn();
}



//----------------------------------------------------------------------
// Function : _parse_xml_file
// Purpose  : parse one module catalog 
//----------------------------------------------------------------------
void 
SALOME_RessourcesCatalogImpl::_parse_xml_file(const char* file,
					    ListOfParserressources& ressourceslist) 
{
  SALOME_RessourcesCatalog_Handler* handler = new SALOME_RessourcesCatalog_Handler();
  QFile xmlFile(file);

  QXmlInputSource source(xmlFile);

  QXmlSimpleReader reader;
  reader.setContentHandler( handler );
  reader.setErrorHandler( handler );
  reader.parse( source );
  xmlFile.close();
  ressourceslist = _ressources_list;
}


//----------------------------------------------------------------------
// Function : _verify_ressources
// Purpose  : verify ressources from the catalog parsing
//            Verify that a computer is'nt defined twice in the catalog
//----------------------------------------------------------------------
bool
SALOME_RessourcesCatalogImpl::_verify_ressources(ListOfParserressources ressourceslist)
{
    bool _return_value = true;
    bool _bool = false ;
    vector<string> _machine_list;
    _machine_list.resize(0);

  // Fill a list of all computers indicated in the ressources list
  for (unsigned int ind = 0; ind < ressourceslist.size(); ind++)
          _machine_list.push_back(ressourceslist[ind].Parsername);   

  // Parse if a computer name is twice in the list of computers
  for (unsigned int ind = 0; ind < _machine_list.size(); ind++)
    {
     for (unsigned int ind1 = ind+1 ; ind1 < _machine_list.size(); ind1++)
       {
         if(_machine_list[ind].compare(_machine_list[ind1]) == 0)
           {
             MESSAGE("The computer " << _machine_list[ind] << " is indicated more than once in the ressources list")
             _return_value = false;
          }
       }
    }

   return _return_value;
}


//----------------------------------------------------------------------
// Function : _parseArguments
// Purpose  : parse arguments to get general and personal catalog files
//----------------------------------------------------------------------
bool
SALOME_RessourcesCatalogImpl::_parseArguments(int argc, char **argv, 
					      char **_path)
{
  bool _return_value = true;
  *_path = NULL;
  for (int ind = 0; ind < argc ; ind++)
    {

      if (strcmp(argv[ind],"-help") == 0)
	{
	  INFOS( "Usage: " << argv[0] << " -common 'path to ressources catalog' -ORBInitRef NameService=corbaname::localhost");
	    _return_value = false ;
	}
      if (strcmp(argv[ind],"-common") == 0)
	{
	  if (ind + 1 <= argc)
	    {
	      // General catalog file
	      *_path= argv[ind + 1] ;
	      ifstream _path_file(*_path);
	      if (!_path_file)
		{
		  MESSAGE( "Sorry the file " << *_path << " can't be open" )
		  *_path = NULL;
		  _return_value = false;
		}
	    }
	}
     
    }
  return _return_value;
}

//----------------------------------------------------------------------
// Function : _duplicate_procs
// Purpose  : create a list of processors information from the catalog parsing
//----------------------------------------------------------------------
SALOME_RessourcesCatalog::ListOfProc
SALOME_RessourcesCatalogImpl::_duplicate_procs(ListOfParserProc list_procs)
{
  SALOME_RessourcesCatalog::ListOfProc _list_procs;
  unsigned int _length = list_procs.size();
  _list_procs.length(_length);
 
  for (unsigned int ind = 0; ind < _length; ind++)
    {
      //duplicate processor number
      _list_procs[ind].number = list_procs[ind].Parsernumber;

      //duplicate model name
      _list_procs[ind].model_name = CORBA::string_dup(list_procs[ind].Parsermodel_name.c_str());

      //duplicate cpu
      _list_procs[ind].cpu_mhz = list_procs[ind].Parsercpu_mhz;

      // duplicate cache size
      _list_procs[ind].cache_size = list_procs[ind].Parsercache_size;
    }
  return _list_procs;
}
