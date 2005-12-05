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
//  File   : SALOMEDSImpl_UseCaseIterator.cxx
//  Author : Serge RUIN
//  Module : SALOME

#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_UseCaseIterator, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_UseCaseIterator, MMgt_TShared )

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseIterator::SALOMEDSImpl_UseCaseIterator(const TDF_Label& theLabel, 
						           const Standard_GUID& theGUID,
						           const bool allLevels)
:_guid(theGUID), _levels(allLevels)
{
  if(theLabel.FindAttribute(_guid, _node)) {
    _it.Initialize (_node, _levels);
  }
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseIterator::~SALOMEDSImpl_UseCaseIterator()
{
}

//============================================================================
/*! Function :Init
 * 
 */
//============================================================================
void SALOMEDSImpl_UseCaseIterator::Init(bool allLevels)
{ 
  _it.Initialize (_node, allLevels);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
bool SALOMEDSImpl_UseCaseIterator::More()
{
  return _it.More();
}

 //============================================================================
/*! Function : Next
 * 
 */
//============================================================================
void SALOMEDSImpl_UseCaseIterator::Next()
{
  _it.Next();
}


//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_UseCaseIterator::Value()
{
  TDF_Label L = _it.Value()->Label();
  return SALOMEDSImpl_Study::SObject(L);
}

