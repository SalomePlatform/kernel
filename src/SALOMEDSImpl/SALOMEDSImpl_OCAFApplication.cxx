//  File   : SALOMEDSImpl_OCAFApplication.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_OCAFApplication.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_OCAFApplication, TDocStd_Application )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_OCAFApplication, TDocStd_Application )

//=======================================================================
//function : SALOMEDSImpl_OCAFApplication
//purpose  : 
//=======================================================================

SALOMEDSImpl_OCAFApplication::SALOMEDSImpl_OCAFApplication() 
{
}


//=======================================================================
//function : Formats
//purpose  : 
//=======================================================================

void SALOMEDSImpl_OCAFApplication::Formats(TColStd_SequenceOfExtendedString& Formats) 
{  
  Formats.Append(TCollection_ExtendedString ("SALOME_STUDY"));
}


//=======================================================================
//function : ResourcesName
//purpose  : 
//=======================================================================

Standard_CString SALOMEDSImpl_OCAFApplication::ResourcesName() 
{
  return Standard_CString ("SALOMEDSImpl_Resources");
}





