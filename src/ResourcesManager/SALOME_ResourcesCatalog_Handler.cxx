//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
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
//  File   : SALOME_ResourcesCatalog_Handler.cxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#include "SALOME_ResourcesCatalog_Handler.hxx"
#include <iostream>
#include <map>
#include <qdom.h>
#include "utilities.h"

using namespace std;

//----------------------------------------------------------------------
//Function : SALOME_ResourcesCatalog_Handler
//Purpose: Constructor
//----------------------------------------------------------------------
SALOME_ResourcesCatalog_Handler::SALOME_ResourcesCatalog_Handler(MapOfParserResourcesType& listOfResources):_resources_list(listOfResources)
{
  MESSAGE("SALOME_ResourcesCatalog_Handler creation");
  //XML tags initialisation
  test_machine="machine";
  test_resources="resources";

  test_hostname="hostname";
  test_alias="alias";
  test_protocol="protocol";
  test_mode="mode";
  test_user_name="userName";
  test_modules="modules";
  test_module_name="moduleName";
  test_module_path="modulePath";
  test_pre_req_file_path="preReqFilePath";
  test_os="OS";
  test_mem_in_mb="memInMB";
  test_cpu_freq_mhz="CPUFreqMHz";
  test_nb_of_nodes="nbOfNodes";
  test_nb_of_proc_per_node="nbOfProcPerNode";
}

//----------------------------------------------------------------------
//Function : ~SALOME_ResourcesCatalog_Handler
//Purpose: Destructor
//----------------------------------------------------------------------
SALOME_ResourcesCatalog_Handler::~SALOME_ResourcesCatalog_Handler()
{
  MESSAGE("SALOME_ResourcesCatalog_Handler destruction");
}

//----------------------------------------------------------------------
//Function : GetResourcesAfterParsing
//Purpose: Retrieves DS after the file parse.
//----------------------------------------------------------------------
const MapOfParserResourcesType& SALOME_ResourcesCatalog_Handler::GetResourcesAfterParsing() const
{
  return _resources_list;
}

//----------------------------------------------------------------------
//Function : startDocument
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool SALOME_ResourcesCatalog_Handler::startDocument()
{
  MESSAGE("Begin parse document");
  // Empty private elements
  _resources_list.clear();
  return true;
}

//----------------------------------------------------------------------
//Function : startElement
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool SALOME_ResourcesCatalog_Handler::startElement(const QString&, 
					       const QString&,
					       const QString& name, 
					       const QXmlAttributes& attrs)
{
  for(int i=0;i<attrs.count();i++)
    {
      QString qName(attrs.localName(i));
      std::string content(attrs.value(i));
      if((qName.compare(QString(test_hostname))==0))
	_resource.DataForSort._hostName = content;
      if((qName.compare(QString(test_alias))==0))
	_resource.Alias = content;
      if((qName.compare(QString(test_protocol))==0)){
	switch(content[0]) {
	case 'r':
	  _resource.Protocol = rsh;
	  break;
	case 's':
	  _resource.Protocol = ssh;
	  break; 
	default:
	  // If it'not in all theses cases, the protocol is affected to rsh
	  _resource.Protocol = rsh;
	  break;
	}
      }
      if((qName.compare(QString(test_mode))==0))
	{
	  switch(content[0]) {
	  case 'i':
	    _resource.Mode = interactive;
	    break;
	  case 'b':
	    _resource.Mode = batch;
	    break; 
	  default:
	    // If it'not in all theses cases, the mode is affected to interactive
	    _resource.Mode = interactive;
	    break;
	  }
	}
      if((qName.compare(QString(test_user_name))==0))
	_resource.UserName = content;
      if((qName.compare(QString(test_module_name))==0))
	previous_module_name = content;
      if((qName.compare(QString(test_module_path))==0))
	previous_module_path = content;
      if((qName.compare(QString(test_pre_req_file_path))==0))
	_resource.PreReqFilePath = content;
      if((qName.compare(QString(test_os))==0))
	_resource.OS = content;
      if((qName.compare(QString(test_mem_in_mb))==0))
	  _resource.DataForSort._memInMB = atoi(content.c_str());
      if((qName.compare(QString(test_cpu_freq_mhz))==0))
	_resource.DataForSort._CPUFreqMHz = atoi(content.c_str());
      if((qName.compare(QString(test_nb_of_nodes))==0))
	_resource.DataForSort._nbOfNodes = atoi(content.c_str());
      if((qName.compare(QString(test_nb_of_proc_per_node))==0))
	_resource.DataForSort._nbOfProcPerNode = atoi(content.c_str());
    }
  return true;
}

//----------------------------------------------------------------------
//Function : endElement
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool SALOME_ResourcesCatalog_Handler::endElement(const QString&, const QString&,
					     const QString& qName)
{
  if((qName.compare(QString(test_modules))==0))
    _resource.ModulesPath[previous_module_name] = previous_module_path;
  if((qName.compare(QString(test_machine))==0))
    _resources_list[_resource.DataForSort._hostName]=_resource;
  return true;
}

//----------------------------------------------------------------------
//Function : characters
//Purpose: overload handler function
//----------------------------------------------------------------------
bool SALOME_ResourcesCatalog_Handler::characters(const QString& chars)
{
  content = (const char *)chars ;
  return true;
}

//----------------------------------------------------------------------
//Function : endDocument
//Purpose: overload handler function 
//----------------------------------------------------------------------
bool SALOME_ResourcesCatalog_Handler::endDocument()
{
  //_resources_list
//   for (unsigned int ind = 0; ind < _resources_list.size(); ind++)
//     {
//       MESSAGE("Resources name :"<<_resources_list[ind].Parsername);
//       MESSAGE("OS :"<<_resources_list[ind].ParserOS); 
//       MESSAGE("OS version :"<<_resources_list[ind].ParserOS_version);
//       for (unsigned int i = 0; i < _resources_list[ind].Parserprocs.size(); i++)
// 	{
// 	  MESSAGE("Proc number :" << _resources_list[ind].Parserprocs[i].Parsernumber);
// 	  MESSAGE("Model name :" << _resources_list[ind].Parserprocs[i].Parsermodel_name);
// 	  MESSAGE("CPU(MHz) :" << _resources_list[ind].Parserprocs[i].Parsercpu_mhz);
// 	  MESSAGE("Cache :" << _resources_list[ind].Parserprocs[i].Parsercache_size);
// 	}
//       for (unsigned int j = 0; j < _resources_list[ind].Parsercontainertype.size(); j++)
// 	MESSAGE("Container Type :" << _resources_list[ind].Parsercontainertype[j]);
//     }
  cout << "This is the end of document" << endl;
  return true;
}

//----------------------------------------------------------------------
//Function : errorProtocol
//Purpose: overload handler function
//----------------------------------------------------------------------
QString SALOME_ResourcesCatalog_Handler::errorProtocol()
{
  cout << "error prot !!!!!!!!!!!!!!!!!" << endl;
  return errorProt;
}

//----------------------------------------------------------------------
//Function : fatalError
//Purpose: overload handler function
//----------------------------------------------------------------------
bool 
SALOME_ResourcesCatalog_Handler::fatalError(const QXmlParseException& exception)
{
  cout << "fatal error !!!!!!!!!!!!!!!!!" << endl;
    errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
    .arg( exception.message() )
    .arg( exception.lineNumber() )
    .arg( exception.columnNumber() );

  return QXmlDefaultHandler::fatalError( exception );
}

//----------------------------------------------------------------------
//Function : FillDocument
//Purpose: Fill the document tree in xml file, used to write in xml file.
//----------------------------------------------------------------------
void SALOME_ResourcesCatalog_Handler::PrepareDocToXmlFile(QDomDocument& doc)
{
  QDomElement root = doc.createElement("resources");
  doc.appendChild(root);
  for(map<string, ParserResourcesType>::iterator iter=_resources_list.begin();iter!=_resources_list.end();iter++)
    {
      QDomElement eltRoot = doc.createElement(test_machine);
      root.appendChild( eltRoot );
      eltRoot.setAttribute((char *)test_hostname,(*iter).first.c_str());
      eltRoot.setAttribute((char *)test_alias,(*iter).second.Alias.c_str());
      switch((*iter).second.Protocol)
	{
	case rsh:
	  eltRoot.setAttribute((char *)test_protocol,"rsh");
	  break;
	case ssh:
	  eltRoot.setAttribute((char *)test_protocol,"ssh");
	  break;
	default:
	  eltRoot.setAttribute((char *)test_protocol,"rsh");
	}
      switch((*iter).second.Mode)
	{
	case interactive:
	  eltRoot.setAttribute((char *)test_mode,"interactive");
	  break;
	case batch:
	  eltRoot.setAttribute((char *)test_mode,"batch");
	  break;
	default:
	  eltRoot.setAttribute((char *)test_mode,"interactive");
	}
      eltRoot.setAttribute((char *)test_user_name,(*iter).second.UserName.c_str());
      for(map<string, string>::const_iterator iter2=(*iter).second.ModulesPath.begin();iter2!=(*iter).second.ModulesPath.end();iter2++)
	{
	  QDomElement rootForModulesPaths=doc.createElement(test_modules);
	  rootForModulesPaths.setAttribute(test_module_name,(*iter2).first.c_str());
	  rootForModulesPaths.setAttribute(test_module_path,(*iter2).second.c_str());
	  eltRoot.appendChild(rootForModulesPaths);
	}
      eltRoot.setAttribute(test_pre_req_file_path,(*iter).second.PreReqFilePath.c_str());
      eltRoot.setAttribute(test_os,(*iter).second.OS.c_str());
      eltRoot.setAttribute(test_mem_in_mb,(*iter).second.DataForSort._memInMB);
      eltRoot.setAttribute(test_cpu_freq_mhz,(*iter).second.DataForSort._CPUFreqMHz);
      eltRoot.setAttribute(test_nb_of_nodes,(*iter).second.DataForSort._nbOfNodes);
      eltRoot.setAttribute(test_nb_of_proc_per_node,(*iter).second.DataForSort._nbOfProcPerNode);
    }
}
