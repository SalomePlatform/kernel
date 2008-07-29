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
//  File   : SALOMEDS_SObject_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME



#include "utilities.h"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOMEDS.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"

#include <map>

#ifdef WIN32
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include "OpUtil.hxx"

using namespace std;

SALOMEDS::SObject_ptr SALOMEDS_SObject_i::New(const SALOMEDSImpl_SObject& theImpl, CORBA::ORB_ptr theORB)
{
  SALOMEDS_SObject_i* so_servant = new SALOMEDS_SObject_i(theImpl, theORB);
  SALOMEDS::SObject_var so  = SALOMEDS::SObject::_narrow(so_servant->_this());

  return so._retn();
}


//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_SObject_i::SALOMEDS_SObject_i(const SALOMEDSImpl_SObject& impl, CORBA::ORB_ptr orb)
{
  if(!impl.IsNull()) {
     if(impl.IsComponent()) {
         SALOMEDSImpl_SComponent sco = impl;
	 _impl = sco.GetPersistentCopy();       
     }
     else {
         _impl = impl.GetPersistentCopy();
     }
  }
  _orb = CORBA::ORB::_duplicate(orb);
   //SALOME::GenericObj_i::myPOA = SALOMEDS_StudyManager_i::GetPOA(GetStudy());
}


//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_SObject_i::~SALOMEDS_SObject_i()
{
   if(_impl) delete _impl;    
}


//============================================================================
/*! Function :GetID
 *  Purpose  :
 */
//============================================================================
char* SALOMEDS_SObject_i::GetID()
{
  SALOMEDS::Locker lock;
  return CORBA::string_dup(_impl->GetID().c_str());
}

//============================================================================
/*! Function : GetFatherComponent
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_SObject_i::GetFatherComponent()
{
  SALOMEDS::Locker lock;
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (_impl->GetFatherComponent(), _orb);
  return sco._retn();
}

//============================================================================
/*! Function : GetFather
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_SObject_i::GetFather()
{
  SALOMEDS::Locker lock;
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (_impl->GetFather(), _orb);
  return so._retn();
}

//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_SObject_i::GetStudy()
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_Study* aStudy = _impl->GetStudy();
  if(!aStudy) {
    MESSAGE("Problem GetStudy");
    return SALOMEDS::Study::_nil();
  }

  string IOR = aStudy->GetTransientReference();
  CORBA::Object_var obj = _orb->string_to_object(IOR.c_str());
  SALOMEDS::Study_var Study = SALOMEDS::Study::_narrow(obj) ;
  ASSERT(!CORBA::is_nil(Study));
  return SALOMEDS::Study::_duplicate(Study);
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type on this SObject
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindAttribute (SALOMEDS::GenericAttribute_out anAttribute,
						  const char* aTypeOfAttribute)
{
  SALOMEDS::Locker lock;
  DF_Attribute* anAttr = NULL;
  if(_impl->FindAttribute(anAttr, (char*)aTypeOfAttribute)) {
    anAttribute = SALOMEDS::GenericAttribute::_duplicate(SALOMEDS_GenericAttribute_i::CreateAttribute(anAttr, _orb));
    return true;
  }

  return false;
}

//============================================================================
/*! Function : GetAllAttributes
 *  Purpose  : Returns list of all attributes for this sobject
 */
//============================================================================

SALOMEDS::ListOfAttributes* SALOMEDS_SObject_i::GetAllAttributes()
{
  SALOMEDS::Locker lock;
  vector<DF_Attribute*> aSeq = _impl->GetAllAttributes();
  SALOMEDS::ListOfAttributes_var SeqOfAttr = new SALOMEDS::ListOfAttributes;
  int length = aSeq.size();

  SeqOfAttr->length(length);

  if (length != 0) {
    for(int i = 0; i < length; i++) {
      SALOMEDSImpl_GenericAttribute* anAttr = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aSeq[i]);
      SALOMEDS::GenericAttribute_var anAttribute;
      anAttribute = SALOMEDS::GenericAttribute::_duplicate(SALOMEDS_GenericAttribute_i::CreateAttribute(anAttr, _orb));
      if (!CORBA::is_nil(anAttribute)) {
	SeqOfAttr[i] = anAttribute;
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
  SALOMEDS::Locker lock;
  SALOMEDSImpl_SObject aRefObj;
  if(!_impl->ReferencedObject(aRefObj)) return false;

  obj = SALOMEDS_SObject_i::New (aRefObj, _orb);
  return true;
}

//============================================================================
/*! Function : FindSubObject
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_SObject_i::FindSubObject(CORBA::Long atag, SALOMEDS::SObject_out obj)
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_SObject aSubObj;
  if(!_impl->FindSubObject(atag, aSubObj)) return false;

  obj = SALOMEDS_SObject_i::New (aSubObj, _orb);
  return true;

}

//============================================================================
/*! Function : Name
 *  Purpose  : gets a name
 */
//============================================================================
char* SALOMEDS_SObject_i::Name()
{
  SALOMEDS::Locker lock;
  return CORBA::string_dup(_impl->Name().c_str());
}

//============================================================================
/*! Function : Name
 *  Purpose  : sets a name
 */
//============================================================================
void  SALOMEDS_SObject_i::Name(const char* name)
{
  SALOMEDS::Locker lock;
  string aName((char*)name);
  _impl->Name(aName);
}

//============================================================================
/*! Function : Tag
 *  Purpose  :
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Tag()
{
  SALOMEDS::Locker lock;
  return _impl->Tag();
}

//============================================================================
/*! Function : Depth
 *  Purpose  :
 */
//============================================================================
CORBA::Short SALOMEDS_SObject_i::Depth()
{
  SALOMEDS::Locker lock;
  return _impl->Depth();
}

//============================================================================
/*! Function : GetObject
 *  Purpose  :
 */
//============================================================================
CORBA::Object_ptr SALOMEDS_SObject_i::GetObject()
{
  SALOMEDS::Locker lock;
  CORBA::Object_ptr obj = CORBA::Object::_nil();
  try {
    string IOR = _impl->GetIOR();
    char* c_ior = CORBA::string_dup(IOR.c_str());
    obj = _orb->string_to_object(c_ior);
    CORBA::string_free(c_ior);
  } catch(...) {}
  return obj;
}

//============================================================================
/*! Function : GetName
 *  Purpose  :
 */
//============================================================================
char* SALOMEDS_SObject_i::GetName()
{
  SALOMEDS::Locker lock;
  CORBA::String_var aStr = CORBA::string_dup(_impl->GetName().c_str());
  return aStr._retn();
}

//============================================================================
/*! Function : GetComment
 *  Purpose  :
 */
//============================================================================
char* SALOMEDS_SObject_i::GetComment()
{
  SALOMEDS::Locker lock;
  CORBA::String_var aStr = CORBA::string_dup(_impl->GetComment().c_str());
  return aStr._retn();
}

//============================================================================
/*! Function : GetIOR
 *  Purpose  :
 */
//============================================================================
char* SALOMEDS_SObject_i::GetIOR()
{
  SALOMEDS::Locker lock;
  CORBA::String_var aStr = CORBA::string_dup(_impl->GetIOR().c_str());
  return aStr._retn();
}

//===========================================================================
//   PRIVATE FUNCTIONS
//===========================================================================
CORBA::LongLong SALOMEDS_SObject_i::GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal)
{
#ifdef WIN32
  long pid = (long)_getpid();
#else
  long pid = (long)getpid();
#endif
  isLocal = (strcmp(theHostname, GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  return reinterpret_cast<CORBA::LongLong>(_impl);
}
