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
//  File   : SALOME_ModuleCatalog_Acomponent_impl.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOME_ModuleCatalog_Acomponent_impl.hxx"

#include "Utils_ExceptHandlers.hxx"
UNEXPECT_CATCH(MC_NotFound, SALOME_ModuleCatalog::NotFound);

#include "utilities.h"

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalog_AcomponentImpl
// Purpose  : Constructor
//            Affect the component name, type,icone,  a bool to define 
//            if it's multistudy or not.
//            Affect too the constraint and the interfaces of the component
//            and the pathes prefixes for all computers
//----------------------------------------------------------------------  
SALOME_ModuleCatalog_AcomponentImpl::SALOME_ModuleCatalog_AcomponentImpl
(SALOME_ModuleCatalog::ComponentDef &C) : _Component(C)
{
  if(MYDEBUG) BEGIN_OF("SALOME_ModuleCatalog_AcomponentImpl");

  if(MYDEBUG) END_OF("SALOME_ModuleCatalog_AcomponentImpl");
}

//----------------------------------------------------------------------
// Function : ~SALOME_ModuleCatalog_AcomponentImpl
// Purpose  : Destructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalog_AcomponentImpl::~SALOME_ModuleCatalog_AcomponentImpl()
{
  if(MYDEBUG) BEGIN_OF("~SALOME_ModuleCatalog_AcomponentImpl");


  if(MYDEBUG) END_OF("~SALOME_ModuleCatalog_AcomponentImpl");
}

//----------------------------------------------------------------------
// Function : GetInterfaceList
// Purpose  : get a list of the interfaces name of a component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfInterfaces* 
SALOME_ModuleCatalog_AcomponentImpl::GetInterfaceList() 
{
  if(MYDEBUG) BEGIN_OF("GetInterfaceList");

  SALOME_ModuleCatalog::ListOfInterfaces_var _list 
    = new SALOME_ModuleCatalog::ListOfInterfaces;

  // All the interfaces are defined in _list_interfaces affected at the
  // component creation
  unsigned int _length_interfaces = _Component.interfaces.length();

  _list->length(_length_interfaces);

  // Parse all the interfaces to get their name
  for (unsigned int ind = 0; ind < _length_interfaces; ind++)
    {
      _list[ind] = CORBA::string_dup(_Component.interfaces[ind].interfacename);
      if(MYDEBUG) MESSAGE("The component " << _Component.name 
                          << " contains " << _list[ind] << " as interface");
    }
  
  if(MYDEBUG) END_OF("GetInterfaceList");
  return _list._retn();
}

//----------------------------------------------------------------------
// Function : GetInterface
// Purpose  : get one interface of a component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::DefinitionInterface*
SALOME_ModuleCatalog_AcomponentImpl::GetInterface(const char* interfacename)
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  if(MYDEBUG) BEGIN_OF("GetInterface");
  if(MYDEBUG) SCRUTE(interfacename);

  SALOME_ModuleCatalog::DefinitionInterface *_interface =
          new SALOME_ModuleCatalog::DefinitionInterface;

  bool _find = false ;
  
  // looking for the specified interface
  for (unsigned int ind = 0; ind < _Component.interfaces.length(); ind++)
    {
      SALOME_ModuleCatalog::DefinitionInterface &I
        = _Component.interfaces[ind];

      if (strcmp(interfacename, I.interfacename) == 0)
        {
          // wanted interface
          _find = true ;
          duplicate(*_interface, I);
        }
    }

  if(MYDEBUG) SCRUTE(_find);
  if (!_find)
    {
      // The interface was not found, the exception should be thrown
		std::string message = "The interface";
      message += interfacename;
      message += " of the component ";
      message += _Component.name;
      message += " was not found"; 
      if(MYDEBUG) MESSAGE(message);
      throw SALOME_ModuleCatalog::NotFound(message.c_str());
    }

  if(MYDEBUG) END_OF("GetInterface");

  return _interface;
}



//----------------------------------------------------------------------
// Function : GetServiceList
// Purpose  : get a list of the services name of an interface 
//            of a component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfServices* 
SALOME_ModuleCatalog_AcomponentImpl::GetServiceList(const char* interfacename)
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  if(MYDEBUG) BEGIN_OF("GetServiceList");
  if(MYDEBUG) SCRUTE(interfacename);

  SALOME_ModuleCatalog::ListOfServices_var _list 
    = new SALOME_ModuleCatalog::ListOfServices;

  // Variables initialisation
  bool _find = false ;

  // looking for the specified interface
  for (unsigned int ind = 0; ind < _Component.interfaces.length(); ind++)
    {
      SALOME_ModuleCatalog::DefinitionInterface & I = _Component.interfaces[ind];

      if (strcmp(interfacename, I.interfacename) == 0)
        {
          _find = true ;
          // wanted interface
          // Get the list of services name for this interface
          unsigned int _length_services = I.interfaceservicelist.length();
          _list->length(_length_services);
          for (unsigned int ind1 = 0; ind1 < _length_services ; ind1++)
            {
              _list[ind1] = CORBA::string_dup(I.interfaceservicelist[ind1].ServiceName);
              if(MYDEBUG) MESSAGE("The interface " << interfacename << " of the component " 
                                  << _Component.name << " contains " << _list[ind1] << " as a service") 
            }
        }
    }

 if (!_find)
    {
      // The interface was not found, the exception should be thrown
      std::string message = "The interface";
      message += interfacename;
      message += " of the component ";
      message += _Component.name;
      message += " was not found"; 
      if(MYDEBUG) MESSAGE(message);
      throw SALOME_ModuleCatalog::NotFound(message.c_str());
    }

  if(MYDEBUG) END_OF("GetServiceList");
  return _list._retn();
}

    
//----------------------------------------------------------------------
// Function : GetService
// Purpose  : get one service of an interface of a component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::Service* 
SALOME_ModuleCatalog_AcomponentImpl::GetService(const char* interfacename, 
                                                const char* servicename) 
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  if(MYDEBUG) BEGIN_OF("GetService");
  if(MYDEBUG) SCRUTE(interfacename);
  if(MYDEBUG) SCRUTE(servicename);

  Unexpect aCatch( MC_NotFound );
  SALOME_ModuleCatalog::Service *service = new SALOME_ModuleCatalog::Service;

  // Variables initialization
  bool _find = false ;
  

  // looking for the specified interface
  for (unsigned int ind = 0; ind < _Component.interfaces.length(); ind++)
    {
      if(MYDEBUG) SCRUTE(ind);
      if(MYDEBUG) SCRUTE(_Component.interfaces[ind].interfacename);

      SALOME_ModuleCatalog::DefinitionInterface &I = _Component.interfaces[ind];
      if (strcmp(interfacename, I.interfacename) == 0)
        {
          // wanted interface
          // looking for the specified service
          for (unsigned int ind1 = 0; ind1 <  I.interfaceservicelist.length() ; ind1++)
            {
              SALOME_ModuleCatalog::Service &S = I.interfaceservicelist[ind1];
              if(MYDEBUG) SCRUTE(ind1);
              if(MYDEBUG) SCRUTE(S.ServiceName);

              if (strcmp(servicename, S.ServiceName) == 0)
              {
                // Wanted Service
                // Affect the service to be returned
                _find = true ;
                duplicate(*service, S);
              }
            }

        }
    }
  
  if(MYDEBUG) SCRUTE(_find);
  if (!_find)
    {
      // The interface was not found, the exception should be thrown
      std::string message = "The service";
      message += servicename;
      message += " of the interface ";
      message += interfacename;
      message += " of the component ";
      message += _Component.name;
      message += " was not found"; 
      if(MYDEBUG) MESSAGE(message);
      throw SALOME_ModuleCatalog::NotFound(message.c_str());
    }

  if(MYDEBUG) END_OF("GetService");
  return service;
}

//----------------------------------------------------------------------
// Function : GetDefaultService
// Purpose  : get the default service of the interface
//----------------------------------------------------------------------
SALOME_ModuleCatalog::Service* 
SALOME_ModuleCatalog_AcomponentImpl::GetDefaultService(const char* interfacename) 
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  if(MYDEBUG) BEGIN_OF("GetDefaultService");
  if(MYDEBUG) SCRUTE(interfacename);

  Unexpect aCatch( MC_NotFound );
  SALOME_ModuleCatalog::Service *_service = new  SALOME_ModuleCatalog::Service;

  // Variables initialisation
  bool _find = false ;

  // looking for the specified interface
  for (unsigned int ind = 0; ind < _Component.interfaces.length(); ind++)
    {
      if (strcmp(interfacename, _Component.interfaces[ind].interfacename) == 0)
        {
          // wanted interface
          // looking for the defautl service of the wanted interface
          for (unsigned int ind1 = 0; ind1 <  _Component.interfaces[ind].interfaceservicelist.length() ; ind1++)
            {
              if (_Component.interfaces[ind].interfaceservicelist[ind1].Servicebydefault)
              {
                // Default Service
                // affect the service to be returned
                _find = true ;
                duplicate(*_service, _Component.interfaces[ind].interfaceservicelist[ind1]);
              }
            }

        }
    }

  if (!_find)
    {
      // The service was not found, the exception should be thrown
      std::string message = "The default service of the interface ";
      message += interfacename;
      message += " of the component ";
      message += _Component.name;
      message += " was not found";
      if(MYDEBUG) MESSAGE(message);
      throw SALOME_ModuleCatalog::NotFound(message.c_str());
    }

  if(MYDEBUG) END_OF("GetDefaultService");
  return _service;
}

//----------------------------------------------------------------------
// Function : GetPathPrefix
// Purpose  : get the PathPrefix of a computer
//----------------------------------------------------------------------
char* 
SALOME_ModuleCatalog_AcomponentImpl::GetPathPrefix(const char* machinename) 
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  if(MYDEBUG) BEGIN_OF("GetPathPrefix");
  if(MYDEBUG) SCRUTE(machinename);
  Unexpect aCatch( MC_NotFound );

 // Variables initialisation
  char* _path = NULL;
  bool _find = false ;

  // Parse all the path prefixes
  // looking for the wanted computer
  for (unsigned int ind = 0 ; ind < _Component.paths.length() ; ind++)
    {
      if (strcmp(machinename, _Component.paths[ind].machine) == 0)
            {
              _find = true ;
              // Wanted computer
              // affect the path to be returned
                const char* _temp = _Component.paths[ind].path ;
                _path = new char[strlen(_temp)+1];
                strcpy(_path,_temp);
            }
     }

   if(MYDEBUG) SCRUTE(_find);
   if (!_find)
     {
       // The computer was not found, the exception should be thrown
       std::string message = "The computer ";
       message += machinename;
       message += " was not found in the catalog associated to the component ";
       message += _Component.name;
       if(MYDEBUG) MESSAGE(message);
       throw SALOME_ModuleCatalog::NotFound(message.c_str());
     }

  if(MYDEBUG) END_OF("GetPathPrefix");
  return _path;
}

//----------------------------------------------------------------------
// Function : constraint
// Purpose  : obtain the constraint affected to a component
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::constraint() 
{
  return CORBA::string_dup(_Component.constraint);
}

//----------------------------------------------------------------------
// Function : componentname
// Purpose  : obtain the componentname
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::componentname()
{
  return CORBA::string_dup(_Component.name);
}

//----------------------------------------------------------------------
// Function : componentusername
// Purpose  : obtain the componentusername
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::componentusername()
{
  return CORBA::string_dup(_Component.username);
}

//----------------------------------------------------------------------
// Function : multistudy
// Purpose  : define if a component can be multistudy or not
//----------------------------------------------------------------------
CORBA::Boolean SALOME_ModuleCatalog_AcomponentImpl::multistudy()
{
  return _Component.multistudy ;
}


//----------------------------------------------------------------------
// Function : implementation type
// Purpose  : return the implementation type :  C++ (dyn lib), Python (module) or executable
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ImplType SALOME_ModuleCatalog_AcomponentImpl::implementation_type()
{
  return _Component.implementationType ;
}

//----------------------------------------------------------------------
// Function : implementation name
// Purpose  : return the implementation name to exec if the default one is not convenient
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::implementation_name()
{
  return _Component.implname ;
}

//----------------------------------------------------------------------
// Function : component_type
// Purpose  : define the type of the component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ComponentType 
SALOME_ModuleCatalog_AcomponentImpl::component_type() 
{
  return _Component.type;
}

//----------------------------------------------------------------------
// Function : icone
// Purpose  : obtain the icone affected to a component (for IAPP)
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::component_icone() 
{
  return CORBA::string_dup(_Component.icon);
}


void SALOME_ModuleCatalog_AcomponentImpl::duplicate
(SALOME_ModuleCatalog::ServicesParameter & P_out,
 const SALOME_ModuleCatalog::ServicesParameter &P_in)
{
  // duplicate parameter name
  P_out.Parametername = CORBA::string_dup(P_in.Parametername);
  
  // duplicate parameter type
  P_out.Parametertype = CORBA::string_dup(P_in.Parametertype);
}


void SALOME_ModuleCatalog_AcomponentImpl::duplicate
(SALOME_ModuleCatalog::ServicesDataStreamParameter & P_out,
 const SALOME_ModuleCatalog::ServicesDataStreamParameter &P_in)
{
  // duplicate parameter name
  P_out.Parametername = CORBA::string_dup(P_in.Parametername);
  
  // duplicate parameter type
  P_out.Parametertype = P_in.Parametertype;

  // duplicate parameter dependency
  P_out.Parameterdependency = P_in.Parameterdependency;

}

void SALOME_ModuleCatalog_AcomponentImpl::duplicate
(SALOME_ModuleCatalog::Service & S_out,
 const SALOME_ModuleCatalog::Service &S_in)
{
  // type of node
  S_out.TypeOfNode = S_in.TypeOfNode;

    // duplicate service name
  S_out.ServiceName = CORBA::string_dup(S_in.ServiceName);
  
  // duplicate service by default
  S_out.Servicebydefault = S_in.Servicebydefault;

  unsigned int _length;

  // duplicate in Parameters
  _length = S_in.ServiceinParameter.length();
  S_out.ServiceinParameter.length(_length);

  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_out.ServiceinParameter[ind2],
              S_in.ServiceinParameter[ind2]);
  
  // duplicate out Parameters
  _length = S_in.ServiceoutParameter.length();
  S_out.ServiceoutParameter.length(_length);

  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_out.ServiceoutParameter[ind2],
              S_in.ServiceoutParameter[ind2]);
  
  // duplicate in DataStreamParameters
  _length = S_in.ServiceinDataStreamParameter.length();
  S_out.ServiceinDataStreamParameter.length(_length);

  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_out.ServiceinDataStreamParameter[ind2],
              S_in.ServiceinDataStreamParameter[ind2]);
  
  // duplicate out DataStreamParameters
  _length = S_in.ServiceoutDataStreamParameter.length();
  if(MYDEBUG) SCRUTE(_length);
  S_out.ServiceoutDataStreamParameter.length(_length);
  
  for (unsigned int ind2 = 0; ind2 < _length ; ind2 ++)
    duplicate(S_out.ServiceoutDataStreamParameter[ind2],
              S_in.ServiceoutDataStreamParameter[ind2]);
}


void SALOME_ModuleCatalog_AcomponentImpl::duplicate
(SALOME_ModuleCatalog::DefinitionInterface & I_out,
 const SALOME_ModuleCatalog::DefinitionInterface & I_in)
{
  //duplicate interface name
  I_out.interfacename = CORBA::string_dup(I_in.interfacename);
  
  // duplicate service list
  unsigned int _length = I_in.interfaceservicelist.length();
  if(MYDEBUG) SCRUTE(_length);
  I_out.interfaceservicelist.length(_length);
  
  for (unsigned int ind1 = 0; ind1 < _length ; ind1 ++)
    duplicate(I_out.interfaceservicelist[ind1],
              I_in.interfaceservicelist[ind1]);
}


