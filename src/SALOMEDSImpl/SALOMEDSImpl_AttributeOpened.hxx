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
//  File   : SALOMEDSImpl_AttributeOpened.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeOpened_HeaderFile
#define _SALOMEDSImpl_AttributeOpened_HeaderFile

#include "DF_Attribute.hxx"
#include <string>
#include "DF_Label.hxx"       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSImpl_AttributeOpened : public SALOMEDSImpl_GenericAttribute 
{
private:
int myValue;

public:

Standard_EXPORT virtual std::string Save() { return (myValue == 0)?(char*)"0":(char*)"1"; }
Standard_EXPORT virtual void Load(const std::string& theValue) { (theValue == "0")?myValue=0:myValue=1; } 

Standard_EXPORT static const std::string& GetID() ;
Standard_EXPORT static SALOMEDSImpl_AttributeOpened* Set(const DF_Label& label,const int value) ;
Standard_EXPORT SALOMEDSImpl_AttributeOpened();
Standard_EXPORT void SetOpened(const int value); 
Standard_EXPORT int IsOpened() const { return myValue; }  
Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT   void Restore(DF_Attribute* with) ;
Standard_EXPORT   DF_Attribute* NewEmpty() const;
Standard_EXPORT   void Paste(DF_Attribute* into);
Standard_EXPORT ~SALOMEDSImpl_AttributeOpened() {}

};

#endif
