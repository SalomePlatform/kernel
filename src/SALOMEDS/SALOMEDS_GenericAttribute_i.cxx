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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_GenericAttribute_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "utilities.h"
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_Attributes.hxx"
#include "SALOMEDS.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "Utils_ExceptHandlers.hxx"
#include <TCollection_AsciiString.hxx>
#include <map>

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "OpUtil.hxx"

using namespace std;

UNEXPECT_CATCH(GALockProtection, SALOMEDS::GenericAttribute::LockProtection);

SALOMEDS_GenericAttribute_i::SALOMEDS_GenericAttribute_i(const Handle(TDF_Attribute)& theImpl, CORBA::ORB_ptr theOrb)
{
  _orb = CORBA::ORB::_duplicate(theOrb);
  _impl = theImpl;
}

void SALOMEDS_GenericAttribute_i::CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection) 
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(GALockProtection);

  if (!_impl.IsNull() && _impl->IsValid() && !CORBA::is_nil(_orb)) {
    try {
      SALOMEDSImpl_GenericAttribute::Impl_CheckLocked(_impl);
    }
    catch (...) {
      throw SALOMEDS::GenericAttribute::LockProtection();
    }
  }
}

SALOMEDS::SObject_ptr SALOMEDS_GenericAttribute_i::GetSObject() 
{
  SALOMEDS::Locker lock;
  if (_impl.IsNull() || _impl->Label().IsNull()) return SALOMEDS::SObject::_nil();
  Handle(SALOMEDSImpl_SObject) so_impl = SALOMEDSImpl_Study::SObject(_impl->Label());
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (so_impl, _orb);
  return so._retn();
}


char* SALOMEDS_GenericAttribute_i::Type() 
{
  SALOMEDS::Locker lock;
  if (!_impl.IsNull()) {
    return CORBA::string_dup(SALOMEDSImpl_GenericAttribute::Impl_GetType(_impl));
  }    

  return "";
}

char* SALOMEDS_GenericAttribute_i::GetClassType()
{
  SALOMEDS::Locker lock;
  if (!_impl.IsNull()) {
    return CORBA::string_dup(SALOMEDSImpl_GenericAttribute::Impl_GetClassType(_impl));
  }

  return "";
}


SALOMEDS::GenericAttribute_ptr SALOMEDS_GenericAttribute_i::CreateAttribute
                                        (const Handle(TDF_Attribute)& theAttr,
                                         CORBA::ORB_ptr theOrb)
{
  SALOMEDS::Locker lock;

/*
  static std::map<TDF_Attribute*, SALOMEDS_GenericAttribute_i*> _mapOfAttrib;
  SALOMEDS::GenericAttribute_var anAttribute;
  SALOMEDS_GenericAttribute_i* attr_servant = NULL;

  if(_mapOfAttrib.find(theAttr.operator->()) != _mapOfAttrib.end()) {
    attr_servant = _mapOfAttrib[theAttr.operator->()];
    anAttribute = SALOMEDS::GenericAttribute::_narrow(attr_servant->_this());
  }
  else {
    char* aTypeOfAttribute = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(theAttr)->GetClassType().ToCString();
    __CreateGenericCORBAAttribute
    _mapOfAttrib[theAttr.operator->()] = attr_servant;
  }
*/
  // mpv: now servants Destroyed by common algos of CORBA
  char* aTypeOfAttribute = Handle(SALOMEDSImpl_GenericAttribute)::
    DownCast(theAttr)->GetClassType().ToCString();
  SALOMEDS::GenericAttribute_var anAttribute;
  SALOMEDS_GenericAttribute_i* attr_servant = NULL;
  __CreateGenericCORBAAttribute

  return anAttribute._retn(); 
}

//===========================================================================
//   PRIVATE FUNCTIONS
//===========================================================================
CORBA::Long SALOMEDS_GenericAttribute_i::GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal)
{
#ifdef WIN32
  long pid = (long)_getpid();
#else
  long pid = (long)getpid();
#endif
  isLocal = (strcmp(theHostname, GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  TDF_Attribute* local_impl = _impl.operator->();
  return ((long)local_impl);
}
