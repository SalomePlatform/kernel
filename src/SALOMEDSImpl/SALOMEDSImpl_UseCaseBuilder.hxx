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
//  File   : SALOMEDSImpl_UseCaseBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_USECaseBuilder_H__
#define __SALOMEDSIMPL_USECaseBuilder_H__

#include <string>

#include "DF_Document.hxx"
#include "DF_Label.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"

class SALOMEDSImpl_UseCaseBuilder
{
private:

  SALOMEDSImpl_AttributeTreeNode*     _root;
  DF_Document*                        _doc;

public:

  //! standard constructor  
  Standard_EXPORT SALOMEDSImpl_UseCaseBuilder(DF_Document* theDocument);
  
  //! standard destructor
  Standard_EXPORT ~SALOMEDSImpl_UseCaseBuilder();
  
  Standard_EXPORT virtual bool Append(const SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT virtual bool Remove(const SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT virtual bool AppendTo(const SALOMEDSImpl_SObject& theFather, const SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT virtual bool InsertBefore(const SALOMEDSImpl_SObject& theFirst, const SALOMEDSImpl_SObject& theNext);

  Standard_EXPORT virtual bool  SetCurrentObject(const SALOMEDSImpl_SObject& theObject);
  
  Standard_EXPORT virtual bool SetRootCurrent();

  Standard_EXPORT virtual bool  HasChildren(const SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT virtual bool  IsUseCase(const SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT virtual bool SetName(const std::string& theName);

  Standard_EXPORT virtual SALOMEDSImpl_SObject GetCurrentObject();

  Standard_EXPORT virtual std::string GetName();

  Standard_EXPORT virtual SALOMEDSImpl_SObject AddUseCase(const std::string& theName);

  Standard_EXPORT virtual SALOMEDSImpl_UseCaseIterator GetUseCaseIterator(const SALOMEDSImpl_SObject& anObject);

  Standard_EXPORT SALOMEDSImpl_SObject GetSObject(const std::string& theEntry);    

};
#endif
