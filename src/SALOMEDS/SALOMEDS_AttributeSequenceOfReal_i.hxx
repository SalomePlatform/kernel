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

//  File   : SALOMEDS_AttributeSequenceOfSequenceOfReal_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeSequenceOfSequenceOfReal_i_HeaderFile
#define SALOMEDS_AttributeSequenceOfSequenceOfReal_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"

class SALOMEDS_AttributeSequenceOfReal_i: public virtual POA_SALOMEDS::AttributeSequenceOfReal,
                                          public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeSequenceOfReal_i(SALOMEDSImpl_AttributeSequenceOfReal* theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeSequenceOfReal_i() {};

  void Assign(const SALOMEDS::DoubleSeq& other);
  SALOMEDS::DoubleSeq* CorbaSequence();
  void Add(CORBA::Double value);
  void Remove(CORBA::Long index);
  void ChangeValue(CORBA::Long index, CORBA::Double value);
  CORBA::Double Value(CORBA::Short index);
  CORBA::Long Length();

};



#endif
