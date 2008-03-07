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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef _SALOMEDSImpl_ChildNodeIterator_HeaderFile
#define _SALOMEDSImpl_ChildNodeIterator_HeaderFile

#include "SALOMEDSImpl_AttributeTreeNode.hxx"

class SALOMEDSImpl_ChildNodeIterator  
{

public:

Standard_EXPORT SALOMEDSImpl_ChildNodeIterator();
Standard_EXPORT SALOMEDSImpl_ChildNodeIterator(const SALOMEDSImpl_AttributeTreeNode* aTreeNode,
					       const bool allLevels = false);
Standard_EXPORT void Initialize(const SALOMEDSImpl_AttributeTreeNode* aTreeNode,
				const bool allLevels = false) ;
Standard_EXPORT bool More() const { return (myNode); }
Standard_EXPORT void Next() ;
Standard_EXPORT void NextBrother() ;
Standard_EXPORT SALOMEDSImpl_AttributeTreeNode* Value() const { return myNode; }

private: 

SALOMEDSImpl_AttributeTreeNode* myNode;
int myFirstLevel;

};


#endif
