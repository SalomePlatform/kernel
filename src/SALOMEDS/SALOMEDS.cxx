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

//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDS.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$
//
#include "SALOMEDS.hxx"
#include "SALOMEDS_Study.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_StudyBuilder.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDSClient.hxx"
#include "SALOMEDSClient_IParameters.hxx"
#include "SALOMEDS_IParameters.hxx"

#include "SALOMEDS_Defines.hxx"

#include <utilities.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_NamingService.hxx>

// PAL8065: san -- Global recursive mutex for SALOMEDS methods
Utils_Mutex SALOMEDS::Locker::MutexDS;

// PAL8065: san -- Global SALOMEDS locker
SALOMEDS::Locker::Locker()
: Utils_Locker( &MutexDS )
{}

SALOMEDS::Locker::~Locker()
{}

void SALOMEDS::lock()
{
  Locker::MutexDS.lock();
}

void SALOMEDS::unlock()
{
  SALOMEDS::Locker::MutexDS.unlock();
}

// srn: Added new library methods that create basic SALOMEDS objects (Study, SComponent, SObject)

//=============================================================================
/*!
 * C factory, accessible with dlsym, after dlopen
 */
//=============================================================================


extern "C"
{
  SALOMEDS_EXPORT
  SALOMEDSClient_Study* StudyFactory(SALOMEDS::Study_ptr theStudy)
  {
    if(CORBA::is_nil(theStudy)) return NULL;
    return new SALOMEDS_Study(theStudy);
  }

  SALOMEDS_EXPORT
  SALOMEDSClient_SObject* SObjectFactory(SALOMEDS::SObject_ptr theSObject)
  {
    if(CORBA::is_nil(theSObject)) return NULL;
    return new SALOMEDS_SObject(theSObject);
  }

  SALOMEDS_EXPORT
  SALOMEDSClient_SComponent* SComponentFactory(SALOMEDS::SComponent_ptr theSComponent)
  {
    if(CORBA::is_nil(theSComponent)) return NULL;
    return new SALOMEDS_SComponent(theSComponent);
  }

  SALOMEDS_EXPORT
  SALOMEDSClient_StudyBuilder* BuilderFactory(SALOMEDS::StudyBuilder_ptr theBuilder)
  {
    if(CORBA::is_nil(theBuilder)) return NULL;
    return new SALOMEDS_StudyBuilder(theBuilder);
  }

  SALOMEDS_EXPORT
  void CreateStudy(CORBA::ORB_ptr orb, PortableServer::POA_ptr root_poa)
  {
    SALOME_NamingService namingService(orb);
    CORBA::Object_var obj = namingService.Resolve( "/Study" );
    SALOMEDS::Study_var aStudy = SALOMEDS::Study::_narrow( obj );
    if( CORBA::is_nil(aStudy) ) {
      PortableServer::POAManager_var pman = root_poa->the_POAManager();
      CORBA::PolicyList policies;
      policies.length(2);
      //PortableServer::ThreadPolicy_var threadPol(root_poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
      PortableServer::ThreadPolicy_var threadPol(root_poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL));
      PortableServer::ImplicitActivationPolicy_var implicitPol(root_poa->create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION));
      policies[0] = PortableServer::ThreadPolicy::_duplicate(threadPol);
      policies[1] = PortableServer::ImplicitActivationPolicy::_duplicate(implicitPol);
      PortableServer::POA_var poa = root_poa->create_POA("KERNELStudySingleThreadPOA",pman,policies);
      MESSAGE("CreateStudy: KERNELStudySingleThreadPOA: "<< poa);
      threadPol->destroy();

      SALOMEDS_Study_i::SetThePOA(poa);  
      SALOMEDS_Study_i* aStudy_i = new SALOMEDS_Study_i(orb);

      // Activate the objects.  This tells the POA that the objects are ready to accept requests.
      PortableServer::ObjectId_var aStudy_iid =  root_poa->activate_object(aStudy_i);
      aStudy = aStudy_i->_this();
      namingService.Register(aStudy, "/Study");
      aStudy_i->GetImpl()->GetDocument()->SetModified(false);
      aStudy_i->_remove_ref();
    }
  }

  SALOMEDS_EXPORT
  SALOMEDSClient_IParameters* GetIParameters(const _PTR(AttributeParameter)& ap)
  {
    return new SALOMEDS_IParameters(ap);
  }

  SALOMEDS_EXPORT
  SALOMEDS::SObject_ptr ConvertSObject(const _PTR(SObject)& theSObject)
  {
    SALOMEDS_SObject* so = _CAST(SObject, theSObject);
    if ( !theSObject || !so )
      return SALOMEDS::SObject::_nil();
    return so->GetSObject();
  }

  SALOMEDS_EXPORT
  SALOMEDS::StudyBuilder_ptr ConvertBuilder(const _PTR(StudyBuilder)& theBuilder)
  {
    SALOMEDS_StudyBuilder* builder = _CAST(StudyBuilder, theBuilder);
    if ( !theBuilder || !builder )
      return SALOMEDS::StudyBuilder::_nil();
    return builder->GetBuilder();
  }
}
