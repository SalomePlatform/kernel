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
using namespace std;

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalog_Handler
// Purpose  : Constructor
//----------------------------------------------------------------------
SALOME_ModuleCatalog_Handler::SALOME_ModuleCatalog_Handler()
{
  MESSAGE("SALOME_ModuleCatalog_Handler creation")
  // XML Tags initialisation
  // Used in the function endElement
  test_path_prefix_name = "path-prefix-name";
  test_computer_name = "computer-name" ;
  test_path_prefix = "path-prefix" ;

  test_component_name = "component-name";
  test_component_username = "component-username";
  test_component_type = "component-type" ;
  test_component_multistudy="component-multistudy";
  test_component_icone="component-icone" ;

  test_interface_name = "component-interface-name" ;
  
  test_service_name = "service-name";
  test_defaultservice = "service-by-default";

  test_inParameter_type="inParameter-type";
  test_inParameter_name="inParameter-name";
  test_inParameter="inParameter";
  test_inParameter_list="inParameter-list";

  test_outParameter_type="outParameter-type";
  test_outParameter_name="outParameter-name";
  test_outParameter="outParameter";
  test_outParameter_list="outParameter-list";

  test_service= "component-service";
  test_service_list="component-service-list";
  test_interface_list="component-interface-list";

  test_constraint="constraint";

  test_component="component";
}

//----------------------------------------------------------------------
// Function : ~SALOME_ModuleCatalog_Handler
// Purpose  : Destructor
//----------------------------------------------------------------------
SALOME_ModuleCatalog_Handler::~SALOME_ModuleCatalog_Handler()
{
  MESSAGE("SALOME_ModuleCatalog_Handler destruction")
}

//----------------------------------------------------------------------
// Function : startDocument
// Purpose  : overload handler function
//----------------------------------------------------------------------
bool SALOME_ModuleCatalog_Handler::startDocument()
{
  MESSAGE("Begin parse document")
  // Empty the private elements
  _pathlist.resize(0);
  _path_prefix.ListOfComputer.resize(0);
  _servicelist.resize(0);
  _interfacelist.resize(0);
  _modulelist.resize(0);
  _inparamlist.resize(0);
  _outparamlist.resize(0);
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
  // Path prefix

  // tag test_path_prefix_name
  if((qName.compare(QString(test_path_prefix_name))==0))
    _path_prefix.path = content;
  // tag test_computer_name
  if((qName.compare(QString(test_computer_name))==0)) 
    _path_prefix.ListOfComputer.push_back(content);
   
  // tag test_path_prefix
  if((qName.compare(QString(test_path_prefix))==0))
    {
      _pathlist.push_back(_path_prefix);
      _path_prefix.ListOfComputer.resize(0);
    }

  // Component identification

  // tag test_component_name
  if((qName.compare(QString(test_component_name))==0)) 
    _amodule.Parsercomponentname = content ;
  // tag test_component_username
  if((qName.compare(QString(test_component_username))==0)) 
    _amodule.Parsercomponentusername = content ;
  // tag test_component_type
   if((qName.compare(QString(test_component_type))==0)) 
     {
       if ((content.compare("MESH") == 0) ||
	   (content.compare("Mesh") == 0) ||
	   (content.compare("mesh") == 0))
	 _amodule.Parsercomponenttype = MESH ;
       else if((content.compare("MED") == 0) ||
	       (content.compare("Med") == 0) ||
	       (content.compare("med") == 0))
	 _amodule.Parsercomponenttype = Med ;
       else if((content.compare("GEOM") == 0) ||
	       (content.compare("Geom") == 0) ||
	       (content.compare("geom") == 0))
	 _amodule.Parsercomponenttype = GEOM ;
       else if((content.compare("SOLVER") == 0) ||
	       (content.compare("Solver") == 0) ||
	       (content.compare("solver") == 0))
	 _amodule.Parsercomponenttype = SOLVER ;
       else if((content.compare("SUPERV") == 0) ||
	       (content.compare("Superv") == 0) ||
	       (content.compare("Supervision") == 0) ||
	       (content.compare("superv") == 0))
	 _amodule.Parsercomponenttype = SUPERV ;
       else if((content.compare("DATA") == 0) ||
	       (content.compare("Data") == 0) ||
	       (content.compare("data") == 0))
	 _amodule.Parsercomponenttype = DATA ; 
       else if((content.compare("VISU") == 0) ||
	       (content.compare("Visu") == 0) ||
	       (content.compare("visu") == 0))
	 _amodule.Parsercomponenttype = VISU ; 
       else if((content.compare("OTHER") == 0) ||
	       (content.compare("Other") == 0) ||
	       (content.compare("other") == 0))                
	 _amodule.Parsercomponenttype = OTHER ;
       else
	 // If it'not in all theses cases, the type is affected to OTHER
	 _amodule.Parsercomponenttype = OTHER ;
     }

   // tag test_component_multistudy
  if((qName.compare(QString(test_component_multistudy))==0)) 
    _amodule.Parsercomponentmultistudy = atoi(content.c_str()) ;

  // tag test_component_icone
  if((qName.compare(QString(test_component_icone))==0)) 
    _amodule.Parsercomponenticone = content ;

   // interface identification

   // tag test_interface_name
   if((qName.compare(QString(test_interface_name))==0)) 
       _aInterface.Parserinterfacename = content ;

   // Service identification

   // tag test_service_name
   if((qName.compare(QString(test_service_name))==0))
     _aService.ParserServiceName = content ;
     
   //tag test_defaultservice
   if((qName.compare(QString(test_defaultservice))==0))
     _aService.ParserServicebydefault = atoi(content.c_str()) ;

   // Parameter in

   // tag test_inParameter_type
   if((qName.compare(QString(test_inParameter_type))==0))
     _inparam.ParserParamtype = content ;
   //tag test_inParameter_name
   if((qName.compare(QString(test_inParameter_name))==0))
     _inparam.ParserParamname = content ; 

   //tag test_inParameter
  if((qName.compare(QString(test_inParameter))==0))
    {
      _inparamlist.push_back(_inparam) ; 

      // Empty temporary structures
      _inparam.ParserParamtype = "";
      _inparam.ParserParamname = "";
    }
  
   //tag test_inParameter_list
   if((qName.compare(QString(test_inParameter_list))==0))
     {
       _aService.ParserServiceinParameter = _inparamlist;
       _inparamlist.resize(0);
     }

   // Parameter out

   // tag test_outParameter_type
   if((qName.compare(QString(test_outParameter_type))==0))
     _outparam.ParserParamtype = content ;
   //tag test_outParameter_name
   if((qName.compare(QString(test_outParameter_name))==0))
     _outparam.ParserParamname = content ; 
   
   //tag test_outParameter
  if((qName.compare(QString(test_outParameter))==0))
    {
      _outparamlist.push_back(_outparam) ; 
     
      // Empty temporary structures
      _outparam.ParserParamtype = "";
      _outparam.ParserParamname = "";
    }
   //tag test_outParameter_list
   if((qName.compare(QString(test_outParameter_list))==0)) 
     {
       _aService.ParserServiceoutParameter=_outparamlist;
       _outparamlist.resize(0);
     }
     

   // tag   test_service
   if((qName.compare(QString(test_service))==0))
     {
       _servicelist.push_back(_aService);

       // Empty temporary structures
       _aService.ParserServiceName = "";
       _aService.ParserServiceinParameter.resize(0);
       _aService.ParserServiceoutParameter.resize(0);
     }

   // tag   test_service_list
   if((qName.compare(QString(test_service_list))==0))
     {
       _aInterface.Parserinterfaceservicelist = _servicelist ;

       // Empty temporary structures
       _servicelist.resize(0); 
       _interfacelist.push_back(_aInterface);  
       _aInterface.Parserinterfacename ="";    
       _aInterface.Parserinterfaceservicelist.resize(0);

     }

   //tag test_interface_list
   if((qName.compare(QString(test_interface_list))==0))
     {
       _amodule.ParserListInterface = _interfacelist ;
       _interfacelist.resize(0);
     }

   //tag test_constraint
   if((qName.compare(QString(test_constraint))==0))
     _amodule.Parserconstraint = content ;

   // tag test_component
   if((qName.compare(QString(test_component))==0))
     {
       _modulelist.push_back(_amodule) ;
       
       // Empty temporary structures
       _amodule.Parsercomponentname = "";
       _amodule.Parserconstraint = "";
       _amodule.Parsercomponenticone="";       
       _amodule.ParserListInterface.resize(0);
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
bool SALOME_ModuleCatalog_Handler::endDocument()
{
  //_pathlist
  for (unsigned int ind = 0; ind < _pathlist.size(); ind++)
    {
      MESSAGE("Path :"<<_pathlist[ind].path)
      for (unsigned int i = 0; i < _pathlist[ind].ListOfComputer.size(); i++)
	  MESSAGE("Computer name :" << _pathlist[ind].ListOfComputer[i])
    }

   // _modulelist
  for (unsigned int ind = 0; ind < _modulelist.size(); ind++)
    {
      MESSAGE("Component name : " << _modulelist[ind].Parsercomponentname);
      //      MESSAGE("Component type : " <<_modulelist[ind].Parsercomponenttype);
//       MESSAGE("Component constraint : " << _modulelist[ind].Parserconstraint);
//       MESSAGE("Component icone : " << _modulelist[ind].Parsercomponenticone);
//       for (unsigned int i = 0; i < _modulelist[ind].ParserListInterface.size(); i++)
// 	{
// 	  MESSAGE("Component interface name : " << _modulelist[ind].ParserListInterface[i].Parserinterfacename);
// 	  for (unsigned int j=0; j< _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist.size(); j++)
// 	    {
// 	      MESSAGE("Service name : " << _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceName);
// 	      MESSAGE("Service default : " << _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServicebydefault);
// 	      for (unsigned int k=0; k< _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceinParameter.size() ; k++)
// 		{
// 		  MESSAGE("Service Parameter in name : " <<_modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceinParameter[k].ParserParamname);
// 		  MESSAGE("Service Parameter in type : " << _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceinParameter[k].ParserParamtype);
// 		}
// 	      for (unsigned int k=0; k< _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceoutParameter.size() ; k++)
// 		{
// 		  MESSAGE("Service Parameter out name : " << _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceoutParameter[k].ParserParamname);
// 		  MESSAGE("Service Parameter out type : " << _modulelist[ind].ParserListInterface[i].Parserinterfaceservicelist[j].ParserServiceoutParameter[k].ParserParamtype);
// 		}
// 	    }  
       }
//     }
  //MESSAGE("Document parsed");
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
