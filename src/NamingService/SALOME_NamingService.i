//=============================================================================
// File      : SALOME_NamingService.i
// Created   : mer aoû 29 11:26:49 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

//%module SALOME_NamingService
%{
#include "SALOME_NamingService.hxx"
#include <omniORB3/CORBA.h>
#include "ServiceUnreachable.hxx"
%}

%except(python)
{
  try 
    {
      $function
    }
  catch (ServiceUnreachable)
    {
      PyErr_SetString(PyExc_IndexError,"Naming Service Unreacheable");
      return NULL;
    }
}

class SALOME_NamingService
{
public:

  //! standard constructor
  SALOME_NamingService();

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

  //! method to destroy an association Path-Object Reference
  void Destroy_Name(const char* Path)
    throw(ServiceUnreachable);
 
  //! method to destroy a directory if it is empty
  virtual void Destroy_Directory(const char* Path)
    throw(ServiceUnreachable);
};

