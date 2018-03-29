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

//  File   : SALOMEDS_StudyManager_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_STUDYMANAGER_I_H__
#define __SALOMEDS_STUDYMANAGER_I_H__

// std C++ headers
#include <iostream>

#ifndef WIN32
#include <unistd.h>
#endif

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// Naming Service header
#include "SALOME_NamingService.hxx"

#include <stdlib.h>

#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDSImpl_StudyManager.hxx"

namespace SALOMEDS{

  // To convert CORBA::Object to  PortableServer::ServantBase
  PortableServer::ServantBase_var GetServant(CORBA::Object_ptr, PortableServer::POA_ptr);

}

class Standard_EXPORT SALOMEDS_StudyManager_i: public POA_SALOMEDS::StudyManager
{
private:

  CORBA::ORB_var                    _orb;
  PortableServer::POA_var           _poa;
  SALOMEDSImpl_StudyManager*        _impl;
  SALOME_NamingService*             _name_service;
  SALOMEDS_DriverFactory_i*         _factory;

public:

  //! standard constructor
  SALOMEDS_StudyManager_i(CORBA::ORB_ptr orb, PortableServer::POA_ptr thePOA);

  //! standard destructor
  virtual  ~SALOMEDS_StudyManager_i();

 //! method to Register study Manager in the naming service
  /*!
    \param char* arguments, the context to register the study manager in the NS
  */
  void register_name(const char * name);

 //! method to Create a New Study of name study_name
  /*!
    \param char* arguments, the new study name
    \return Study_ptr arguments
  */
  virtual SALOMEDS::Study_ptr NewStudy(const char* study_name) throw (SALOME::SALOME_Exception);

  //! method to Open a Study from it's persistent reference
  /*!
    \param char* arguments, the study URL
    \return Study_ptr arguments
  */
  virtual SALOMEDS::Study_ptr Open(const char* aStudyUrl) throw (SALOME::SALOME_Exception);


  //! method to close a Study
  /*!
    \param Study_ptr arguments
  */
  virtual void Close( SALOMEDS::Study_ptr aStudy);

  //! method to save a Study
  /*!
    \param Study_ptr arguments
  */
  virtual CORBA::Boolean Save( SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);

  virtual CORBA::Boolean SaveASCII( SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);

  //! method to save a Study to the persistent reference aUrl
  /*!
    \param char* arguments, the new URL of the study
    \param Study_ptr arguments
  */
  virtual CORBA::Boolean SaveAs(const char* aUrl,  SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);
  virtual CORBA::Boolean SaveAsASCII(const char* aUrl,  SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);

 //! method to Get name list of open studies in the session
  /*!
    \return ListOfOpenStudies* arguments
  */
  virtual SALOMEDS::ListOfOpenStudies* GetOpenStudies();

  //! method to get a Study from it's name
  /*!
    \param char* arguments, the study name
    \return Study_ptr arguments
  */
  virtual SALOMEDS::Study_ptr GetStudyByName(const char* aStudyName) ;

  //! method to get a Study from it's ID
  /*!
    \param char* arguments, the study ID
    \return Study_ptr arguments
  */
  virtual SALOMEDS::Study_ptr GetStudyByID(CORBA::Short aStudyID) ;

  virtual CORBA::Boolean CanCopy(SALOMEDS::SObject_ptr theObject);
  virtual CORBA::Boolean Copy(SALOMEDS::SObject_ptr theObject);
  virtual CORBA::Boolean CanPaste(SALOMEDS::SObject_ptr theObject);
  virtual SALOMEDS::SObject_ptr Paste(SALOMEDS::SObject_ptr theObject) throw(SALOMEDS::StudyBuilder::LockProtection);

  virtual char* ConvertObjectToIOR(CORBA::Object_ptr theObject) {return _orb->object_to_string(theObject); }
  virtual CORBA::Object_ptr ConvertIORToObject(const char* theIOR) { return _orb->string_to_object(theIOR); };

  void ping(){};
  CORBA::Long getPID();
  void ShutdownWithExit();

  virtual CORBA::LongLong GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);

  static PortableServer::POA_ptr GetPOA(const SALOMEDS::Study_ptr theStudy);

  void Shutdown() { if(!CORBA::is_nil(_orb)) _orb->shutdown(0); }
};

#endif
