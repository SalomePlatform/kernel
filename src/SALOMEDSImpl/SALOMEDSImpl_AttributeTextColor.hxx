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

//  File   : SALOMEDSImpl_AttributeTextColor.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef _SALOMEDSImpl_AttributeTextColor_HeaderFile
#define _SALOMEDSImpl_AttributeTextColor_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include <vector>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeTextColor : 
  public SALOMEDSImpl_GenericAttribute 
{
private:
  std::vector<double> myValue;

public:
  virtual std::string Save();
  virtual void Load(const std::string&); 
  static const std::string& GetID() ;
  SALOMEDSImpl_AttributeTextColor();
  const std::string& ID() const;
  void SetTextColor(const double& R, const double& G, const double& B); 
  std::vector<double> TextColor();  
  void ChangeArray(const std::vector<double>& newArray);
  void Restore(DF_Attribute* with) ;
  DF_Attribute* NewEmpty() const;
  void Paste( DF_Attribute* into);
  ~SALOMEDSImpl_AttributeTextColor() {}

};

#endif
