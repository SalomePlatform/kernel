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

//  File   : SALOMEDSClient_ClientFactory.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:
//
#ifndef SALOMEDSClient_ClientFactory_HeaderFile
#define SALOMEDSClient_ClientFactory_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_SObject.hxx"
#include "SALOMEDSClient_SComponent.hxx"
#include "SALOMEDSClient_Study.hxx"
#include "SALOMEDSClient_StudyManager.hxx"
#include "SALOMEDSClient_IParameters.hxx"

#include <string>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOMEDS)

class SALOMEDSCLIENT_EXPORT ClientFactory {

public:
  /*!
   *  Returns a client SObject wrapper that corresponds %theSObject
   */
  static _PTR(SObject) SObject(SALOMEDS::SObject_ptr theSObject);

  /*!
   *  Returns a client SComponent wrapper that corresponds %theSComponent
   */
  static _PTR(SComponent) SComponent(SALOMEDS::SComponent_ptr theSComponent);

  /*!
   *  Returns a client Study wrapper that corresponds %theStudy
   */
  static _PTR(Study) Study(SALOMEDS::Study_ptr theStudy);

  /*!
   *  Returns a client StudyBuilder wrapper that corresponds %theStudy
   */
  static _PTR(StudyBuilder) StudyBuilder(SALOMEDS::StudyBuilder_ptr theBuilder);

  /*!
   *  Returns a client StudyManager wrapper
   */
  static _PTR(StudyManager) StudyManager();

  /*!
   *  Creates and returns a client StudyManager wrapper
   */
  static _PTR(StudyManager) createStudyManager(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  /*!
   * Returns an IParameters interface
   */
  static _PTR(IParameters) getIParameters(const _PTR(AttributeParameter)& ap);
    

  /*!
   *  Returns a CORBA SObject that corresponds %theStudy
   */
  static SALOMEDS::SObject_ptr crbSObject(const _PTR(SObject)& theSObject);

  /*!
   *  Returns a CORBA Study that corresponds %theStudy
   */
  static SALOMEDS::Study_ptr crbStudy(const _PTR(Study)& theStudy);  

  /*!
   *  Returns a CORBA StudyBuilder that corresponds %theStudyBuilder
   */
  static SALOMEDS::StudyBuilder_ptr crbStudyBuilder(const _PTR(StudyBuilder)& theStudyBuilder);  

};

#endif
