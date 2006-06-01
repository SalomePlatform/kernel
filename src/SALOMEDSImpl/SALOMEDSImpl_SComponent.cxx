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
//  File   : SALOMEDSImpl_SComponent.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx" 

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_SComponent, SALOMEDSImpl_SObject )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_SComponent, SALOMEDSImpl_SObject )

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent::SALOMEDSImpl_SComponent(const TDF_Label& theLabel)
  :SALOMEDSImpl_SObject(theLabel)
{
}
  
//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent::~SALOMEDSImpl_SComponent()
{}
  
  
//============================================================================
/*! Function : ComponentDataType
 *  Purpose  : 
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_SComponent::ComponentDataType()
{
  TCollection_AsciiString res = "";
  Handle(SALOMEDSImpl_AttributeComment) type;
  if ( _lab.FindAttribute(SALOMEDSImpl_AttributeComment::GetID(),type) ) {
    res = type->Value();
  }

  return res;
}
  

//============================================================================
/*! Function : ComponentIOR
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SComponent::ComponentIOR(TCollection_AsciiString& IOR)
{
  Handle(SALOMEDSImpl_AttributeIOR) ior;
  if (!_lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(),ior) )
      return false;
  IOR = ior->Value();
  return true;
}
  

//============================================================================
/*! Function : IsA
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SComponent::IsA(const TDF_Label& theLabel)
{
  // scomponent must contain comment and belong to the 2th depth label
  if ( theLabel.IsAttribute(SALOMEDSImpl_AttributeComment::GetID()) && theLabel.Depth() == 2) {
    return true;
  }
  return false;
}
