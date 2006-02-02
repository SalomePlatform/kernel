// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME



#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TColStd_HSequenceOfTransient.hxx>

#include "SALOMEDS_SObject.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDS_Study.hxx"

#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_Study.hxx"

#include "Utils_ORB_INIT.hxx" 
#include "Utils_SINGLETON.hxx" 

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "OpUtil.hxx"
#include "utilities.h"

using namespace std;  

SALOMEDS_SObject::SALOMEDS_SObject(SALOMEDS::SObject_ptr theSObject)
{
#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif  

  long addr = theSObject->GetLocalImpl(GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = ((SALOMEDSImpl_SObject*)(addr));
    _corba_impl = SALOMEDS::SObject::_duplicate(theSObject);
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::SObject::_duplicate(theSObject);
  }

  init_orb();
}

SALOMEDS_SObject::SALOMEDS_SObject(const Handle(SALOMEDSImpl_SObject)& theSObject)
:_isLocal(true)
{
  _corba_impl = SALOMEDS::SObject::_nil();
  _local_impl = theSObject;

  init_orb();
}

SALOMEDS_SObject::~SALOMEDS_SObject()
{
  if (!_isLocal) {
    _corba_impl->Destroy();
  }
}

std::string SALOMEDS_SObject::GetID()
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = _local_impl->GetID().ToCString();
  }
  else aValue = _corba_impl->GetID();  
  return aValue;
}

_PTR(SComponent) SALOMEDS_SObject::GetFatherComponent()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_SComponent) aSCO =
      Handle(SALOMEDSImpl_SComponent)::DownCast(_local_impl->GetFatherComponent());
    return _PTR(SComponent)(new SALOMEDS_SComponent(aSCO));
  }
  return _PTR(SComponent)(new SALOMEDS_SComponent(_corba_impl->GetFatherComponent()));
}

_PTR(SObject) SALOMEDS_SObject::GetFather()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _PTR(SObject)(new SALOMEDS_SObject(_local_impl->GetFather()));
  }
  return _PTR(SObject)(new SALOMEDS_SObject(_corba_impl->GetFather()));
}

bool SALOMEDS_SObject::FindAttribute(_PTR(GenericAttribute)& anAttribute,
                                     const std::string& aTypeOfAttribute)
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_GenericAttribute) anAttr;
    ret = _local_impl->FindAttribute(anAttr, (char*)aTypeOfAttribute.c_str());
    if(ret) anAttribute = _PTR(GenericAttribute)(SALOMEDS_GenericAttribute::CreateAttribute(anAttr));
  }
  else {
    SALOMEDS::GenericAttribute_var anAttr;
    ret = _corba_impl->FindAttribute(anAttr.out(), aTypeOfAttribute.c_str());
    if(ret) anAttribute = _PTR(GenericAttribute)(SALOMEDS_GenericAttribute::CreateAttribute(anAttr));
  }

  return ret;
}

bool SALOMEDS_SObject::ReferencedObject(_PTR(SObject)& theObject)
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_SObject) aSO;
    ret = _local_impl->ReferencedObject(aSO);
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }
  else {
    SALOMEDS::SObject_var aSO;
    ret = _corba_impl->ReferencedObject(aSO.out());
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }

  return ret; 
}


bool SALOMEDS_SObject::FindSubObject(int theTag, _PTR(SObject)& theObject)
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_SObject) aSO;
    ret = _local_impl->FindSubObject(theTag, aSO);
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }
  else {
    SALOMEDS::SObject_var aSO;
    ret = _corba_impl->FindSubObject(theTag, aSO.out());
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }

  return ret;   
}

_PTR(Study) SALOMEDS_SObject::GetStudy()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _PTR(Study)(new SALOMEDS_Study(_local_impl->GetStudy()));
  }
  return _PTR(Study)(new SALOMEDS_Study(_corba_impl->GetStudy()));
}

std::string SALOMEDS_SObject::Name()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = _local_impl->Name().ToCString();
  }
  else aName = _corba_impl->Name();

  return aName;
}

void  SALOMEDS_SObject::Name(const std::string& theName)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Name((char*)theName.c_str());
  }
  else _corba_impl->Name(theName.c_str());
}

vector<_PTR(GenericAttribute)> SALOMEDS_SObject::GetAllAttributes()
{
  vector<_PTR(GenericAttribute)> aVector;
  int aLength = 0;
  SALOMEDSClient_GenericAttribute* anAttr;

  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfTransient) aSeq = _local_impl->GetAllAttributes();
    aLength = aSeq->Length();
    for (int i = 1; i <= aLength; i++) {
      anAttr = SALOMEDS_GenericAttribute::CreateAttribute
        (Handle(SALOMEDSImpl_GenericAttribute)::DownCast(aSeq->Value(i)));
      aVector.push_back(_PTR(GenericAttribute)(anAttr));
    }
  }
  else {
    SALOMEDS::ListOfAttributes_var aSeq = _corba_impl->GetAllAttributes();
    aLength = aSeq->length();
    for (int i = 0; i < aLength; i++) {
      anAttr = SALOMEDS_GenericAttribute::CreateAttribute(aSeq[i]);
      aVector.push_back(_PTR(GenericAttribute)(anAttr));
    }
  }

  return aVector;
}

std::string SALOMEDS_SObject::GetName()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = _local_impl->GetName().ToCString();
  }
  else aName = _corba_impl->GetName();

  return aName;
}

std::string SALOMEDS_SObject::GetComment()
{
  std::string aComment;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aComment = _local_impl->GetComment().ToCString();
  }
  else aComment = _corba_impl->GetComment();

  return aComment;
}

std::string SALOMEDS_SObject::GetIOR()
{
  std::string anIOR;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    anIOR = _local_impl->GetIOR().ToCString();
  }
  else anIOR = _corba_impl->GetIOR();

  return anIOR;
}

int SALOMEDS_SObject::Tag()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _local_impl->Tag();
  }
  return _corba_impl->Tag(); 
}

int SALOMEDS_SObject::Depth()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _local_impl->Depth();
  }
  return _corba_impl->Depth();  
}

CORBA::Object_ptr SALOMEDS_SObject::GetObject()
{
  CORBA::Object_var obj;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    std::string anIOR = GetIOR();
    if (!anIOR.empty())
      obj = _orb->string_to_object(anIOR.c_str());
    return obj._retn();
  }
  else {
    obj = _corba_impl->GetObject();
    return obj._retn();
  }

  return CORBA::Object::_nil();
}

SALOMEDS::SObject_ptr SALOMEDS_SObject::GetSObject()
{
  if(_isLocal) {
    if(!CORBA::is_nil(_corba_impl)) return _corba_impl;
    SALOMEDS::SObject_var aSO = SALOMEDS_SObject_i::New(_local_impl, _orb);
    return aSO._retn();
  }
  else {
    return _corba_impl;
  }
  return SALOMEDS::SObject::_nil();
}


void SALOMEDS_SObject::init_orb()
{
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(0 , 0 ) ;     
}
