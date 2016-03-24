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

//  File   : SALOMEDSImpl_SComponent.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx" 

//============================================================================
/*! Function : Empty constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent::SALOMEDSImpl_SComponent()
{
}

//============================================================================
/*! Function : Copy constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent::SALOMEDSImpl_SComponent(const SALOMEDSImpl_SComponent& theSCO)
{
  _lab   = theSCO._lab;
  _value = theSCO._value;
  _type  = theSCO._type;
  _name  = theSCO._name;
}

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent::SALOMEDSImpl_SComponent(const DF_Label& theLabel)
  :SALOMEDSImpl_SObject(theLabel)
{
}
  
//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent::~SALOMEDSImpl_SComponent()
{
}
  
  
//============================================================================
/*! Function : ComponentDataType
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_SComponent::ComponentDataType()
{
  std::string res = "";
  SALOMEDSImpl_AttributeComment* type;
  if ( (type = (SALOMEDSImpl_AttributeComment*)_lab.FindAttribute(SALOMEDSImpl_AttributeComment::GetID())) ) {
    res = type->Value();
  }

  return res;
}
  

//============================================================================
/*! Function : ComponentIOR
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SComponent::ComponentIOR(std::string& IOR)
{
  SALOMEDSImpl_AttributeIOR* ior;
  if (!(ior = (SALOMEDSImpl_AttributeIOR*)_lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID())) )
      return false;
  IOR = ior->Value();
  return true;
}


//============================================================================
/*! Function : IsA
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SComponent::IsA(const DF_Label& theLabel)
{
  // scomponent must contain comment and belong to the 2th depth label
  if ( theLabel.IsAttribute(SALOMEDSImpl_AttributeComment::GetID()) && theLabel.Depth() == 2) {
    return true;
  }
  return false;
}

//============================================================================
/*! Function :  GetPersistentCopy
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SComponent* SALOMEDSImpl_SComponent::GetPersistentCopy() const
{
  SALOMEDSImpl_SComponent* sco = new SALOMEDSImpl_SComponent;
  sco->_lab = _lab;
  sco->_name = _name;
  sco->_type = _type;
  sco->_value = _value; 
  return sco;
}
