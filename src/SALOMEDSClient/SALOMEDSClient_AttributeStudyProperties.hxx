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

//  File   : SALOMEDSClient_AttributeStudyProperties.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDSClient_AttributeStudyProperties_HeaderFile
#define SALOMEDSClient_AttributeStudyProperties_HeaderFile

#include <vector>
#include <string>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx"

class SALOMEDSClient_AttributeStudyProperties: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual void SetUserName(const std::string& theName) = 0;
  virtual std::string GetUserName() = 0;
  virtual void SetCreationDate(int theMinute, int theHour, int theDay, int theMonth, int theYear) = 0;
  virtual bool GetCreationDate(int& theMinute, int& theHour, int& theDay, int& theMonth, int& theYear) = 0;
  virtual void SetCreationMode(const std::string& theMode) = 0;
  virtual std::string GetCreationMode() = 0;
  virtual void SetModified(int theModified) = 0;
  virtual bool IsModified() = 0;
  virtual int GetModified() = 0;
  virtual void SetLocked(bool theLocked) = 0;
  virtual bool IsLocked() = 0;
  virtual void SetModification(const std::string& theName,
                               int theMinute,
                               int theHour,
                               int theDay,
                               int theMonth,
                               int theYear) = 0;
  virtual void GetModificationsList(std::vector<std::string>& theNames,
                                    std::vector<int>& theMinutes,
                                    std::vector<int>& theHours,
                                    std::vector<int>& theDays,
                                    std::vector<int>& theMonths,
                                    std::vector<int>& theYears,
                                    bool theWithCreator) = 0;
  
  virtual void SetUnits(const std::string& theUnits) = 0;
  virtual std::string GetUnits() = 0;

  virtual void SetComment(const std::string& theComment) = 0;
  virtual std::string GetComment() = 0;

  virtual std::vector<std::string> GetStoredComponents() = 0;
  virtual std::string GetComponentVersion( const std::string& theComponent ) = 0;
  virtual std::vector<std::string> GetComponentVersions( const std::string& theComponent ) = 0;
};


#endif
