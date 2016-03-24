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
// File      : SALOMEDS_Attributes_wrap.hxx
// Created   : Wed Dec 12 15:24:03 2012
// Author    : Edward AGAPOV (eap)

#ifndef __SALOMEDS_Attributes_wrap_HXX__
#define __SALOMEDS_Attributes_wrap_HXX__

// Define wrapper classes for SALOMEDS interfaces to replace
// e.g. SALOMEDS::SObject_var by SALOMEDS::SObject_wrap etc.
// Use of these wrappers is needed to avoid memory leaks, since
// for example all methods returning SALOMEDS::SObject_ptr create
// a new instance of SALOMEDSImpl_SObject_i which will be lost if
// UnRegister() is not called. The defined wrappers call UnRegister()
// at destruction thus preventing memory leaks.
//
// NOTES
// - To pass XXX_wrap object to an XXX_out corba variable, call XXX_wrap::inout()
// - Do not pass the same CORBA Attribute to two AttributeXXX_wrap's. Example of bad code:
//    SALOMEDS::GenericAttribute_wrap anAttr =      /// it will UnRegister() the found object
//       aStudyBuilder->FindOrCreateAttribute(aSO, "AttributePythonObject");
//    SALOMEDS::AttributePythonObject_wrap pyAttr = /// IT MUST BE _var, not _wrap !!!!!!!
//       SALOMEDS::AttributePythonObject::_narrow(anAttr);

#include "SALOME_GenericObj_wrap.hxx"
namespace SALOMEDS
{
  typedef SALOME::GenericObj_wrap< AttributeName               > AttributeName_wrap;
  typedef SALOME::GenericObj_wrap< AttributeComment            > AttributeComment_wrap;
  typedef SALOME::GenericObj_wrap< AttributeInteger            > AttributeInteger_wrap;
  typedef SALOME::GenericObj_wrap< AttributeReal               > AttributeReal_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTreeNode           > AttributeTreeNode_wrap;
  typedef SALOME::GenericObj_wrap< AttributeUserID             > AttributeUserID_wrap;
  typedef SALOME::GenericObj_wrap< AttributeSequenceOfReal     > AttributeSequenceOfReal_wrap;
  typedef SALOME::GenericObj_wrap< AttributeSequenceOfInteger  > AttributeSequenceOfInteger_wrap;
  typedef SALOME::GenericObj_wrap< AttributeIOR                > AttributeIOR_wrap;
  typedef SALOME::GenericObj_wrap< AttributePersistentRef      > AttributePersistentRef_wrap;
  typedef SALOME::GenericObj_wrap< AttributeDrawable           > AttributeDrawable_wrap;
  typedef SALOME::GenericObj_wrap< AttributeSelectable         > AttributeSelectable_wrap;
  typedef SALOME::GenericObj_wrap< AttributeExpandable         > AttributeExpandable_wrap;
  typedef SALOME::GenericObj_wrap< AttributeOpened             > AttributeOpened_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTextColor          > AttributeTextColor_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTextHighlightColor > AttributeTextHighlightColor_wrap;
  typedef SALOME::GenericObj_wrap< AttributePixMap             > AttributePixMap_wrap;
  typedef SALOME::GenericObj_wrap< AttributeLocalID            > AttributeLocalID_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTarget             > AttributeTarget_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTableOfInteger     > AttributeTableOfInteger_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTableOfReal        > AttributeTableOfReal_wrap;
  typedef SALOME::GenericObj_wrap< AttributeTableOfString      > AttributeTableOfString_wrap;
  typedef SALOME::GenericObj_wrap< AttributeStudyProperties    > AttributeStudyProperties_wrap;
  typedef SALOME::GenericObj_wrap< AttributePythonObject       > AttributePythonObject_wrap;
  typedef SALOME::GenericObj_wrap< AttributeExternalFileDef    > AttributeExternalFileDef_wrap;
  typedef SALOME::GenericObj_wrap< AttributeFileType           > AttributeFileType_wrap;
  typedef SALOME::GenericObj_wrap< AttributeFlags              > AttributeFlags_wrap;
  typedef SALOME::GenericObj_wrap< AttributeGraphic            > AttributeGraphic_wrap;
  typedef SALOME::GenericObj_wrap< AttributeParameter          > AttributeParameter_wrap;
  typedef SALOME::GenericObj_wrap< AttributeString             > AttributeString_wrap;
}

#endif
