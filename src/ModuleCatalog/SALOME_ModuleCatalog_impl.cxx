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
//  File   : SALOME_ModuleCatalog_impl.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOME_ModuleCatalog_impl.hxx"
#include "SALOME_ModuleCatalog_Acomponent_impl.hxx"
#include "SALOME_ModuleCatalog_Handler.hxx"
#include <libxml/parser.h>
#include <fstream>
#include <map>
#include "utilities.h"

#ifdef WIN32
# include <process.h>
#else
# include <unistd.h>
#endif

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

static const char* SEPARATOR     = "::";
static const char* OLD_SEPARATOR = ":";


std::list<std::string> splitStringToList(const std::string& theString, const std::string& theSeparator)
{
  std::list<std::string> aList;

  int sepLen = theSeparator.length();
  int startPos = 0, sepPos = theString.find(theSeparator, startPos);

  while (1)
    {
      std::string anItem ;
      if(sepPos != std::string::npos)
        anItem = theString.substr(startPos, sepPos - startPos);
      else
        anItem = theString.substr(startPos);
      if (anItem.length() > 0)
        aList.push_back(anItem);
      if(sepPos == std::string::npos)
        break;
      startPos = sepPos + sepLen;
      sepPos = theString.find(theSeparator, startPos);
    }

  return aList;
}

class SALOME_ModuleCatalogImpl::Private
{
  friend class SALOME_ModuleCatalogImpl;
public:
  //! method to parse one module catalog
  /*! 
    \param file const char* arguments
    \param modulelist ParserComponents arguments
    \param pathlist ParserPathPrefixes arguments
    \param typeMap ParserTypes arguments
  */
  virtual void _parse_xml_file(const char* file, 
			       ParserComponents & modulelist, 
			       ParserPathPrefixes & pathlist,
			       ParserTypes& typeMap,
			       TypeList& typeList);

  //! method to find component in the parser list
  /*!
    \param name  string argument
    \return pointer on a component, NULL if not found
  */
  ParserComponent *findComponent(const std::string & name);

  //! method to create a CORBA component description from parser
  /*!
    \param C_corba  Component argument
    \param C_parser const ParserComponent argument
  */
  void duplicate(SALOME_ModuleCatalog::ComponentDef & C_corba,
                 const ParserComponent & C_parser);
    
  //! method to create a CORBA interface description from parser
  /*!
    \param I_corba  DefinitionInterface argument
    \param I_parser const ParserInterface argument
  */
  void duplicate(SALOME_ModuleCatalog::DefinitionInterface & I_corba,
                 const ParserInterface & I_parser);
  
  //! method to create a CORBA service description from parser
  /*!
    \param S_corba  Service argument
    \param S_parser const ParserService argument
  */
  void duplicate(SALOME_ModuleCatalog::Service & S_corba,
                 const ParserService & service);
  
  //! method to create a CORBA parameter description from parser
  /*!
    \param P_corba  ServicesParameter argument
    \param P_parser const ParserParameter argument
  */
  void duplicate(SALOME_ModuleCatalog::ServicesParameter & P_corba,
                 const ParserParameter & P_parser);
  
  //! method to create a CORBA datastream parameter description from parser
  /*!
    \param P_corba  ServicesDataStreamParameter argument
    \param P_parser const ParserDataStreamParameter argument
  */
  void duplicate(SALOME_ModuleCatalog::ServicesDataStreamParameter & P_corba,
                 const ParserDataStreamParameter & P_parser);
  
  //! method to create the path prefix structures from the catalog parsing
  /*!
    \param pathes ParserPathPrefixes arguments
    \return the pathes
  */
  void duplicate(ParserPathPrefixes & p_out, const ParserPathPrefixes & P_in);

 //! method to verify path prefix content
  /*!
    \param pathlist ListOfParserPathPrefix arguments
    \return true if verfication is OK
  */
  virtual bool _verify_path_prefix(ParserPathPrefixes & pathlist);

  // Theses variables will contain the path to the general and personal catalogs
  char* _general_path;
  char* _personal_path;

  // These variables will contain the informations on the general common catalog
  ParserComponents    _general_module_list ;
  ParserPathPrefixes  _general_path_list ;
  ParserTypes _typeMap;
  TypeList _typeList;

  // These variables will contain the informations on the personal catalog
  ParserComponents    _personal_module_list ;
  ParserPathPrefixes  _personal_path_list ; 

  std::map <std::string, SALOME_ModuleCatalog::DataStreamDependency> 
  DataStreamDepConvert;

  std::map <ParserComponentType, SALOME_ModuleCatalog::ComponentType> 
  ComponentTypeConvert;
};

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalogImpl
// Purpose  : Constructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalogImpl::SALOME_ModuleCatalogImpl(int argc, char** argv, CORBA::ORB_ptr orb) : _orb(orb)
{
  myPrivate = new Private;
  if(MYDEBUG) MESSAGE("Catalog creation");
  /* Init libxml */
  xmlInitParser();

  // Conversion rules for component types
  myPrivate->ComponentTypeConvert[GEOM]
    = SALOME_ModuleCatalog::GEOM;
  myPrivate->ComponentTypeConvert[MESH]
    = SALOME_ModuleCatalog::MESH;
  myPrivate->ComponentTypeConvert[Med]
    = SALOME_ModuleCatalog::Med;
  myPrivate->ComponentTypeConvert[SOLVER]
    = SALOME_ModuleCatalog::SOLVER;
  myPrivate->ComponentTypeConvert[DATA]
    = SALOME_ModuleCatalog::DATA;
  myPrivate->ComponentTypeConvert[VISU]
    = SALOME_ModuleCatalog::VISU;
  myPrivate->ComponentTypeConvert[SUPERV]
    = SALOME_ModuleCatalog::SUPERV;
  myPrivate->ComponentTypeConvert[OTHER]
    = SALOME_ModuleCatalog::OTHER;

  // Conversion rules for datastream parameters dependency
  myPrivate->DataStreamDepConvert["UNDEFINED"] 
    = SALOME_ModuleCatalog::DATASTREAM_UNDEFINED;
  myPrivate->DataStreamDepConvert["T"]
    = SALOME_ModuleCatalog::DATASTREAM_TEMPORAL;
  myPrivate->DataStreamDepConvert["I"] 
    = SALOME_ModuleCatalog::DATASTREAM_ITERATIVE;

  // Empty used variables
  myPrivate->_general_module_list.resize(0);
  myPrivate->_general_path_list.resize(0);

  myPrivate->_personal_module_list.resize(0);
  myPrivate->_personal_path_list.resize(0);
  
  // Parse the arguments given at server run
  if (!_parseArguments(argc, argv,&myPrivate->_general_path,&myPrivate->_personal_path))
    if(MYDEBUG) MESSAGE( "Error while argument parsing" );

  // Test existency of files
  if (myPrivate->_general_path == NULL)
  {
    if(MYDEBUG) MESSAGE( "Error the general catalog should be indicated" );
  }
  else
  {
    // Affect the _general_module_list and _general_path_list members
    // with the common catalog

    std::list<std::string> dirList;

#ifdef WIN32
    dirList = splitStringToList(myPrivate->_general_path, SEPARATOR);
#else
    //check for new format
    bool isNew = (std::string( myPrivate->_general_path ).find(SEPARATOR) != std::string::npos);
    if ( isNew ) {
      //using new format
      dirList = splitStringToList(myPrivate->_general_path, SEPARATOR);
    } else {
      //support old format
      dirList = splitStringToList(myPrivate->_general_path, OLD_SEPARATOR);
    }
#endif

    for (std::list<std::string>::iterator iter = dirList.begin(); iter != dirList.end(); iter++)
    {
      std::string aPath = (*iter);
      //remove inverted commas from filename
      while (aPath.find('\"') != std::string::npos)
        aPath.erase(aPath.find('\"'), 1);

      myPrivate->_parse_xml_file(aPath.c_str(), 
				 myPrivate->_general_module_list, 
				 myPrivate->_general_path_list,
				 myPrivate->_typeMap,
				 myPrivate->_typeList);
    }

    // Verification of _general_path_list content
    if (!myPrivate->_verify_path_prefix(myPrivate->_general_path_list)) {
      if(MYDEBUG) MESSAGE( "Error while parsing the general path list, "
                           "differents paths are associated to the same computer," 
                           "the first one will be choosen");
    } else {
      if(MYDEBUG) MESSAGE("General path list OK");
    }

    if (myPrivate->_personal_path != NULL) {
      // Initialize the _personal_module_list and 
      // _personal_path_list members with the personal catalog files
      myPrivate->_parse_xml_file(myPrivate->_personal_path,
				 myPrivate->_personal_module_list, 
				 myPrivate->_personal_path_list,
				 myPrivate->_typeMap,
				 myPrivate->_typeList);
      
      // Verification of _general_path_list content
      if(!myPrivate->_verify_path_prefix(myPrivate->_personal_path_list)){
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
  delete myPrivate;
}


//! Get the list of all types of the catalog
/*!
 *   \return  the list of types
 */
SALOME_ModuleCatalog::ListOfTypeDefinition* SALOME_ModuleCatalogImpl::GetTypes()
{
  SALOME_ModuleCatalog::ListOfTypeDefinition_var type_list = new SALOME_ModuleCatalog::ListOfTypeDefinition();
  type_list->length(myPrivate->_typeList.size());

  for (unsigned int ind = 0 ; ind < myPrivate->_typeList.size() ; ind++)
    {
      //no real need to call string_dup, omniorb calls it on operator= (const char *) but it is safer
      type_list[ind].name=CORBA::string_dup(myPrivate->_typeList[ind].name.c_str());
      type_list[ind].kind=SALOME_ModuleCatalog::NONE;
      if(myPrivate->_typeList[ind].kind=="double")
        type_list[ind].kind=SALOME_ModuleCatalog::Dble;
      else if(myPrivate->_typeList[ind].kind=="int")
        type_list[ind].kind=SALOME_ModuleCatalog::Int;
      else if(myPrivate->_typeList[ind].kind=="bool")
        type_list[ind].kind=SALOME_ModuleCatalog::Bool;
      else if(myPrivate->_typeList[ind].kind=="string")
        type_list[ind].kind=SALOME_ModuleCatalog::Str;
      else if(myPrivate->_typeList[ind].kind=="objref")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Objref;
          type_list[ind].id=CORBA::string_dup(myPrivate->_typeList[ind].id.c_str());
          //bases
          type_list[ind].bases.length(myPrivate->_typeList[ind].bases.size());
          std::vector<std::string>::const_iterator miter;
          miter=myPrivate->_typeList[ind].bases.begin();
          int n_memb=0;
          while(miter != myPrivate->_typeList[ind].bases.end())
            {
              type_list[ind].bases[n_memb]=CORBA::string_dup(miter->c_str());
              miter++;
              n_memb++;
            }
        }
      else if(myPrivate->_typeList[ind].kind=="sequence")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Seq;
          type_list[ind].content=CORBA::string_dup(myPrivate->_typeList[ind].content.c_str());
        }
      else if(myPrivate->_typeList[ind].kind=="array")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Array;
          type_list[ind].content=CORBA::string_dup(myPrivate->_typeList[ind].content.c_str());
        }
      else if(myPrivate->_typeList[ind].kind=="struct")
        {
          type_list[ind].kind=SALOME_ModuleCatalog::Struc;
          //members
          type_list[ind].members.length(myPrivate->_typeList[ind].members.size());

          std::vector< std::pair<std::string,std::string> >::const_iterator miter;
          miter=myPrivate->_typeList[ind].members.begin();
          int n_memb=0;
          while(miter != myPrivate->_typeList[ind].members.end())
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
  for (unsigned int ind = 0 ; ind < myPrivate->_personal_path_list.size() ; ind++)
    {
      for (unsigned int ind1 = 0 ; ind1 < myPrivate->_personal_path_list[ind].listOfComputer.size() ; ind1++)    
        {
          if (strcmp(machinename, myPrivate->_personal_path_list[ind].listOfComputer[ind1].c_str()) == 0)
            {
              _find = true ;
              // Wanted computer
              // affect the path to be returned
                const char* _temp = myPrivate->_personal_path_list[ind].path.c_str() ;
                  _path = new char[strlen(_temp)+1];
              strcpy(_path,_temp);
            }
        }
    }

  if (!_find)
    {
    for (unsigned int ind = 0 ; ind < myPrivate->_general_path_list.size() ; ind++)
      {
        for (unsigned int ind1 = 0 ; ind1 < myPrivate->_general_path_list[ind].listOfComputer.size() ; ind1++)    
          {
            if (strcmp(machinename, myPrivate->_general_path_list[ind].listOfComputer[ind1].c_str()) == 0)
              {
                _find = true ;
                // Wanted computer
                // affect the path to be returned
                  const char* _temp = myPrivate->_general_path_list[ind].path.c_str() ;
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

  _list_components->length(myPrivate->_personal_module_list.size());

  // All the components defined in the personal catalog are taken
  for(unsigned int ind=0; ind < myPrivate->_personal_module_list.size();ind++){
    _list_components[ind]=(myPrivate->_personal_module_list[ind].name).c_str();
    if(MYDEBUG) SCRUTE(_list_components[ind]) ;
  }

  int indice = myPrivate->_personal_module_list.size() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
  for(unsigned int ind=0; ind < myPrivate->_general_module_list.size();ind++){
    _find = false;
    for(unsigned int ind1=0; ind1 < myPrivate->_personal_module_list.size();ind1++){
      // searching if the component is already defined in 
      // the personal catalog
      if ((myPrivate->_general_module_list[ind].name.compare(myPrivate->_personal_module_list[ind1].name)) == 0)
        _find = true;
    }
    if(!_find){
      if(MYDEBUG) MESSAGE("A new component " << myPrivate->_general_module_list[ind].name 
                          << " has to be to added in the list");
      _list_components->length(indice+1);
      // The component is not already defined => has to be taken
      _list_components[indice]=(myPrivate->_general_module_list[ind].name).c_str();   
      if(MYDEBUG) SCRUTE(_list_components[indice]) ;
      
      indice++;
    }else{
      if(MYDEBUG) MESSAGE("The component " <<myPrivate->_general_module_list[ind].name 
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

  _list_components_icone->length(myPrivate->_personal_module_list.size());

  // All the components defined in the personal catalog are taken
  for(unsigned int ind=0; ind < myPrivate->_personal_module_list.size();ind++){
    _list_components_icone[ind].modulename=(myPrivate->_personal_module_list[ind].name).c_str();
    _list_components_icone[ind].moduleusername=(myPrivate->_personal_module_list[ind].username).c_str();
    _list_components_icone[ind].moduleicone=(myPrivate->_personal_module_list[ind].icon).c_str();
    _list_components_icone[ind].moduleversion=(myPrivate->_personal_module_list[ind].version).c_str();
    _list_components_icone[ind].modulecomment=(myPrivate->_personal_module_list[ind].comment).c_str();
    //if(MYDEBUG) SCRUTE(_list_components_icone[ind].modulename); 
    //if(MYDEBUG) SCRUTE(_list_components_icone[ind].moduleicone);
  }
  
  int indice = myPrivate->_personal_module_list.size() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
  for(unsigned int ind=0; ind < myPrivate->_general_module_list.size();ind++){
    _find = false;
    for(unsigned int ind1=0; ind1 < myPrivate->_personal_module_list.size();ind1++){
      // searching if the component is aleready defined in 
      // the personal catalog
      if((myPrivate->_general_module_list[ind].name.compare(myPrivate->_personal_module_list[ind1].name)) == 0)
        _find = true;
    }
    if(!_find){
      //          if(MYDEBUG) MESSAGE("A new component " << _general_module_list[ind].name << " has to be to added in the list");
      _list_components_icone->length(indice+1);
      // The component is not already defined => has to be taken
      _list_components_icone[indice].modulename=myPrivate->_general_module_list[ind].name.c_str();  
      _list_components_icone[indice].moduleusername=myPrivate->_general_module_list[ind].username.c_str();  
      _list_components_icone[indice].moduleicone=myPrivate->_general_module_list[ind].icon.c_str(); 
      _list_components_icone[indice].moduleversion=myPrivate->_general_module_list[ind].version.c_str();
      _list_components_icone[indice].modulecomment=myPrivate->_general_module_list[ind].comment.c_str();
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
  for (unsigned int ind=0; ind < myPrivate->_personal_module_list.size();ind++)
    {
      if  (myPrivate->_personal_module_list[ind].type == _temp_component_type)
        {
          _list_typed_component->length(_j + 1); 
           _list_typed_component[_j] = myPrivate->_personal_module_list[ind].name.c_str();
          //if(MYDEBUG) SCRUTE(_list_typed_component[_j]);
          _j++;
        }
    }

  int indice = _list_typed_component->length() ;
  bool _find = false;
  
  // The components in the general catalog are taken only if they're
  // not defined in the personal catalog
  for (unsigned int ind=0; ind < myPrivate->_general_module_list.size();ind++)
    {
      _find = false;

      if(myPrivate->_general_module_list[ind].type == _temp_component_type)
        {
          for (unsigned int ind1=0; ind1 < myPrivate->_personal_module_list.size();ind1++)
            {
              // searching if the component is aleready defined in 
              // the personal catalog
              if ((myPrivate->_general_module_list[ind].name.compare(myPrivate->_personal_module_list[ind1].name)) == 0)
                _find = true;
            }
          if (!_find)
            {
              //if(MYDEBUG) MESSAGE("A new component " << _general_module_list[ind].name << " has to be to added in the list");
              _list_typed_component->length(indice+1);
              // The component is not already defined => has to be taken
              _list_typed_component[indice]=(myPrivate->_general_module_list[ind].name).c_str();   
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
  C_parser = myPrivate->findComponent(s);
  if (C_parser) {
    
    //    DebugParserComponent(*C_parser);

    SALOME_ModuleCatalog::ComponentDef C_corba;
    myPrivate->duplicate(C_corba, *C_parser);

    
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

  ParserComponent * C_parser = myPrivate->findComponent(s);
  
  if (C_parser) {
    
    SALOME_ModuleCatalog::ComponentDef * C_corba 
      = new SALOME_ModuleCatalog::ComponentDef; 
    myPrivate->duplicate(*C_corba, *C_parser);
    return C_corba;
  }

  return NULL;
}

void SALOME_ModuleCatalogImpl::ping()
{
}

CORBA::Long SALOME_ModuleCatalogImpl::getPID()
{ 
  return 
#ifndef WIN32
    (CORBA::Long)getpid();
#else
    (CORBA::Long)_getpid();
#endif
}

void SALOME_ModuleCatalogImpl::ShutdownWithExit()
{
  exit( EXIT_SUCCESS );
}

void SALOME_ModuleCatalogImpl::shutdown()
{
  if (!CORBA::is_nil(_orb)) _orb->shutdown(0);
};



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

ParserComponent *
SALOME_ModuleCatalogImpl::Private::findComponent(const std::string & name)
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
            //      if(MYDEBUG) MESSAGE("Component named " << name 
            //                  << " found in the general catalog");
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
SALOME_ModuleCatalogImpl::Private::_parse_xml_file(const char* file, 
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
  myPrivate->_parse_xml_file(file, myPrivate->_personal_module_list, myPrivate->_personal_path_list,myPrivate->_typeMap,myPrivate->_typeList);
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
void SALOME_ModuleCatalogImpl::Private::duplicate
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
void SALOME_ModuleCatalogImpl::Private::duplicate
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
void SALOME_ModuleCatalogImpl::Private::duplicate
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
  //  if(MYDEBUG) SCRUTE(_length);
  S_corba.ServiceoutDataStreamParameter.length(_length);

  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_corba.ServiceoutDataStreamParameter[ind2],
              S_parser.outDataStreamParameters[ind2]);
}

//----------------------------------------------------------------------
// Function : duplicate
// Purpose  : create a service parameter from the catalog parsing
//----------------------------------------------------------------------
void SALOME_ModuleCatalogImpl::Private::duplicate
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
void SALOME_ModuleCatalogImpl::Private::duplicate
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
SALOME_ModuleCatalogImpl::Private::duplicate(ParserPathPrefixes &L_out, 
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
SALOME_ModuleCatalogImpl::Private::_verify_path_prefix(ParserPathPrefixes & pathList)
{
  bool _return_value = true;
  std::vector<std::string> _machine_list;

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
             if(MYDEBUG) MESSAGE( "The computer " << _machine_list[ind] << " is indicated more than once in the path list");
             _return_value = false; 
           }
       }
    }
  return _return_value;
}
