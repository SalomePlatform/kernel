// File: SALOME_NamingService.hxx
// Created: Tue June 12 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$

#ifndef SALOME_NAMINGSERVICE_H
#define SALOME_NAMINGSERVICE_H

#include "utilities.h"
#include <omniORB3/CORBA.h>
#include <vector>
#include <string>

//class ServiceUnreachable;
#include "ServiceUnreachable.hxx"

class SALOME_NamingService
{
public:
  //! default constructor
  SALOME_NamingService();

  //! standard constructor
  SALOME_NamingService(CORBA::ORB_ptr orb);

  //! standard destructor
  virtual ~SALOME_NamingService();

  //! initialize ORB reference after default constructor
  void init_orb(CORBA::ORB_ptr orb);
 
  //! method to create an association in the NamingService between an object reference and a path
  void Register(CORBA::Object_ptr ObjRef, const char* Path) 
    throw(ServiceUnreachable);

  //! method to get the ObjRef of a symbolic name
  CORBA::Object_ptr Resolve(const char* Path)
    throw( ServiceUnreachable); 

  //! method to research a name from the naming service's current directory 
  int Find(const char* name)
    throw(ServiceUnreachable);
  
  //! method to create a directory from the current directory
  bool Create_Directory(const char* Path)
    throw(ServiceUnreachable);
  
  //! method to change the current directory to the directory Path indicated in "in" Parameter
  bool Change_Directory(const char* Path)
    throw(ServiceUnreachable);
 
  //!method to get the current directory
  char* Current_Directory()
    throw(ServiceUnreachable);

  //!method to print all the contexts contained from the current directory
  void list()
    throw(ServiceUnreachable);

  //!method to get all the contexts contained in the current direcotry
  // Get only objects, isn't iterative
  vector<string> list_directory()
    throw(ServiceUnreachable);
 
  //! method to destroy an association Path-Object Reference
  void Destroy_Name(const char* Path)
    throw(ServiceUnreachable);
 
  //! method to destroy a directory if it is empty
  virtual void Destroy_Directory(const char* Path)
    throw(ServiceUnreachable);

protected:
  CORBA::ORB_ptr _orb;
  CosNaming::NamingContext_var _root_context, _current_context;

  //! method called by constructor to initialize _root_context
  void _initialize_root_context();
  
  //! method to decompose a Path : /Kernel/Services/Sessions
  char* _resolve_Path(char* Path);

  //! method to decompose a Path : /Kernel/Services/Sessions
  void _result_resolve_Path(const char* Path, int& j,
			    char ** resultat_resolve_Path);

  //! internal method called by Find to research a name from the naming service's current directory 
  void _Find(const char* name, CORBA::Long& occurence_number);

  //! internal method to create a context name from a Path
  void _create_context_name_dir(char** resultat_resolve_Path,
				int length_copy,
				CosNaming::Name& _context_name);

  //! internal method to create a parse the naming service tree 
  void _current_directory(char** result_path,
			  int& length_result,
			  CosNaming::NamingContext_var context_to_found,
			  CORBA::Boolean& _continue);
};

#endif // SALOME_NAMINGSERVICE_H
