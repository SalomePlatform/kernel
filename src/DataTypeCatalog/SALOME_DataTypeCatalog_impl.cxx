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
//  File   : SALOME_DataTypeCatalog_impl.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$

#include "SALOME_DataTypeCatalog_impl.hxx"
#include "Utils_ExceptHandlers.hxx"
#include <fstream>
using namespace std;

UNEXPECT_CATCH(DTC_NotFound, SALOME_DataTypeCatalog::NotFound);

//----------------------------------------------------------------------
// Function : SALOME_DataTypeCatalogImpl
// Purpose  : Constructor 
//----------------------------------------------------------------------
SALOME_DataTypeCatalogImpl::SALOME_DataTypeCatalogImpl(int argc, char** argv)
{
  MESSAGE("DataType Catalog creation");

  // Empty used variables
  _datatype_list.resize(0);

  // Parse the arguments given at server run
  if (!_parseArguments(argc, argv,&_path_datatype))
    MESSAGE( "Error while argument parsing" )

  // Test existency of files
  if (_path_datatype == NULL)
    MESSAGE( "Error the data type catalog should be indicated" )
  else
    {
      MESSAGE("Parse data type catalog");
      // Affect the variable _datatype_list 
      _parse_xml_file(_path_datatype,_datatype_list);

      // Verification of _datatype_list content
      if(!_verify_data_type(_datatype_list))
	MESSAGE( "Error while parsing the data type list" )
      else MESSAGE("Data Type list OK");
    }
}

//----------------------------------------------------------------------
// Function : ~SALOME_DataTypeCatalogImpl
// Purpose  : Destructor 
//----------------------------------------------------------------------
SALOME_DataTypeCatalogImpl::~SALOME_DataTypeCatalogImpl()
{
  MESSAGE("DataType Catalog Destruction");
}


//----------------------------------------------------------------------
// Function : GetDataTypeList
// Purpose  : get a data type name list
//----------------------------------------------------------------------
SALOME_DataTypeCatalog::ListOfDataTypeName* 
SALOME_DataTypeCatalogImpl::GetDataTypeList()
{
  MESSAGE("Begin of GetDataTypeList");
  SALOME_DataTypeCatalog::ListOfDataTypeName_var _list_data = 
    new SALOME_DataTypeCatalog::ListOfDataTypeName;

  _list_data->length(_datatype_list.size());

  // Duplicate each data type name defined in the catalog
  for (unsigned int ind=0; ind < _datatype_list.size();ind++)
    {
       _list_data[ind]=CORBA::string_dup(_datatype_list[ind].Parserdata_name.c_str());
       // SCRUTE(_list_data[ind]) ;
    }

  return _list_data._retn();
}

//----------------------------------------------------------------------
// Function : isDerivedFrom
// Purpose  : return true if type_out is derived from type_in
//----------------------------------------------------------------------
CORBA::Boolean 
SALOME_DataTypeCatalogImpl::isDerivedFrom(const char* type_in,
					  const char* type_out)
                            throw(SALOME_DataTypeCatalog::NotFound)
{
  Unexpect aCatch(DTC_NotFound);
  CORBA::Boolean _return_value = false ;
  bool _found_in = false;
  bool _found_out = false;
  for (unsigned int ind=0; ind < _datatype_list.size();ind++)
    {
      if (strcmp((_datatype_list[ind].Parserdata_name).c_str(),type_out) == 0)
	{
	  //type_out found
	  _found_out = true;
	  // Looking if type_in is indicated in the parents list
	  _return_value = _explore_parents(type_in,_datatype_list[ind]); 
	}
      if (strcmp((_datatype_list[ind].Parserdata_name).c_str(),type_in) == 0)
	_found_in = true;
    }

  if(!_found_in)
    {
      MESSAGE("The data type " << type_in << " was not found in the data type catalog")
      throw SALOME_DataTypeCatalog::NotFound() ;
    }
  if(!_found_out)
    {
      MESSAGE( "The data type " << type_out << " was not found in the data type catalog")
      throw SALOME_DataTypeCatalog::NotFound() ;
    }

  return _return_value;

}

//----------------------------------------------------------------------
// Function : GetDataInterfaceRead
// Purpose  : get the read interface name
//----------------------------------------------------------------------
char*
SALOME_DataTypeCatalogImpl::GetDataInterfaceRead(const char* type)
                            throw(SALOME_DataTypeCatalog::NotFound)
{
  Unexpect aCatch(DTC_NotFound);
  char* return_value = NULL;
  bool _find = false;
  for (unsigned int ind=0; ind < _datatype_list.size();ind++)
    {
      if (strcmp((_datatype_list[ind].Parserdata_name).c_str(),type) == 0)
	{
	  _find = true;
	  return_value =  CORBA::string_dup(_datatype_list[ind].Parserdata_interface_read.c_str());
	}
    }
  
  // NotFound Exceptin is thrown if the data type indicated is not found in the catalog
  if(!_find)
    {
      MESSAGE( "The data type " << type << " was not found in the data type catalog")
      throw SALOME_DataTypeCatalog::NotFound() ;
    }

  return return_value;
}

//----------------------------------------------------------------------
// Function : GetDataInterfaceWrite
// Purpose  : get the write interface name
//----------------------------------------------------------------------
char*
SALOME_DataTypeCatalogImpl::GetDataInterfaceWrite(const char* type)
                            throw(SALOME_DataTypeCatalog::NotFound)
{
  Unexpect aCatch(DTC_NotFound);
  char* return_value = NULL;
  bool _find = false;
  for (unsigned int ind=0; ind < _datatype_list.size();ind++)
    {
      if (strcmp((_datatype_list[ind].Parserdata_name).c_str(),type) == 0)
	{
	  _find = true;
	  return_value =  CORBA::string_dup(_datatype_list[ind].Parserdata_interface_write.c_str());
	}
    }

  // NotFound Exceptin is thrown if the data type indicated is not found in the catalog
  if(!_find)
    {
      MESSAGE( "The data type " << type << " was not found in the data type catalog")
      throw SALOME_DataTypeCatalog::NotFound() ;
    }
  return return_value;
}

//----------------------------------------------------------------------
// Function : GetDataTypeParents
// Purpose  : get Parents data type name list
//----------------------------------------------------------------------
SALOME_DataTypeCatalog::ListOfDataTypeName* 
SALOME_DataTypeCatalogImpl::GetDataTypeParents(const char* type) throw(SALOME_DataTypeCatalog::NotFound)
{
  Unexpect aCatch(DTC_NotFound);

  bool _find = false ;

  SALOME_DataTypeCatalog::ListOfDataTypeName_var _list_data = 
    new SALOME_DataTypeCatalog::ListOfDataTypeName;

  for (unsigned int ind=0; ind < _datatype_list.size();ind++)
    {
      if (strcmp((_datatype_list[ind].Parserdata_name).c_str(),type) == 0)
	{
	  // Wanted type is found
	  // Get all the parents data type name of the type
	  _find = true; 
	  _list_data->length(_datatype_list[ind].Parserdata_parents.size());
	  for (unsigned int ind1=0; ind1 < _datatype_list[ind].Parserdata_parents.size();ind1++)
	    {
	      _list_data[ind1]=CORBA::string_dup(_datatype_list[ind].Parserdata_parents[ind1].c_str());
	      // SCRUTE(_list_data[ind1]) ;
	    }
	}
    }

  // NotFound Exception is thrown if the data type indicated is not found 
  // in the catalog
  if(!_find)
    {
      MESSAGE( "The data type " << type << " was not found in the data type catalog")
      throw SALOME_DataTypeCatalog::NotFound() ;
    }

  return _list_data._retn();
}

//----------------------------------------------------------------------
// Function : _explore_parents
// Purpose  : return true if type_out is derived from type_in
//----------------------------------------------------------------------
CORBA::Boolean 
SALOME_DataTypeCatalogImpl::_explore_parents(const char* type_in,
					     ParserDataType& data_out)
{

  MESSAGE( "Begin of _explore_parents with data_out_name  " << data_out.Parserdata_name);

  CORBA::Boolean found = false ;
  if (data_out.Parserdata_name == type_in)
    found = true ;
  else
    if (data_out.Parserdata_parents.size() >0)
      for (unsigned int ind=0; ind < data_out.Parserdata_parents.size();ind++)
	{
	  if (data_out.Parserdata_parents[ind] == type_in)
	    found = true ;
	  else
	    for (unsigned ip =0; ip < _datatype_list.size();ip++)
	      {
		// run _explore_parents for each parent data of data_out
		if (_datatype_list[ip].Parserdata_name == data_out.Parserdata_parents[ind])
		  found = _explore_parents(type_in, _datatype_list[ip]);
	      }
	}
  return found ;
}

//----------------------------------------------------------------------
// Function : _parse_xml_file
// Purpose  : parse one module catalog 
//----------------------------------------------------------------------
void 
SALOME_DataTypeCatalogImpl::_parse_xml_file(const char* file,
					    ListOfParserDataType& datatypelist) 
{
  SALOME_DataTypeCatalog_Handler* handler = new SALOME_DataTypeCatalog_Handler();
  QFile xmlFile(file);

  QXmlInputSource source(xmlFile);

  QXmlSimpleReader reader;
  reader.setContentHandler( handler );
  reader.setErrorHandler( handler );
  reader.parse( source );
  xmlFile.close();
  datatypelist = _datatypelist;
}


//----------------------------------------------------------------------
// Function : _verify_data_type
// Purpose  : verify the data type structures from the catalog parsing
//            Verify that the parents type associated to a type are defined
//            in the catalog
//----------------------------------------------------------------------
bool
SALOME_DataTypeCatalogImpl::_verify_data_type(ListOfParserDataType datatypelist)
{
   bool _return_value = true;
   bool _bool = false ;
   vector<string> _data_name_list;
   
   _data_name_list.resize(0);;
  for (unsigned int ind = 0; ind < _datatype_list.size(); ind++)
    _data_name_list.push_back(_datatype_list[ind].Parserdata_name) ;

   // Parse if parents data type name of a data type are defined in the 
   // datatype catalog
  for (unsigned int ind = 0; ind < _datatype_list.size(); ind++)
    {
      // Scrute data type parents
      // MESSAGE("Treatment of " << _datatype_list[ind].Parserdata_name);
      for (unsigned int ind1 = 0 ; ind1 < _datatype_list[ind].Parserdata_parents.size(); ind1++)
       {
	 // MESSAGE("Looking for " << _datatype_list[ind].Parserdata_parents[ind1] << " in the catalog data type");
	 _bool = false;
	 // Compare parent data type name to all data type names defined in the catalog
	 for (unsigned int ind2 = 0 ; ind2 < _data_name_list.size(); ind2++)
	   {
	     if(_datatype_list[ind].Parserdata_parents[ind1].compare(_data_name_list[ind2]) == 0)
	       {
		 // Type found : OK
		 MESSAGE("The parents data type " << _datatype_list[ind].Parserdata_parents[ind1] << " of " << _datatype_list[ind].Parserdata_name << " was found in the catalog type");
		 _bool = true; 
	       }
	   }
	 if(!_bool)
	   {
	     MESSAGE( "The parents data type " << _datatype_list[ind].Parserdata_parents[ind1] << " of " << _datatype_list[ind].Parserdata_name << " is not defined in the data type catalog" )
	     _return_value = false ;
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
SALOME_DataTypeCatalogImpl::_parseArguments(int argc, char **argv, 
					    char **_path_data)
{
  bool _return_value = true;
  *_path_data = NULL;
  for (int ind = 0; ind < argc ; ind++)
    {

      if (strcmp(argv[ind],"-help") == 0)
	{
	  MESSAGE( "Usage: " << argv[0] << " -common 'path to data type catalog' -ORBInitRef NameService=corbaname::localhost");
	    _return_value = false ;
	}
      if (strcmp(argv[ind],"-common") == 0)
	{
	  if (ind + 1 <= argc)
	    {
	      // General catalog file
	      *_path_data = argv[ind + 1] ;
	      ifstream _path_data_file(*_path_data);
	      if (!_path_data_file)
		{
		  MESSAGE( "Sorry the file " << *_path_data << " can't be open" )
		  *_path_data = NULL;
		  _return_value = false;
		}
	    }
	}
     
    }
  return _return_value;
}
