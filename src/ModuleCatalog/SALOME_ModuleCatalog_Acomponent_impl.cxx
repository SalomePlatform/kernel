using namespace std;
// File: SALOME_ModuleCatalog_Acomponent_impl.cxx
// Created: Tue June 28 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$


#include "SALOME_ModuleCatalog_Acomponent_impl.hxx"

//----------------------------------------------------------------------
// Function : SALOME_ModuleCatalog_AcomponentImpl
// Purpose  : Constructor
//            Affect the component name, type,icone,  a bool to define 
//            if it's multistudy or not.
//            Affect too the constraint and the interfaces of the component
//            and the pathes prefixes for all computers
//----------------------------------------------------------------------  
SALOME_ModuleCatalog_AcomponentImpl::SALOME_ModuleCatalog_AcomponentImpl(
                   const char* name,
                   const char* username,
		   const char* constraint,  
		   SALOME_ModuleCatalog::ComponentType componenttype,
		   CORBA::Boolean componentmultistudy,
		   const char* icone,
		   SALOME_ModuleCatalog::ListOfDefInterface list_interfaces,
		   ListOfPathPrefix pathes)
{
  MESSAGE("Component creation")
  // Affect component name
  _component_name = new char[strlen(name)+1];
 strcpy(_component_name, name);

  // Affect component user name
  _component_user_name = new char[strlen(username)+1];
 strcpy(_component_user_name, username);

 // Affect constraint
 _constraint =new char[strlen(constraint)+1];
 strcpy(_constraint, constraint);

 // Affect component type
 _componenttype = componenttype;
 
 // Affect component multistudy
 _componentmultistudy = componentmultistudy;

 // Affect icone
 _icone =new char[strlen(icone)+1];
 strcpy(_icone, icone);

 //Affect interfaces
 _list_interfaces.length(list_interfaces.length());
 _list_interfaces = list_interfaces;

 // affect path prefixes
 _pathes.resize(pathes.size());
 _pathes = pathes ;
}

//----------------------------------------------------------------------
// Function : ~SALOME_ModuleCatalog_AcomponentImpl
// Purpose  : Destructor 
//----------------------------------------------------------------------
SALOME_ModuleCatalog_AcomponentImpl::~SALOME_ModuleCatalog_AcomponentImpl()
{
  MESSAGE("Component destruction")
  // empty memory
  delete [] _component_name;
  delete [] _component_user_name;
  delete [] _constraint;
}

//----------------------------------------------------------------------
// Function : GetInterfaceList
// Purpose  : get a list of the interfaces name of a component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ListOfInterfaces* 
SALOME_ModuleCatalog_AcomponentImpl::GetInterfaceList() 
{
  SALOME_ModuleCatalog::ListOfInterfaces_var _list = new SALOME_ModuleCatalog::ListOfInterfaces;

  // All the interfaces are defined in _list_interfaces affected at the
  // component creation
  unsigned int _length_interfaces =_list_interfaces.length();

  _list->length(_length_interfaces);

  // Parse all the interfaces to get their name
  for (unsigned int ind = 0; ind < _length_interfaces; ind++)
    {
      _list[ind] = CORBA::string_dup(_list_interfaces[ind].interfacename);
      MESSAGE("The component " << _component_name << " contains " << _list[ind] << " as interface") 
    }
  
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
  SALOME_ModuleCatalog::DefinitionInterface_var _interface = new SALOME_ModuleCatalog::DefinitionInterface;
  SALOME_ModuleCatalog::Service_var _service = new SALOME_ModuleCatalog::Service;
  // Variables initialisation
  _interface->interfaceservicelist.length(0);
  int _length = 0;
  bool _find = false ;
  
  // looking for the specified interface
  for (unsigned int ind = 0; ind < _list_interfaces.length(); ind++)
    {
      if (strcmp(interfacename, _list_interfaces[ind].interfacename) == 0)
	{
	  // wanted interface
	  _find = true ;

	  // Affect the name of the interface
	  _interface->interfacename = interfacename;

	  // Affect each service of the wanted interface
	  for (unsigned int ind1 = 0; ind1 <  _list_interfaces[ind].interfaceservicelist.length() ; ind1++)
	    {
	      _interface->interfaceservicelist.length(_length+1);
	      _service = _duplicate_service(_list_interfaces[ind].interfaceservicelist[ind1]);
	      _interface->interfaceservicelist[_length] = _service;	
	     
	      _length++;
	    }

	}
    }
  
  if (!_find)
    {
      // The interface was not found, the exception should be thrown
      char * message = new char[100];
      strcpy(message, "The interface ");
      strcat(message,interfacename);
      strcat(message, " of the component ");
      strcat(message,_component_name); 
      strcat(message, " was not found") ;
      MESSAGE("The interface " << interfacename << " of the component " << _component_name << " was not found")
	throw SALOME_ModuleCatalog::NotFound(message);
      delete [] message;
    }

  return _interface._retn();
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
  SALOME_ModuleCatalog::ListOfServices_var _list = new SALOME_ModuleCatalog::ListOfServices;

  // Variables initialisation
  _list->length(0); 
  bool _find = false ;

  // looking for the specified interface
  for (unsigned int ind = 0; ind < _list_interfaces.length(); ind++)
    {
      if (strcmp(interfacename, _list_interfaces[ind].interfacename) == 0)
	{
	  _find = true ;
	  // wanted interface
	  // Get the list of services name for this interface
	  unsigned int _length_services = _list_interfaces[ind].interfaceservicelist.length();
	  _list->length(_length_services);
	  for (unsigned int ind1 = 0; ind1 < _length_services ; ind1++)
	    {
	      _list[ind1] = CORBA::string_dup(_list_interfaces[ind].interfaceservicelist[ind1].ServiceName);
	      MESSAGE("The interface " << interfacename << " of the component " << _component_name << " contains " << _list[ind1] << " as a service") 
	    }
	}
    }

 if (!_find)
    {
      // The interface was not found, the exception should be thrown
      char * message = new char[100];
      strcpy(message, "The interface ");
      strcat(message,interfacename);
      strcat(message, " of the component ");
      strcat(message,_component_name); 
      strcat(message, " was not found") ;
      MESSAGE("The interface " << interfacename << " of the component " << _component_name << " was not found")
	throw SALOME_ModuleCatalog::NotFound(message);
      delete [] message;
    }

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
  SALOME_ModuleCatalog::Service_var _service = new SALOME_ModuleCatalog::Service;
  // Varaibles initialisation
  bool _find = false ;
  
  // looking for the specified interface
  for (unsigned int ind = 0; ind < _list_interfaces.length(); ind++)
    {
      if (strcmp(interfacename, _list_interfaces[ind].interfacename) == 0)
	{
	  // wanted interface
	  // looking for the specified service
	  for (unsigned int ind1 = 0; ind1 <  _list_interfaces[ind].interfaceservicelist.length() ; ind1++)
	    {
	      if (strcmp(servicename, _list_interfaces[ind].interfaceservicelist[ind1].ServiceName) == 0)
	      {
		// Wanted Service
		// Affect the service to be returned
		_find = true ;
		_service = _duplicate_service(_list_interfaces[ind].interfaceservicelist[ind1]);
	      }
	    }

	}
    }
  
  if (!_find)
    {
      // The service was not found, the exception should be thrown
      char * message = new char[100];
      strcpy(message, "The service ");
      strcat(message, servicename);
      strcat(message," of the interface ");
      strcat(message,interfacename);
      strcat(message, " of the component ");
      strcat(message,_component_name); 
      strcat(message, " was not found") ;
      MESSAGE("The service " << servicename << " of the interface " << interfacename << " of the component " << _component_name << " was not found")
	throw SALOME_ModuleCatalog::NotFound(message);
      delete [] message;
    }

  return _service._retn();
}

//----------------------------------------------------------------------
// Function : GetDefaultService
// Purpose  : get the default service of the interface
//----------------------------------------------------------------------
SALOME_ModuleCatalog::Service* 
SALOME_ModuleCatalog_AcomponentImpl::GetDefaultService(const char* interfacename) 
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  SALOME_ModuleCatalog::Service_var _service = new SALOME_ModuleCatalog::Service;

  // Variables initialisation
  bool _find = false ;

  // looking for the specified interface
  for (unsigned int ind = 0; ind < _list_interfaces.length(); ind++)
    {
      if (strcmp(interfacename, _list_interfaces[ind].interfacename) == 0)
	{
	  // wanted interface
	  // looking for the defautl service of the wanted interface
	  for (unsigned int ind1 = 0; ind1 <  _list_interfaces[ind].interfaceservicelist.length() ; ind1++)
	    {
	      if (_list_interfaces[ind].interfaceservicelist[ind1].Servicebydefault)
	      {
		// Default Service
		// affect the service to be returned
		_find = true ;
		_service = _duplicate_service(_list_interfaces[ind].interfaceservicelist[ind1]);
	      }
	    }

	}
    }

  if (!_find)
    {
      // The service was not found, the exception should be thrown
      char * message = new char[100];
      strcpy(message, "The default service of the interface ");
      strcat(message,interfacename);
      strcat(message, " of the component ");
      strcat(message,_component_name); 
      strcat(message, " was not found") ;
      MESSAGE("The default service of the interface " << interfacename << " of the component " << _component_name << " was not found")
	throw SALOME_ModuleCatalog::NotFound(message);
      delete [] message;
    }

  return _service._retn();
}

//----------------------------------------------------------------------
// Function : GetPathPrefix
// Purpose  : get the PathPrefix of a computer
//----------------------------------------------------------------------
char* 
SALOME_ModuleCatalog_AcomponentImpl::GetPathPrefix(const char* machinename) 
                                     throw(SALOME_ModuleCatalog::NotFound)
{
  MESSAGE("Begin of GetPathPrefix")
  // Variables initialisation
  char* _path = NULL;
  bool _find = false ;

  // Parse all the path prefixes
  // looking for the wanted computer
  for (unsigned int ind = 0 ; ind < _pathes.size() ; ind++)
    {
      for (unsigned int ind1 = 0 ; ind1 < _pathes[ind].ListOfComputer.size() ; ind1++)    
	{
	  if (strcmp(machinename, _pathes[ind].ListOfComputer[ind1].c_str()) == 0)
	    {
	      _find = true ;
	      // Wanted computer
	      // affect the path to be returned
	        const char* _temp = _pathes[ind].path.c_str() ;
		  _path = new char[strlen(_temp)+1];
	      strcpy(_path,_temp);
	    }
	}
    }

  if (!_find)
    {
      // The computer was not found, the exception should be thrown
      char * message = new char[100];
      strcpy(message, "The computer ");
      strcat(message,machinename); 
      strcat(message, " was not found in the catalog associated to the component ") ;
      strcat(message,_component_name);      
      MESSAGE("The computer " << machinename << " was not found in the catalog associated to the component " << _component_name)
	throw SALOME_ModuleCatalog::NotFound(message);
      delete [] message;
    }

  return _path;
}

//----------------------------------------------------------------------
// Function : constraint
// Purpose  : obtain the constraint affected to a component
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::constraint() 
{
  return CORBA::string_dup(_constraint);
}

//----------------------------------------------------------------------
// Function : componentname
// Purpose  : obtain the componentname
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::componentname()
{
  return CORBA::string_dup(_component_name);
}

//----------------------------------------------------------------------
// Function : componentusername
// Purpose  : obtain the componentusername
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::componentusername()
{
  return CORBA::string_dup(_component_user_name);
}

//----------------------------------------------------------------------
// Function : multistudy
// Purpose  : define if a component can be multistudy or not
//----------------------------------------------------------------------
CORBA::Boolean SALOME_ModuleCatalog_AcomponentImpl::multistudy()
{
  return _componentmultistudy ;
}

//----------------------------------------------------------------------
// Function : component_type
// Purpose  : define the type of the component
//----------------------------------------------------------------------
SALOME_ModuleCatalog::ComponentType 
SALOME_ModuleCatalog_AcomponentImpl::component_type() 
{
  return _componenttype;
}

//----------------------------------------------------------------------
// Function : icone
// Purpose  : obtain the icone affected to a component (for IAPP)
//----------------------------------------------------------------------
char* SALOME_ModuleCatalog_AcomponentImpl::component_icone() 
{
  return CORBA::string_dup(_icone);
}

//----------------------------------------------------------------------
// Function :  _duplicate_service
// Purpose  : duplicate a service
//----------------------------------------------------------------------
SALOME_ModuleCatalog::Service_var 
SALOME_ModuleCatalog_AcomponentImpl::_duplicate_service(SALOME_ModuleCatalog::Service service)
{
  SALOME_ModuleCatalog::Service_var _service = new SALOME_ModuleCatalog::Service;

  // service name
  _service->ServiceName = CORBA::string_dup(service.ServiceName);
  // service by default
  _service->Servicebydefault = service.Servicebydefault;

  // in Parameters service
  unsigned int _length_in_param = service.ServiceinParameter.length();
  for(unsigned int ind = 0; ind < _length_in_param; ind++)
    {
      _service->ServiceinParameter.length(_length_in_param);
      // in Parameter type
      _service->ServiceinParameter[ind].Parametertype = CORBA::string_dup(service.ServiceinParameter[ind].Parametertype);
      // in Parameter name
       _service->ServiceinParameter[ind].Parametername = CORBA::string_dup(service.ServiceinParameter[ind].Parametername);
    }

   // out Parameters service
  unsigned int _length_out_param = service.ServiceoutParameter.length();
  for(unsigned int ind = 0; ind < _length_out_param; ind++)
    {
      _service->ServiceoutParameter.length(_length_out_param);
      // out Parameter type
      _service->ServiceoutParameter[ind].Parametertype = CORBA::string_dup(service.ServiceoutParameter[ind].Parametertype);
      // out Parameter name
       _service->ServiceoutParameter[ind].Parametername = CORBA::string_dup(service.ServiceoutParameter[ind].Parametername);
    }    

  return _service;
}
