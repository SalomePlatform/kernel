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

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif

#include "SALOMEDSImpl_AttributeTreeNode.hxx"


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDSImpl_ChildNodeIterator  
{

public:

Standard_EXPORT SALOMEDSImpl_ChildNodeIterator();
Standard_EXPORT SALOMEDSImpl_ChildNodeIterator(const Handle(SALOMEDSImpl_AttributeTreeNode)& aTreeNode,
					       const Standard_Boolean allLevels = Standard_False);
Standard_EXPORT void Initialize(const Handle(SALOMEDSImpl_AttributeTreeNode)& aTreeNode,
				const Standard_Boolean allLevels = Standard_False) ;
Standard_EXPORT Standard_Boolean More() const { return !myNode.IsNull(); }
Standard_EXPORT void Next() ;
Standard_EXPORT void NextBrother() ;
Standard_EXPORT Handle_SALOMEDSImpl_AttributeTreeNode Value() const { return myNode; }

private: 

Handle_SALOMEDSImpl_AttributeTreeNode myNode;
Standard_Integer myFirstLevel;

};


#endif
