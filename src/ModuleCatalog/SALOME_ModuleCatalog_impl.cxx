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
//  File   : SALOME_ModuleCatalog_impl.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOME_ModuleCatalog_impl.hxx"
#include "SALOME_ModuleCatalog_Acomponent_impl.hxx"
#include <fstream>
#include <map>

#include <qstringlist.h>
#include <qfileinfo.h>
using namespace std;

static const char* SEPARATOR    = ":";

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalogImpl
// Purpose  : Constructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalogImpl::SALOME_ModuleCatalogImpl(int argc, char** argv, CORBA::ORB_ptr orb) : _orb(orb)
{
  MESSAGE("Catalog creation");

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

  // Conversion rules for datastream parameters type and dependency
  DataStreamTypeConvert["UNKNOWN"] 
    = SALOME_ModuleCatalog::DATASTREAM_UNKNOWN;
  DataStreamTypeConvert["INTEGER"] 
    = SALOME_ModuleCatalog::DATASTREAM_INTEGER;
  DataStreamTypeConvert["FLOAT"]   
    = SALOME_ModuleCatalog::DATASTREAM_FLOAT;
  DataStreamTypeConvert["DOUBLE"]  
    = SALOME_ModuleCatalog::DATASTREAM_DOUBLE;
  DataStreamTypeConvert["STRING"]  
    = SALOME_ModuleCatalog::DATASTREAM_STRING;
  DataStreamTypeConvert["BOOLEAN"] 
    = SALOME_ModuleCatalog::DATASTREAM_BOOLEAN;

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
    MESSAGE( "Error while argument parsing" );

  // Test existency of files
  if (_general_path == NULL)
    MESSAGE( "Error the general catalog should be indicated" )
  else
    {
      // Affect the _general_module_list and _general_path_list members
      // with the common catalog
      
      QStringList dirList 
	= QStringList::split( SEPARATOR, _general_path, 
			      false ); // skip empty entries

      for ( int i = 0; i < dirList.count(); i++ ) {
	QFileInfo fileInfo( dirList[ i ] );
	if ( fileInfo.isFile() && fileInfo.exists() ) {
	  _parse_xml_file(fileInfo.filePath(), 
			  _general_module_list, 
			  _general_path_list);
	}
      }

      // Verification of _general_path_list content
      if(!_verify_path_prefix(_general_path_list))
	MESSAGE( "Error while parsing the general path list, "
		 "differents paths are associated to the same computer," 
		 "the first one will be choosen")
      else 
	MESSAGE("General path list OK");
    
      if(_personal_path != NULL)
	{
	  // Initialize the _personal_module_list and 
	  // _personal_path_list members with the personal catalog files
	  _parse_xml_file(_personal_path,
			  _personal_module_list, 
			  _personal_path_list);
	  
	  // Verification of _general_path_list content
	  if(!_verify_path_prefix(_personal_path_list))
	    MESSAGE("Error while parsing the personal path list, "
		    "differents paths are associated to the same computer, "
		    "the first one will be choosen" )
	  else 
	    MESSAGE("Personal path list OK");
	}
      else 
	MESSAGE("No personal catalog indicated or error while "
		"opening the personal catalog");
    }
}

//----------------------------------------------------------------------
// Function : ~SALOME_ModuleCatalogImpl
// Purpose  : Destructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalogImpl::~SALOME_ModuleCatalogImpl()
{
  MESSAGE("Catalog Destruction");
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
  MESSAGE("Begin of GetPathPrefix")
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
  MESSAGE("Begin of GetComponentList");
  SALOME_ModuleCatalog::ListOfComponents_var _list_components = 
    new SALOME_ModuleCatalog::ListOfComponents;

  _list_components->length(_personal_module_list.size());

  // All the components defined in the personal catalog are taken
  for (unsigned int ind=0; ind < _personal_module_list.size();ind++)
    {
       _list_components[ind]=(_personal_module_list[ind].name).c_str();
       SCRUTE(_list_components[ind]) ;
    }

  int indice = _personal_module_list.size() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
  for (unsigned int ind=0; ind < _general_module_list.size();ind++)
    {
      _find = false;
      for (unsigned int ind1=0; ind1 < _personal_module_list.size();ind1++)
	{
	  // searching if the component is already defined in 
	  // the personal catalog
	  if ((_general_module_list[ind].name.compare(_personal_module_list[ind1].name)) == 0)
	    _find = true;
	}
      if (!_find)
	{
	  MESSAGE("A new component " << _general_module_list[ind].name 
		  << " has to be to added in the list");
          _list_components->length(indice+1);
	  // The component is not already defined => has to be taken
	  _list_components[indice]=(_general_module_list[ind].name).c_str();   
	  SCRUTE(_list_components[indice]) ;

	  indice++;
	}
      else 
	MESSAGE("The component " <<_general_module_list[ind].name 
		<< " was already defined in the personal catalog") ;
     }

  MESSAGE ( "End of GetComponentList" )
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
  MESSAGE("Begin of GetComponentIconeList");

  SALOME_ModuleCatalog::ListOfIAPP_Affich_var _list_components_icone = 
    new SALOME_ModuleCatalog::ListOfIAPP_Affich;

  _list_components_icone->length(_personal_module_list.size());

  // All the components defined in the personal catalog are taken
  for (unsigned int ind=0; ind < _personal_module_list.size();ind++)
    {
       _list_components_icone[ind].modulename=(_personal_module_list[ind].name).c_str();
       _list_components_icone[ind].moduleusername=(_personal_module_list[ind].username).c_str();
       _list_components_icone[ind].moduleicone=(_personal_module_list[ind].icon).c_str();
       //SCRUTE(_list_components_icone[ind].modulename); 
       //SCRUTE(_list_components_icone[ind].moduleicone);
    }

  int indice = _personal_module_list.size() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
  for (unsigned int ind=0; ind < _general_module_list.size();ind++)
    {
      _find = false;
      for (unsigned int ind1=0; ind1 < _personal_module_list.size();ind1++)
	{
	  // searching if the component is aleready defined in 
	  // the personal catalog
	  if ((_general_module_list[ind].name.compare(_personal_module_list[ind1].name)) == 0)
	    _find = true;
	}
      if (!_find)
	{
	  //	  MESSAGE("A new component " << _general_module_list[ind].name << " has to be to added in the list");
          _list_components_icone->length(indice+1);
	  // The component is not already defined => has to be taken
	  _list_components_icone[indice].modulename=_general_module_list[ind].name.c_str();  
	  _list_components_icone[indice].moduleusername=_general_module_list[ind].username.c_str();  
	  _list_components_icone[indice].moduleicone=_general_module_list[ind].icon.c_str(); 
	  //SCRUTE(_list_components_icone[indice].modulename) ;
	  //SCRUTE(_list_components_icone[indice].moduleicone);

	  indice++;
	}
      // else 
	//MESSAGE("The component " <<_general_module_list[ind].name << " was already defined in the personal catalog"); 
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
  MESSAGE("Begin of GetTypedComponentList");
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
	  _list_typed_component[_j] = (_moduleList[ind].name).c_str();
	  //SCRUTE(_list_typed_component[_j])
	  _j++;
	}
    }

  int indice = _list_typed_component->length() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
  for (unsigned int ind=0; ind < _general_module_list.size();ind++)
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
	      //MESSAGE("A new component " << _general_module_list[ind].name << " has to be to added in the list");
              _list_typed_component->length(indice+1);
	      // The component is not already defined => has to be taken
	      _list_typed_component[indice]=(_general_module_list[ind].name).c_str();   
	      //SCRUTE(_list_typed_component[indice]) ;

	      indice++;
	    }
	  //else 
	    //MESSAGE("The component " <<_general_module_list[ind].name << " was already defined in the personal catalog") ;
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
  ParserPathPrefixes *pp = NULL;

  SALOME_ModuleCatalog::Acomponent_ptr compo = NULL;
  
  C_parser = findComponent(s);
  if (C_parser) {
    
//    DebugParserComponent(*C_parser);

    SALOME_ModuleCatalog::Component C_corba;
    duplicate(C_corba, *C_parser);

    
    SALOME_ModuleCatalog_AcomponentImpl * aComponentImpl = 
      new SALOME_ModuleCatalog_AcomponentImpl(C_corba);
    
    compo = aComponentImpl->_this();
  }
  else {
    // Not found in the personal catalog and in the general catalog
    // return NULL object
    MESSAGE("Component with name  " << name 
	    << " not found in catalog");
    compo = NULL;
  }
  
  return compo;
}

SALOME_ModuleCatalog::Component *
SALOME_ModuleCatalogImpl::GetComponentInfo(const char *name)
{
  std::string s(name);

  ParserComponent * C_parser = findComponent(s);
  
  if (C_parser) {
    
    SALOME_ModuleCatalog::Component * C_corba 
      = new SALOME_ModuleCatalog::Component; 
    duplicate(*C_corba, *C_parser);
    return C_corba;
  }

  return NULL;
}

ParserComponent *
SALOME_ModuleCatalogImpl::findComponent(const string & name)
{
  ParserComponent * C_parser = NULL;

  if (!C_parser)
    for (unsigned int ind=0; ind < _personal_module_list.size();ind++)
      if (name.compare(_personal_module_list[ind].name) == 0)
        {
          MESSAGE("Component named " << name 
		  << " found in the personal catalog");
	  C_parser = &(_personal_module_list[ind]);
	  break;
	}

  if (!C_parser)
    for (unsigned int ind=0; ind < _general_module_list.size();ind++)
      {
	if (name.compare(_general_module_list[ind].name) == 0)
	  {
	    MESSAGE("Component named " << name 
		    << " found in the general catalog");
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
					  ParserPathPrefixes& pathList)
{
  BEGIN_OF("_parse_xml_file");
  SCRUTE(file);

  SALOME_ModuleCatalog_Handler* handler = new SALOME_ModuleCatalog_Handler();
  QFile xmlFile(file);

  QXmlInputSource source(xmlFile);

  QXmlSimpleReader reader;
  reader.setContentHandler( handler );
  reader.setErrorHandler( handler );
  reader.parse( source );
  xmlFile.close();

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
  _parse_xml_file(file, _personal_module_list, _personal_path_list);
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
(SALOME_ModuleCatalog::Component & C_corba, 
 const ParserComponent & C_parser)
{
  C_corba.name = CORBA::string_dup(C_parser.name.c_str());
  C_corba.username = CORBA::string_dup(C_parser.username.c_str());
  C_corba.multistudy = C_parser.multistudy;
  C_corba.icon = CORBA::string_dup(C_parser.icon.c_str());
  C_corba.type = ComponentTypeConvert[C_parser.type];
  C_corba.implementationType = C_parser.implementationType;

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
  SCRUTE(_length);
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
  
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_corba.ServiceoutParameter[ind2],
	      S_parser.outParameters[ind2]);
  
  // duplicate in DataStreamParameters
  _length = S_parser.inDataStreamParameters.size();
  S_corba.ServiceinDataStreamParameter.length(_length);
  
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_corba.ServiceinDataStreamParameter[ind2],
	      S_parser.inDataStreamParameters[ind2]);
  
  // duplicate out DataStreamParameters
  _length = S_parser.outDataStreamParameters.size();
  SCRUTE(_length);
  S_corba.ServiceoutDataStreamParameter.length(_length);
  
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
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
    SALOME_ModuleCatalog::DataStreamType >::const_iterator it_type;

  std::map < std::string, 
    SALOME_ModuleCatalog::DataStreamDependency >::const_iterator it_dep;

  // duplicate parameter name
  P_corba.Parametername = CORBA::string_dup(P_parser.name.c_str());
  
  // doesn't work ??? 
  //   it_type = DataStreamTypeConvert.find(P_parser.type);
  //   P_corba.Parametertype
  //     = (it_type == DataStreamTypeConvert.end()) 
  //     ? it_type->second : SALOME_ModuleCatalog::DATASTREAM_UNKNOWN;

  SCRUTE(P_parser.type);
  P_corba.Parametertype = SALOME_ModuleCatalog::DATASTREAM_UNKNOWN;
  for (it_type = DataStreamTypeConvert.begin(); 
       it_type != DataStreamTypeConvert.end(); 
       it_type++)
    if (P_parser.type.compare(it_type->first) == 0) {
      P_corba.Parametertype = it_type->second;
      break;
    }
  SCRUTE(P_corba.Parametertype);

  // duplicate parameter type

  // doesn't work ??? 
  //   it_type = DataStreamTypeConvert.find(P_parser.type);
  //   P_corba.Parametertype
  //     = (it_type == DataStreamTypeConvert.end()) 
  //     ? it_type->second : SALOME_ModuleCatalog::DATASTREAM_UNKNOWN;
  
  SCRUTE(P_parser.dependency);
  P_corba.Parameterdependency = SALOME_ModuleCatalog::DATASTREAM_UNDEFINED;
  for (it_dep = DataStreamDepConvert.begin(); 
       it_dep != DataStreamDepConvert.end(); 
       it_dep++)
    if (P_parser.dependency.compare(it_dep->first) == 0) {
      P_corba.Parameterdependency = it_dep->second;
      break;
    }

  SCRUTE(P_corba.Parameterdependency);
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
  for (unsigned int ind = 0; ind < _machine_list.size(); ind++)
    {
     for (unsigned int ind1 = ind+1 ; ind1 < _machine_list.size(); ind1++)
       {
	 if(_machine_list[ind].compare(_machine_list[ind1]) == 0)
	   {
	     MESSAGE( "The computer " << _machine_list[ind] << " is indicated more than once in the path list")
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


