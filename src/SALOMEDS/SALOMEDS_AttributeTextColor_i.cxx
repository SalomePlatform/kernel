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

//  File   : SALOMEDS_AttributeTextColor_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS.hxx" 
#include <vector>

SALOMEDS::Color SALOMEDS_AttributeTextColor_i::TextColor() 
{
  SALOMEDS::Locker lock;
  SALOMEDS::Color TextColor;
  std::vector<double> anArray = dynamic_cast<SALOMEDSImpl_AttributeTextColor*>(_impl)->TextColor();
  if (anArray.size()!=3) { 
    TextColor.R = 0;
    TextColor.G = 0;
    TextColor.B = 0;
  }
  else {
    TextColor.R = anArray[0];
    TextColor.G = anArray[1];
    TextColor.B = anArray[2];
  }
  return TextColor;
}

void SALOMEDS_AttributeTextColor_i::SetTextColor(const SALOMEDS::Color& value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  std::vector<double> anArray;
  anArray.push_back(value.R);
  anArray.push_back(value.G);
  anArray.push_back(value.B);
  dynamic_cast<SALOMEDSImpl_AttributeTextColor*>(_impl)->ChangeArray(anArray);
}

