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
// File      : SALOMEDS_wrap.hxx
// Created   : Wed Dec 12 15:24:03 2012
// Author    : Edward AGAPOV (eap)

#ifndef __SALOMEDS_wrap_HXX__
#define __SALOMEDS_wrap_HXX__

// Define wrapper classes for SALOMEDS interfaces to replace
// e.g. SALOMEDS::SObject_var by SALOMEDS::SObject_wrap etc.
// Use of these wrappers is needed to avoid memory leaks, since
// for example all methods returning SALOMEDS::SObject_ptr create
// a new instance of SALOMEDSImpl_SObject_i which will be lost if
// UnRegister() is not called. The defined wrappers call UnRegister()
// at destruction thus preventing memory leaks.

#include "SALOME_GenericObj_wrap.hxx"
namespace SALOMEDS
{
  typedef SALOME::GenericObj_wrap< SObject            > SObject_wrap;
  typedef SALOME::GenericObj_wrap< SComponent         > SComponent_wrap;
  typedef SALOME::GenericObj_wrap< SComponentIterator > SComponentIterator_wrap;
  typedef SALOME::GenericObj_wrap< ChildIterator      > ChildIterator_wrap;
  typedef SALOME::GenericObj_wrap< UseCaseIterator    > UseCaseIterator_wrap;
  typedef SALOME::GenericObj_wrap< UseCaseBuilder     > UseCaseBuilder_wrap;
  typedef SALOME::GenericObj_wrap< GenericAttribute   > GenericAttribute_wrap;
}

#endif
