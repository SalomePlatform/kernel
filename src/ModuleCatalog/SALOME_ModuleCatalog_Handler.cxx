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
//  File   : SALOME_ModuleCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$ 

#define WRITE_CATA_COMPONENT

#include "SALOME_ModuleCatalog_Handler.hxx"
#include "SALOME_ModuleCatalog_Parser_IO.hxx"
#include "utilities.h"

#include <sstream>
using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalog_Handler
// Purpose  : Constructor
//----------------------------------------------------------------------
SALOME_ModuleCatalog_Handler::SALOME_ModuleCatalog_Handler()
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
      // Process path prefix list (tag test_path_prefix_list)
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
		  if ( !xmlStrcmp(aCurSubSubNode->name, (const xmlChar*)test_path_prefix_name) )
		    _pathPrefix.path = (const char*)aCurSubSubNode->content;

		  // Tag test_computer_list
		  if ( !xmlStrcmp(aCurSubSubNode->name, (const xmlChar*)test_computer_list) ) {
		    xmlNodePtr aComputerNode = aCurSubSubNode->xmlChildrenNode;
		    while (aComputerNode != NULL) {
		      // Tag test_computer_name
		      if ( !xmlStrcmp(aComputerNode->name, (const xmlChar*) test_computer_name) )
			_pathPrefix.listOfComputer.push_back((const char*)aComputerNode->content);
		      
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

      //@ Process list of components (tag test_component_list)
      if ( !xmlStrcmp(aCurNode->name,(const xmlChar*)test_component_list) )
	{
	  xmlNodePtr aComponentNode = aCurNode->xmlChildrenNode;
	  while(aComponentNode != NULL)
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
	      
	      xmlNodePtr aComponentSubNode = aComponentNode->xmlChildrenNode;
	      while(aComponentSubNode != NULL) {
		
		std::string aContent = (const char*)aComponentSubNode->content;

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
		  _aModule.multistudy = atoi( aContent.c_str() );

		// Tag test_component_impltype
		if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_component_impltype) )
		  _aModule.implementationType = atoi(aContent.c_str());

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
		
		// Process tag test_interface_list:
		if ( !xmlStrcmp(aComponentSubNode->name, (const xmlChar*)test_interface_list) ) {

		  // Form an interface list for the component
		  xmlNodePtr aSubNode = aComponentSubNode->xmlChildrenNode;
		  while(aSubNode != NULL) {
		    // Tag test_interface_name
		    if ( !xmlStrcmp(aSubNode->name, (const xmlChar*)test_interface_name) )
		      _aInterface.name = (const char*)aSubNode->content;
		    
		    // Tag test_service_list
		    if ( !xmlStrcmp(aSubNode->name, (const xmlChar*)test_service_list) ) {
		      // Form a service list for the interface
		      xmlNodePtr aCompServiceNode = aSubNode->xmlChildrenNode;
		      while(aCompServiceNode != NULL) {
			// Tag test_service
			if ( !xmlStrcmp(aCompServiceNode->name, (const xmlChar*)test_interface_name) ) {
			  xmlNodePtr aCompServiceSubNode = aCompServiceNode->xmlChildrenNode;
			  while(aCompServiceSubNode != NULL) {
			    // Tag test_service_name
			    if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_service_name) )
			      _aService.name = (const char*)aCompServiceSubNode->content;

			    // Tag test_defaultservice
			    if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_defaultservice) )
			      _aService.byDefault = (const char*)aCompServiceSubNode->content;

			    // Tag test_typeofnode
			    if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_typeofnode) )
			      _aService.typeOfNode = (const char*)aCompServiceSubNode->content;

			    // Tag test_inParameter_list
			    if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_inParameter_list) ) {
			      xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
			      while(aParamNode != NULL) {
				
				// Tag test_inParameter
				if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_inParameter)) {
				  aParamNode = aParamNode->next;
				  continue;
				}
				
				xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
				while(aParamItemNode != NULL) {

				  // Tag test_inParameter_name
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inParameter_name) )
				    _inParam.name = (const char*)aParamItemNode->content;

				  // Tag test_inParameter_type
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inParameter_type) )
				    _inParam.type = (const char*)aParamItemNode->content;
				  
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
			      while(aParamNode != NULL) {
				
				// Tag test_outParameter
				if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_outParameter)) {
				  aParamNode = aParamNode->next;
				  continue;
				}
				
				xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
				while(aParamItemNode != NULL) {

				  // Tag test_outParameter_name
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outParameter_name) )
				    _outParam.name = (const char*)aParamItemNode->content;

				  // Tag test_outParameter_type
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outParameter_type) )
				    _outParam.type = (const char*)aParamItemNode->content;
				  
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
			    if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_inDataStreamParameter_list) ) {
			      xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
			      while(aParamNode != NULL) {
				
				// Tag test_inDataStreamParameter
				if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_inDataStreamParameter)) {
				  aParamNode = aParamNode->next;
				  continue;
				}
				
				xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
				while(aParamItemNode != NULL) {

				  // Tag test_inDataStreamParameter_name
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inDataStreamParameter_name) )
				    _inDataStreamParam.name = (const char*)aParamItemNode->content;

				  // Tag test_inDataStreamParameter_type
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inDataStreamParameter_type) )
				    _inDataStreamParam.type = (const char*)aParamItemNode->content;

				  // Tag test_inDataStreamParameter_dependency
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_inDataStreamParameter_dependency) )
				    _inDataStreamParam.dependency = (const char*)aParamItemNode->content;
				  
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
			    if ( !xmlStrcmp(aCompServiceSubNode->name, (const xmlChar*)test_outDataStreamParameter_list) ) {
			      xmlNodePtr aParamNode = aCompServiceSubNode->xmlChildrenNode;
			      while(aParamNode != NULL) {
				
				// Tag test_outDataStreamParameter
				if (xmlStrcmp(aParamNode->name, (const xmlChar*)test_outDataStreamParameter)) {
				  aParamNode = aParamNode->next;
				  continue;
				}
				
				xmlNodePtr aParamItemNode = aParamNode->xmlChildrenNode;
				while(aParamItemNode != NULL) {

				  // Tag test_outDataStreamParameter_name
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outDataStreamParameter_name) )
				    _outDataStreamParam.name = (const char*)aParamItemNode->content;

				  // Tag test_outDataStreamParameter_type
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outDataStreamParameter_type) )
				    _outDataStreamParam.type = (const char*)aParamItemNode->content;

				  // Tag test_outDataStreamParameter_dependency
				  if ( !xmlStrcmp(aParamItemNode->name, (const xmlChar*)test_outDataStreamParameter_dependency) )
				    _outDataStreamParam.dependency = (const char*)aParamItemNode->content;
				  
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
	    
	      
	      aComponentNode = aComponentNode->next;
	    }
	}

      
      aCurNode = aCurNode->next;
    }
}
