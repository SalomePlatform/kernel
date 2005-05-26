//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDSImpl_SObject.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Study.hxx"

#include <TDF_AttributeIterator.hxx>
#include <map>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_SObject, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_SObject, MMgt_TShared )

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SObject::SALOMEDSImpl_SObject(const TDF_Label& theLabel)
  :_lab(theLabel)
{
  _value = "";
  _type = "";
  _name = "";
}

//============================================================================
/*! Function : Desctructor
 *  Purpose  : 
 */
//============================================================================    
SALOMEDSImpl_SObject::~SALOMEDSImpl_SObject() 
{}

//============================================================================
/*! Function : GetID
 *  Purpose  : 
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_SObject::GetID()
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry (_lab,anEntry);
  return anEntry;
}
  
//============================================================================
/*! Function : GetFatherComponent
 *  Purpose  : 
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_SObject::GetFatherComponent()
{
  TDF_Label LF = _lab;
  while (!SALOMEDSImpl_SComponent::IsA(LF) && !LF.IsRoot()) {
    LF = LF.Father();
  }
  
  if(LF.IsRoot()) return NULL;
  
  return GetStudy()->GetSComponent(LF);
}
  
//============================================================================
/*! Function : GetFather
 *  Purpose  : 
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_SObject::GetFather()
{
  return GetStudy()->GetSObject(_lab.Father());    
}


//============================================================================
/*! Function : GetStudy
 *  Purpose  : 
 */
//============================================================================
Handle(SALOMEDSImpl_Study) SALOMEDSImpl_SObject::GetStudy()
{
  return SALOMEDSImpl_Study::GetStudy(_lab);
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
bool SALOMEDSImpl_SObject::FindAttribute(Handle(TDF_Attribute)& theAttribute, 
					 const TCollection_AsciiString& theTypeOfAttribute)
{
  if(_lab.IsNull()) return Standard_False;
  Standard_GUID aGUID = GetGUID(theTypeOfAttribute);
  if (_lab.FindAttribute(aGUID, theAttribute)) return Standard_True;
  return Standard_False;
}



//============================================================================
/*! Function : GetAllAttributes
 *  Purpose  : Returns list of all attributes for this sobject
 */
//============================================================================
Handle(TColStd_HSequenceOfTransient) SALOMEDSImpl_SObject::GetAllAttributes()
{
  Standard_Integer NumAttr = _lab.NbAttributes();
  Handle(TColStd_HSequenceOfTransient) SeqOfAttr = new TColStd_HSequenceOfTransient();
  Handle(SALOMEDSImpl_GenericAttribute) anAttr;
  if (NumAttr != 0) {
    for(TDF_AttributeIterator iter(_lab);iter.More();iter.Next()) {
      anAttr = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(iter.Value());
      if(!anAttr.IsNull() && anAttr->Type() != "AttributeReference")
	SeqOfAttr->Append(anAttr);
    }
  }
  return SeqOfAttr;
}


//============================================================================
/*! Function : ReferencedObject
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SObject::ReferencedObject(Handle(SALOMEDSImpl_SObject)& theObject)
{
  Handle(SALOMEDSImpl_AttributeReference) Ref;
  if (!_lab.FindAttribute(SALOMEDSImpl_AttributeReference::GetID(),Ref))
    return false;
  
  theObject =  GetStudy()->GetSObject(Ref->Get());
  return true;
}

//============================================================================
/*! Function : FindSubObject
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SObject::FindSubObject(int theTag, Handle(SALOMEDSImpl_SObject)& theObject)
{
  TDF_Label L = _lab.FindChild(theTag, false);
  if (L.IsNull()) return false;
  
  theObject = GetStudy()->GetSObject(L);
  return true;
    
}  

  
//============================================================================
/*! Function : GetName
 *  Purpose  : 
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_SObject::GetName() 
{
  TCollection_AsciiString aStr = "";
  Handle(SALOMEDSImpl_AttributeName) aName;
  if (_lab.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aName)) {
    aStr = aName->Value();
  }
  return aStr;
}

//============================================================================
/*! Function : GetComment
 *  Purpose  : 
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_SObject::GetComment() 
{
  TCollection_AsciiString aStr = "";
  Handle(SALOMEDSImpl_AttributeComment) aComment;
  if (_lab.FindAttribute(SALOMEDSImpl_AttributeComment::GetID(), aComment)) {
    aStr = aComment->Value();
  }
  return aStr;
}

//============================================================================
/*! Function : GetIOR
 *  Purpose  : 
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_SObject::GetIOR() 
{
  TCollection_AsciiString aStr = "";
  Handle(SALOMEDSImpl_AttributeIOR) anIOR;
  if (_lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), anIOR)) {
    aStr = anIOR->Value();
  }
  return aStr;
}


Standard_GUID SALOMEDSImpl_SObject::GetGUID(const TCollection_AsciiString& theType) 
{
  __AttributeTypeToGUIDForSObject

  if (strncmp(theType.ToCString(), "AttributeTreeNodeGUID",21) == 0) {
    const char* aCType = theType.ToCString();
    char* aGUIDString = new char[40]; 
    sprintf(aGUIDString, &(aCType[21]));
    Standard_GUID aGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
    delete(aGUIDString);
    return aGUID;
  }
  return Standard_GUID();
}


