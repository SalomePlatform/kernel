#ifndef __SALOMEDS_DRIVER_DEFAULT_IMPL_H__
#define __SALOMEDS_DRIVER_DEFAULT_IMPL_H__

#include "SALOME_Component_i.hxx"
#include "SALOME_NamingService.hxx"

#include CORBA_CLIENT_HEADER(SALOMEDS)


//---------------------------------------------------------

class SALOMEDS_DriverDefaultImpl:
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
