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

//  File   : SALOMEDSImpl_SComponentIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_Study.hxx"

//============================================================================
/*! Function : constructor
 * 
 */
//============================================================================

SALOMEDSImpl_SComponentIterator::SALOMEDSImpl_SComponentIterator(DF_Document* theDocument)
{
  _lab = theDocument->Main();
  _it.Init (_lab);
}

//============================================================================
/*! Function : Init
 * 
 */
//============================================================================
void SALOMEDSImpl_SComponentIterator::Init()
{ 
  _it.Init (_lab);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
bool SALOMEDSImpl_SComponentIterator::More()
{
  if (!_it.More())
    return false;
  DF_Label L = _it.Value();
  return SALOMEDSImpl_SComponent::IsA(L);
}

//============================================================================
/*! Function : Next
  */
//============================================================================
void SALOMEDSImpl_SComponentIterator::Next()
{
  _it.Next();
}


//============================================================================
/*! Function : Value
 * 
 */
//============================================================================
SALOMEDSImpl_SComponent SALOMEDSImpl_SComponentIterator::Value()
{
  return SALOMEDSImpl_Study::SComponent(_it.Value());
}

//============================================================================
/*! Function : GetPersistentCopy
  */
//============================================================================
SALOMEDSImpl_SComponentIterator* SALOMEDSImpl_SComponentIterator::GetPersistentCopy() const
{
  SALOMEDSImpl_SComponentIterator* itr = new SALOMEDSImpl_SComponentIterator();
  itr->_lab = _lab;
  itr->_it = _it; 
  
  return itr;
}
