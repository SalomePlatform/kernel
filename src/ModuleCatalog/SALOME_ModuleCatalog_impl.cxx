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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_ModuleCatalog_impl.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOME_ModuleCatalog_impl.hxx"
#include "SALOME_ModuleCatalog_Acomponent_impl.hxx"
#include <fstream>
#include <map>

using namespace std;

#include "utilities.h"

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

static const char* SEPARATOR     = "::";
static const char* OLD_SEPARATOR = ":";


list<string> splitStringToList(const string& theString, const string& theSeparator)
{
  list<string> aList;

  int sepLen = theSeparator.length();
  int startPos = 0, sepPos = theString.find(theSeparator, startPos);

  while (1)
    {
      string anItem ;
      if(sepPos != string::npos)
        anItem = theString.substr(startPos, sepPos - startPos);
      else
        anItem = theString.substr(startPos);
      if (anItem.length() > 0)
	aList.push_back(anItem);
      if(sepPos == string::npos)
        break;
      startPos = sepPos + sepLen;
      sepPos = theString.find(theSeparator, startPos);
    }

  return aList;
}

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalogImpl
// Purpose  : Constructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalogImpl::SALOME_ModuleCatalogImpl(int argc, char** argv, CORBA::ORB_ptr orb) : _orb(orb)
{
  if(MYDEBUG) MESSAGE("Catalog creation");

  // Conversion rules for component types
  ComponentTypeConvert[GEOM]
    = SALOME_ModuleCatalog::GEOM;
  ComponentTypeConvert[MESH]
    = SALOME_ModuleCatalog::MESH;
  ComponentTypeConvert[Med]
    = SALOME_ModuleCatalog::Med;
  ComponentTypeConvert[SOLVER]
    = SALOME_ModuleCatalog::SOLVER;
  ComponentTypeConvert[DATA]
    = SALOME_ModuleCatalog::DATA;
  ComponentTypeConvert[VISU]
    = SALOME_ModuleCatalog::VISU;
  ComponentTypeConvert[SUPERV]
    = SALOME_ModuleCatalog::SUPERV;
  ComponentTypeConvert[OTHER]
    = SALOME_ModuleCatalog::OTHER;

  // Conversion rules for datastream parameters dependency
  DataStreamDepConvert["UNDEFINED"] 
    = SALOME_ModuleCatalog::DATASTREAM_UNDEFINED;
  DataStreamDepConvert["T"]
    = SALOME_ModuleCatalog::DATASTREAM_TEMPORAL;
  DataStreamDepConvert["I"] 
    = SALOME_ModuleCatalog::DATASTREAM_ITERATIVE;

  // Empty used variables
  _general_module_list.resize(0);
  _general_path_list.resize(0);

  _personal_module_list.resize(0);
  _personal_path_list.resize(0);

  // Parse the arguments given at server run
  if (!_parseArguments(argc, argv,&_general_path,&_personal_path))
    if(MYDEBUG) MESSAGE( "Error while argument parsing" );

  // Test existency of files
  if (_general_path == NULL)
  {
    if(MYDEBUG) MESSAGE( "Error the general catalog should be indicated" );
  }
  else
  {
    // Affect the _general_module_list and _general_path_list members
    // with the common catalog

    list<string> dirList;

#ifdef WNT
    dirList = splitStringToList(_general_path, SEPARATOR);
#else
    //check for new format
    bool isNew = (std::string( _general_path ).find(SEPARATOR) != string::npos);
    if ( isNew ) {
      //using new format
      dirList = splitStringToList(_general_path, SEPARATOR);
    } else {
      //support old format
      dirList = splitStringToList(_general_path, OLD_SEPARATOR);
    }
#endif

    for (list<string>::iterator iter = dirList.begin(); iter != dirList.end(); iter++)
    {
      string aPath = (*iter);
      //remove inverted commas from filename
      while (aPath.find('\"') != string::npos)
	aPath.erase(aPath.find('\"'), 1);

      _parse_xml_file(aPath.c_str(), 
		      _general_module_list, 
		      _general_path_list,
                      _typeMap,
                      _typeList);
    }

    // Verification of _general_path_list content
    if (!_verify_path_prefix(_general_path_list)) {
      if(MYDEBUG) MESSAGE( "Error while parsing the general path list, "
			   "differents paths are associated to the same computer," 
			   "the first one will be choosen");
    } else {
      if(MYDEBUG) MESSAGE("General path list OK");
    }

    if (_personal_path != NULL) {
      // Initialize the _personal_module_list and 
      // _personal_path_list members with the personal catalog files
      _parse_xml_file(_personal_path,
		      _personal_module_list, 
		      _personal_path_list,
                      _typeMap,
                      _typeList);
      
      // Verification of _general_path_list content
      if(!_verify_path_prefix(_personal_path_list)){
	if(MYDEBUG) MESSAGE("Error while parsing the personal path list, "
			    "differents paths are associated to the same computer, "
			    "the first one will be choosen" );
      }else {
	if(MYDEBUG) MESSAGE("Personal path list OK");
      }
    }else 
      if(MYDEBUG) MESSAGE("No personal catalog indicated or error while "
			  "opening the personal catalog");
  }
}

//----------------------------------------------------------------------
// Function : ~SALOME_ModuleCatalogImpl
// Purpose  : Destructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalogImpl::~SALOME_ModuleCatalogImpl()
{
  if(MYDEBUG) MESSAGE("Catalog Destruction");
}


//! Get the list of all types of the catalog
/*!
 *   \return  the list of types
 */
SALOME_ModuleCatalog::ListOfTypeDefinition* SALOME_ModuleCatalogImpl::GetTypes()
{
  SALOME_ModuleCatalog::ListOfTypeDefinition_var type_list = new SALOME_ModuleCatalog::ListOfTypeDefinition();
  type_list->length(_typeList.size());

  for (int ind = 0 ; ind < _typeList.size() ; ind++)
    {
      //no real need to call string_dup, omniorb calls it on operator= (const char *) but it is safer
      type_list[ind].name=CORBA::string_dup(_typeList[ind].name.c_str());
      type_list[ind].kind=SALOME_ModuleCatalog::NONE;
      if(_typeList[ind].kind=="double")
        type_list[ind].kind=SALOME_ModuleCatalog::Dble;
      else if(_typeList[ind].kind=="int")
        type_list[ind].kind=SALOME_ModuleCatalog::Int;
      else if(_typeList[ind].kind=="bool")
        type_list[ind].kind=SALOME_ModuleCatalog::Bool;
      else if(_typeList[ind].kind=="string")
        type_list[ind].kind=SALOME_ModuleCatalog::Str;
      else if(_typeList[ind].kind=="objref")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Objref;
          type_list[ind].id=CORBA::string_dup(_typeList[ind].id.c_str());
          //bases
          type_list[ind].bases.length(_typeList[ind].bases.size());
          std::vector<std::string>::const_iterator miter;
          miter=_typeList[ind].bases.begin();
          int n_memb=0;
          while(miter != _typeList[ind].bases.end())
            {
              type_list[ind].bases[n_memb]=CORBA::string_dup(miter->c_str());
              miter++;
              n_memb++;
            }
        }
      else if(_typeList[ind].kind=="sequence")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Seq;
          type_list[ind].content=CORBA::string_dup(_typeList[ind].content.c_str());
        }
      else if(_typeList[ind].kind=="array")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Array;
          type_list[ind].content=CORBA::string_dup(_typeList[ind].content.c_str());
        }
      else if(_typeList[ind].kind=="struct")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Struc;
          //members
          type_list[ind].members.length(_typeList[ind].members.size());

          std::vector< std::pair<std::string,std::string> >::const_iterator miter;
          miter=_typeList[ind].members.begin();
          int n_memb=0;
          while(miter != _typeList[ind].members.end())
            {
              type_list[ind].members[n_memb].name=CORBA::string_dup(miter->first.c_str());
              type_list[ind].members[n_memb].type=CORBA::string_dup(miter->second.c_str());
              n_memb++;
              miter++;
            }
        }
    }
  return type_list._retn();
}

//----------------------------------------------------------------------
// Function : GetComputerList
// Purpose  : get a computer list
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfComputers* 
SALOME_ModuleCatalogImpl::GetComputerList()
{
  SALOME_ModuleCatalog::ListOfComputers_var _list_computers = 
    new SALOME_ModuleCatalog::ListOfComputers;
  return _list_computers._retn();
}

//----------------------------------------------------------------------
// Function : GetPathPrefix
// Purpose  : get the PathPrefix of a computer
//----------------------------------------------------------------------
char * 
SALOME_ModuleCatalogImpl::GetPathPrefix(const char* machinename) {
  if(MYDEBUG) MESSAGE("Begin of GetPathPrefix");
  // Variables initialisation
  char* _path = NULL;
  bool _find = false ;

  // Parse all the path prefixes
  // looking for the wanted computer
  for (unsigned int ind = 0 ; ind < _personal_path_list.size() ; ind++)
    {
      for (unsigned int ind1 = 0 ; ind1 < _personal_path_list[ind].listOfComputer.size() ; ind1++)    
	{
	  if (strcmp(machinename, _personal_path_list[ind].listOfComputer[ind1].c_str()) == 0)
	    {
	      _find = true ;
	      // Wanted computer
	      // affect the path to be returned
	        const char* _temp = _personal_path_list[ind].path.c_str() ;
		  _path = new char[strlen(_temp)+1];
	      strcpy(_path,_temp);
	    }
	}
    }

  if (!_find)
    {
    for (unsigned int ind = 0 ; ind < _general_path_list.size() ; ind++)
      {
        for (unsigned int ind1 = 0 ; ind1 < _general_path_list[ind].listOfComputer.size() ; ind1++)    
	  {
	    if (strcmp(machinename, _general_path_list[ind].listOfComputer[ind1].c_str()) == 0)
	      {
	        _find = true ;
	        // Wanted computer
	        // affect the path to be returned
	          const char* _temp = _general_path_list[ind].path.c_str() ;
		    _path = new char[strlen(_temp)+1];
	        strcpy(_path,_temp);
	      }
	  }
      }
    }

  return _path;
}

//----------------------------------------------------------------------
// Function : GetComponentList
// Purpose  : get a component list
//            If a component is defined in the personal catalog and 
//            in the general catalog (same name), the component defined
//            in the personal catalog is used
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfComponents* 
SALOME_ModuleCatalogImpl::GetComponentList()
{
  if(MYDEBUG) MESSAGE("Begin of GetComponentList");
  SALOME_ModuleCatalog::ListOfComponents_var _list_components = 
    new SALOME_ModuleCatalog::ListOfComponents;

  _list_components->length(_personal_module_list.size());

  // All the components defined in the personal catalog are taken
  for(unsigned int ind=0; ind < _personal_module_list.size();ind++){
    _list_components[ind]=(_personal_module_list[ind].name).c_str();
    if(MYDEBUG) SCRUTE(_list_components[ind]) ;
  }

  int indice = _personal_module_list.size() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
#ifndef WNT
  for(unsigned int ind=0; ind < _general_module_list.size();ind++){
#else
  for(ind=0; ind < _general_module_list.size();ind++){
#endif
    _find = false;
    for(unsigned int ind1=0; ind1 < _personal_module_list.size();ind1++){
      // searching if the component is already defined in 
      // the personal catalog
      if ((_general_module_list[ind].name.compare(_personal_module_list[ind1].name)) == 0)
	_find = true;
    }
    if(!_find){
      if(MYDEBUG) MESSAGE("A new component " << _general_module_list[ind].name 
			  << " has to be to added in the list");
      _list_components->length(indice+1);
      // The component is not already defined => has to be taken
      _list_components[indice]=(_general_module_list[ind].name).c_str();   
      if(MYDEBUG) SCRUTE(_list_components[indice]) ;
      
      indice++;
    }else{
      if(MYDEBUG) MESSAGE("The component " <<_general_module_list[ind].name 
			  << " was already defined in the personal catalog") ;
    }
  }
  
  if(MYDEBUG) MESSAGE ( "End of GetComponentList" );
  return _list_components._retn();
}


//----------------------------------------------------------------------
// Function : GetComponentIconeList
// Purpose  : get a component list of component name and component icone
//            If a component is defined in the personal catalog and 
//            in the general catalog (same name), the component defined
//            in the personal catalog is used
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfIAPP_Affich* 
SALOME_ModuleCatalogImpl::GetComponentIconeList()
{
  if(MYDEBUG) MESSAGE("Begin of GetComponentIconeList");

  SALOME_ModuleCatalog::ListOfIAPP_Affich_var _list_components_icone = 
    new SALOME_ModuleCatalog::ListOfIAPP_Affich;

  _list_components_icone->length(_personal_module_list.size());

  // All the components defined in the personal catalog are taken
  for(unsigned int ind=0; ind < _personal_module_list.size();ind++){
    _list_components_icone[ind].modulename=(_personal_module_list[ind].name).c_str();
    _list_components_icone[ind].moduleusername=(_personal_module_list[ind].username).c_str();
    _list_components_icone[ind].moduleicone=(_personal_module_list[ind].icon).c_str();
    _list_components_icone[ind].moduleversion=(_personal_module_list[ind].version).c_str();
    _list_components_icone[ind].modulecomment=(_personal_module_list[ind].comment).c_str();
    //if(MYDEBUG) SCRUTE(_list_components_icone[ind].modulename); 
    //if(MYDEBUG) SCRUTE(_list_components_icone[ind].moduleicone);
  }
  
  int indice = _personal_module_list.size() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
#ifndef WNT
  for(unsigned int ind=0; ind < _general_module_list.size();ind++){
#else
  for(ind=0; ind < _general_module_list.size();ind++){
#endif
    _find = false;
    for(unsigned int ind1=0; ind1 < _personal_module_list.size();ind1++){
      // searching if the component is aleready defined in 
      // the personal catalog
      if((_general_module_list[ind].name.compare(_personal_module_list[ind1].name)) == 0)
	_find = true;
    }
    if(!_find){
      //	  if(MYDEBUG) MESSAGE("A new component " << _general_module_list[ind].name << " has to be to added in the list");
      _list_components_icone->length(indice+1);
      // The component is not already defined => has to be taken
      _list_components_icone[indice].modulename=_general_module_list[ind].name.c_str();  
      _list_components_icone[indice].moduleusername=_general_module_list[ind].username.c_str();  
      _list_components_icone[indice].moduleicone=_general_module_list[ind].icon.c_str(); 
      _list_components_icone[indice].moduleversion=_general_module_list[ind].version.c_str();
      _list_components_icone[indice].modulecomment=_general_module_list[ind].comment.c_str();
      //if(MYDEBUG) SCRUTE(_list_components_icone[indice].modulename) ;
      //if(MYDEBUG) SCRUTE(_list_components_icone[indice].moduleicone);
      
      indice++;
    }
    // else 
    //if(MYDEBUG) MESSAGE("The component " <<_general_module_list[ind].name << " was already defined in the personal catalog"); 
  }
  
  return _list_components_icone._retn() ;
}

//----------------------------------------------------------------------
// Function : GetTypedComponentList
// Purpose  : get a component list of a wanted type
//            If a component is defined in the personal catalog and 
//            in the general catalog (same name), the component defined
//            in the personal catalog is used
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfComponents* 
SALOME_ModuleCatalogImpl::GetTypedComponentList(SALOME_ModuleCatalog::ComponentType component_type)
{
  if(MYDEBUG) MESSAGE("Begin of GetTypedComponentList");
  SALOME_ModuleCatalog::ListOfComponents_var _list_typed_component = 
    new SALOME_ModuleCatalog::ListOfComponents;
  int _j = 0;

  _list_typed_component->length(0);
  // Transform SALOME_ModuleCatalog::ComponentType in ParserComponentType
  ParserComponentType _temp_component_type;
  switch(component_type){
  case SALOME_ModuleCatalog::GEOM:
    _temp_component_type = GEOM ;
    break;
  case SALOME_ModuleCatalog::MESH:
    _temp_component_type = MESH;
    break;   
  case SALOME_ModuleCatalog::Med:
    _temp_component_type = Med;
    break;    
  case SALOME_ModuleCatalog::SOLVER:   
    _temp_component_type = SOLVER;
    break;
  case SALOME_ModuleCatalog::DATA:
    _temp_component_type = DATA;
    break;
  case SALOME_ModuleCatalog::VISU:
    _temp_component_type = VISU;
    break;  
  case SALOME_ModuleCatalog::SUPERV:
    _temp_component_type = SUPERV;
    break;
  case SALOME_ModuleCatalog::OTHER:
    _temp_component_type = OTHER;
    break;
  }

  // All the components in the personal catalog are taken
  for (unsigned int ind=0; ind < _personal_module_list.size();ind++)
    {
      if  (_personal_module_list[ind].type == _temp_component_type)
	{
	  _list_typed_component->length(_j + 1); 
           _list_typed_component[_j] = _personal_module_list[ind].name.c_str();
	  //if(MYDEBUG) SCRUTE(_list_typed_component[_j]);
	  _j++;
	}
    }

  int indice = _list_typed_component->length() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
#ifndef WNT
  for (unsigned int ind=0; ind < _general_module_list.size();ind++)
#else
  for (ind=0; ind < _general_module_list.size();ind++)
#endif
    {
      _find = false;

      if(_general_module_list[ind].type == _temp_component_type)
	{
	  for (unsigned int ind1=0; ind1 < _personal_module_list.size();ind1++)
	    {
	      // searching if the component is aleready defined in 
	      // the personal catalog
	      if ((_general_module_list[ind].name.compare(_personal_module_list[ind1].name)) == 0)
		_find = true;
	    }
	  if (!_find)
	    {
	      //if(MYDEBUG) MESSAGE("A new component " << _general_module_list[ind].name << " has to be to added in the list");
              _list_typed_component->length(indice+1);
	      // The component is not already defined => has to be taken
	      _list_typed_component[indice]=(_general_module_list[ind].name).c_str();   
	      //if(MYDEBUG) SCRUTE(_list_typed_component[indice]) ;

	      indice++;
	    }
	  //else 
	    //if(MYDEBUG) MESSAGE("The component " <<_general_module_list[ind].name << " was already defined in the personal catalog") ;
        }
    }


  return _list_typed_component._retn();
}

//----------------------------------------------------------------------
// Function : GetComponent
// Purpose  : get a component 
//            If a component is defined in the personal catalog and 
//            in the general catalog (same name), the component defined
//            in the personal catalog is used
//----------------------------------------------------------------------
SALOME_ModuleCatalog::Acomponent_ptr 
SALOME_ModuleCatalogImpl::GetComponent(const char* name)
{
  // Looking for component named "componentname" in the personal catalog
  // If found, get name, interfaces and constraint
  // If not found, looking for component named "componentname" in
  // the general catalog
  // If found, get name, interfaces and constraint
  // If not found, NULL pointer is returned

  std::string s(name);
  ParserComponent *C_parser = NULL;
  //ParserPathPrefixes *pp = NULL;

  SALOME_ModuleCatalog::Acomponent_ptr compo
    = SALOME_ModuleCatalog::Acomponent::_nil();
  C_parser = findComponent(s);
  if (C_parser) {
    
    //    DebugParserComponent(*C_parser);

    SALOME_ModuleCatalog::ComponentDef C_corba;
    duplicate(C_corba, *C_parser);

    
    SALOME_ModuleCatalog_AcomponentImpl * aComponentImpl = 
      new SALOME_ModuleCatalog_AcomponentImpl(C_corba);
    
    compo = aComponentImpl->_this();
  }
  else {
    // Not found in the personal catalog and in the general catalog
    // return NULL object
    if(MYDEBUG) MESSAGE("Component with name  " << name 
			<< " not found in catalog");
  }
  
  return compo;
}

SALOME_ModuleCatalog::ComponentDef *
SALOME_ModuleCatalogImpl::GetComponentInfo(const char *name)
{
  std::string s(name);

  ParserComponent * C_parser = findComponent(s);
  
  if (C_parser) {
    
    SALOME_ModuleCatalog::ComponentDef * C_corba 
      = new SALOME_ModuleCatalog::ComponentDef; 
    duplicate(*C_corba, *C_parser);
    return C_corba;
  }

  return NULL;
}

CORBA::Long SALOME_ModuleCatalogImpl::getPID()
{ 
  return (CORBA::Long)getpid();
}

void SALOME_ModuleCatalogImpl::ShutdownWithExit()
{
  exit( EXIT_SUCCESS );
}

ParserComponent *
SALOME_ModuleCatalogImpl::findComponent(const string & name)
{
  ParserComponent * C_parser = NULL;

  if (!C_parser)
    for (unsigned int ind=0; ind < _personal_module_list.size();ind++)
      {
	if (name.compare(_personal_module_list[ind].name) == 0)
	  {
	    if(MYDEBUG) MESSAGE("Component named " << name 
				<< " found in the personal catalog");
	    C_parser = &(_personal_module_list[ind]);
	    break;
	  }
      }

  if (!C_parser)
    for (unsigned int ind=0; ind < _general_module_list.size();ind++)
      {
	if (name.compare(_general_module_list[ind].name) == 0)
	  {
	    //	    if(MYDEBUG) MESSAGE("Component named " << name 
	    //			<< " found in the general catalog");
	    C_parser = &(_general_module_list[ind]);
	    break;
	  }
      }

  return C_parser;
}

//----------------------------------------------------------------------
// Function : _parse_xml_file
// Purpose  : parse one module catalog 
//----------------------------------------------------------------------
void 
SALOME_ModuleCatalogImpl::_parse_xml_file(const char* file, 
					  ParserComponents& modulelist, 
					  ParserPathPrefixes& pathList,
                                          ParserTypes& typeMap,
                                          TypeList& typeList)
{
  if(MYDEBUG) BEGIN_OF("_parse_xml_file");
  if(MYDEBUG) SCRUTE(file);

  //Local path and module list for the file to parse
  ParserPathPrefixes  _pathList;
  ParserComponents    _moduleList;
 
  SALOME_ModuleCatalog_Handler* handler = new SALOME_ModuleCatalog_Handler(_pathList,_moduleList,typeMap,typeList);

  FILE* aFile = fopen(file, "r");

  if (aFile != NULL)
    {
      xmlDocPtr aDoc = xmlReadFile(file, NULL, 0);
      
      if (aDoc != NULL) 
	handler->ProcessXmlDocument(aDoc);
      else
	MESSAGE("ModuleCatalog: could not parse file "<<file);

      xmlFreeDoc(aDoc);
      xmlCleanupParser();
      fclose(aFile);
    }
  else
    MESSAGE("ModuleCatalog: file "<<file<<" is not readable.");
  
  delete handler;
  
  unsigned int i, j;

  for ( i = 0; i < _moduleList.size(); i++) {
    for (j=0; j<modulelist.size(); j++) {
      if (modulelist[j].name == _moduleList[i].name)
	break;
    }
    if (j < modulelist.size())
      modulelist[j] = _moduleList[i];
    else
      modulelist.push_back(_moduleList[i]);
  }

  for ( i=0; i < _pathList.size(); i++)
    pathList.push_back(_pathList[i]) ;

  for (j=0; j<modulelist.size(); j++)
    modulelist[j].prefixes = pathList;
}

void 
SALOME_ModuleCatalogImpl::ImportXmlCatalogFile(const char* file)
{
  _parse_xml_file(file, _personal_module_list, _personal_path_list,_typeMap,_typeList);
}


//
//  Duplicate functions create a Corba structure (component,
//  interface, service, parameter) from the corresponding C++ 
//  parser structure
//

//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create a component from the catalog parsing
//----------------------------------------------------------------------
void SALOME_ModuleCatalogImpl::duplicate
(SALOME_ModuleCatalog::ComponentDef & C_corba, 
 const ParserComponent & C_parser)
{
  C_corba.name = CORBA::string_dup(C_parser.name.c_str());
  C_corba.username = CORBA::string_dup(C_parser.username.c_str());
  C_corba.multistudy = C_parser.multistudy;
  C_corba.icon = CORBA::string_dup(C_parser.icon.c_str());
  C_corba.type = ComponentTypeConvert[C_parser.type];
  if(C_parser.implementationType == "EXE")
    C_corba.implementationType=SALOME_ModuleCatalog::EXE;
  else if(C_parser.implementationType == "CEXE")
    C_corba.implementationType=SALOME_ModuleCatalog::CEXE;
  else if(C_parser.implementationType == "PY")
    C_corba.implementationType=SALOME_ModuleCatalog::PY;
  else
    C_corba.implementationType=SALOME_ModuleCatalog::SO;
  C_corba.implname = CORBA::string_dup(C_parser.implementationName.c_str());

  unsigned int _length = C_parser.interfaces.size();
  C_corba.interfaces.length(_length);
  
  for (unsigned int ind = 0; ind < _length; ind++)
    duplicate(C_corba.interfaces[ind], C_parser.interfaces[ind]);
}


//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create an interface from the catalog parsing
//----------------------------------------------------------------------
void SALOME_ModuleCatalogImpl::duplicate
(SALOME_ModuleCatalog::DefinitionInterface & I_corba,
 const ParserInterface & I_parser)
{
  //duplicate interface name
  I_corba.interfacename = CORBA::string_dup(I_parser.name.c_str());
  
  // duplicate service list
  unsigned int _length = I_parser.services.size();
  //  if(MYDEBUG) SCRUTE(_length);
  //  I_corba.interfaceservicelist 
  //  = new SALOME_ModuleCatalog::ListOfInterfaceService;
  I_corba.interfaceservicelist.length(_length);
  
  for (unsigned int ind1 = 0; ind1 < _length ; ind1 ++)
    duplicate(I_corba.interfaceservicelist[ind1],
	      I_parser.services[ind1]);
}

//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create a service from the catalog parsing
//----------------------------------------------------------------------
void SALOME_ModuleCatalogImpl::duplicate
(SALOME_ModuleCatalog::Service & S_corba,
 const ParserService & S_parser)
{
  // duplicate service name
  S_corba.ServiceName = CORBA::string_dup(S_parser.name.c_str());
  
  // duplicate service by default
  S_corba.Servicebydefault = S_parser.byDefault;

  S_corba.TypeOfNode = S_parser.typeOfNode;

  unsigned int _length;

  // duplicate in Parameters
  _length = S_parser.inParameters.size();
  S_corba.ServiceinParameter.length(_length);

  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_corba.ServiceinParameter[ind2],
	      S_parser.inParameters[ind2]);
  
  // duplicate out Parameters
  _length = S_parser.outParameters.size();
  S_corba.ServiceoutParameter.length(_length);

#ifndef WNT
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
#else
  for (ind2 = 0; ind2 < _length ; ind2 ++)
#endif
    duplicate(S_corba.ServiceoutParameter[ind2],
	      S_parser.outParameters[ind2]);
  
  // duplicate in DataStreamParameters
  _length = S_parser.inDataStreamParameters.size();
  S_corba.ServiceinDataStreamParameter.length(_length);

#ifndef WNT
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
#else
  for (ind2 = 0; ind2 < _length ; ind2 ++)
#endif
    duplicate(S_corba.ServiceinDataStreamParameter[ind2],
	      S_parser.inDataStreamParameters[ind2]);
  
  // duplicate out DataStreamParameters
  _length = S_parser.outDataStreamParameters.size();
  //  if(MYDEBUG) SCRUTE(_length);
  S_corba.ServiceoutDataStreamParameter.length(_length);

#ifndef WNT
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
#else
  for (ind2 = 0; ind2 < _length ; ind2 ++)
#endif
    duplicate(S_corba.ServiceoutDataStreamParameter[ind2],
	      S_parser.outDataStreamParameters[ind2]);
}

//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create a service parameter from the catalog parsing
//----------------------------------------------------------------------
void SALOME_ModuleCatalogImpl::duplicate
(SALOME_ModuleCatalog::ServicesParameter & P_corba,
 const ParserParameter & P_parser)
{
  // duplicate parameter name
  P_corba.Parametername = CORBA::string_dup(P_parser.name.c_str());
  
  // duplicate parameter type
  P_corba.Parametertype = CORBA::string_dup(P_parser.type.c_str());
}


//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create a service datastream parameter from the catalog parsing
//----------------------------------------------------------------------
void SALOME_ModuleCatalogImpl::duplicate
(SALOME_ModuleCatalog::ServicesDataStreamParameter & P_corba,
 const ParserDataStreamParameter & P_parser)
{
  std::map < std::string, 
    SALOME_ModuleCatalog::DataStreamDependency >::const_iterator it_dep;

  // duplicate parameter name
  P_corba.Parametername = CORBA::string_dup(P_parser.name.c_str());
  
  // duplicate parameter type

  // doesn't work ??? 
  //   it_type = DataStreamTypeConvert.find(P_parser.type);
  //   P_corba.Parametertype
  //     = (it_type == DataStreamTypeConvert.end()) 
  //     ? it_type->second : SALOME_ModuleCatalog::DATASTREAM_UNKNOWN;

  P_corba.Parametertype = CORBA::string_dup(P_parser.type.c_str());

  // duplicate parameter dependency
  
  if(MYDEBUG) SCRUTE(P_parser.dependency);
  P_corba.Parameterdependency = SALOME_ModuleCatalog::DATASTREAM_UNDEFINED;
  for (it_dep = DataStreamDepConvert.begin(); 
       it_dep != DataStreamDepConvert.end(); 
       it_dep++)
    if (P_parser.dependency.compare(it_dep->first) == 0) {
      P_corba.Parameterdependency = it_dep->second;
      break;
    }

  if(MYDEBUG) SCRUTE(P_corba.Parameterdependency);
}

//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create the path prefix structures from the catalog parsing
//----------------------------------------------------------------------
void
SALOME_ModuleCatalogImpl::duplicate(ParserPathPrefixes &L_out, 
				    const ParserPathPrefixes &L_in)
{
  L_out = L_in;
}


//----------------------------------------------------------------------
// Function : _verify_path_prefix
// Purpose  : verify the path prefix structures from the catalog parsing
//            Verify that there only one path prefix associated to a 
//            particular computer
//----------------------------------------------------------------------
bool
SALOME_ModuleCatalogImpl::_verify_path_prefix(ParserPathPrefixes & pathList)
{
  bool _return_value = true;
  vector<string> _machine_list;

  // Fill a list of all computers indicated in the path list
  for (unsigned int ind = 0; ind < pathList.size(); ind++)
    { 
      for (unsigned int ind1 = 0 ; ind1 < pathList[ind].listOfComputer.size(); ind1++)
	{
	  _machine_list.push_back(pathList[ind].listOfComputer[ind1]);
	}
    }

  // Parse if a computer name is twice in the list of computers
#ifndef WNT
  for (unsigned int ind = 0; ind < _machine_list.size(); ind++)
#else
  for (ind = 0; ind < _machine_list.size(); ind++)
#endif
    {
     for (unsigned int ind1 = ind+1 ; ind1 < _machine_list.size(); ind1++)
       {
	 if(_machine_list[ind].compare(_machine_list[ind1]) == 0)
	   {
	     if(MYDEBUG) MESSAGE( "The computer " << _machine_list[ind] << " is indicated more than once in the path list");
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
SALOME_ModuleCatalogImpl::_parseArguments(int argc, char **argv, 
					  char **_general, 
					  char** _personal)
{
  bool _return_value = true;
  *_general = NULL;
  *_personal = NULL ;
  for (int ind = 0; ind < argc ; ind++)
    {

      if (strcmp(argv[ind],"-help") == 0)
	{
	  INFOS( "Usage: " << argv[0] 
		 << " -common 'path to general catalog' "
		 " -personal 'path to personal catalog' "
		 " -ORBInitRef NameService=corbaname::localhost");
	    _return_value = false ;
	}

      if (strcmp(argv[ind],"-common") == 0)
	{
	  if (ind + 1 < argc)
	    {
	      // General catalog file
	      *_general = argv[ind + 1] ;
	    }
	}
      else if (strcmp(argv[ind],"-personal") == 0)
	{
	  if (ind + 1 < argc)
	    {
	      // Personal catalog file
	      *_personal = argv[ind + 1] ;
	    }
	}
    }
  return _return_value;
}
