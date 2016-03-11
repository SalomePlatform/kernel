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

//  File   : SALOMEDSClient_AttributeSequenceOfSequenceOfReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDSClient_AttributeSequenceOfSequenceOfReal_HeaderFile
#define SALOMEDSClient_AttributeSequenceOfSequenceOfReal_HeaderFile

#include <vector>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeSequenceOfReal: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual void Assign(const std::vector<double>& other) = 0;
  virtual std::vector<double> CorbaSequence() = 0;
  virtual void Add(double value) = 0;
  virtual void Remove(int index) = 0;
  virtual void ChangeValue(int index, double value) = 0;
  virtual double Value(int index) = 0;
  virtual int Length() = 0;

};



#endif
