//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_SObject_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "utilities.h"
#include "SALOMEDS_SObject_i.hxx"
//SALOMEDS Headers
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_IORAttribute.hxx"
#include <TDF_AttributeIterator.hxx>
using namespace std;

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SObject_i::SALOMEDS_SObject_i(const TDF_Label lab, CORBA::ORB_ptr orb)
  :_lab(lab)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _value = NULL;
  _type = NULL;
  _name = NULL;
  _liste_ba_type.resize(0);
}
  

//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SObject_i::~SALOMEDS_SObject_i()
{
  CORBA::string_free(_value);
  CORBA::string_free(_type);
  CORBA::string_free(_name);
}
  
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetID()
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry (_lab,anEntry);
  return CORBA::string_dup(anEntry.ToCString());
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_SObject_i::GetFatherComponent()
{
  TDF_Label LF = _lab;
  while (!SALOMEDS_SComponent_i::IsA(LF) && !LF.IsRoot()) {
    LF = LF.Father();
  }
  SALOMEDS_SComponent_i *  so_servant = new SALOMEDS_SComponent_i (LF,_orb);
  SALOMEDS::SComponent_var so;
  so= SALOMEDS::SComponent::_narrow(so_servant->SComponent::_this()); 
  return so;
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_SObject_i::GetFather()
{
  TDF_Label LF = _lab.Father();

  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (LF,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return so;
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_SObject_i::GetStudy()
{
  TDF_Label Root = _lab.Root();
  Handle(SALOMEDS_IORAttribute) Att;
  char* IOR;
  if (Root.FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){
    TCollection_AsciiString ch(Att->Get());
    IOR = CORBA::string_dup(ch.ToCString());
    CORBA::Object_var obj = _orb->string_to_object(IOR);
    SALOMEDS::Study_var Study = SALOMEDS::Study::_narrow(obj) ;
    ASSERT(!CORBA::is_nil(Study));
    return SALOMEDS::Study::_duplicate(Study); //return Study = abort...
  }
  MESSAGE("Problem GetStudy");
  return SALOMEDS::Study::_nil();
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindAttribute (SALOMEDS::GenericAttribute_out anAttribute, 
						  const char* aTypeOfAttribute)
{
  Handle(TDF_Attribute) anAttr;
  if (_lab.FindAttribute(SALOMEDS_GenericAttribute_i::GetGUID(aTypeOfAttribute), anAttr)) {
    anAttribute = SALOMEDS::GenericAttribute::_duplicate(SALOMEDS_GenericAttribute_i::CreateAttribute(_orb, anAttr));
    return Standard_True;
  }
  return Standard_False;
}

//============================================================================
/*! Function : GetAllAttributes
 *  Purpose  : Returns list of all attributes for this sobject
 */
//============================================================================

SALOMEDS::ListOfAttributes* SALOMEDS_SObject_i::GetAllAttributes()
{
  Standard_Integer NumAttr = _lab.NbAttributes();
  SALOMEDS::ListOfAttributes_var SeqOfAttr = new SALOMEDS::ListOfAttributes;
  //SeqOfAttr->length(NumAttr);
  if (NumAttr != 0) {
    Standard_Integer i = 0;
    for(TDF_AttributeIterator iter(_lab);iter.More();iter.Next()) {
      Handle(TDF_Attribute) anAttr = iter.Value();
      SALOMEDS::GenericAttribute_var anAttribute = SALOMEDS_GenericAttribute_i::CreateAttribute(_orb, anAttr);
      if (!CORBA::is_nil(anAttribute)) {
	SeqOfAttr->length(++i);
	SeqOfAttr[i - 1] = anAttribute;
      }
    }
  }
  return SeqOfAttr._retn();
}


//============================================================================
/*! Function : ReferencedObject
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::ReferencedObject(SALOMEDS::SObject_out obj)
{
  Handle(TDF_Reference) Ref;
  if (!_lab.FindAttribute(TDF_Reference::GetID(),Ref))
    return false;
  
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (Ref->Get(),_orb);
  obj  = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return true;
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindSubObject(long atag, SALOMEDS::SObject_out obj)
{
  TDF_Label L = _lab.FindChild(atag,false);
  if (L.IsNull()) return false;
  
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (L,_orb);
  obj  = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  return true;
    
}  

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::Name()
{
  return CORBA::string_dup(_name);
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
void  SALOMEDS_SObject_i::Name(const char* name)
{
  _name = CORBA::string_dup(name);
}
  
//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Tag()
{
  return _lab.Tag();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Depth()
{
  return _lab.Depth();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
CORBA::Object_ptr SALOMEDS_SObject_i::GetObject()
{
  CORBA::Object_ptr obj = CORBA::Object::_nil();
  try {
    Handle(SALOMEDS_IORAttribute) Att;
    if (_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)) {
      TCollection_AsciiString ch(Att->Get());
      char* IOR = CORBA::string_dup(ch.ToCString());
      obj = _orb->string_to_object(IOR);
    }
  } catch(...) {}
  return obj;
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetName() {
  CORBA::String_var aStr = CORBA::string_dup( "" );
  Handle(TDataStd_Name) aName;
  if (_lab.FindAttribute(TDataStd_Name::GetID(), aName)) {
    aStr = CORBA::string_dup(TCollection_AsciiString(aName->Get()).ToCString());
  }
  return aStr._retn();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetComment() {
  CORBA::String_var aStr = CORBA::string_dup( "" );
  Handle(TDataStd_Comment) aComment;
  if (_lab.FindAttribute(TDataStd_Comment::GetID(), aComment)) {
    aStr = CORBA::string_dup(TCollection_AsciiString(aComment->Get()).ToCString());
  }
  return aStr._retn();
}

//============================================================================
/*! Function :
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SObject_i::GetIOR() {
  CORBA::String_var aStr = CORBA::string_dup( "" );
  Handle(SALOMEDS_IORAttribute) anIOR;
  if (_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anIOR)) {
    aStr = CORBA::string_dup(TCollection_AsciiString(anIOR->Get()).ToCString());
  }
  return aStr._retn();
}
