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

//  File   : SALOMEDS_AttributeStudyProperties.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeStudyProperties_HeaderFile
#define SALOMEDS_AttributeStudyProperties_HeaderFile

#include "SALOMEDSClient_AttributeStudyProperties.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"

#include <vector>
#include <string>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#ifdef GetUserName
#undef GetUserName
#endif

class Standard_EXPORT SALOMEDS_AttributeStudyProperties: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeStudyProperties
{
public:
  
  SALOMEDS_AttributeStudyProperties(SALOMEDSImpl_AttributeStudyProperties* theAttr);
  SALOMEDS_AttributeStudyProperties(SALOMEDS::AttributeStudyProperties_ptr theAttr);
  ~SALOMEDS_AttributeStudyProperties();

  virtual void SetUserName(const std::string& theName);
  virtual std::string GetUserName();
  virtual void SetCreationDate(int theMinute, int theHour, int theDay, int theMonth, int theYear);
  virtual bool GetCreationDate(int& theMinute, int& theHour, int& theDay, int& theMonth, int& theYear);
  virtual void SetCreationMode(const std::string& theMode);
  virtual std::string GetCreationMode();
  virtual void SetModified(int theModified);
  virtual bool IsModified();
  virtual int GetModified();
  virtual void SetLocked(bool theLocked);
  virtual bool IsLocked();
  virtual void SetModification(const std::string& theName,
                               int theMinute,
                               int theHour,
                               int theDay,
                               int theMonth,
                               int theYear);
  virtual void GetModificationsList(std::vector<std::string>& theNames,
                                    std::vector<int>& theMinutes,
                                    std::vector<int>& theHours,
                                    std::vector<int>& theDays,
                                    std::vector<int>& theMonths,
                                    std::vector<int>& theYears,
                                    bool theWithCreator);

  virtual void SetUnits(const std::string& theUnits);
  virtual std::string GetUnits();

  virtual void SetComment(const std::string& theComment);
  virtual std::string GetComment();

  virtual std::vector<std::string> GetStoredComponents();
  virtual std::string GetComponentVersion( const std::string& theComponent );
  virtual std::vector<std::string> GetComponentVersions( const std::string& theComponent );
};


#endif
