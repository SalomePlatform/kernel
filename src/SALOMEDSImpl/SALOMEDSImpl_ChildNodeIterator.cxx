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

// File:        SALOMEDSImpl_ChildNodeIterator.cxx
// Created:     Wed Jan 26 16:43:08 2000
// Author:      Denis PASCAL
//              <dp@dingox.paris1.matra-dtv.fr>
//
#include "SALOMEDSImpl_ChildNodeIterator.hxx"

#define ChildNodeIterator_UpToBrother \
{ \
    while (myNode && (myNode->Depth() > myFirstLevel) && myNode->myNext == NULL) \
      myNode = myNode->myFather; \
        if (myNode && (myNode->Depth() > myFirstLevel) && myNode->myFather != NULL) \
          myNode = myNode->myNext; \
        else \
          myNode = NULL; \
}

//=======================================================================
//function : SALOMEDSImpl_ChildNodeIterator
//purpose  : 
//=======================================================================
SALOMEDSImpl_ChildNodeIterator::SALOMEDSImpl_ChildNodeIterator()
  :myNode(0), myFirstLevel(0)
{}

//=======================================================================
//function : SALOMEDSImpl_ChildNodeIterator
//purpose  : 
//=======================================================================

SALOMEDSImpl_ChildNodeIterator::SALOMEDSImpl_ChildNodeIterator (const SALOMEDSImpl_AttributeTreeNode* aNode,
                                                                const bool allLevels)
: myNode(aNode->myFirst),
  myFirstLevel(allLevels ? aNode->Depth() : -1)
{}

//=======================================================================
//function : Initialize
//purpose  : 
//=======================================================================

void SALOMEDSImpl_ChildNodeIterator::Initialize(const SALOMEDSImpl_AttributeTreeNode* aNode,
                                                const bool allLevels)
{
  myNode = aNode->myFirst;
  myFirstLevel = allLevels ? aNode->Depth() : -1;
}

//=======================================================================
//function : Next
//purpose  : 
//=======================================================================

void SALOMEDSImpl_ChildNodeIterator::Next() 
{
  if (myFirstLevel == -1) {
    myNode = myNode->myNext;
  }
  else {
    if (myNode->myFirst != NULL) myNode = myNode->myFirst;
    else ChildNodeIterator_UpToBrother;
  }
}

//=======================================================================
//function : NextBrother
//purpose  : 
//=======================================================================

void SALOMEDSImpl_ChildNodeIterator::NextBrother() 
{
  if (myNode->myNext != NULL) myNode = myNode->myNext;
  else ChildNodeIterator_UpToBrother;
}
