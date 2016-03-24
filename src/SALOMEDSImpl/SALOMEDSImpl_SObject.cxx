// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDSImpl_SObject.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Study.hxx"

#include <map>
#include <string.h>

//============================================================================
/*! Function : empty constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SObject::SALOMEDSImpl_SObject()
{
}

//============================================================================
/*! Function : copy constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SObject::SALOMEDSImpl_SObject(const SALOMEDSImpl_SObject& theSObject)
{
  _lab   = theSObject._lab;
  _value = theSObject._value;
  _type  = theSObject._type;
  _name  = theSObject._name;
 
}


//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SObject::SALOMEDSImpl_SObject(const DF_Label& theLabel)
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
{
}

//============================================================================
/*! Function : GetID
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_SObject::GetID() const
{
  return _lab.Entry();
}

//============================================================================
/*! Function : GetFatherComponent
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SComponent SALOMEDSImpl_SObject::GetFatherComponent() const
{
  SALOMEDSImpl_SComponent sco;
  DF_Label LF = _lab;
  while (!SALOMEDSImpl_SComponent::IsA(LF) && !LF.IsRoot()) {
    LF = LF.Father();
  }

  if(LF.IsRoot()) return sco;

  return GetStudy()->GetSComponent(LF);
}

//============================================================================
/*! Function : GetFather
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_SObject::GetFather() const
{
  return GetStudy()->GetSObject(_lab.Father());
}

//============================================================================
/*! Function : GetLastChild
 *  Purpose  :
 */
//============================================================================
int SALOMEDSImpl_SObject::GetLastChildTag() const
{
  return _lab.LastChildTag();
}

//============================================================================
/*! Function : GetStudy
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_Study* SALOMEDSImpl_SObject::GetStudy() const
{
  return SALOMEDSImpl_Study::GetStudy(_lab);
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
bool SALOMEDSImpl_SObject::FindAttribute(DF_Attribute*& theAttribute, 
                                         const std::string& theTypeOfAttribute) const
{
  if(_lab.IsNull()) return false;
  std::string aGUID = GetGUID(theTypeOfAttribute);
  if ((theAttribute = _lab.FindAttribute(aGUID))) return true;
  return false;
}



//============================================================================
/*! Function : GetAllAttributes
 *  Purpose  : Returns list of all attributes for this sobject
 */
//============================================================================
std::vector<DF_Attribute*> SALOMEDSImpl_SObject::GetAllAttributes() const
{
  std::vector<DF_Attribute*> va1, va = _lab.GetAttributes();
  for(int i = 0, len = va.size(); i<len; i++) {
    SALOMEDSImpl_GenericAttribute* ga = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(va[i]); 
    if(ga && ga->Type() != std::string("AttributeReference"))
        va1.push_back(va[i]);
  }

  return va1;
}


//============================================================================
/*! Function : ReferencedObject
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SObject::ReferencedObject(SALOMEDSImpl_SObject& theObject) const
{
  SALOMEDSImpl_AttributeReference* Ref;
  if (!(Ref=(SALOMEDSImpl_AttributeReference*)_lab.FindAttribute(SALOMEDSImpl_AttributeReference::GetID())))
    return false;
  
  theObject =  GetStudy()->GetSObject(Ref->Get());
  return true;
}

//============================================================================
/*! Function : FindSubObject
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SObject::FindSubObject(int theTag, SALOMEDSImpl_SObject& theObject)
{
  DF_Label L = _lab.FindChild(theTag, false);
  if (L.IsNull()) return false;
  
  theObject = GetStudy()->GetSObject(L);
  return true;
    
}  

  
//============================================================================
/*! Function : GetName
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_SObject::GetName() const
{
  std::string aStr = "";
  SALOMEDSImpl_AttributeName* aName;
  if ((aName=(SALOMEDSImpl_AttributeName*)_lab.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
    aStr =aName->Value();
  }
  return aStr;
}

//============================================================================
/*! Function : GetComment
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_SObject::GetComment() const
{
  std::string aStr = "";
  SALOMEDSImpl_AttributeComment* aComment;
  if ((aComment=(SALOMEDSImpl_AttributeComment*)_lab.FindAttribute(SALOMEDSImpl_AttributeComment::GetID()))) {
    aStr = aComment->Value();
  }
  return aStr;
}

//============================================================================
/*! Function : GetIOR
 *  Purpose  : 
 */
//============================================================================
std::string SALOMEDSImpl_SObject::GetIOR() const 
{
  std::string aStr = "";
  SALOMEDSImpl_AttributeIOR* anIOR;
  if ((anIOR=(SALOMEDSImpl_AttributeIOR*)_lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
    aStr = dynamic_cast<SALOMEDSImpl_AttributeIOR*>(anIOR)->Value();
  }
  return aStr;
}


std::string SALOMEDSImpl_SObject::GetGUID(const std::string& theType) 
{
  __AttributeTypeToGUIDForSObject

  if (strncmp(theType.c_str(), "AttributeTreeNodeGUID",21) == 0) {
    return theType.substr(21, theType.size()); 
  }
  if (strncmp(theType.c_str(), "AttributeUserID",15) == 0) {
    return theType.substr(15, theType.size()); 
  }
  return "";
}

//============================================================================
/*! Function :  SALOMEDSImpl_SComponent
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SObject::operator SALOMEDSImpl_SComponent() const
{
  SALOMEDSImpl_SComponent sco;
  sco._lab = _lab;
  sco._name = _name;
  sco._type = _type;
  sco._value = _value;
  return sco;
}

//============================================================================
/*! Function :  GetPersistentCopy
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_SObject* SALOMEDSImpl_SObject::GetPersistentCopy() const
{
  SALOMEDSImpl_SObject* so = new SALOMEDSImpl_SObject;
  so->_lab = _lab;
  so->_name = _name;
  so->_type = _type;
  so->_value = _value; 
  return so;
}

//============================================================================
/*! Function :  IsComponent
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_SObject::IsComponent() const
{
    return SALOMEDSImpl_SComponent::IsA(_lab);
}

void SALOMEDSImpl_SObject::SetAttrString(const std::string& name, const std::string& value)
{
  if(name=="AttributeName")SALOMEDSImpl_AttributeName::Set(GetLabel(), value);
  else if(name=="AttributeIOR")SALOMEDSImpl_AttributeIOR::Set(GetLabel(), value);
  else if(name=="AttributeString")SALOMEDSImpl_AttributeString::Set(GetLabel(), value);
  else if(name=="AttributePixMap")SALOMEDSImpl_AttributePixMap::Set(GetLabel(), value);
}
