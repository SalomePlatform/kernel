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
//  File   : SALOME_ModuleCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$ 

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
SALOME_ModuleCatalog_Handler::SALOME_ModuleCatalog_Handler()
{
  if(MYDEBUG) BEGIN_OF("SALOME_ModuleCatalog_Handler");

  // XML Tags initialisation
  // Used in the function endElement
  test_path_prefix_name     = "path-prefix-name";
  test_computer_name        = "computer-name" ;
  test_path_prefix          = "path-prefix" ;

  test_component_name       = "component-name";
  test_component_username   = "component-username";
  test_component_type       = "component-type" ;
  test_component_multistudy = "component-multistudy";
  test_component_icon       = "component-icone" ;
  test_component_impltype   = "component-impltype";

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

//----------------------------------------------------------------------
// Function : startDocument
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_ModuleCatalog_Handler::startDocument()
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
  return true;
}

//----------------------------------------------------------------------
// Function : startElement
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_ModuleCatalog_Handler::startElement(const QString&, 
						const QString &,
						const QString& qName, 
						const QXmlAttributes& atts)
{
  _xml_pile.push(qName);

  return true;
} 

//----------------------------------------------------------------------
// Function : endElement
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_ModuleCatalog_Handler::endElement(const QString&, 
					      const QString &,
					      const QString& qName)
{
  QString parent, grandparent;
  _xml_pile.pop();
  if (!_xml_pile.empty()) {
    parent = _xml_pile.top();
    _xml_pile.pop();
    if (!_xml_pile.empty()) grandparent = _xml_pile.top();
    _xml_pile.push(parent);
  }

  // Path prefix

  // tag test_path_prefix_name
  if((qName.compare(test_path_prefix_name)==0)) {
    _pathPrefix.path = content;
    return true;
  }

  // tag test_computer_name
  if((qName.compare(test_computer_name)==0)) {
    _pathPrefix.listOfComputer.push_back(content);
    return true;
  }
   
  // tag test_path_prefix
  if((qName.compare(test_path_prefix)==0)) {
    _pathList.push_back(_pathPrefix);
    _pathPrefix.listOfComputer.resize(0);
    return true;
  }

  // Component identification

  // tag test_component_name
  if((qName.compare(test_component_name)==0)) {
    _aModule.name = content ;
    return true;
  }

  // tag test_component_username
  if((qName.compare(test_component_username)==0)) {
    _aModule.username = content ;
    return true;
  }

  // tag test_component_type
   if((qName.compare(test_component_type)==0)) 
     {
       if ((content.compare("MESH") == 0) ||
	   (content.compare("Mesh") == 0) ||
	   (content.compare("mesh") == 0))
	 _aModule.type = MESH ;
       else if((content.compare("MED") == 0) ||
	       (content.compare("Med") == 0) ||
	       (content.compare("med") == 0))
	 _aModule.type = Med ;
       else if((content.compare("GEOM") == 0) ||
	       (content.compare("Geom") == 0) ||
	       (content.compare("geom") == 0))
	 _aModule.type = GEOM ;
       else if((content.compare("SOLVER") == 0) ||
	       (content.compare("Solver") == 0) ||
	       (content.compare("solver") == 0))
	 _aModule.type = SOLVER ;
       else if((content.compare("SUPERV") == 0) ||
	       (content.compare("Superv") == 0) ||
	       (content.compare("Supervision") == 0) ||
	       (content.compare("superv") == 0))
	 _aModule.type = SUPERV ;
       else if((content.compare("DATA") == 0) ||
	       (content.compare("Data") == 0) ||
	       (content.compare("data") == 0))
	 _aModule.type = DATA ; 
       else if((content.compare("VISU") == 0) ||
	       (content.compare("Visu") == 0) ||
	       (content.compare("visu") == 0))
	 _aModule.type = VISU ; 
       else if((content.compare("OTHER") == 0) ||
	       (content.compare("Other") == 0) ||
	       (content.compare("other") == 0))                
	 _aModule.type = OTHER ;
       else
	 // If it'not in all theses cases, the type is affected to OTHER
	 _aModule.type = OTHER ;
       return true;
     }

   // tag test_component_multistudy
   if((qName.compare(test_component_multistudy)==0)) {
    _aModule.multistudy = atoi(content.c_str()) ;
    return true;
   }

   // tag test_component_impltype
  if((qName.compare(QString(test_component_impltype))==0)) 
    _aModule.implementationType = atoi(content.c_str()) ;

  // tag test_component_icon
   if((qName.compare(test_component_icon)==0)) {
     _aModule.icon = content ;
     return true;
   }

   // interface identification

   // tag test_interface_name
   if ((qName.compare(test_interface_name)==0)) {
       _aInterface.name = content ;
       return true;
   }

   // Service identification

   // tag test_service_name
   if ((qName.compare(test_service_name)==0)) {
     _aService.name = content ;
     return true;
   }

   //tag test_defaultservice
   if ((qName.compare(test_defaultservice)==0)) {
     _aService.byDefault = atoi(content.c_str()) ;
     return true;
   }

   //tag test_typeofnode
   if((qName.compare(QString(test_typeofnode))==0))
     _aService.typeOfNode = atoi(content.c_str()) ;

   // Parameter in

   // tag test_inDataStreamParameter_type
   if ((qName.compare(test_inDataStreamParameter_type)==0)) {
     if (grandparent.compare(test_inDataStreamParameter_list) == 0)
       _inDataStreamParam.type = content ;
     else 
        _inParam.type = content ;
     return true;
   }
	 
   // tag test_inParameter_type
   if ((qName.compare(test_inParameter_type)==0)) {
     if (grandparent.compare(test_inDataStreamParameter_list) == 0)
       _inDataStreamParam.type = content ;
     else 
        _inParam.type = content ;
     return true;
   }
	 
   //tag test_inDataStreamParameter_name
   if ((qName.compare(test_inDataStreamParameter_name)==0)) {
     if (grandparent.compare(test_inDataStreamParameter_list) == 0)
       _inDataStreamParam.name = content ;
     else 
        _inParam.name = content ;
     return true;
   }

   //tag test_inParameter_name
   if ((qName.compare(test_inParameter_name)==0)) {
     if(MYDEBUG) SCRUTE(parent);
     if(MYDEBUG) SCRUTE(grandparent);
     if (grandparent.compare(test_inDataStreamParameter_list) == 0)
       _inDataStreamParam.name = content ;
     else 
       _inParam.name = content ;
     return true;
   }

   //tag test_inDataStreamParameter_dependency
   if ((qName.compare(test_inDataStreamParameter_dependency)==0)) {
       _inDataStreamParam.dependency = content ;
     return true;
   }

   //tag test_inParameter
  if ((qName.compare(test_inParameter)==0))
    {
      if (parent.compare(test_inParameter_list)==0) {
	
	if(MYDEBUG) MESSAGE("add inParameter : " << _inParam.name);
	_inParamList.push_back(_inParam) ; 
	
	// Empty temporary structures
	_inParam.type = "";
	_inParam.name = "";
      }
      else if ((qName.compare(test_inDataStreamParameter)==0)) {
	
	if(MYDEBUG) MESSAGE("add inDataStreamParameter : " << _inDataStreamParam.name);
	_inDataStreamParamList.push_back(_inDataStreamParam) ; 
	
	// Empty temporary structures
	_inDataStreamParam.type = "";
	_inDataStreamParam.name = "";
	_inDataStreamParam.dependency = "";
      }
      return true;
    }
  
   //tag test_inParameter_list
   if((qName.compare(test_inParameter_list)==0))
     {
       if(MYDEBUG) SCRUTE(_inParamList.size());
       _aService.inParameters = _inParamList;
       _inParamList.resize(0);
       return true;
     }
  
   //tag test_inDataStreamParameter_list
   if((qName.compare(test_inDataStreamParameter_list)==0))
     {
       if(MYDEBUG) SCRUTE(_inDataStreamParamList.size());
       _aService.inDataStreamParameters = _inDataStreamParamList;
       _inDataStreamParamList.resize(0);
     }
   //tag test_outDataStreamParameter_list
   if((qName.compare(test_outDataStreamParameter_list)==0))
     {
       if(MYDEBUG) SCRUTE(_outDataStreamParamList.size());
       _aService.outDataStreamParameters = _outDataStreamParamList;
       _outDataStreamParamList.resize(0);
       return true;
     }



   // Parameter out
   if(MYDEBUG) SCRUTE(qName);

   // tag test_outParameter_type
   if ((qName.compare(test_outParameter_type)==0)) {
     if (grandparent.compare(test_outDataStreamParameter_list) == 0)
       _outDataStreamParam.type = content ;
     else 
        _outParam.type = content ;
     return true;
   }
	 
   // tag test_outDataStreamParameter_type
   if ((qName.compare(test_outDataStreamParameter_type)==0)) {
     if (grandparent.compare(test_outDataStreamParameter_list) == 0)
       _outDataStreamParam.type = content ;
     else 
        _outParam.type = content ;
     return true;
   }
	 
   //tag test_outParameter_name
   if ((qName.compare(test_outParameter_name)==0)) {
     if (grandparent.compare(test_outDataStreamParameter_list) == 0)
       _outDataStreamParam.name = content ;
     else 
        _outParam.name = content ;
     return true;
   }
	 
   //tag test_outDataStreamParameter_name
   if ((qName.compare(test_outDataStreamParameter_name)==0)) {
     if(MYDEBUG) SCRUTE(grandparent);
     if(MYDEBUG) SCRUTE(test_outDataStreamParameter_list);
     if (grandparent.compare(test_outDataStreamParameter_list) == 0)
       _outDataStreamParam.name = content ;
     else 
        _outParam.name = content ;
     return true;
   }
	 
   //tag test_outParameter_dependency
   if ((qName.compare(test_outDataStreamParameter_dependency)==0)) {
     _outDataStreamParam.dependency = content ;
     return true;
   }

   //tag test_outParameter
  if ((qName.compare(test_outParameter)==0))
    {
      if (parent.compare(test_outParameter_list)==0) {
	
	if(MYDEBUG) MESSAGE("add outParameter : " << _outParam.name);
	_outParamList.push_back(_outParam) ; 
	
	// Empty temporary structures
	_outParam.type = "";
	_outParam.name = "";
      }
      else if ((qName.compare(test_outDataStreamParameter)==0)) {
	
	if(MYDEBUG) MESSAGE("add outDataStreamParameter : " << _outDataStreamParam.name);
	_outDataStreamParamList.push_back(_outDataStreamParam) ; 
	
	// Empty temporary structures
	_outDataStreamParam.type = "";
	_outDataStreamParam.name = "";
	_outDataStreamParam.dependency = "";
      }
      return true;
    }
  
   //tag test_outParameter_list
   if((qName.compare(test_outParameter_list)==0))
     {
       if(MYDEBUG) SCRUTE(_outParamList.size());
       _aService.outParameters = _outParamList;
       _outParamList.resize(0);
       return true;
     }
  
   // tag   test_service
   if((qName.compare(test_service)==0))
     {
       _serviceList.push_back(_aService);

       // Empty temporary structures
       _aService.name = "";
       _aService.typeOfNode = 1;
       _aService.inParameters.resize(0);
       _aService.outParameters.resize(0);
       _aService.inDataStreamParameters.resize(0);
       _aService.outDataStreamParameters.resize(0);
     }

   // tag   test_service_list
   if((qName.compare(test_service_list)==0))
     {
       _aInterface.services = _serviceList ;

       // Empty temporary structures
       _serviceList.resize(0); 
       _interfaceList.push_back(_aInterface);  
       _aInterface.name ="";    
       _aInterface.services.resize(0);

     }

   //tag test_interface_list
   if((qName.compare(test_interface_list)==0))
     {
       _aModule.interfaces = _interfaceList ;
       _interfaceList.resize(0);
     }

   //tag test_constraint
   if((qName.compare(test_constraint)==0))
     _aModule.constraint = content ;

   // tag test_component
   if((qName.compare(test_component)==0))
     {
       _moduleList.push_back(_aModule) ;
       
       // Empty temporary structures
       _aModule.name = "";
       _aModule.constraint = "";
       _aModule.icon="";       
       _aModule.interfaces.resize(0);
     }
    
  return true;
}
  
//----------------------------------------------------------------------
// Function : characters
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_ModuleCatalog_Handler::characters(const QString& chars)
{
  content = (const char*)chars ;
  return true;
}

//----------------------------------------------------------------------
// Function : endDocument
// Purpose  : overload handler function
//            Print all informations find in the catalog 
//            (only in DEBUG mode!!)
//----------------------------------------------------------------------  
#include <fstream>
bool SALOME_ModuleCatalog_Handler::endDocument()
{
//  ofstream f("/tmp/logs/xxx.log", std::ofstream::app);
//  f << "---------------------------------------------------------" << std::endl;
  if(MYDEBUG) BEGIN_OF("endDocument");
  //_pathlist
  for (unsigned int ind = 0; ind < _pathList.size(); ind++)
    {
      if(MYDEBUG) MESSAGE("Path :"<<_pathList[ind].path);
      for (unsigned int i = 0; i < _pathList[ind].listOfComputer.size(); i++)
	if(MYDEBUG) MESSAGE("Computer name :" << _pathList[ind].listOfComputer[i]);
    }

   // _moduleList
//  if(MYDEBUG) SCRUTE(_moduleList.size());
//  for (unsigned int ind = 0; ind < _moduleList.size(); ind++)
//    {
//      f << _moduleList[ind] << std::endl;
//    }

  if(MYDEBUG) MESSAGE("Document parsed");
  if(MYDEBUG) END_OF("endDocument");
  return true;
}
 
//----------------------------------------------------------------------
// Function : errorProtocol
// Purpose  : overload handler function
//----------------------------------------------------------------------  
QString SALOME_ModuleCatalog_Handler::errorProtocol()
{
  return errorProt ;
}


//----------------------------------------------------------------------
// Function : fatalError
// Purpose  : overload handler function
//----------------------------------------------------------------------  
bool SALOME_ModuleCatalog_Handler::fatalError(const QXmlParseException& exception)
{
    errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
    .arg( exception.message() )
    .arg( exception.lineNumber() )
    .arg( exception.columnNumber() );

  return QXmlDefaultHandler::fatalError( exception );
}
