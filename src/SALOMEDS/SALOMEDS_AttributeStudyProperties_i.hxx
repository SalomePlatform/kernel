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

//  File   : SALOMEDS_AttributeStudyProperties_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeStudyProperties_i_HeaderFile
#define SALOMEDS_AttributeStudyProperties_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"

class SALOMEDS_AttributeStudyProperties_i: public virtual POA_SALOMEDS::AttributeStudyProperties,
                                           public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeStudyProperties_i(SALOMEDSImpl_AttributeStudyProperties* theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeStudyProperties_i() {};

  virtual void SetUserName(const char* theName);
  virtual char* GetUserName() ;
  virtual void SetCreationDate(CORBA::Long theMinute, CORBA::Long theHour, CORBA::Long theDay, CORBA::Long theMonth, CORBA::Long theYear);
  virtual CORBA::Boolean GetCreationDate(CORBA::Long& theMinute, CORBA::Long& theHour, CORBA::Long& theDay, CORBA::Long& theMonth, CORBA::Long& theYear);
  virtual void SetCreationMode(const char* theMode);
  virtual char* GetCreationMode();
  virtual void SetModified(CORBA::Long theModified);
  virtual CORBA::Boolean IsModified();
  virtual CORBA::Long GetModified();
  virtual void SetLocked(CORBA::Boolean theLocked);
  virtual CORBA::Boolean IsLocked();
  virtual void SetModification(const char* theName,
                               CORBA::Long theMinute,
                               CORBA::Long theHour,
                               CORBA::Long theDay,
                               CORBA::Long theMonth,
                               CORBA::Long theYear);
  virtual void GetModificationsList(SALOMEDS::StringSeq_out theNames,
                                    SALOMEDS::LongSeq_out theMinutes,
                                    SALOMEDS::LongSeq_out theHours,
                                    SALOMEDS::LongSeq_out theDays,
                                    SALOMEDS::LongSeq_out theMonths,
                                    SALOMEDS::LongSeq_out theYears,
                                    CORBA::Boolean theWithCreator);

  virtual void SetComment(const char* theComment);
  virtual char* GetComment();

  virtual void SetUnits(const char* theUnits);
  virtual char* GetUnits();

  virtual SALOMEDS::StringSeq* GetStoredComponents();
  virtual char* GetComponentVersion(const char* theComponent);
  virtual SALOMEDS::StringSeq* GetComponentVersions(const char* theComponent);
};


#endif
