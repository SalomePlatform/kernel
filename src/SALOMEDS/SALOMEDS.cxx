//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#ifndef WNT
#include <SALOMEDS.hxx>
#include <SALOMEDS_StudyManager.hxx>
#include <SALOMEDS_Study.hxx>
#include <SALOMEDS_SObject.hxx>
#include <SALOMEDS_SComponent.hxx>
#include <SALOMEDSClient.hxx>
#include <SALOMEDS_StudyManager_i.hxx>
#else
#include "SALOMEDS.hxx"
#include "SALOMEDS_StudyManager.hxx"
#include "SALOMEDS_Study.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDSClient.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#endif

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

using namespace SALOMEDS;

// PAL8065: san -- Global recursive mutex for SALOMEDS methods
Utils_Mutex Locker::MutexDS;

// PAL8065: san -- Global SALOMEDS locker
Locker::Locker()
: Utils_Locker( &MutexDS )
{}

Locker::~Locker()
{}

void SALOMEDS::lock()
{
  Locker::MutexDS.lock();
}

void SALOMEDS::unlock()
{
  Locker::MutexDS.unlock();
}



// srn: Added new library methods that create basic SALOMEDS objects (StudyManager, Study, SComponent, SObject)

//=============================================================================
/*!
 * C factory, accessible with dlsym, after dlopen
 */
//=============================================================================


extern "C"
{

SALOMEDSClient_StudyManager* StudyManagerFactory()
{
  return new SALOMEDS_StudyManager();
}

SALOMEDSClient_Study* StudyFactory(SALOMEDS::Study_ptr theStudy)
{
  return new SALOMEDS_Study(theStudy);
}

SALOMEDSClient_SObject* SObjectFactory(SALOMEDS::SObject_ptr theSObject)
{
  return new SALOMEDS_SObject(theSObject);
}

SALOMEDSClient_SComponent* SComponentFactory(SALOMEDS::SComponent_ptr theSComponent)
{
  return new SALOMEDS_SComponent(theSComponent);
}

SALOMEDSClient_StudyManager* CreateStudyManager(CORBA::ORB_ptr orb, PortableServer::POA_ptr root_poa)
{
  SALOMEDS_StudyManager_i * aStudyManager_i = new  SALOMEDS_StudyManager_i(orb, root_poa);
  // Activate the objects.  This tells the POA that the objects are ready to accept requests.
  PortableServer::ObjectId_var aStudyManager_iid =  root_poa->activate_object(aStudyManager_i);
  aStudyManager_i->register_name("/myStudyManager");
  return new SALOMEDS_StudyManager();
}

}
