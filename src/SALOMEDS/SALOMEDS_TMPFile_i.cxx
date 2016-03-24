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

//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDSImpl_SObject.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_TMPFile_i.hxx"

#include <stdexcept>

SALOMEDS_TMPFile_i
::SALOMEDS_TMPFile_i(SALOMEDS::TMPFile* theStream):
  myStream(theStream)
{}

size_t
SALOMEDS_TMPFile_i
::Size()
{
  if(!&myStream.in())
    return 0;

  return myStream->length();
}

SALOMEDSImpl_TMPFile::TOctet&
SALOMEDS_TMPFile_i
::Get(size_t theIndex)
{
  if(!&myStream.in())
    throw std::out_of_range("SALOMEDS_TMPFile_i::Get(size_t) const - !&myStream.in()");

  if(theIndex < 0 || theIndex >= myStream->length())
    throw std::out_of_range("SALOMEDS_TMPFile_i::Get(size_t) const - theIndex < 0 || theIndex >= myStream->length()");

  return myStream[theIndex];
}


//-----------------------------------------------------------------------------
Engines_TMPFile_i
::Engines_TMPFile_i(Engines::TMPFile* theStream):
  myStream(theStream)
{}

size_t
Engines_TMPFile_i
::Size()
{
  if(!&myStream.in())
    return 0;

  return myStream->length();
}

SALOMEDSImpl_TMPFile::TOctet&
Engines_TMPFile_i
::Get(size_t theIndex)
{
  if(!&myStream.in())
    throw std::out_of_range("Engines_TMPFile_i::Get(size_t) const - !&myStream.in()");

  if(theIndex < 0 || theIndex >= myStream->length())
    throw std::out_of_range("Engines_TMPFile_i::Get(size_t) const - theIndex < 0 || theIndex >= myStream->length()");

  return myStream[theIndex];
}
