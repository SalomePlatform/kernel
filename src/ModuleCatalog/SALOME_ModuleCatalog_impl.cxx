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

using namespace std;
#include "SALOME_ModuleCatalog_impl.hxx"
#include "SALOME_ModuleCatalog_Acomponent_impl.hxx"
#include <fstream>

#include <qstringlist.h>
#include <qfileinfo.h>

static const char* SEPARATOR    = ":";

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalogImpl
// Purpose  : Constructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalogImpl::SALOME_ModuleCatalogImpl(int argc, char** argv)
{
  MESSAGE("Catalog creation");

  // Empty used variables
  _general_module_list.resize(0);
  _general_path_list.resize(0);

  _personal_module_list.resize(0);
  _personal_path_list.resize(0);

  // Parse the arguments given at server run
  if (!_parseArguments(argc, argv,&_general_path,&_personal_path))
    MESSAGE( "Error while argument parsing" )

  // Test existency of files
  if (_general_path == NULL)
    MESSAGE( "Error the general catalog should be indicated" )
  else
    {
      //MESSAGE("Parse general catalog");
      // Affect the variables _general_module_list and _general_path_list 
      // with the common catalog
      
      QStringList dirList = QStringList::split( SEPARATOR, _general_path, false ); // skip empty entries
      for ( int i = 0; i < dirList.count(); i++ ) {
	QFileInfo fileInfo( dirList[ i ] );
	if ( fileInfo.isFile() && fileInfo.exists() ) {
	  _parse_xml_file(fileInfo.filePath(),_general_module_list, _general_path_list);
	}
      }

      // Verification of _general_path_list content
      if(!_verify_path_prefix(_general_path_list))
	MESSAGE( "Error while parsing the general path list, differents pathes are associated to one computer, the first will be choosen" )
      else MESSAGE("General path list OK");
    
      if(_personal_path != NULL)
	{
	  //MESSAGE("Parse personal catalog");
	  // Affect the variables _personal_module_list and _personal_path_list 
	  // with the personal catalog
	  _parse_xml_file(_personal_path,_personal_module_list, _personal_path_list);
	  
	  // Verification of _general_path_list content
	  if(!_verify_path_prefix(_personal_path_list))
	    MESSAGE("Error while parsing the personal path list, differents pathes are associated to one computer, the first will be choosen" )
	  else MESSAGE("Personal path list OK");
	}
      else MESSAGE("No personal catalog indicated or error while opening the personal catalog");
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
char* 
SALOME_ModuleCatalogImpl::GetPathPrefix(const char* machinename) {
  MESSAGE("Begin of GetPathPrefix")
  // Variables initialisation
  char* _path = NULL;
  bool _find = false ;

  // Parse all the path prefixes
  // looking for the wanted computer
  for (unsigned int ind = 0 ; ind < _personal_path_list.size() ; ind++)
    {
      for (unsigned int ind1 = 0 ; ind1 < _personal_path_list[ind].ListOfComputer.size() ; ind1++)    
	{
	  if (strcmp(machinename, _personal_path_list[ind].ListOfComputer[ind1].c_str()) == 0)
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
        for (unsigned int ind1 = 0 ; ind1 < _general_path_list[ind].ListOfComputer.size() ; ind1++)    
	  {
	    if (strcmp(machinename, _general_path_list[ind].ListOfComputer[ind1].c_str()) == 0)
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
       _list_components[ind]=(_personal_module_list[ind].Parsercomponentname).c_str();
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
	  if ((_general_module_list[ind].Parsercomponentname.compare(_personal_module_list[ind1].Parsercomponentname)) == 0)
	    _find = true;
	}
      if (!_find)
	{
	  MESSAGE("A new component " << _general_module_list[ind].Parsercomponentname << " has to be to added in the list");
          _list_components->length(indice+1);
	  // The component is not already defined => has to be taken
	  _list_components[indice]=(_general_module_list[ind].Parsercomponentname).c_str();   
	  SCRUTE(_list_components[indice]) ;

	  indice++;
	}
      else 
	MESSAGE("The component " <<_general_module_list[ind].Parsercomponentname << " was already defined in the personal catalog") ;
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
       _list_components_icone[ind].modulename=(_personal_module_list[ind].Parsercomponentname).c_str();
       _list_components_icone[ind].moduleusername=(_personal_module_list[ind].Parsercomponentusername).c_str();
       _list_components_icone[ind].moduleicone=(_personal_module_list[ind].Parsercomponenticone).c_str();
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
	  if ((_general_module_list[ind].Parsercomponentname.compare(_personal_module_list[ind1].Parsercomponentname)) == 0)
	    _find = true;
	}
      if (!_find)
	{
	  //	  MESSAGE("A new component " << _general_module_list[ind].Parsercomponentname << " has to be to added in the list");
          _list_components_icone->length(indice+1);
	  // The component is not already defined => has to be taken
	  _list_components_icone[indice].modulename=(_general_module_list[ind].Parsercomponentname).c_str();  
	  _list_components_icone[indice].moduleusername=(_general_module_list[ind].Parsercomponentusername).c_str();  
	  _list_components_icone[indice].moduleicone=(_general_module_list[ind].Parsercomponenticone).c_str(); 
	  //SCRUTE(_list_components_icone[indice].modulename) ;
	  //SCRUTE(_list_components_icone[indice].moduleicone);

	  indice++;
	}
      // else 
	//MESSAGE("The component " <<_general_module_list[ind].Parsercomponentname << " was already defined in the personal catalog"); 
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
      if  (_personal_module_list[ind].Parsercomponenttype == _temp_component_type)
	{
	  _list_typed_component->length(_j + 1); 
	  _list_typed_component[_j] = (_modulelist[ind].Parsercomponentname).c_str();
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

      if(_general_module_list[ind].Parsercomponenttype == _temp_component_type)
	{
	  for (unsigned int ind1=0; ind1 < _personal_module_list.size();ind1++)
	    {
	      // searching if the component is aleready defined in 
	      // the personal catalog
	      if ((_general_module_list[ind].Parsercomponentname.compare(_personal_module_list[ind1].Parsercomponentname)) == 0)
		_find = true;
	    }
	  if (!_find)
	    {
	      //MESSAGE("A new component " << _general_module_list[ind].Parsercomponentname << " has to be to added in the list");
              _list_typed_component->length(indice+1);
	      // The component is not already defined => has to be taken
	      _list_typed_component[indice]=(_general_module_list[ind].Parsercomponentname).c_str();   
	      //SCRUTE(_list_typed_component[indice]) ;

	      indice++;
	    }
	  //else 
	    //MESSAGE("The component " <<_general_module_list[ind].Parsercomponentname << " was already defined in the personal catalog") ;
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
SALOME_ModuleCatalogImpl::GetComponent(const char* componentname)
{
  SALOME_ModuleCatalog::Acomponent_ptr compo;
  SALOME_ModuleCatalog::ListOfDefInterface _list_interfaces;
  _list_interfaces.length(0);
  char* _constraint = NULL;
  char* _icone = NULL;
  char* _componentusername = NULL;
  SALOME_ModuleCatalog::ComponentType _componenttype = SALOME_ModuleCatalog::OTHER; // default initialisation
  CORBA::Boolean _componentmultistudy = false ; // default initialisation
  ListOfPathPrefix _pathes ;
  _pathes.resize(0);
  

  bool find = false ;

  // Looking for component named "componentname" in the personal catalog
  // If found, get name, interfaces and constraint
  // If not found, looking for component named "componentname" in
  // the general catalog
  // If found, get name, interfaces and constraint
  // If not found, NULL pointer is returned

  for (unsigned int ind=0; ind < _personal_module_list.size();ind++)
    {
     if (strcmp((_personal_module_list[ind].Parsercomponentname).c_str(),componentname) == 0)
        {
          //MESSAGE("Component named " << componentname << " found in the personal catalog");
	    find = true;

	  // get constraint
	  _constraint = new char[strlen(_personal_module_list[ind].Parserconstraint.c_str()) + 1];
	  _constraint = CORBA::string_dup(_personal_module_list[ind].Parserconstraint.c_str());

	  // get component type
	  switch(_personal_module_list[ind].Parsercomponenttype){
	  case GEOM:
	    _componenttype = SALOME_ModuleCatalog::GEOM;
	    break;
	  case MESH:
	    _componenttype = SALOME_ModuleCatalog::MESH;
	    break;
	  case Med:
	    _componenttype = SALOME_ModuleCatalog::Med;
	    break;
	  case SOLVER:
	    _componenttype = SALOME_ModuleCatalog::SOLVER;
	    break;
	  case DATA:
	    _componenttype = SALOME_ModuleCatalog::DATA;
	    break;
	  case VISU:
	    _componenttype = SALOME_ModuleCatalog::VISU;
	    break;
	  case SUPERV:
	    _componenttype = SALOME_ModuleCatalog::SUPERV;
	    break;
	  case OTHER:
	    _componenttype = SALOME_ModuleCatalog::OTHER;
	    break;
	  }
	  
	  // get component multistudy
	  _componentmultistudy = _personal_module_list[ind].Parsercomponentmultistudy ;

	  // get component icone
	  _icone = CORBA::string_dup(_personal_module_list[ind].Parsercomponenticone.c_str());

	  // get component user name
	  _componentusername = CORBA::string_dup(_personal_module_list[ind].Parsercomponentusername.c_str());

	  // get component interfaces
	  _list_interfaces = duplicate_interfaces(_personal_module_list[ind].ParserListInterface);

	  // get pathes prefix
	  _pathes = duplicate_pathes(_personal_path_list);

	}
    }
  
  if (find)
    {
      SALOME_ModuleCatalog_AcomponentImpl* aComponentImpl = 
	new SALOME_ModuleCatalog_AcomponentImpl(componentname,
						_componentusername,
						_constraint,
						_componenttype,
						_componentmultistudy,
						_icone,
						_list_interfaces,
						_pathes);
      
      compo = aComponentImpl->_this();
    }
  else
    // Not found in the personal catalog => searching in the general catalog
    {
      for (unsigned int ind=0; ind < _general_module_list.size();ind++)
	{
	  if (strcmp((_general_module_list[ind].Parsercomponentname).c_str(),componentname) == 0)
	    {
	      //MESSAGE("Component named " << componentname << " found in the general catalog");
	      find = true;

	      // get constraint
	      _constraint = new char[strlen(_general_module_list[ind].Parserconstraint.c_str()) + 1];
	      _constraint = CORBA::string_dup(_general_module_list[ind].Parserconstraint.c_str());


	      // get component type
	      switch(_general_module_list[ind].Parsercomponenttype){
	      case GEOM:
		_componenttype = SALOME_ModuleCatalog::GEOM;
		break;
	      case MESH:
		_componenttype = SALOME_ModuleCatalog::MESH;
		break;
	      case Med:
		_componenttype = SALOME_ModuleCatalog::Med;
		break;
	      case SOLVER:
		_componenttype = SALOME_ModuleCatalog::SOLVER;
		break;
	      case DATA:
		_componenttype = SALOME_ModuleCatalog::DATA;
		break;
	      case VISU:
		_componenttype = SALOME_ModuleCatalog::VISU;
		break;
	      case SUPERV:
		_componenttype = SALOME_ModuleCatalog::SUPERV;
		break;
	      case OTHER:
		_componenttype = SALOME_ModuleCatalog::OTHER;
		break;
	      }


	      // get component multistudy
	      _componentmultistudy = _general_module_list[ind].Parsercomponentmultistudy ;

	      // get component icone
	      _icone = CORBA::string_dup(_general_module_list[ind].Parsercomponenticone.c_str());

	      // get component user name
	      _componentusername = CORBA::string_dup(_general_module_list[ind].Parsercomponentusername.c_str());

	      // get component interfaces
	      _list_interfaces = duplicate_interfaces(_general_module_list[ind].ParserListInterface);

	      // get pathes prefix
	      _pathes = duplicate_pathes(_general_path_list);
	    }
	}
  
      if (find)
	{
	  SALOME_ModuleCatalog_AcomponentImpl* aComponentImpl = 
	    new SALOME_ModuleCatalog_AcomponentImpl(componentname,
						    _componentusername,
						    _constraint,
						    _componenttype,
						    _componentmultistudy,
						    _icone,
						    _list_interfaces,
						    _pathes);
      
	  compo = aComponentImpl->_this();
	}
      else
	// Not found in the personal catalog and in the general catalog
	// return NULL object
	{
	  MESSAGE("Component with name  " << componentname << " not found in catalog");
          compo = NULL;
	}
    }
  return compo;
}

//----------------------------------------------------------------------
// Function : _parse_xml_file
// Purpose  : parse one module catalog 
//----------------------------------------------------------------------
void 
SALOME_ModuleCatalogImpl::_parse_xml_file(const char* file, 
					  ListOfParserComponent& modulelist, 
					  ListOfParserPathPrefix& pathlist)
{
  SALOME_ModuleCatalog_Handler* handler = new SALOME_ModuleCatalog_Handler();
  QFile xmlFile(file);

  QXmlInputSource source(xmlFile);

  QXmlSimpleReader reader;
  reader.setContentHandler( handler );
  reader.setErrorHandler( handler );
  reader.parse( source );
  xmlFile.close();
  unsigned int ind;
  for ( ind = 0; ind < _modulelist.size(); ind++)
    modulelist.push_back(_modulelist[ind]) ;
  for ( ind = 0; ind < _pathlist.size(); ind++)
    pathlist.push_back(_pathlist[ind]) ;
}

//----------------------------------------------------------------------
// Function : duplicate_interfaces
// Purpose  : create a list of interfaces from the parsing of the catalog
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfDefInterface
SALOME_ModuleCatalogImpl::duplicate_interfaces(ListOfDefinitionInterface list_interface)
{
  SALOME_ModuleCatalog::ListOfDefInterface _list_interfaces;
  unsigned int _length_interfaces = list_interface.size();
  _list_interfaces.length(_length_interfaces);
  
  for (unsigned int ind = 0; ind < _length_interfaces; ind++)
    {
      //duplicate interface name
      _list_interfaces[ind].interfacename = CORBA::string_dup(list_interface[ind].Parserinterfacename.c_str());

      // duplicate service list
      unsigned int _length_services = list_interface[ind].Parserinterfaceservicelist.size();
      _list_interfaces[ind].interfaceservicelist.length(_length_services);

      for (unsigned int ind1 = 0; ind1 < _length_services ; ind1 ++)
	{
	  // duplicate service name
	  _list_interfaces[ind].interfaceservicelist[ind1].ServiceName =
	    CORBA::string_dup(list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceName.c_str());

	  // duplicate service by default
	  _list_interfaces[ind].interfaceservicelist[ind1].Servicebydefault =
	    list_interface[ind].Parserinterfaceservicelist[ind1].ParserServicebydefault;

	  // duplicate in Parameters
	  unsigned int _length_in_param = list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceinParameter.size();
	  _list_interfaces[ind].interfaceservicelist[ind1].ServiceinParameter.length(_length_in_param);
	  for (unsigned int ind2 = 0; ind2 < _length_in_param ; ind2 ++)
	    {
	      // duplicate parameter type
	      _list_interfaces[ind].interfaceservicelist[ind1].ServiceinParameter[ind2].Parametertype = CORBA::string_dup(list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceinParameter[ind2].ParserParamtype.c_str());
	      
	      // duplicate parameter name
	      _list_interfaces[ind].interfaceservicelist[ind1].ServiceinParameter[ind2].Parametername = CORBA::string_dup(list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceinParameter[ind2].ParserParamname.c_str());
	    }

	  // duplicate out Parameters
	  unsigned int _length_out_param = list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceoutParameter.size();
	  _list_interfaces[ind].interfaceservicelist[ind1].ServiceoutParameter.length(_length_out_param);
	  for (unsigned int ind2 = 0; ind2 < _length_out_param ; ind2 ++)
	    {
	      // duplicate parameter type
	      _list_interfaces[ind].interfaceservicelist[ind1].ServiceoutParameter[ind2].Parametertype = CORBA::string_dup(list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceoutParameter[ind2].ParserParamtype.c_str());
	      
	      // duplicate parameter name
	      _list_interfaces[ind].interfaceservicelist[ind1].ServiceoutParameter[ind2].Parametername = CORBA::string_dup(list_interface[ind].Parserinterfaceservicelist[ind1].ParserServiceoutParameter[ind2].ParserParamname.c_str());
	    }
      
	}
    }
  return _list_interfaces;
}


//----------------------------------------------------------------------
// Function : duplicate_pathes
// Purpose  : create the path prefix structures from the catalog parsing
//----------------------------------------------------------------------
ListOfPathPrefix 
SALOME_ModuleCatalogImpl::duplicate_pathes(ListOfParserPathPrefix pathes)
{
  ListOfPathPrefix _pathes ;

  unsigned int _length = pathes.size() ;
  _pathes.resize(_length);
  unsigned int _length_comput = 0;
  
  for (unsigned int ind = 0; ind < _length ; ind++)
    {
      // duplicate path
      _pathes[ind].path = CORBA::string_dup(pathes[ind].path.c_str()) ;
      //MESSAGE("Prefix : " << _pathes[ind].path);

      _length_comput = pathes[ind].ListOfComputer.size() ;
      _pathes[ind].ListOfComputer.resize(_length_comput);
      for (unsigned int ind1 = 0; ind1 <_length_comput  ; ind1++)
	{
	  // duplicate computer name
	  _pathes[ind].ListOfComputer[ind1] = CORBA::string_dup(pathes[ind].ListOfComputer[ind1].c_str());
	  //MESSAGE("Computer associated to the prefix : " <<_pathes[ind].ListOfComputer[ind1]);  
	}
    } 
  return _pathes ;
}


//----------------------------------------------------------------------
// Function : _verify_path_prefix
// Purpose  : verify the path prefix structures from the catalog parsing
//            Verify that there only one path prefix associated to a 
//            particular computer
//----------------------------------------------------------------------
bool
SALOME_ModuleCatalogImpl::_verify_path_prefix(ListOfParserPathPrefix pathlist)
{
  bool _return_value = true;
  vector<string> _machine_list;
  _machine_list.resize(0);

  // Fill a list of all computers indicated in the path list
  for (unsigned int ind = 0; ind < pathlist.size(); ind++)
    { 
      for (unsigned int ind1 = 0 ; ind1 < pathlist[ind].ListOfComputer.size(); ind1++)
	{
	  _machine_list.push_back(pathlist[ind].ListOfComputer[ind1]);
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
	  INFOS( "Usage: " << argv[0] << " -common 'path to general catalog' -personal 'path to personal catalog' -ORBInitRef NameService=corbaname::localhost");
	    _return_value = false ;
	}
      if (strcmp(argv[ind],"-common") == 0)
	{
	  if (ind + 1 < argc)
	    {
	      // General catalog file
	      *_general = argv[ind + 1] ;
/*	      ifstream _general_file(*_general);
	      if (!_general_file)
		{
		  MESSAGE( "Sorry the file " << *_general << " can't be open" )
		  *_general = NULL;
		  _return_value = false;
		}
*/
	    }
	}
      else if (strcmp(argv[ind],"-personal") == 0)
	{
	  if (ind + 1 < argc)
	    {
	      // Personal catalog file
	      *_personal = argv[ind + 1] ;
/*	      ifstream _personal_file(*_personal);
	      if (!_personal_file)
		{
		  MESSAGE("Sorry the file " << *_personal << " can't be open" )
		  *_personal = NULL;
		  _return_value = false;
		}
*/
	    }
	}
    }
  return _return_value;
}


