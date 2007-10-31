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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_AttributeStudyProperties.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeStudyProperties_HeaderFile
#define _SALOMEDSImpl_AttributeStudyProperties_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include <vector>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSImpl_AttributeStudyProperties : public SALOMEDSImpl_GenericAttribute
{
public:
Standard_EXPORT SALOMEDSImpl_AttributeStudyProperties();
Standard_EXPORT ~SALOMEDSImpl_AttributeStudyProperties() {}

Standard_EXPORT static const std::string& GetID();
Standard_EXPORT        const std::string&    ID() const;

Standard_EXPORT static  SALOMEDSImpl_AttributeStudyProperties* Set(const DF_Label& label);

Standard_EXPORT void Init();

Standard_EXPORT virtual std::string Save();
Standard_EXPORT virtual void Load(const std::string&);

Standard_EXPORT   void SetModification(const std::string& theUserName,
                                       const int          theMinute,
                                       const int          theHour,
                                       const int          theDay,
                                       const int          theMonth,
                                       const int          theYear);
Standard_EXPORT   void GetModifications(std::vector<std::string>& theUserNames,
                                        std::vector<int>&         theMinutes,
                                        std::vector<int>&         theHours,
                                        std::vector<int>&         theDays,
                                        std::vector<int>&         theMonths,
                                        std::vector<int>&         theYears) const;

Standard_EXPORT   std::string GetCreatorName() const;
Standard_EXPORT   bool GetCreationDate(int&           theMinute,
				       int&           theHour,
				       int&           theDay,
				       int&           theMonth,
				       int&           theYear) const;

Standard_EXPORT   void ChangeCreatorName(const std::string& theUserName);

Standard_EXPORT   void SetCreationMode(const int theMode);
Standard_EXPORT   int GetCreationMode() const;

Standard_EXPORT   void SetModified(const int theModified);
Standard_EXPORT   bool IsModified() const;
Standard_EXPORT   int GetModified() const;

Standard_EXPORT   void SetLocked(const bool theLocked);
Standard_EXPORT   bool IsLocked() const;
Standard_EXPORT   bool IsLockChanged(const bool theErase);

Standard_EXPORT   void Restore(DF_Attribute* with);
Standard_EXPORT   DF_Attribute* NewEmpty() const;
Standard_EXPORT   void Paste(DF_Attribute* into);

private:

std::vector<std::string> myUserName;
std::vector<int> myMinute;
std::vector<int> myHour;
std::vector<int> myDay;
std::vector<int> myMonth;
std::vector<int> myYear;
int myMode;
int myModified;
bool myLocked;
bool myLockChanged;

};

#endif
