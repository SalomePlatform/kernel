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
//  File   : SALOME_ModuleCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$ 
//
#define WRITE_CATA_COMPONENT

#include "SALOME_ModuleCatalog_Handler.hxx"
#include "SALOME_ModuleCatalog_Parser_IO.hxx"
#include "utilities.h"

#include <sstream>

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalog_Handler
// Purpose  : Constructor
//----------------------------------------------------------------------
SALOME_ModuleCatalog_Handler::SALOME_ModuleCatalog_Handler(ParserPathPrefixes& pathList,
                                                           ParserComponents& moduleList,
                                                           ParserTypes& typeMap,
                                                           TypeList& typeList):_typeMap(typeMap),_typeList(typeList),
                                                                               _pathList(pathList),_moduleList(moduleList)

{
  if(MYDEBUG) BEGIN_OF("SALOME_ModuleCatalog_Handler");

  // XML Tags initialisation
  // Used in the function endElement
  test_path_prefix_name     = "path-prefix-name";
  test_computer_name        = "computer-name" ;
  test_computer_list        = "computer-list" ;
  test_path_prefix          = "path-prefix" ;
  test_path_prefix_list     = "path-prefix-list" ;

  test_component_name       = "component-name";
  test_component_username   = "component-username";
  test_component_type       = "component-type" ;
  test_component_multistudy = "component-multistudy";
  test_component_icon       = "component-icone" ;
  test_component_impltype   = "component-impltype";
  test_component_implname   = "component-implname";
  test_component_version    = "component-version";
  test_component_comment    = "component-comment";

  test_interface_name       = "component-interface-name" ;
  
  test_service_name         = "service-name";
  test_defaultservice       = "service-by-default";

  test_typeofnode           = "type-of-node";

  test_inParameter_type     = "inParameter-type";
  test_inParameter_name     = "inParameter-name";
  test_inParameter          = "inParameter";
  test_inParameter_list     = "inParameter-list";

  test_outParameter_type    = "outParameter-type";
  test_outParameter_name    = "outParameter-name";
  test_outParameter         = "outParameter";
  test_outParameter_list    = "outParameter-list";

  test_inDataStreamParameter_type = "inParameter-type";
  test_inDataStreamParameter_name = "inParameter-name";
  test_inDataStreamParameter_dependency = "inParameter-dependency";
  test_inDataStreamParameter = "inParameter";
  test_inDataStreamParameter_list = "DataStream-list";

  test_outDataStreamParameter_type = "outParameter-type";
  test_outDataStreamParameter_name = "outParameter-name";
  test_outDataStreamParameter_dependency = "outParameter-dependency";
  test_outDataStreamParameter = "outParameter";
  test_outDataStreamParameter_list = "DataStream-list";

  test_service =  "component-service";
  test_service_list = "component-service-list";
  test_interface_list = "component-interface-list";

  test_constraint = "constraint";

  test_component_list = "component-list";
  test_component="component";

  if(MYDEBUG) END_OF("SALOME_ModuleCatalog_Handler");
}

//----------------------------------------------------------------------
// Function : ~SALOME_ModuleCatalog_Handler
// Purpose  : Destructor
//----------------------------------------------------------------------
SALOME_ModuleCatalog_Handler::~SALOME_ModuleCatalog_Handler()
{
  if(MYDEBUG) BEGIN_OF("~SALOME_ModuleCatalog_Handler()")
  if(MYDEBUG) END_OF("~SALOME_ModuleCatalog_Handler()")
}


//=============================================================================
/*!
 *  Processes XML document and fills the list of modules
 */ 
//=============================================================================

void SALOME_ModuleCatalog_Handler::ProcessXmlDocument(xmlDocPtr theDoc)
{
  if(MYDEBUG) MESSAGE("Begin parse document");
  // Empty the private elements
  _pathList.resize(0);
  _pathPrefix.listOfComputer.resize(0);
  _serviceList.resize(0);
  _interfaceList.resize(0);
  _moduleList.resize(0);
  _inDataStreamParamList.resize(0);
  _outDataStreamParamList.resize(0);
  _inParamList.resize(0);
  _outParamList.resize(0);

  // Get the document root node
  xmlNodePtr aCurNode = xmlDocGetRootElement(theDoc);
  aCurNode = aCurNode->xmlChildrenNode;
  
  // Processing the document nodes
  while(aCurNode != NULL)
  {
    // Part 1: Process path prefix list (tag test_path_prefix_list)
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_path_prefix_list) )
    {
      xmlNodePtr aCurSubNode = aCurNode->xmlChildrenNode;
      while(aCurSubNode != NULL)
      {
        // Forming a PathPrefix structure (tag test_path_prefix)
        if ( xmlStrcmp(aCurSubNode->name, (const xmlChar*)test_path_prefix) ) {
          aCurSubNode = aCurSubNode->next;
          continue;
        }

        xmlNodePtr aCurSubSubNode = aCurSubNode->xmlChildrenNode;
        while(aCurSubSubNode != NULL)
        {
          // Tag test_path_prefix_name
          if ( !xmlStrcmp(aCurSubSubNode->name, (const xmlChar*)test_path_prefix_name) ) {
            xmlChar* aPath = xmlNodeGetContent(aCurSubSubNode);
            if (aPath != NULL) {
              _pathPrefix.path = (const char*)aPath;
              xmlFree(aPath);
            }
          }

          // Tag test_computer_list
          if ( !xmlStrcmp(aCurSubSubNode->name, (const xmlChar*)test_computer_list) ) {
            xmlNodePtr aComputerNode = aCurSubSubNode->xmlChildrenNode;
            while (aComputerNode != NULL) {
              // Tag test_computer_name
              if ( !xmlStrcmp(aComputerNode->name, (const xmlChar*) test_computer_name) ) {
                xmlChar* aCompName = xmlNodeGetContent(aComputerNode);
                if (aCompName != NULL) {
                  _pathPrefix.listOfComputer.push_back((const char*)aCompName);
                  xmlFree(aCompName);
                }
              }

              aComputerNode = aComputerNode->next;
            }
          }

          aCurSubSubNode = aCurSubSubNode->next;
        }

        _pathList.push_back(_pathPrefix);
        _pathPrefix.listOfComputer.resize(0);

        aCurSubNode = aCurSubNode->next;
      }
    }

    //Part 2: Process list of types
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)"type-list") )
      {
        xmlNodePtr aTypeNode = aCurNode->xmlChildrenNode;
        while (aTypeNode != NULL)
          {
            //  match "type"
            if ( !xmlStrcmp(aTypeNode->name, (const xmlChar*)"type" )) 
              {
                // Here is a basic type description
                ParserType aType;
                xmlChar * name=xmlGetProp(aTypeNode,(const xmlChar*)"name");
                if(name)
                  {
                    aType.name = (const char*)name;
                    xmlFree(name);
                  }
                xmlChar *kind=xmlGetProp(aTypeNode,(const xmlChar*)"kind");
                if(kind)
                  {
                    aType.kind = (const char*)kind;
                    xmlFree(kind);
                  }
                if(aType.kind == "double" ||
                   aType.kind == "int"    ||
                   aType.kind == "bool"    ||
                   aType.kind == "string")
                  {
                    if ( _typeMap.find(aType.name) == _typeMap.end() )
                      {
                        MESSAGE("Registered basic type: " << aType.name << " " << aType.kind );
                        _typeMap[aType.name]=aType;
                        _typeList.push_back(aType);
                      }
                    else
                      std::cerr << "Warning: this type (" << aType.name << "," << aType.kind << ") already exists, it will be ignored."  << std::endl;
                  }
                else
                  std::cerr << "Warning: this type (" << aType.name << "," << aType.kind << ") has incorrect kind, it will be ignored." << std::endl;
              }
            else if ( !xmlStrcmp(aTypeNode->name, (const xmlChar*)"sequence" )) 
              {
                // Here is a sequence type description
                ParserSequence aType;
                xmlChar * name=xmlGetProp(aTypeNode,(const xmlChar*)"name");
                if(name)
                  {
                    aType.name = (const char*)name;
                    xmlFree(name);
                  }
                xmlChar *content=xmlGetProp(aTypeNode,(const xmlChar*)"content");
                if(content)
                  {
                    aType.content = (const char*)content;
                    xmlFree(content);
                  }
                if ( _typeMap.find(aType.content) != _typeMap.end() )
                  {
                    if ( _typeMap.find(aType.name) == _typeMap.end() )
                      {
                        MESSAGE("Registered sequence type: " << aType.name << " " << aType.content );
                        _typeMap[aType.name]=aType;
                        _typeList.push_back(aType);
                      }
                    else
                      std::cerr << "Warning: this type (" << aType.name << "," << aType.kind << ") already exists, it will be ignored."  << std::endl;
                  }
                else
                  {
                    std::cerr << "Warning: this sequence type (" << aType.name << "," << aType.content << ") has unknown content type, it will be ignored." << std::endl;
                  }
              }
            else if ( !xmlStrcmp(aTypeNode->name, (const xmlChar*)"objref" )) 
              {
                // Here is an objref type description
                ParserObjref aType;
                int error=0;
                xmlChar * name=xmlGetProp(aTypeNode,(const xmlChar*)"name");
                if(name)
                  {
                    aType.name = (const char*)name;
                    xmlFree(name);
                  }
                xmlChar *id=xmlGetProp(aTypeNode,(const xmlChar*)"id");
                if(id)
                  {
                    aType.id = (const char*)id;
                    xmlFree(id);
                  }

                xmlNodePtr aTypeSubNode = aTypeNode->xmlChildrenNode;
                while (aTypeSubNode != NULL)
                  {
                    if ( !xmlStrcmp(aTypeSubNode->name, (const xmlChar*)"base" )) 
                      {
                        //a base type
                        xmlChar* content = xmlNodeGetContent(aTypeSubNode);
                        if(content)
                          {
                            std::string base=(const char*)content;
                            xmlFree(content);
                            if ( _typeMap.find(base) != _typeMap.end() && _typeMap[base].kind == "objref")
                              {
                                aType.bases.push_back(base);
                              }
                            else
                              {
                                std::cerr << "Warning: this objref type (" << aType.name << ") has unknown base type (" << base << "), it will be ignored." << std::endl;
                                error=1;
                                break;
                              }
                          }
                      }
                    aTypeSubNode = aTypeSubNode->next;
                  }
                if(!error)
                  {
                    if ( _typeMap.find(aType.name) == _typeMap.end() )
                      {
                        MESSAGE("Registered objref type: " << aType.name << " " << aType.id );
                        _typeMap[aType.name]=aType;
                        _typeList.push_back(aType);
                      }
                    else
                      std::cerr << "Warning: this type (" << aType.name << "," << aType.kind << ") already exists, it will be ignored."  << std::endl;
                  }
              }
            else if ( !xmlStrcmp(aTypeNode->name, (const xmlChar*)"struct" )) 
              {
                // Here is a struct type description
                ParserStruct aType;
                int error=0;
                xmlChar * name=xmlGetProp(aTypeNode,(const xmlChar*)"name");
                if(name)
                  {
                    aType.name = (const char*)name;
                    xmlFree(name);
                  }
                xmlChar *id=xmlGetProp(aTypeNode,(const xmlChar*)"id");
                if(id)
                  {
                    aType.id = (const char*)id;
                    xmlFree(id);
                  }

                xmlNodePtr aTypeSubNode = aTypeNode->xmlChildrenNode;
                while (aTypeSubNode != NULL)
                  {
                    if ( !xmlStrcmp(aTypeSubNode->name, (const xmlChar*)"member" )) 
                      {
                        std::pair<std::string,std::string> member;
                        xmlChar * m_name=xmlGetProp(aTypeSubNode,(const xmlChar*)"name");
                        if(m_name)
                          {
                            member.first=(const char*)m_name;
                            xmlFree(m_name);
                          }
                        xmlChar * m_type=xmlGetProp(aTypeSubNode,(const xmlChar*)"type");
                        if(m_type)
                          {
                            member.second=(const char*)m_type;
                            xmlFree(m_type);
                          }
                        if ( _typeMap.find(member.second) != _typeMap.end() )
                          {
                            aType.members.push_back(member);
                          }
                        else
                          {
                            std::cerr << "Warning: this struct type (" << aType.name << ") has unknown member type (" << member.first << "," << member.second << "), it will be ignored." << std::endl;
                            error=1;
                            break;
                          }
                      }
                    aTypeSubNode = aTypeSubNode->next;
                  }
                if(!error)
                  {
                    if ( _typeMap.find(aType.name) == _typeMap.end() )
                      {
                        MESSAGE("Registered struct type: " << aType.name << " " << aType.id );
                        _typeMap[aType.name]=aType;
                        _typeList.push_back(aType);
                      }
                    else
                      std::cerr << "Warning: this type (" << aType.name << "," << aType.kind << ") already exists, it will be ignored."  << std::endl;
                  }
              } // end of struct
            aTypeNode = aTypeNode->next;
          }
      }

    //Part 3: Process list of components (tag test_component_list)
    if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_component_list) )
    {
      xmlNodePtr aComponentNode = aCurNode->xmlChildrenNode;
      while (aComponentNode != NULL)
      {
        // Do not process tags differ from test_component here
        if ( xmlStrcmp(aComponentNode->name, (const xmlChar*)test_component) ) {
          aComponentNode = aComponentNode->next;
          continue;
        }

        // Component identification

        // Empty temporary structures
        _aModule.name = "";
        _aModule.constraint = "";
        _aModule.icon="";       
        _aModule.interfaces.resize(0);
        _aModule.implementationType ="";
        _aModule.implementationName ="";

        xmlNodePtr aComponentSubNode = aComponentNode->xmlChildrenNode;
        while(aComponentSubNode != NULL)
        {
          xmlChar* aNodeContent = xmlNodeGetContent(aComponentSubNode);

          if (aNodeContent == NULL) {
            aComponentSubNode = aComponentSubNode->next;
            continue;
          }

          std::string aContent = (const char*)aNodeContent;

          // Tag test_component_name
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_name) )
            _aModule.name = aContent;

          // Tag test_component_username
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_username) )
            _aModule.username = aContent;

          // Tag test_component_type
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_type) ) {
            std::string aType = aContent;

            if ((aType.compare("MESH") == 0) ||
                (aType.compare("Mesh") == 0) ||
                (aType.compare("mesh") == 0))
              _aModule.type = MESH ;
            else if((aType.compare("MED") == 0) ||
                    (aType.compare("Med") == 0) ||
                    (aType.compare("med") == 0))
              _aModule.type = Med ;
            else if((aType.compare("GEOM") == 0) ||
                    (aType.compare("Geom") == 0) ||
                    (aType.compare("geom") == 0))
              _aModule.type = GEOM ;
            else if((aType.compare("SOLVER") == 0) ||
                    (aType.compare("Solver") == 0) ||
                    (aType.compare("solver") == 0))
              _aModule.type = SOLVER ;
            else if((aType.compare("SUPERV") == 0) ||
                    (aType.compare("Superv") == 0) ||
                    (aType.compare("Supervision") == 0) ||
                    (aType.compare("superv") == 0))
              _aModule.type = SUPERV ;
            else if((aType.compare("DATA") == 0) ||
                    (aType.compare("Data") == 0) ||
                    (aType.compare("data") == 0))
              _aModule.type = DATA ; 
            else if((aType.compare("VISU") == 0) ||
                    (aType.compare("Visu") == 0) ||
                    (aType.compare("visu") == 0))
              _aModule.type = VISU ; 
            else if((aType.compare("OTHER") == 0) ||
                    (aType.compare("Other") == 0) ||
                    (aType.compare("other") == 0))                
              _aModule.type = OTHER ;
            else
              // If it'not in all theses cases, the type is affected to OTHER
              _aModule.type = OTHER ;
          }

          // Tag test_component_multistudy
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_multistudy) )
            _aModule.multistudy = atoi( aContent.c_str() ) != 0;

          // Tag test_component_impltype
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_impltype) )
            _aModule.implementationType = aContent;

          // Tag test_component_implname
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_implname) )
            _aModule.implementationName = aContent;

          // Tag test_component_icon
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_icon) )
            _aModule.icon = aContent;

          // Tag test_component_version
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_version) )
            _aModule.version = aContent;

          // Tag test_component_comment
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_comment) )
            _aModule.comment = aContent;

          // Tag test_constraint
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_constraint) )
            _aModule.constraint = aContent;

          xmlFree(aNodeContent);

          // Process tag test_interface_list:
          if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_interface_list) ) {

            // Form an interface list for the component
            xmlNodePtr aSubNode = aComponentSubNode->xmlChildrenNode;
            while(aSubNode != NULL) {
              // Tag test_interface_name
              if ( !xmlStrcmp(aSubNode->name, (const xmlChar*)test_interface_name) ) {
                xmlChar* anInterfaceName = xmlNodeGetContent(aSubNode);
                if (anInterfaceName != NULL) {
                  _aInterface.name = (const char*)anInterfaceName;
                  xmlFree(anInterfaceName);
                }
              }

              // Tag test_service_list
              if ( !xmlStrcmp(aSubNode->name, (const xmlChar*)test_service_list) ) {
                // Form a service list for the interface
                xmlNodePtr aCompServiceNode = aSubNode->xmlChildrenNode;
                while(aCompServiceNode != NULL) {
                  // Tag test_service
                  if ( !xmlStrcmp(aCompServiceNode->name, (const xmlChar*)"component-service") ) {
                    xmlNodePtr aCompServiceSubNode = aCompServiceNode->xmlChildrenNode;
                    while(aCompServiceSubNode != NULL)
                    {
                      xmlChar* aCompServiceData = xmlNodeGetContent(aCompServiceSubNode);

                      if ( aCompServiceData != NULL)
                      {
                        // Tag test_service_name
                        if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_service_name) )
                          _aService.name = (const char*)aCompServiceData;

                        // Tag test_defaultservice
                        if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_defaultservice) )
                          _aService.byDefault = atoi((const char*)aCompServiceData) !=0 ;

                        // Tag test_typeofnode
                        if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_typeofnode) )
                          _aService.typeOfNode = atoi((const char*)aCompServiceData) !=0 ;

                        xmlFree(aCompServiceData);
                      }

                      // Tag test_inParameter_list
                      if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_inParameter_list) ) {
                        xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
                        while (aParamNode != NULL)
                        {
                          // Tag test_inParameter
                          if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_inParameter)) {
                            aParamNode = aParamNode->next;
                            continue;
                          }

                          xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
                          while (aParamItemNode != NULL)
                          {
                            xmlChar* aParamData = xmlNodeGetContent(aParamItemNode);

                            if (aParamData != NULL)
                            {
                              // Tag test_inParameter_name
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inParameter_name) )
                                _inParam.name = (const char*)aParamData;

                              // Tag test_inParameter_type
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inParameter_type) )
                                _inParam.type = (const char*)aParamData;

                              xmlFree(aParamData);
                            }

                            aParamItemNode = aParamItemNode->next;
                          }

                          _inParamList.push_back(_inParam) ; 

                          // Empty temporary structures
                          _inParam.type = "";
                          _inParam.name = "";

                          aParamNode = aParamNode->next;
                        }

                        _aService.inParameters = _inParamList;
                        _inParamList.resize(0);
                      }

                      // Tag test_outParameter_list
                      if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_outParameter_list) ) {
                        xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
                        while (aParamNode != NULL)
                        {
                          // Tag test_outParameter
                          if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_outParameter)) {
                            aParamNode = aParamNode->next;
                            continue;
                          }

                          xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
                          while (aParamItemNode != NULL)
                          {
                            xmlChar* anOutParamData = xmlNodeGetContent(aParamItemNode);

                            if (anOutParamData != NULL)
                            {
                              // Tag test_outParameter_name
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outParameter_name) )
                                _outParam.name = (const char*)anOutParamData;

                              // Tag test_outParameter_type
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outParameter_type) )
                                _outParam.type = (const char*)anOutParamData;

                              xmlFree(anOutParamData);
                            }

                            aParamItemNode = aParamItemNode->next;
                          }

                          _outParamList.push_back(_outParam) ; 

                          // Empty temporary structures
                          _outParam.type = "";
                          _outParam.name = "";

                          aParamNode = aParamNode->next;
                        }

                        _aService.outParameters = _outParamList;
                        _outParamList.resize(0);
                      }

                      //@ Tag test_inDataStreamParameter_list
                      if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_inDataStreamParameter_list) )
                      {
                        xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
                        while (aParamNode != NULL)
                        {
                          // Tag test_inDataStreamParameter
                          if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_inDataStreamParameter)) {
                            aParamNode = aParamNode->next;
                            continue;
                          }

                          xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
                          while (aParamItemNode != NULL)
                          {
                            xmlChar* inDataStreamParamData = xmlNodeGetContent(aParamItemNode);

                            if (inDataStreamParamData != NULL)
                            {
                              // Tag test_inDataStreamParameter_name
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inDataStreamParameter_name) )
                                _inDataStreamParam.name = (const char*)inDataStreamParamData;

                              // Tag test_inDataStreamParameter_type
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inDataStreamParameter_type) )
                                _inDataStreamParam.type = (const char*)inDataStreamParamData;

                              // Tag test_inDataStreamParameter_dependency
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inDataStreamParameter_dependency) )
                                _inDataStreamParam.dependency = (const char*)inDataStreamParamData;

                              xmlFree(inDataStreamParamData);
                            }

                            aParamItemNode = aParamItemNode->next;
                          }

                          _inDataStreamParamList.push_back(_inDataStreamParam) ; 

                          // Empty temporary structures
                          _inDataStreamParam.type = "";
                          _inDataStreamParam.name = "";
                          _inDataStreamParam.dependency = "";

                          aParamNode = aParamNode->next;
                        }

                        _aService.inDataStreamParameters = _inDataStreamParamList;
                        _inDataStreamParamList.resize(0);
                      }

                      // Tag test_outDataStreamParameter_list
                      if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_outDataStreamParameter_list) )
                      {
                        xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
                        while (aParamNode != NULL)
                        {
                          // Tag test_outDataStreamParameter
                          if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_outDataStreamParameter)) {
                            aParamNode = aParamNode->next;
                            continue;
                          }

                          xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
                          while (aParamItemNode != NULL)
                          {
                            xmlChar* outDataStreamParamData = xmlNodeGetContent(aParamItemNode);

                            if (outDataStreamParamData != NULL)
                            {
                              // Tag test_outDataStreamParameter_name
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outDataStreamParameter_name) )
                                _outDataStreamParam.name = (const char*)outDataStreamParamData;
                              
                              // Tag test_outDataStreamParameter_type
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outDataStreamParameter_type) )
                                _outDataStreamParam.type = (const char*)outDataStreamParamData;
                              
                              // Tag test_outDataStreamParameter_dependency
                              if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outDataStreamParameter_dependency) )
                                _outDataStreamParam.dependency = (const char*)outDataStreamParamData;
                              
                              xmlFree(outDataStreamParamData);
                            }

                            aParamItemNode = aParamItemNode->next;
                          }
                          
                          _outDataStreamParamList.push_back(_outDataStreamParam) ; 
                          
                          // Empty temporary structures
                          _outDataStreamParam.type = "";
                          _outDataStreamParam.name = "";
                          _outDataStreamParam.dependency = "";

                          aParamNode = aParamNode->next;
                        }
                        
                        _aService.outDataStreamParameters = _outDataStreamParamList;
                        _outDataStreamParamList.resize(0);
                      }
                      
                      aCompServiceSubNode = aCompServiceSubNode->next;
                    }

                    // Put formed service into the list
                    _serviceList.push_back(_aService);

                    // Empty temporary structures
                    _aService.name = "";
                    _aService.typeOfNode = 1;
                    _aService.inParameters.resize(0);
                    _aService.outParameters.resize(0);
                    _aService.inDataStreamParameters.resize(0);
                    _aService.outDataStreamParameters.resize(0);
                  }
                  
                  aCompServiceNode = aCompServiceNode->next;
                }
                
                _aInterface.services = _serviceList ;
                
                // Empty temporary structures
                _serviceList.resize(0);
                _interfaceList.push_back(_aInterface);  
                _aInterface.name ="";    
                _aInterface.services.resize(0);
              }
              
              aSubNode = aSubNode->next;
            }
            
            _aModule.interfaces = _interfaceList ;
            _interfaceList.resize(0);
          }

          aComponentSubNode = aComponentSubNode->next;
        }

        _moduleList.push_back(_aModule);

        aComponentNode = aComponentNode->next;
      }
    }

    aCurNode = aCurNode->next;
  }
}
