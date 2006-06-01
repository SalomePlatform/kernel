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
//  File   : SALOMEDSImpl_UseCaseIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_USECASEITERATOR_H__
#define __SALOMEDSIMPL_USECASEITERATOR_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_UseCaseIterator, MMgt_TShared )

#include "SALOMEDSImpl_SObject.hxx"

// Cascade headers
#include <TDF_ChildIterator.hxx>
#include <SALOMEDSImpl_ChildNodeIterator.hxx>
#include <Standard_GUID.hxx>

class SALOMEDSImpl_UseCaseIterator : public MMgt_TShared 
{

private:
  Standard_GUID                              _guid;
  bool                                       _levels;
  Handle(SALOMEDSImpl_AttributeTreeNode)     _node;
  SALOMEDSImpl_ChildNodeIterator             _it;

public:

  //! standard constructor  
  SALOMEDSImpl_UseCaseIterator(const TDF_Label& theLabel, 
			       const Standard_GUID& theGUID, 
			       const bool allLevels);
  
  //! standard destructor
  ~SALOMEDSImpl_UseCaseIterator();
  
  virtual void Init(bool);
  virtual bool More();
  virtual void Next();
  virtual Handle(SALOMEDSImpl_SObject) Value();

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_UseCaseIterator )
};
#endif
