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
//  File   : SALOMEDSImpl_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeReference_HeaderFile
#define _SALOMEDSImpl_AttributeReference_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"       
#include <string> 
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSImpl_AttributeReference : public SALOMEDSImpl_GenericAttribute 
{
private:
 DF_Label myLabel;

public:

Standard_EXPORT static const std::string& GetID() ;

Standard_EXPORT SALOMEDSImpl_AttributeReference():SALOMEDSImpl_GenericAttribute("AttributeReference") {}
Standard_EXPORT static SALOMEDSImpl_AttributeReference* Set(const DF_Label& theLabel, const DF_Label& theRefLabel);
Standard_EXPORT void Set(const DF_Label& theLabel);
Standard_EXPORT DF_Label Get() const { return myLabel; }
Standard_EXPORT virtual std::string Save();
Standard_EXPORT virtual void Load(const std::string& theValue);

Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT  void Restore(DF_Attribute* with) ;
Standard_EXPORT  DF_Attribute* NewEmpty() const;
Standard_EXPORT  void Paste(DF_Attribute* into);

Standard_EXPORT ~SALOMEDSImpl_AttributeReference() {}
};

#endif
