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

//  SALOME NamingService : wrapping NamingService services
//  File   : SALOME_NamingService.i
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$
//
%{
#include "SALOME_NamingService.hxx"
//#include <omniORB4/CORBA.h>
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

