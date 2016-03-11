// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include <utilities.h>
#include "SALOMEDS_DriverDefaultImpl.hxx"


//===========================================================================
    SALOMEDS_DriverDefaultImpl::SALOMEDS_DriverDefaultImpl(CORBA::ORB_ptr orb,
			                 PortableServer::POA_ptr poa,
				         PortableServer::ObjectId * contId,
					 const char *instanceName,
					 const char *interfaceName) 
//     :Engines_Component_i(orb, poa, contId, instanceName, interfaceName)
  {
    MESSAGE("SALOMEDS_DriverDefaultImpl::SALOMEDS_DriverDefaultImpl : ");
  };

//===========================================================================
  SALOMEDS_DriverDefaultImpl::~SALOMEDS_DriverDefaultImpl()
  {
    MESSAGE("SALOMEDS_DriverDefaultImpl::~SALOMEDS_DriverDefaultImpl");
  };

//===========================================================================
  CORBA::Boolean SALOMEDS_DriverDefaultImpl::Load(SALOMEDS::SComponent_ptr theComponent,
				  const SALOMEDS::TMPFile & theStream,
				  const char* theURL,
				  bool isMultiFile)
  {
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::Load");
    MESSAGE("Should be implemented");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    return false;
  };

//===========================================================================
  CORBA::Boolean SALOMEDS_DriverDefaultImpl::LoadASCII(SALOMEDS::SComponent_ptr theComponent,
				       const SALOMEDS::TMPFile & theStream,
				       const char* theURL,
				       bool isMultiFile)
  {
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::LoadASCII");
    MESSAGE("Should be implemented");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    return false;
  };

//===========================================================================
  char* SALOMEDS_DriverDefaultImpl::LocalPersistentIDToIOR(SALOMEDS::SObject_ptr theSObject,
					   const char* aLocalPersistentID,
					   CORBA::Boolean isMultiFile,
					   CORBA::Boolean isASCII)
  {
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::LocalPersistentIDToIOR");
    MESSAGE("Should be implemented");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    CORBA::String_var aString("");
    return aString._retn();
  };

//===========================================================================
  SALOMEDS::TMPFile* SALOMEDS_DriverDefaultImpl::Save(SALOMEDS::SComponent_ptr theComponent,
				      const char* theURL,
				      bool isMultiFile)
  {
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::Save");
    MESSAGE("Should be implemented");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(0);
    return aStreamFile._retn();
  };

//===========================================================================
  SALOMEDS::TMPFile* SALOMEDS_DriverDefaultImpl::SaveASCII(SALOMEDS::SComponent_ptr theComponent,
					   const char* theURL,
					   bool isMultiFile)
  {
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::SaveASCII");
    MESSAGE("Should be implemented");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(0);
    return aStreamFile._retn();
  };

//===========================================================================
  char* SALOMEDS_DriverDefaultImpl::IORToLocalPersistentID(SALOMEDS::SObject_ptr theSObject,
					   const char* IORString,
					   CORBA::Boolean isMultiFile,
					   CORBA::Boolean isASCII)
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::IORToLocalPersistentID");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    CORBA::String_var aString("");
    return aString._retn();
  };


//===========================================================================
  void SALOMEDS_DriverDefaultImpl::Close(SALOMEDS::SComponent_ptr theComponent)
  {
    MESSAGE("------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::Close");
    MESSAGE("------------------------");
  };

//===========================================================================
  char* SALOMEDS_DriverDefaultImpl::ComponentDataType()
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::ComponentDataType");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    return CORBA::string_dup("J aurais du nommer mon type prefere");
  };

//===========================================================================
  bool SALOMEDS_DriverDefaultImpl::CanPublishInStudy(CORBA::Object_ptr theIOR) 
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::CanPublishInStudy");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    return false;
  };

//===========================================================================
  SALOMEDS::SObject_ptr SALOMEDS_DriverDefaultImpl::PublishInStudy(SALOMEDS::Study_ptr theStudy,
						   SALOMEDS::SObject_ptr theSObject,
						   CORBA::Object_ptr theObject,
						   const char* theName)
    throw (SALOME::SALOME_Exception)
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::PublishInStudy");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    SALOMEDS::SObject_var aResultSO;
    return aResultSO._retn();
  };

//===========================================================================
  CORBA::Boolean SALOMEDS_DriverDefaultImpl::CanCopy(SALOMEDS::SObject_ptr theObject) 
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::CanCopy");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    //return false;
    return true;
  };

//===========================================================================
  SALOMEDS::TMPFile* SALOMEDS_DriverDefaultImpl::CopyFrom(SALOMEDS::SObject_ptr theObject,
		  			     CORBA::Long& theObjectID)
{
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::CopyFrom");
    MESSAGE("Should be implemented");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    MESSAGE("--------------------------------------");
    SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(0);
    return aStreamFile._retn();
};

//===========================================================================
  SALOMEDS::SObject_ptr SALOMEDS_DriverDefaultImpl::PasteInto( const SALOMEDS::TMPFile & theStream,
		  			              CORBA::Long theObjectID,
						      SALOMEDS::SObject_ptr theSObject)
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::PasteInto");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    SALOMEDS::SObject_var aResultSO;
    return aResultSO._retn();
  };

//===========================================================================
   CORBA::Boolean  SALOMEDS_DriverDefaultImpl::CanPaste ( const char *theComponentName,
		  			         CORBA::Long theObjectID)
  {
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("SALOMEDS_DriverDefaultImpl::CanPaste");
    MESSAGE("Should be implemented");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    MESSAGE("-----------------------------------------");
    return false;
  };

  
