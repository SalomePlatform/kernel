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

//  File   : SALOMEDS_AttributeSequenceOfInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeSequenceOfInteger_HeaderFile
#define SALOMEDS_AttributeSequenceOfInteger_HeaderFile

#include "SALOMEDSClient_AttributeSequenceOfInteger.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeSequenceOfInteger: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeSequenceOfInteger
{
public:  
  SALOMEDS_AttributeSequenceOfInteger(SALOMEDSImpl_AttributeSequenceOfInteger* theAttr);
  SALOMEDS_AttributeSequenceOfInteger(SALOMEDS::AttributeSequenceOfInteger_ptr theAttr);
  ~SALOMEDS_AttributeSequenceOfInteger();

  virtual void Assign(const std::vector<int>& other);
  virtual std::vector<int> CorbaSequence();
  virtual void Add(int value);
  virtual void Remove(int index);
  virtual void ChangeValue(int index, int value);
  virtual int Value(int index);
  virtual int Length();

};

#endif
