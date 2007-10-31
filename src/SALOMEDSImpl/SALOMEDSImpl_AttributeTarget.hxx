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
//  File   : SALOMEDSImpl_AttributeTarget.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTarget_HeaderFile
#define _SALOMEDSImpl_AttributeTarget_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_SObject.hxx"

class SALOMEDSImpl_AttributeTarget :  public SALOMEDSImpl_GenericAttribute 
{
private:
std::string myRelation;
std::vector<DF_Attribute*>  myVariables;

public:
Standard_EXPORT static const std::string& GetID() ;
Standard_EXPORT static SALOMEDSImpl_AttributeTarget* Set(const DF_Label& label) ;
Standard_EXPORT SALOMEDSImpl_AttributeTarget();
Standard_EXPORT void Add(const SALOMEDSImpl_SObject& theSO);
Standard_EXPORT std::vector<SALOMEDSImpl_SObject> Get();
Standard_EXPORT void Remove(const SALOMEDSImpl_SObject& theSO);
Standard_EXPORT std::string GetRelation() { return myRelation; }
Standard_EXPORT void SetRelation(const std::string& theRelation); 
Standard_EXPORT std::vector<DF_Attribute*>& GetVariables() { return myVariables; }
Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT  void Restore(DF_Attribute* with) ;
Standard_EXPORT  DF_Attribute* NewEmpty() const;
Standard_EXPORT  void Paste(DF_Attribute* into);
Standard_EXPORT ~SALOMEDSImpl_AttributeTarget() {}

};

#endif
