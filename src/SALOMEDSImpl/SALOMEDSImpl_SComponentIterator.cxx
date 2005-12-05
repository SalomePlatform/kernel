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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_SComponentIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_Study.hxx"

using namespace std;

//============================================================================
/*! Function : constructor
 * 
 */
//============================================================================

SALOMEDSImpl_SComponentIterator::SALOMEDSImpl_SComponentIterator(const Handle(TDocStd_Document)& theDocument)
{
  _lab = theDocument->Main();
  _it.Initialize (_lab);
}

//============================================================================
/*! Function : Init
 * 
 */
//============================================================================
void SALOMEDSImpl_SComponentIterator::Init()
{ 
  _it.Initialize (_lab);
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
  TDF_Label L = _it.Value();
  if (SALOMEDSImpl_SComponent::IsA(L))
     return true;

  return _it.More();
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
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_SComponentIterator::Value()
{
  return SALOMEDSImpl_Study::SComponent(_it.Value());
}

