//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  SALOME_GenericObj_i_HH
//  File   : SALOME_GenericObj_i.hh
//  Author : Alexey PETROV
//  Module : SALOME
//
#ifndef SALOME_GenericObj_i_HH
#define SALOME_GenericObj_i_HH

// IDL headers
#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_GenericObj)

#ifdef WIN32
# if defined GENERICOBJ_EXPORTS || defined SalomeGenericObj_EXPORTS
#  define GENERICOBJ_EXPORT __declspec( dllexport )
# else
#  define GENERICOBJ_EXPORT __declspec( dllimport )
# endif
#else
# define GENERICOBJ_EXPORT
#endif

namespace SALOME{
  class GENERICOBJ_EXPORT GenericObj_i : 
    public virtual POA_SALOME::GenericObj,
    public virtual PortableServer::ServantBase
  {
  protected:
    PortableServer::POA_var myPOA;
    int myRefCounter;
  public:
    // In the constructor you can provide default POA for the servant
    GenericObj_i(PortableServer::POA_ptr thePOA = PortableServer::POA::_nil());
    // The function is used implicetly in "_this" function
    virtual PortableServer::POA_ptr _default_POA();
  public: // Follow functions is IDL defined
    /*! Increase the reference count (mark as used by another object).*/
    virtual void Register();
    /*! Decrease the reference count (release by another object).*/
    virtual void Destroy();
  };
};

#endif

