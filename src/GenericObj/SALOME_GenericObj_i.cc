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

//  File   : SALOME_GenericObj_i.cc
//  Author : Alexey PETROV, Open CASCADE S.A.S. (alexey.petrov@opencascade.com)

#include "SALOME_GenericObj_i.hh"
#include "utilities.h"

#include <iostream>
#include <typeinfo>

// note: in KERNEL _DEBUG_ is not defined by default
#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

//#define IS_OBJ_IN_QUESTION(where) is_obj_in_question(this, myRefCounter, where)
#define IS_OBJ_IN_QUESTION(where)

namespace SALOME
{
  void is_obj_in_question( const GenericObj_i* o, int myRefCounter,const char* where)
  {
    if ( std::string( typeid(*o).name() ).find("SALOMEDS") != std::string::npos )
      return;
    // if ( std::string( typeid(*o).name() ).find("SMESH_") != std::string::npos ||
    //      std::string( typeid(*o).name() ).find("StdMesher") != std::string::npos )
    {
      std::cout<< typeid(*o).name() << " " << o << " " << where << "  myRefCounter ==> " << myRefCounter;
      if ( myRefCounter == 0 ) std::cout << " DELETE !";
      std::cout << std::endl;
    }
  }

  /*!
    \class SALOME::GenericObj_i
    \brief Implementation of the base servant for SALOME objects with reference counter.

    This class can be used to implement data entities with life-cycle management based on
    the reference counting. 

    The object is initially created with the reference counter equal to 1.
    The function Register() can be used to incrfement the reference counter.
    Function UnRegister() should be used to decrement reference counter.
    As soon as reference counter goes to zero, the object is automatically deactivated in POA
    (and, eventually its destructor is automatically called).
  */
  
  /*!
    \brief Constructor.
    Creates an object with the reference counter initially set to 1.
  
    The default POA for the servant can be passed as a parameter \a thePOA.
    By default, root POA is used.

    \param thePOA optional default POA for the servant
  */
  GenericObj_i::GenericObj_i(PortableServer::POA_ptr thePOA): myRefCounter(1)
  {
    if(MYDEBUG) 
      MESSAGE("GenericObj_i::GenericObj_i() - this = "<<this<<
	      "; CORBA::is_nil(thePOA) = "<<CORBA::is_nil(thePOA));
    if(CORBA::is_nil(thePOA)) {
#ifndef WIN32
      myPOA = PortableServer::ServantBase::_default_POA();
#else
      myPOA = ServantBase::_default_POA();
#endif
    }
    else {
      myPOA = PortableServer::POA::_duplicate(thePOA);
    }
  }

  /*!
    \brief Get default POA for the servant object.

    This function is implicitly called from "_this()" function.
    Default POA can be set via the constructor.

    \return reference to the default POA for the servant
  */
  PortableServer::POA_ptr GenericObj_i::_default_POA()
  {
    return PortableServer::POA::_duplicate(myPOA);
  }

  /*!
    \brief Increment reference counter.
  */
  void GenericObj_i::Register()
  {
    if(MYDEBUG)
      MESSAGE("GenericObj_i::Register "<<this<<"; myRefCounter = "<<myRefCounter);
    ++myRefCounter;
    IS_OBJ_IN_QUESTION( "Register" );
  }

  /*!
    \brief Decrement reference counter.

    As soon as reference counter goes to zero, the object is automatically
    deactivated.
  */
  void GenericObj_i::UnRegister()
  {
    if(MYDEBUG)
      MESSAGE("GenericObj_i::UnRegister "<<this<<"; myRefCounter = "<<myRefCounter);
    --myRefCounter;
    IS_OBJ_IN_QUESTION( "UnRegister" );
    if(myRefCounter <= 0){
      PortableServer::ObjectId_var anObjectId = myPOA->servant_to_id(this);
      myPOA->deactivate_object(anObjectId.in());
      _remove_ref();
    }
  }

  /*!
    \brief Decrement reference counter.
    \deprecated Use UnRegister() instead.
  */
  void GenericObj_i::Destroy()
  {
    MESSAGE("WARNING SALOME::GenericObj::Destroy() function is obsolete! Use UnRegister() instead.");
    UnRegister();
  }

}; // end of namespace SALOME
