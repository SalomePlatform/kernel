// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
#ifndef __SALOMEDSIMPL_DRIVER_H__
#define __SALOMEDSIMPL_DRIVER_H__

#include <TCollection_AsciiString.hxx>
#include <SALOMEDSImpl_SComponent.hxx>
#include <SALOMEDSImpl_SObject.hxx>
#include <SALOMEDSImpl_TMPFile.hxx>


class SALOMEDSImpl_Driver
{
public:

  virtual TCollection_AsciiString GetIOR() = 0;

  virtual Handle(SALOMEDSImpl_TMPFile) Save(const Handle(SALOMEDSImpl_SComponent)& theComponent,
					    const TCollection_AsciiString& theURL,
					    long& theStreamLength,
					    bool isMultiFile) = 0;

  virtual Handle(SALOMEDSImpl_TMPFile) SaveASCII(const Handle(SALOMEDSImpl_SComponent)& theComponent,
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

  virtual Handle(SALOMEDSImpl_TMPFile) CopyFrom(const Handle(SALOMEDSImpl_SObject)& theObject, 
						int& theObjectID,
						long& theStreamLength) = 0;
  
  virtual bool CanPaste(const TCollection_AsciiString& theComponentName, int theObjectID) = 0;

  virtual TCollection_AsciiString PasteInto(const unsigned char* theStream,
					    const long theStreamLength,
					    int theObjectID,
					    const Handle(SALOMEDSImpl_SObject)& theObject) = 0;

  virtual Handle(SALOMEDSImpl_TMPFile) DumpPython(const Handle(SALOMEDSImpl_Study)& theStudy, 
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
