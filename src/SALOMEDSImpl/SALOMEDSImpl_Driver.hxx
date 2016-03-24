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

#ifndef __SALOMEDSIMPL_DRIVER_H__
#define __SALOMEDSIMPL_DRIVER_H__

#include "SALOMEDSImpl_Defines.hxx"
#include <string>
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_TMPFile.hxx"


class SALOMEDSIMPL_EXPORT SALOMEDSImpl_Driver
{
public:

  virtual std::string GetIOR() = 0;

  virtual SALOMEDSImpl_TMPFile* Save(const SALOMEDSImpl_SComponent& theComponent,
    const std::string& theURL,
    long& theStreamLength,
    bool isMultiFile) = 0;

  virtual SALOMEDSImpl_TMPFile* SaveASCII(const SALOMEDSImpl_SComponent& theComponent,
    const std::string& theURL,
    long& theStreamLength,
    bool isMultiFile) = 0;

  virtual bool Load(const SALOMEDSImpl_SComponent& theComponent,
    const unsigned char* theStream,
    const long theStreamLength,
    const std::string& theURL,
    bool isMultiFile) = 0;

  virtual bool LoadASCII(const SALOMEDSImpl_SComponent& theComponent,
    const unsigned char* theStream,
    const long theStreamLength,
    const std::string& theURL,
    bool isMultiFile) = 0;

  virtual void Close(const SALOMEDSImpl_SComponent& theComponent) = 0;

  virtual std::string ComponentDataType() = 0;

  virtual std::string Version() = 0;

  virtual std::string IORToLocalPersistentID(const SALOMEDSImpl_SObject& theSObject,
    const std::string& IORString,
    bool isMultiFile,
    bool isASCII) = 0;

  virtual std::string LocalPersistentIDToIOR(const SALOMEDSImpl_SObject& theSObject,
    const std::string& aLocalPersistentID,
    bool isMultiFile,
    bool isASCII) = 0;

  virtual bool CanCopy(const SALOMEDSImpl_SObject& theObject) = 0;

  virtual SALOMEDSImpl_TMPFile* CopyFrom(const SALOMEDSImpl_SObject& theObject, 
    int& theObjectID,
    long& theStreamLength) = 0;

  virtual bool CanPaste(const std::string& theComponentName, int theObjectID) = 0;

  virtual std::string PasteInto(const unsigned char* theStream,
    const long theStreamLength,
    int theObjectID,
    const SALOMEDSImpl_SObject& theObject) = 0;

  virtual SALOMEDSImpl_TMPFile* DumpPython(SALOMEDSImpl_Study* theStudy, 
    bool isPublished, 
    bool isMultiFile, 
    bool& isValidScript,
    long& theStreamLength) = 0;
};

class SALOMEDSImpl_DriverFactory
{
public:

  virtual SALOMEDSImpl_Driver* GetDriverByType(const std::string& theComponentType) = 0;

  virtual SALOMEDSImpl_Driver* GetDriverByIOR(const std::string& theIOR) = 0;
};

#endif 
