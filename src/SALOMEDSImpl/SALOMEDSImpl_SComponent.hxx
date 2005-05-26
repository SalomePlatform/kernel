//  File   : SALOMEDSImpl_SComponent.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SCOMPONENT_H__
#define __SALOMEDSIMPL_SCOMPONENT_H__

//SALOMEDSImpl headers
#include "SALOMEDSImpl_SObject.hxx"

//Handle definition
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_SComponent, SALOMEDSImpl_SObject )

// std C++ headers
#include <iostream.h>

// Cascade headers
#include <TDF_Label.hxx>
#include <SALOMEDSImpl_SObject.hxx>
#include <TCollection_AsciiString.hxx> 
#include <TDF_Tool.hxx>
#include <stdio.h>

class SALOMEDSImpl_SComponent : public SALOMEDSImpl_SObject
{
public:
  
  SALOMEDSImpl_SComponent(const TDF_Label& lab);
  
  ~SALOMEDSImpl_SComponent();
 
  virtual TCollection_AsciiString ComponentDataType();
  virtual bool ComponentIOR(TCollection_AsciiString& theID);
  static bool IsA(const TDF_Label& theLabel);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SComponent )    

};
#endif
