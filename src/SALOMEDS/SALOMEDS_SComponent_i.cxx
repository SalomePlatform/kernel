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
//  File   : SALOMEDS_SComponent_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS.hxx"

#include "utilities.h"

using namespace std;

SALOMEDS_Study_i::TSObjectHolder 
SALOMEDS_SComponent_i::New(SALOMEDS_Study_i* theStudy,
			   const TDF_Label& theLabel)
{
  SALOMEDS_Study_i::TSObjectHolder aSObjectHolder;
  SALOMEDS_Study_i::TSObjectMap& anSObjectMap = theStudy->GetSObjectMap();
  SALOMEDS_Study_i::TSObjectMap::const_iterator anIter = anSObjectMap.find(theLabel);
  if(anIter != anSObjectMap.end()){
    aSObjectHolder = anIter->second;
    SALOMEDS_SObject_i* aSObject = aSObjectHolder.first;
    if(dynamic_cast<SALOMEDS_SComponent_i*>(aSObject))
      return aSObjectHolder;
  }
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(theLabel,anEntry);
  SALOMEDS_SComponent_i* aSComponent = new SALOMEDS_SComponent_i(theStudy,theLabel);
  aSObjectHolder.first = aSComponent;
  aSObjectHolder.second = aSComponent->_this();
  anSObjectMap[theLabel] = aSObjectHolder;

  return aSObjectHolder;
}

SALOMEDS_SComponent_i*
SALOMEDS_SComponent_i::NewPtr(SALOMEDS_Study_i* theStudy,
			      const TDF_Label& theLabel)
{
  return dynamic_cast<SALOMEDS_SComponent_i*>(New(theStudy,theLabel).first);
}

SALOMEDS::SComponent_var
SALOMEDS_SComponent_i::NewRef(SALOMEDS_Study_i* theStudy,
			      const TDF_Label& theLabel)
{
  return SALOMEDS::SComponent::_narrow(New(theStudy,theLabel).second);
}

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::SALOMEDS_SComponent_i(SALOMEDS_Study_i* theStudy,
					     const TDF_Label& theLabel):
  SALOMEDS_SObject_i(theStudy,theLabel)
{
}
  
//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::~SALOMEDS_SComponent_i()
{
}
  
  
//============================================================================
/*! Function : ComponentDataType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SComponent_i::ComponentDataType()
{
  SALOMEDS::Locker lock;

  //DEB
  //    MESSAGE("In SALOMEDS_SComponent_i::ComponentDataType");
  //    TCollection_AsciiString anEntry;
  //    TDF_Tool::Entry (_lab,anEntry);
  //    MESSAGE("in SALOMEDS_SComponent_i, Entry :"<<anEntry);
  // END DEB

  Standard_CString res = "";
  Handle(TDataStd_Comment) type;
  if ( _lab.FindAttribute(TDataStd_Comment::GetID(),type) ) {
    TCollection_AsciiString ch(type->Get());
    res = ch.ToCString();
  }

  return CORBA::string_dup(res);
}
  

//============================================================================
/*! Function : ComponentIOR
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponent_i::ComponentIOR(CORBA::String_out IOR)
{
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_IORAttribute) ior;
  if (!_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),ior) )
      return false;
  TCollection_AsciiString ch(ior->Get());
  IOR = CORBA::string_dup(ch.ToCString());
  return true;
}
  

//============================================================================
/*! Function : IsA
 *  Purpose  : 
 */
//============================================================================
Standard_Boolean SALOMEDS_SComponent_i::IsA(const TDF_Label& Lab)
{
  Handle(TDF_Attribute) Att;
  // scomponent must contain comment and belong to the 2th depth label
  if ( Lab.FindAttribute(TDataStd_Comment::GetID(), Att) && Lab.Depth() == 2) {
    return Standard_True;
  }
  return Standard_False;
}
  
    
