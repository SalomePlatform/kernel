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

//  File   : SALOMEDS_StudyBuilder_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_STUDYBUILDER_I_H__
#define __SALOMEDS_STUDYBUILDER_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDSImpl_StudyBuilder.hxx"

class SALOMEDS_StudyBuilder_i: public POA_SALOMEDS::StudyBuilder
{
private:
  CORBA::ORB_var                    _orb;
  SALOMEDSImpl_StudyBuilder*        _impl;  
public:
    
  SALOMEDS_StudyBuilder_i(SALOMEDSImpl_StudyBuilder*, CORBA::ORB_ptr);

  ~SALOMEDS_StudyBuilder_i();

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

  virtual void SetName(SALOMEDS::SObject_ptr theSO, const char* theValue) throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual void SetComment(SALOMEDS::SObject_ptr theSO, const char* theValue) throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual void SetIOR(SALOMEDS::SObject_ptr theSO, const char* theValue) throw(SALOMEDS::StudyBuilder::LockProtection);

  SALOMEDSImpl_StudyBuilder* GetImpl() { return _impl; }

};
#endif
