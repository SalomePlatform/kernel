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

//  File   : SALOMEDS_AttributeTextColor.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTextColor.hxx"
#include "SALOMEDS.hxx"

#include <vector>

SALOMEDS_AttributeTextColor::SALOMEDS_AttributeTextColor(SALOMEDSImpl_AttributeTextColor* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTextColor::SALOMEDS_AttributeTextColor(SALOMEDS::AttributeTextColor_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTextColor::~SALOMEDS_AttributeTextColor()
{}


STextColor SALOMEDS_AttributeTextColor::TextColor()
{
  STextColor aColor;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    std::vector<double> aSeq = dynamic_cast<SALOMEDSImpl_AttributeTextColor*>(_local_impl)->TextColor();
    aColor.R = aSeq[0];
    aColor.G = aSeq[1];
    aColor.B = aSeq[2]; 
  }
  else {
    SALOMEDS::Color anImplColor = SALOMEDS::AttributeTextColor::_narrow(_corba_impl)->TextColor();
    aColor.R = anImplColor.R;
    aColor.G = anImplColor.G;
    aColor.B = anImplColor.B;
  }
  return aColor;
}

void SALOMEDS_AttributeTextColor::SetTextColor(STextColor value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    std::vector<double> aSeq;
    aSeq.push_back( value.R );
    aSeq.push_back( value.G );
    aSeq.push_back( value.B );
    dynamic_cast<SALOMEDSImpl_AttributeTextColor*>(_local_impl)->ChangeArray(aSeq);
  }
  else {
    SALOMEDS::Color aColor;
    aColor.R = value.R;
    aColor.G = value.G;
    aColor.B = value.B;
    SALOMEDS::AttributeTextColor::_narrow(_corba_impl)->SetTextColor(aColor);
  }
}
