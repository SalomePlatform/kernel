#ifndef __SALOMEDSIMPL_DRIVER_H__
#define __SALOMEDSIMPL_DRIVER_H__

#include <TCollection_AsciiString.hxx>
#include <SALOMEDSImpl_SComponent.hxx>
#include <SALOMEDSImpl_SObject.hxx>


class SALOMEDSImpl_Driver
{
public:

  virtual TCollection_AsciiString GetIOR() = 0;

  virtual unsigned char* Save(const Handle(SALOMEDSImpl_SComponent)& theComponent,
			      const TCollection_AsciiString& theURL,
			      long& theStreamLength,
			      bool isMultiFile) = 0;

  virtual unsigned char* SaveASCII(const Handle(SALOMEDSImpl_SComponent)& theComponent,
				   const TCollection_AsciiString& theURL,
				   long& theStreamLength,
				   bool isMultiFile) = 0;
  
  virtual bool Load(const Handle(SALOMEDSImpl_SComponent)& theComponent,
		    const unsigned char* theStream,
		    const long theStreamLength,
		    const TCollection_AsciiString& theURL,
		    bool isMultiFile) = 0;

  virtual bool LoadASCII(const Handle(SALOMEDSImpl_SComponent)& theComponent,
			 const unsigned char* theStream,
			 const long theStreamLength,
			 const TCollection_AsciiString& theURL,
			 bool isMultiFile) = 0;

  virtual void Close(const Handle(SALOMEDSImpl_SComponent)& theComponent) = 0;
 
  virtual TCollection_AsciiString ComponentDataType() = 0;


  virtual TCollection_AsciiString IORToLocalPersistentID(const Handle(SALOMEDSImpl_SObject)& theSObject,
							 const TCollection_AsciiString& IORString,
							 bool isMultiFile,
							 bool isASCII) = 0;

  virtual TCollection_AsciiString LocalPersistentIDToIOR(const Handle(SALOMEDSImpl_SObject)& theSObject,
							 const TCollection_AsciiString& aLocalPersistentID,
							 bool isMultiFile,
							 bool isASCII) = 0;

  virtual bool CanCopy(const Handle(SALOMEDSImpl_SObject)& theObject) = 0;

  virtual unsigned char* CopyFrom(const Handle(SALOMEDSImpl_SObject)& theObject, 
				  int& theObjectID,
				  long& theStreamLength) = 0;

  virtual bool CanPaste(const TCollection_AsciiString& theComponentName, int theObjectID) = 0;

  virtual TCollection_AsciiString PasteInto(const unsigned char* theStream,
					    const long theStreamLength,
					    int theObjectID,
					    const Handle(SALOMEDSImpl_SObject)& theObject) = 0;

  virtual unsigned char* DumpPython(const Handle(SALOMEDSImpl_Study)& theStudy, 
                                    bool isPublished, 
		                    bool& isValidScript,
				    long& theStreamLength) = 0;
};

class SALOMEDSImpl_DriverFactory
{
public:
  
  virtual SALOMEDSImpl_Driver* GetDriverByType(const TCollection_AsciiString& theComponentType) = 0;

  virtual SALOMEDSImpl_Driver* GetDriverByIOR(const TCollection_AsciiString& theIOR) = 0;
};

#endif 
