using namespace std;
//  File      : SALOMEDS_OCAFApplication.cxx
//  Created   : Wed Nov 28 16:22:26 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$


#include "SALOMEDS_OCAFApplication.ixx"

//=======================================================================
//function : SALOMEDS_OCAFApplication
//purpose  : 
//=======================================================================

SALOMEDS_OCAFApplication::SALOMEDS_OCAFApplication() 
{
}


//=======================================================================
//function : Formats
//purpose  : 
//=======================================================================

void SALOMEDS_OCAFApplication::Formats(TColStd_SequenceOfExtendedString& Formats) 
{  
  Formats.Append(TCollection_ExtendedString ("SALOME_STUDY"));
}


//=======================================================================
//function : ResourcesName
//purpose  : 
//=======================================================================

Standard_CString SALOMEDS_OCAFApplication::ResourcesName() 
{
  return Standard_CString ("SALOMEDS_Resources");
}





