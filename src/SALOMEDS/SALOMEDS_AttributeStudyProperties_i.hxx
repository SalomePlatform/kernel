//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributeStudyProperties_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeStudyProperties_i_HeaderFile
#define SALOMEDS_AttributeStudyProperties_i_HeaderFile

// IDL headers
#include <SALOMEDS_StudyPropertiesAttribute.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeStudyProperties_i: public virtual POA_SALOMEDS::AttributeStudyProperties,
                                           public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeStudyProperties_i(const Handle(SALOMEDS_StudyPropertiesAttribute)& theAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theAttr;
  };
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

  char* Store();
  void Restore(const char*);

};


#endif
