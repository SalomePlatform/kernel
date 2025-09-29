// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

// Author: Guillaume Boulant (EDF/R&D) 
#ifndef __SALOME_KERNEL_SERVICES_HXX__
#define __SALOME_KERNEL_SERVICES_HXX__
#pragma once

#include "KernelHelpers.hxx"

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include CORBA_SERVER_HEADER(SALOME_Launcher)
#include CORBA_CLIENT_HEADER(SALOME_Session)
#include CORBA_SERVER_HEADER(SALOME_Exception)

#include "SALOME_NamingService.hxx"
#include "SALOME_LifeCycleCORBA.hxx"

#include "SALOME_KernelORB.hxx"

namespace KERNEL {

  // ---------------------------------------------
  // SALOME KERNEL main services
  KERNELHELPERS_EXPORT SALOME_NamingService_Abstract *getNamingService();
  KERNELHELPERS_EXPORT void assignNamingServiceSL();
  KERNELHELPERS_EXPORT SALOME_LifeCycleCORBA *       getLifeCycleCORBA();
  KERNELHELPERS_EXPORT SALOME_CMOD::Session_ptr           getSalomeSession();
  KERNELHELPERS_EXPORT SALOMEDS::Study_ptr           getStudyServant();
  KERNELHELPERS_EXPORT Engines::SalomeLauncher_ptr   getSalomeLauncher();
  KERNELHELPERS_EXPORT Engines::ResourcesManager_ptr getResourcesManager();

  // ---------------------------------------------
  // SALOME KERNEL services to deal with a SALOME study, SObject and
  // SComponent.
  //
  KERNELHELPERS_EXPORT CORBA::Object_ptr   IORToObject(char * IOR);
  KERNELHELPERS_EXPORT CORBA::Object_ptr   SObjectToObject(SALOMEDS::SObject_ptr);
  

  KERNELHELPERS_EXPORT void RegisterCompo(const std::string& compoName, CORBA::Object_var compoPtr);
  KERNELHELPERS_EXPORT CORBA::Object_var RetrieveCompo(const std::string& compoName);
  /*!
   * This template function provides you with the servant (CORBA
   * object narrowed to its interface) corresponding to the specified
   * CORBA object (naked CORBA pointer).
   */
  template<class TInterface> typename TInterface::_var_type
  ObjectToInterface(CORBA::Object_ptr object) {
    if(!CORBA::is_nil(object))
      return TInterface::_narrow(object);
    return TInterface::_nil();
  }

  template<class TInterface> typename TInterface::_var_type
  SObjectToInterface(SALOMEDS::SObject_ptr sobject) {
    CORBA::Object_ptr object = SObjectToObject(sobject);
    return ObjectToInterface<TInterface>(object);
  }

  // _MEM_: note that template functions have to be declared AND
  // implemented in the header because they are not compiled in this
  // library but in every client library that includes this header
  // file. The effective compilation depends on the particular class
  // used for TInterface.

  // ---------------------------------------------
  // To create a standard SALOME exception embedding a simple text
  KERNELHELPERS_EXPORT SALOME_CMOD::SALOME_Exception createSalomeException(const char * text);
}


//
// =============================================================
// SALOME Logger macros
// =============================================================
//
// We can use the macros defined by SALOMELocalTrace/utilities.h
#include "utilities.h"
#define SALOMELOG(msg) {MESS_BEGIN("[XSALOME]") << msg << MESS_END}
#ifdef LOG
#undef LOG
#endif
#define LOG SALOMELOG

#include "Utils_CorbaException.hxx"
// Tip: CORBA exceptions can be used with LOG (or more generally in streams)
// Ex: LOG("An exception occurs: "<<e) will log the data of the exception e
#endif //__SALOME_KERNEL_SERVICES_HXX__
