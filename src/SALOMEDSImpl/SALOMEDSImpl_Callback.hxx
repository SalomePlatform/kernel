//  File   : SALOMEDSImpl_Callback.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_CALLBACK_H__
#define __SALOMEDSIMPL_CALLBACK_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_Callback, MMgt_TShared )

#include "SALOMEDSImpl_UseCaseBuilder.hxx"

class SALOMEDSImpl_Callback : public MMgt_TShared  
{
private:
  Handle(SALOMEDSImpl_UseCaseBuilder) _builder;

public:

 SALOMEDSImpl_Callback(const Handle(SALOMEDSImpl_UseCaseBuilder)& builder) 
 {
   _builder = builder;
 }

 virtual void OnAddSObject(Handle(SALOMEDSImpl_SObject) theObject) 
 {
   if(_builder != NULL && theObject != NULL) _builder->Append(theObject);
 }

 virtual void OnRemoveSObject(Handle(SALOMEDSImpl_SObject) theObject) 
 {
   if(_builder != NULL && theObject != NULL) _builder->Remove(theObject);
 }

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_Callback )

};

#endif
