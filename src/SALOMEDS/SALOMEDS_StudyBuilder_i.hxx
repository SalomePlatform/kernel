//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_StudyBuilder_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_STUDYBUIlDER_I_H__
#define __SALOMEDS_STUDYBUILDER_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

// Cascade header
#include <TDocStd_Document.hxx>

class SALOMEDS_Study_i;
class SALOMEDS_SObject_i;
class SALOMEDS_Callback_i;

class SALOMEDS_StudyBuilder_i: public virtual POA_SALOMEDS::StudyBuilder,
			       public virtual PortableServer::RefCountServantBase 
{
  SALOMEDS_StudyBuilder_i(); // Not implemented
  void operator=(const SALOMEDS_StudyBuilder_i&); // Not implemented

  SALOMEDS_Study_i*        _study;
  Handle(TDocStd_Document) _doc;  // OCAF Document

  void OnAddSObject(SALOMEDS::SObject_ptr theObject);
  void OnRemoveSObject(SALOMEDS::SObject_ptr theObject);

public:
  SALOMEDS_StudyBuilder_i(SALOMEDS_Study_i* theStudy,
			  const Handle(TDocStd_Document)& theDocument);

  ~SALOMEDS_StudyBuilder_i();

  CORBA::ORB_var GetORB() const;
  PortableServer::POA_var GetPOA() const;

  //! NewComponent
  /*!
    \param ComponentDataType    
    \return <ReturnValue>
  */
  virtual SALOMEDS::SComponent_ptr NewComponent(const char* ComponentDataType);

  //! DefineComponentInstance
  /*!
    \param SComponent_ptr       
    \param ComponentIOR	
  */
  virtual void DefineComponentInstance (SALOMEDS::SComponent_ptr, CORBA::Object_ptr ComponentIOR);
  
  //! 
  /*!
    <long-description>

    \param aComponent	
  */
  virtual void RemoveComponent(SALOMEDS::SComponent_ptr aComponent);

  //! 
  /*!
    <long-description>

    \param theFatherObject      
    \return <ReturnValue>
  */
  virtual SALOMEDS::SObject_ptr NewObject(SALOMEDS::SObject_ptr theFatherObject);
  //! 
  /*!
    <long-description>

    \param theFatherObject      
    \param atag 
    \return <ReturnValue>
  */
  virtual SALOMEDS::SObject_ptr NewObjectToTag(SALOMEDS::SObject_ptr theFatherObject, CORBA::Long atag);

  /*!
    The methods adds a new subdirectory, the path can be absolute or relative (then the current context is used)
  */
  virtual void AddDirectory(const char* thePath);

  virtual void LoadWith(SALOMEDS::SComponent_ptr sco, SALOMEDS::Driver_ptr Engine)
    throw(SALOME::SALOME_Exception);
  virtual void Load(SALOMEDS::SObject_ptr sco);

  SALOMEDS_SObject_i* RemoveSObject(SALOMEDS::SObject_ptr theSObject,
				    bool theIsForgetAllAttributes = true);
  virtual void RemoveObject(SALOMEDS::SObject_ptr anObject);
  virtual void RemoveObjectWithChildren(SALOMEDS::SObject_ptr anObject);

  virtual SALOMEDS::GenericAttribute_ptr FindOrCreateAttribute(SALOMEDS::SObject_ptr anObject, const char* aTypeOfAttribute);
  virtual CORBA::Boolean FindAttribute(SALOMEDS::SObject_ptr anObject, SALOMEDS::GenericAttribute_out anAttribute, const char* aTypeOfAttribute);
  virtual void RemoveAttribute(SALOMEDS::SObject_ptr anObject, const char* aTypeOfAttribute);

  virtual void Addreference(SALOMEDS::SObject_ptr me, SALOMEDS::SObject_ptr thereferencedObject);

  virtual void RemoveReference(SALOMEDS::SObject_ptr me);

  virtual void SetGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID);
  virtual bool IsGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID);

  virtual void NewCommand();
  virtual void CommitCommand() throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual CORBA::Boolean HasOpenCommand();
  virtual void AbortCommand();
  virtual void Undo() throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual void Redo() throw(SALOMEDS::StudyBuilder::LockProtection);
  CORBA::Boolean GetAvailableUndos();
  CORBA::Boolean GetAvailableRedos();
  CORBA::Boolean IsSaved();
  CORBA::Boolean IsModified();
  virtual CORBA::Long UndoLimit();
  virtual void UndoLimit(CORBA::Long);

  void CheckLocked() throw (SALOMEDS::StudyBuilder::LockProtection);

  virtual SALOMEDS::Callback_ptr SetOnAddSObject(SALOMEDS::Callback_ptr theCallback);
  virtual SALOMEDS::Callback_ptr SetOnRemoveSObject(SALOMEDS::Callback_ptr theCallback);

  virtual void SetName(SALOMEDS::SObject_ptr theSO, const char* theValue) throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual void SetComment(SALOMEDS::SObject_ptr theSO, const char* theValue) throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual void SetIOR(SALOMEDS::SObject_ptr theSO, const char* theValue) throw(SALOMEDS::StudyBuilder::LockProtection);
};
#endif
