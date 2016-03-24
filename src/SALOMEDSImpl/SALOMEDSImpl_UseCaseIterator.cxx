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

//  File   : SALOMEDSImpl_UseCaseIterator.cxx
//  Author : Serge RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"

//============================================================================
/*! Function : empty constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseIterator::SALOMEDSImpl_UseCaseIterator()
{
  _node = NULL;    
}


//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseIterator::SALOMEDSImpl_UseCaseIterator(const DF_Label& theLabel, 
                                                           const std::string& theGUID,
                                                           const bool allLevels)
:_guid(theGUID), _levels(allLevels)
{
  if((_node = (SALOMEDSImpl_AttributeTreeNode*)theLabel.FindAttribute(_guid))) {
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

SALOMEDSImpl_SObject SALOMEDSImpl_UseCaseIterator::Value()
{
  DF_Label L = _it.Value()->Label();
  return SALOMEDSImpl_Study::SObject(L);
}

//============================================================================
/*! Function : GetPersistentCopy
 * 
 */
//============================================================================
SALOMEDSImpl_UseCaseIterator* SALOMEDSImpl_UseCaseIterator::GetPersistentCopy() const
{
  SALOMEDSImpl_UseCaseIterator* itr = new SALOMEDSImpl_UseCaseIterator();
  itr->_it = _it;
  itr->_node = _node;
  itr->_guid = _guid;
  itr->_levels = _levels;
  
  return itr;
}
