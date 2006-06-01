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
//  File   : SALOMEDSImpl_SComponentIterator_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SCOMPONENTITERATOR_H__
#define __SALOMEDSIMPL_SCOMPONENTITERATOR_H__


//SALOMEDSImpl headers
#include "SALOMEDSImpl_SComponent.hxx"

// Cascade headers
#include <TDocStd_Document.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <stdio.h>

class Standard_EXPORT SALOMEDSImpl_SComponentIterator
{
private:

  TDF_ChildIterator        _it;
  TDF_Label                _lab;

public:
  
  SALOMEDSImpl_SComponentIterator(const SALOMEDSImpl_SComponentIterator& theIterator) 
  {
    _it = theIterator._it;
    _lab = theIterator._lab;
  } 

  SALOMEDSImpl_SComponentIterator() {};
  
  SALOMEDSImpl_SComponentIterator(const Handle(TDocStd_Document)& theDocument);
 
  ~SALOMEDSImpl_SComponentIterator() {};
  
  virtual void Init();
  virtual bool More();
  virtual void Next();
  virtual Handle(SALOMEDSImpl_SComponent) Value();  
};
#endif
