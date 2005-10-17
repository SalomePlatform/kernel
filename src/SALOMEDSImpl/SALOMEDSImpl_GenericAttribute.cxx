//  File   : SALOMEDSImpl_GenericAttribute.hxx
//  Author : SERGEY_RUIN
//  Module : SALOME


#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_GenericAttribute, TDF_Attribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_GenericAttribute, TDF_Attribute )

char* SALOMEDSImpl_GenericAttribute::Impl_GetType(const Handle(TDF_Attribute)& theAttr)
{
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr);  
  return ga->Type().ToCString();
}

char* SALOMEDSImpl_GenericAttribute::Impl_GetClassType(const Handle(TDF_Attribute)& theAttr)
{
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr);
  return ga->GetClassType().ToCString();
} 

void SALOMEDSImpl_GenericAttribute::Impl_CheckLocked(const Handle(TDF_Attribute)& theAttr)
{
  Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr);
  ga->CheckLocked();
}

TCollection_AsciiString SALOMEDSImpl_GenericAttribute::Type() 
{ 
    return _type; 
}


void SALOMEDSImpl_GenericAttribute::CheckLocked()
{
  TDF_Label aLabel = Label();
  if(aLabel.IsNull()) return;

  Handle(SALOMEDSImpl_Study) aStudy = SALOMEDSImpl_Study::GetStudy(aLabel);
  if(aStudy.IsNull() || aStudy->NewBuilder()->HasOpenCommand()) return;
  if(aStudy->IsLocked()) {
    aStudy->_errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }                                         
}

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_GenericAttribute::GetSObject()
{
  TDF_Label aLabel = Label();
  if(aLabel.IsNull()) return NULL;
  return SALOMEDSImpl_Study::SObject(aLabel);
}

void SALOMEDSImpl_GenericAttribute::SetModifyFlag()
{
   TDF_Label aLabel = Label();
   if(aLabel.IsNull()) return; 

  Handle(SALOMEDSImpl_Study) aStudy = SALOMEDSImpl_Study::GetStudy(aLabel);
  if(!aStudy.IsNull()) aStudy->Modify();
}

