//  File   : SALOMEDS_Attributes.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSCLIENT_ATTRIBUTES__H__
#define __SALOMEDSCLIENT_ATTRIBUTES__H__

#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeName.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeInteger.hxx"
#include "SALOMEDSImpl_AttributeReal.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDSImpl_AttributeUserID.hxx"
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

#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDS_AttributeName.hxx"
#include "SALOMEDS_AttributeComment.hxx"
#include "SALOMEDS_AttributeInteger.hxx"
#include "SALOMEDS_AttributeReal.hxx"
#include "SALOMEDS_AttributeTreeNode.hxx"
#include "SALOMEDS_AttributeUserID.hxx"
#include "SALOMEDS_AttributeSequenceOfReal.hxx"
#include "SALOMEDS_AttributeSequenceOfInteger.hxx" 
#include "SALOMEDS_AttributeIOR.hxx"
#include "SALOMEDS_AttributePersistentRef.hxx" 
#include "SALOMEDS_AttributeDrawable.hxx"
#include "SALOMEDS_AttributeSelectable.hxx"
#include "SALOMEDS_AttributeExpandable.hxx"
#include "SALOMEDS_AttributeOpened.hxx"
#include "SALOMEDS_AttributeTextColor.hxx"
#include "SALOMEDS_AttributeTextHighlightColor.hxx"
#include "SALOMEDS_AttributePixMap.hxx"
#include "SALOMEDS_AttributeLocalID.hxx"
#include "SALOMEDS_AttributeTarget.hxx"
#include "SALOMEDS_AttributeTableOfInteger.hxx"
#include "SALOMEDS_AttributeTableOfReal.hxx"
#include "SALOMEDS_AttributeTableOfString.hxx"
#include "SALOMEDS_AttributeStudyProperties.hxx"
#include "SALOMEDS_AttributePythonObject.hxx"
#include "SALOMEDS_AttributeExternalFileDef.hxx"
#include "SALOMEDS_AttributeFileType.hxx"
#include "SALOMEDS_AttributeFlags.hxx"
#include "SALOMEDS_AttributeGraphic.hxx"

#define __CreateClientAttributeLocal(CORBA_Name) if (strcmp(aTypeOfAttribute.c_str(), #CORBA_Name) == 0) { \
    Handle(SALOMEDSImpl_##CORBA_Name) A = Handle(SALOMEDSImpl_##CORBA_Name)::DownCast(theGA); \
    aGA = new SALOMEDS_##CORBA_Name(A); \
  }

#define __CreateClientAttributeCORBA(CORBA_Name) if (strcmp(aTypeOfAttribute.c_str(), #CORBA_Name) == 0) { \
    SALOMEDS::CORBA_Name##_var A = SALOMEDS::CORBA_Name::_narrow(theGA); \
    aGA = new SALOMEDS_##CORBA_Name(A); \
  }

#define __CreateGenericClientAttributeLocal \
__CreateClientAttributeLocal(AttributeReal) \
__CreateClientAttributeLocal(AttributeInteger) \
__CreateClientAttributeLocal(AttributeSequenceOfReal) \
__CreateClientAttributeLocal(AttributeSequenceOfInteger) \
__CreateClientAttributeLocal(AttributeName) \
__CreateClientAttributeLocal(AttributeComment) \
__CreateClientAttributeLocal(AttributeIOR) \
__CreateClientAttributeLocal(AttributePixMap) \
__CreateClientAttributeLocal(AttributeLocalID) \
__CreateClientAttributeLocal(AttributeTableOfInteger) \
__CreateClientAttributeLocal(AttributeTableOfReal) \
__CreateClientAttributeLocal(AttributeTableOfString) \
__CreateClientAttributeLocal(AttributePythonObject) \
__CreateClientAttributeLocal(AttributePersistentRef) \
__CreateClientAttributeLocal(AttributeDrawable) \
__CreateClientAttributeLocal(AttributeSelectable) \
__CreateClientAttributeLocal(AttributeExpandable) \
__CreateClientAttributeLocal(AttributeOpened) \
__CreateClientAttributeLocal(AttributeTextColor) \
__CreateClientAttributeLocal(AttributeTextHighlightColor) \
__CreateClientAttributeLocal(AttributeTarget) \
__CreateClientAttributeLocal(AttributeStudyProperties) \
__CreateClientAttributeLocal(AttributeExternalFileDef) \
__CreateClientAttributeLocal(AttributeFileType) \
__CreateClientAttributeLocal(AttributeFlags) \
__CreateClientAttributeLocal(AttributeGraphic) \
__CreateClientAttributeLocal(AttributeTreeNode) \
__CreateClientAttributeLocal(AttributeUserID)

#define __CreateGenericClientAttributeCORBA \
__CreateClientAttributeCORBA(AttributeReal) \
__CreateClientAttributeCORBA(AttributeInteger) \
__CreateClientAttributeCORBA(AttributeSequenceOfReal) \
__CreateClientAttributeCORBA(AttributeSequenceOfInteger) \
__CreateClientAttributeCORBA(AttributeName) \
__CreateClientAttributeCORBA(AttributeComment) \
__CreateClientAttributeCORBA(AttributeIOR) \
__CreateClientAttributeCORBA(AttributePixMap) \
__CreateClientAttributeCORBA(AttributeLocalID) \
__CreateClientAttributeCORBA(AttributeTableOfInteger) \
__CreateClientAttributeCORBA(AttributeTableOfReal) \
__CreateClientAttributeCORBA(AttributeTableOfString) \
__CreateClientAttributeCORBA(AttributePythonObject) \
__CreateClientAttributeCORBA(AttributePersistentRef) \
__CreateClientAttributeCORBA(AttributeDrawable) \
__CreateClientAttributeCORBA(AttributeSelectable) \
__CreateClientAttributeCORBA(AttributeExpandable) \
__CreateClientAttributeCORBA(AttributeOpened) \
__CreateClientAttributeCORBA(AttributeTextColor) \
__CreateClientAttributeCORBA(AttributeTextHighlightColor) \
__CreateClientAttributeCORBA(AttributeTarget) \
__CreateClientAttributeCORBA(AttributeStudyProperties) \
__CreateClientAttributeCORBA(AttributeExternalFileDef) \
__CreateClientAttributeCORBA(AttributeFileType) \
__CreateClientAttributeCORBA(AttributeFlags) \
__CreateClientAttributeCORBA(AttributeGraphic) \
__CreateClientAttributeCORBA(AttributeTreeNode) \
__CreateClientAttributeCORBA(AttributeUserID)

#endif
