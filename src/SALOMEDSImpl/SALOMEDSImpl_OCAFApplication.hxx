//  File   : SALOMEDSImpl_OCAFApplication.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_OCAFApplication_HeaderFile
#define _SALOMEDSImpl_OCAFApplication_HeaderFile


//Handle definition
#include <TDocStd_Application.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_OCAFApplication,  TDocStd_Application)

#include <Standard_CString.hxx>
#include <TColStd_SequenceOfExtendedString.hxx>


class SALOMEDSImpl_OCAFApplication : public TDocStd_Application {

public:

Standard_EXPORT SALOMEDSImpl_OCAFApplication();
Standard_EXPORT virtual  void Formats(TColStd_SequenceOfExtendedString& Formats) ;
Standard_EXPORT   Standard_CString ResourcesName() ;
Standard_EXPORT ~SALOMEDSImpl_OCAFApplication() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_OCAFApplication )

};

#endif
