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

//  File   : SALOMEDS_GenericAttribute.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include <string>

#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_ClientAttributes.hxx"
#include "SALOMEDS.hxx"

#include "Basics_Utils.hxx"

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

SALOMEDS_GenericAttribute::SALOMEDS_GenericAttribute(SALOMEDSImpl_GenericAttribute* theGA)
{
  _isLocal = true;
  _local_impl = theGA;
  _corba_impl = SALOMEDS::GenericAttribute::_nil();
}

SALOMEDS_GenericAttribute::SALOMEDS_GenericAttribute(SALOMEDS::GenericAttribute_ptr theGA)
{
#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif  

  CORBA::LongLong addr = theGA->GetLocalImpl(Kernel_Utils::GetHostname().c_str(), pid, _isLocal);
  if(_isLocal) {
    _local_impl = reinterpret_cast<SALOMEDSImpl_GenericAttribute*>(addr);
    _corba_impl = SALOMEDS::GenericAttribute::_nil();
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::GenericAttribute::_duplicate(theGA);
  }
}

SALOMEDS_GenericAttribute::~SALOMEDS_GenericAttribute() 
{
  if (!_isLocal) {
    _corba_impl->UnRegister();
  }
}

void SALOMEDS_GenericAttribute::CheckLocked() 
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      _local_impl->CheckLocked();
    }
    catch(...) {
      throw SALOMEDS::GenericAttribute::LockProtection();
    }
  }
  else {
    _corba_impl->CheckLocked();
  }
}

std::string SALOMEDS_GenericAttribute::Type()
{
  std::string aType;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aType = _local_impl->Type();
  }
  else {
    aType = (CORBA::String_var)_corba_impl->Type();
  }
  return aType;
}

std::string SALOMEDS_GenericAttribute::GetClassType()
{
  std::string aType;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aType = _local_impl->GetClassType();
  }
  else {
    aType = (CORBA::String_var)_corba_impl->GetClassType();
  }
  return aType;
}

_PTR(SObject) SALOMEDS_GenericAttribute::GetSObject()
{
  SALOMEDSClient_SObject* aSO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aSO = new SALOMEDS_SObject(_local_impl->GetSObject());
  }
  else {
    aSO = new SALOMEDS_SObject((SALOMEDS::SObject_var)_corba_impl->GetSObject());
  }

  return _PTR(SObject)(aSO);
}


SALOMEDS_GenericAttribute* SALOMEDS_GenericAttribute::CreateAttribute(SALOMEDSImpl_GenericAttribute* theGA)
{
  SALOMEDS::Locker lock;

  SALOMEDS_GenericAttribute* aGA = NULL;
  if(theGA) {
     std::string aTypeOfAttribute = theGA->GetClassType();
      __CreateGenericClientAttributeLocal
  }
  return aGA;
}

SALOMEDS_GenericAttribute* SALOMEDS_GenericAttribute::CreateAttribute(SALOMEDS::GenericAttribute_ptr theGA)
{
  SALOMEDS_GenericAttribute* aGA = NULL;
  if(!CORBA::is_nil(theGA)) {
      CORBA::String_var astr=theGA->GetClassType();
      std::string aTypeOfAttribute = astr.in();
      __CreateGenericClientAttributeCORBA
  }
  return aGA;
}
