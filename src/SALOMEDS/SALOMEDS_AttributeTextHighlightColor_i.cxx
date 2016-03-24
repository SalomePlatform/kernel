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

//  File   : SALOMEDS_AttributeTextHighlightColor_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS.hxx"
#include <vector>

SALOMEDS::Color SALOMEDS_AttributeTextHighlightColor_i::TextHighlightColor() 
{
  SALOMEDS::Locker lock;
  SALOMEDS::Color TextHighlightColor;
  std::vector<double> anArray = dynamic_cast<SALOMEDSImpl_AttributeTextHighlightColor*>(_impl)->TextHighlightColor();
  if (anArray.size()!=3) { 
    TextHighlightColor.R = 0;
    TextHighlightColor.G = 0;
    TextHighlightColor.B = 0;
  }
  else {
    TextHighlightColor.R = anArray[0];
    TextHighlightColor.G = anArray[1];
    TextHighlightColor.B = anArray[2];
  }
  return TextHighlightColor;
}

void SALOMEDS_AttributeTextHighlightColor_i::SetTextHighlightColor(const SALOMEDS::Color& value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  std::vector<double> anArray;
  anArray.push_back(value.R);
  anArray.push_back(value.G);
  anArray.push_back(value.B);
  dynamic_cast<SALOMEDSImpl_AttributeTextHighlightColor*>(_impl)->ChangeArray(anArray);
}
