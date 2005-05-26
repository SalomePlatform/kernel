//  File   : SALOMEDS_Attributes.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_ATTRIBUTES__H__
#define __SALOMEDS_ATTRIBUTES__H__

#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeName.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeInteger.hxx"
#include "SALOMEDSImpl_AttributeReal.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDSImpl_AttributeUserID.hxx"
#include "SALOMEDSImpl_AttributeReference.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx" 
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include "SALOMEDSImpl_AttributePersistentRef.hxx" 
#include "SALOMEDSImpl_AttributeDrawable.hxx"
#include "SALOMEDSImpl_AttributeSelectable.hxx"
#include "SALOMEDSImpl_AttributeExpandable.hxx"
#include "SALOMEDSImpl_AttributeOpened.hxx"
#include "SALOMEDSImpl_AttributeTextColor.hxx"
#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"
#include "SALOMEDSImpl_AttributePixMap.hxx"
#include "SALOMEDSImpl_AttributeLocalID.hxx"
#include "SALOMEDSImpl_AttributeTarget.hxx"
#include "SALOMEDSImpl_AttributeTableOfInteger.hxx"
#include "SALOMEDSImpl_AttributeTableOfReal.hxx"
#include "SALOMEDSImpl_AttributeTableOfString.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributePythonObject.hxx"
#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"
#include "SALOMEDSImpl_AttributeFileType.hxx"
#include "SALOMEDSImpl_AttributeFlags.hxx"
#include "SALOMEDSImpl_AttributeGraphic.hxx"

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_AttributeName_i.hxx"
#include "SALOMEDS_AttributeComment_i.hxx"
#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "SALOMEDS_AttributeUserID_i.hxx"
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx" 
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS_AttributePersistentRef_i.hxx" 
#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS_AttributeSelectable_i.hxx"
#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_AttributeTableOfInteger_i.hxx"
#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_AttributePythonObject_i.hxx"
#include "SALOMEDS_AttributeExternalFileDef_i.hxx"
#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS_AttributeFlags_i.hxx"
#include "SALOMEDS_AttributeGraphic_i.hxx"

#define __CreateCORBAAttribute(CORBA_Name) if (strcmp(aTypeOfAttribute, #CORBA_Name) == 0) { \
    Handle(SALOMEDSImpl_##CORBA_Name) A = Handle(SALOMEDSImpl_##CORBA_Name)::DownCast(theAttr); \
    SALOMEDS_##CORBA_Name##_i* Attr = new SALOMEDS_##CORBA_Name##_i(A, theOrb); \
    attr_servant = Attr; \
    anAttribute = Attr->CORBA_Name::_this(); \
  }


#define __CreateGenericCORBAAttribute \
__CreateCORBAAttribute(AttributeReal) \
__CreateCORBAAttribute(AttributeInteger) \
__CreateCORBAAttribute(AttributeSequenceOfReal) \
__CreateCORBAAttribute(AttributeSequenceOfInteger) \
__CreateCORBAAttribute(AttributeName) \
__CreateCORBAAttribute(AttributeComment) \
__CreateCORBAAttribute(AttributeIOR) \
__CreateCORBAAttribute(AttributePixMap) \
__CreateCORBAAttribute(AttributeLocalID) \
__CreateCORBAAttribute(AttributeTableOfInteger) \
__CreateCORBAAttribute(AttributeTableOfReal) \
__CreateCORBAAttribute(AttributeTableOfString) \
__CreateCORBAAttribute(AttributePythonObject) \
__CreateCORBAAttribute(AttributePersistentRef) \
__CreateCORBAAttribute(AttributeDrawable) \
__CreateCORBAAttribute(AttributeSelectable) \
__CreateCORBAAttribute(AttributeExpandable) \
__CreateCORBAAttribute(AttributeOpened) \
__CreateCORBAAttribute(AttributeTextColor) \
__CreateCORBAAttribute(AttributeTextHighlightColor) \
__CreateCORBAAttribute(AttributeTarget) \
__CreateCORBAAttribute(AttributeStudyProperties) \
__CreateCORBAAttribute(AttributeExternalFileDef) \
__CreateCORBAAttribute(AttributeFileType) \
__CreateCORBAAttribute(AttributeFlags) \
__CreateCORBAAttribute(AttributeGraphic) \
__CreateCORBAAttribute(AttributeTreeNode) \
__CreateCORBAAttribute(AttributeUserID)

#endif
