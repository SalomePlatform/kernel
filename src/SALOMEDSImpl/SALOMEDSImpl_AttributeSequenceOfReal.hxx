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
//  File   : SALOMEDSImpl_AttributeSequenceOfReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeSequenceOfReal_HeaderFile
#define _SALOMEDSImpl_AttributeSequenceOfReal_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include <vector>
#include "SALOMEDSImpl_GenericAttribute.hxx"


class SALOMEDSImpl_AttributeSequenceOfReal : public SALOMEDSImpl_GenericAttribute 
{

public:

Standard_EXPORT virtual std::string Save();
Standard_EXPORT virtual void Load(const std::string&);
 
Standard_EXPORT static const std::string& GetID() ;
Standard_EXPORT static SALOMEDSImpl_AttributeSequenceOfReal* Set(const DF_Label& label) ;
Standard_EXPORT SALOMEDSImpl_AttributeSequenceOfReal();
Standard_EXPORT   void Assign(const std::vector<double>& other) ;
Standard_EXPORT   void ChangeValue(const int Index,const double& Value) ;
Standard_EXPORT   void Add(const double& Value) ;
Standard_EXPORT   double Value(const int Index) ;
Standard_EXPORT   void Remove(const int Index) ;
Standard_EXPORT   int Length() ;
Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT   void Restore(DF_Attribute* with) ;
Standard_EXPORT   DF_Attribute* NewEmpty() const;
Standard_EXPORT   void Paste(DF_Attribute* into);
Standard_EXPORT ~SALOMEDSImpl_AttributeSequenceOfReal() {}
Standard_EXPORT  const std::vector<double>&  Array() { return myValue; }

private: 
std::vector<double> myValue;

};

#endif
