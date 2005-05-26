//  File   : SALOMEDSImpl_SComponent.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx" 

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
