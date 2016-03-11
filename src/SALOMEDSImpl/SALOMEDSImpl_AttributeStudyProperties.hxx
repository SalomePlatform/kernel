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

//  File   : SALOMEDSImpl_AttributeStudyProperties.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef _SALOMEDSImpl_AttributeStudyProperties_HeaderFile
#define _SALOMEDSImpl_AttributeStudyProperties_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include <vector>
#include <map>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeStudyProperties : public SALOMEDSImpl_GenericAttribute
{
public:
  SALOMEDSImpl_AttributeStudyProperties();
  ~SALOMEDSImpl_AttributeStudyProperties() {}

  static const std::string& GetID();
  const std::string&    ID() const;

  static  SALOMEDSImpl_AttributeStudyProperties* Set(const DF_Label& label);

  void Init();

  virtual std::string Save();
  virtual void Load(const std::string&);

  void SetModification(const std::string& theUserName,
    const int          theMinute,
    const int          theHour,
    const int          theDay,
    const int          theMonth,
    const int          theYear);
  void GetModifications(std::vector<std::string>& theUserNames,
    std::vector<int>&         theMinutes,
    std::vector<int>&         theHours,
    std::vector<int>&         theDays,
    std::vector<int>&         theMonths,
    std::vector<int>&         theYears) const;

  std::string GetCreatorName() const;
  bool GetCreationDate(int&           theMinute,
    int&           theHour,
    int&           theDay,
    int&           theMonth,
    int&           theYear) const;

  void ChangeCreatorName(const std::string& theUserName);

  void SetUnits(const std::string& theUnits);
  std::string GetUnits() const;

  void SetComment(const std::string& theComment);
  std::string GetComment() const;

  void SetCreationMode(const int theMode);
  int GetCreationMode() const;

  void SetModified(const int theModified);
  bool IsModified() const;
  int GetModified() const;

  void SetLocked(const bool theLocked);
  bool IsLocked() const;
  bool IsLockChanged(const bool theErase);

  void SetComponentsVersions( const std::map< std::string, std::vector<std::string> >& theVersions );
  void SetComponentVersion(const std::string& theComponent, const std::string& theVersion);
  std::vector<std::string> GetStoredComponents() const;
  std::string GetComponentVersion(const std::string& theComponent) const;
  std::vector<std::string> GetComponentVersions(const std::string& theComponent) const;
  std::map< std::string, std::vector<std::string> > GetComponentsVersions() const;

  void Restore(DF_Attribute* with);
  DF_Attribute* NewEmpty() const;
  void Paste(DF_Attribute* into);

private:
  typedef std::vector<std::string> versionList;
  typedef std::map<std::string, versionList> versionMap;

  std::vector<std::string> myUserName;
  std::vector<int> myMinute;
  std::vector<int> myHour;
  std::vector<int> myDay;
  std::vector<int> myMonth;
  std::vector<int> myYear;
  std::string      myUnits;
  std::string      myComment;
  int              myMode;
  int              myModified;
  bool             myLocked;
  bool             myLockChanged;
  versionMap       myComponentVersions;
};

#endif
