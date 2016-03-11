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

#ifndef __SALOMEDS_DRIVER_DEFAULT_IMPL_H__
#define __SALOMEDS_DRIVER_DEFAULT_IMPL_H__

#include "KernelHelpers.hxx"

#include "SALOME_Component_i.hxx"
#include "SALOME_NamingService.hxx"

#include CORBA_CLIENT_HEADER(SALOMEDS)


//---------------------------------------------------------

class KERNELHELPERS_EXPORT SALOMEDS_DriverDefaultImpl:
  public virtual POA_SALOMEDS::Driver
//---------------------------------------------------------
  {


    SALOMEDS_DriverDefaultImpl();
  public:
    SALOMEDS_DriverDefaultImpl(CORBA::ORB_ptr orb,
             PortableServer::POA_ptr poa,
             PortableServer::ObjectId * contId,
             const char *instanceName,
             const char *interfaceName);


    virtual ~SALOMEDS_DriverDefaultImpl();

    //---------------------------------------------
    // inherited methods from SALOMEDS::Driver
    //---------------------------------------------

    //virtual SALOMEDS::TMPFile* Save(SALOMEDS::SComponent_ptr theComponent,
     SALOMEDS::TMPFile* Save(SALOMEDS::SComponent_ptr theComponent,
            const char* theURL,
            bool isMultiFile);

    virtual SALOMEDS::TMPFile* SaveASCII(SALOMEDS::SComponent_ptr theComponent,
           const char* theURL,
           bool isMultiFile);

    virtual bool Load(SALOMEDS::SComponent_ptr,
          const SALOMEDS::TMPFile &,
          const char* theURL,
          bool isMultiFile);

    virtual bool LoadASCII(SALOMEDS::SComponent_ptr,
         const SALOMEDS::TMPFile &,
         const char* theURL,
         bool isMultiFile);

    virtual void Close(SALOMEDS::SComponent_ptr IORSComponent);

    virtual char* ComponentDataType();

    virtual char* IORToLocalPersistentID(SALOMEDS::SObject_ptr theSObject,
           const char* IORString,
           CORBA::Boolean isMultiFile,
           CORBA::Boolean isASCII);

    virtual char* LocalPersistentIDToIOR(SALOMEDS::SObject_ptr theSObject,
           const char* aLocalPersistentID,
           CORBA::Boolean isMultiFile,
           CORBA::Boolean isASCII);

    virtual bool CanPublishInStudy(CORBA::Object_ptr theIOR);

    virtual SALOMEDS::SObject_ptr PublishInStudy
                             (SALOMEDS::Study_ptr theStudy,
                  SALOMEDS::SObject_ptr theSObject,
            CORBA::Object_ptr theObject,
            const char* theName) 
              throw (SALOME::SALOME_Exception);

    virtual CORBA::Boolean CanCopy(SALOMEDS::SObject_ptr theObject);

    virtual SALOMEDS::TMPFile* CopyFrom(SALOMEDS::SObject_ptr theObject, 
                                CORBA::Long& theObjectID);

    virtual CORBA::Boolean CanPaste(const char* theComponentName, 
                            CORBA::Long theObjectID);

    virtual SALOMEDS::SObject_ptr PasteInto(const SALOMEDS::TMPFile& theStream,
            CORBA::Long theObjectID,
            SALOMEDS::SObject_ptr theObject);
  };
#endif
