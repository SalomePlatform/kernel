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

//  File   : SALOMEDS_AttributeParameter_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeParameter_i.hxx"
#include "SALOMEDS.hxx"

#include <vector>

#include "Utils_ExceptHandlers.hxx"
UNEXPECT_CATCH(AP_InvalidIdentifier, SALOMEDS::AttributeParameter::InvalidIdentifier);

//=======================================================================
/*!
 * Function : SetInt
 * Purpose  : Associates a integer value with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::SetInt(const char* theID, CORBA::Long theValue)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->SetInt(theID, theValue);
}

//=======================================================================
/*!
 * Function : GetInt
 * Purpose  : Returns a int value associated with the given ID
 */
//=======================================================================
CORBA::Long SALOMEDS_AttributeParameter_i::GetInt(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetInt(theID);
}

//=======================================================================
/*!
 * Function : SetReal
 * Purpose  : Associates a double value with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::SetReal(const char* theID, CORBA::Double theValue)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->SetReal(theID, theValue);
}

//=======================================================================
/*!
 * Function : GetReal
 * Purpose  : Returns a double value associated with the given ID
 */
//=======================================================================
CORBA::Double SALOMEDS_AttributeParameter_i::GetReal(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetReal(theID);
}

//=======================================================================
/*!
 * Function : SetString
 * Purpose  : Associates a string with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::SetString(const char* theID, const char* theValue)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  SALOMEDSImpl_AttributeParameter* impl = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl);
  impl->SetString(theID, theValue);
}

//=======================================================================
/*!
 * Function : GetString
 * Purpose  : Returns a string associated with the given ID
 */
//=======================================================================
char* SALOMEDS_AttributeParameter_i::GetString(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  SALOMEDSImpl_AttributeParameter* impl = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl);
  CORBA::String_var c_s = CORBA::string_dup(impl->GetString(theID).c_str());
  return c_s._retn();
}

//=======================================================================
/*!
 * Function : SetBool
 * Purpose  : Associates a bool value with the ID
 */
//=======================================================================  
void SALOMEDS_AttributeParameter_i::SetBool(const char* theID, CORBA::Boolean theValue)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->SetBool(theID, theValue);
}

//=======================================================================
/*!
 * Function : GetBool
 * Purpose  : Returns a bool value associated with the ID
 */
//=======================================================================
CORBA::Boolean SALOMEDS_AttributeParameter_i::GetBool(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetBool(theID);
}
  
//=======================================================================
/*!
 * Function : SetRealArray
 * Purpose  : Associates an array of double values with the given ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::SetRealArray(const char* theID, const SALOMEDS::DoubleSeq& theArray)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  std::vector<double> v;
  int length = theArray.length();
  if(length) {
    v.resize(length);
    for(int i = 0; i<length; i++) v[i] = theArray[i];
  }
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->SetRealArray(theID, v);
}

//=======================================================================
/*!
 * Function : GetRealArray
 * Purpose  : Returns an array of double values associated with the ID
 */
//=======================================================================
SALOMEDS::DoubleSeq* SALOMEDS_AttributeParameter_i::GetRealArray(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq;
  std::vector<double> v = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetRealArray(theID);
  int length = v.size();
  if(length) {
    aSeq->length(length);
    for(int i = 0; i<length; i++) aSeq[i] = v[i];
  }
  return aSeq._retn();
}
 
//=======================================================================
/*!
 * Function : SetIntArray
 * Purpose  : Associates an array of int values with the given ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::SetIntArray(const char* theID, const SALOMEDS::LongSeq& theArray)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  std::vector<int> v;
  int length = theArray.length();
  if(length) {
    v.resize(length);
    for(int i = 0; i<length; i++) v[i] = theArray[i];
  }
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->SetIntArray(theID, v);
}

//=======================================================================
/*!
 * Function : GetIntArray
 * Purpose  : Returns an array of int values associated with the ID
 */
//=======================================================================
SALOMEDS::LongSeq* SALOMEDS_AttributeParameter_i::GetIntArray(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq;
  std::vector<int> v = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetIntArray(theID);
  int length = v.size();
  if(length) {
    aSeq->length(length);
    for(int i = 0; i<length; i++) aSeq[i] = v[i];
  }
  return aSeq._retn();
}
  
//=======================================================================
/*!
 * Function : SetStrArray
 * Purpose  : Associates an array of string values with the given ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::SetStrArray(const char* theID, const SALOMEDS::StringSeq& theArray)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  std::vector<std::string> v;
  int length = theArray.length();
  if(length) {
    v.resize(length);
    for(int i = 0; i<length; i++) v[i] = std::string(theArray[i].in());
  }
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->SetStrArray(theID, v);
}

//=======================================================================
/*!
 * Function : GetStrArray
 * Purpose  : Returns an array of string values associated with the ID
 */
//=======================================================================
SALOMEDS::StringSeq* SALOMEDS_AttributeParameter_i::GetStrArray(const char* theID)
  throw (SALOMEDS::AttributeParameter::InvalidIdentifier)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (AP_InvalidIdentifier);
  SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq;
  std::vector<std::string> v = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetStrArray(theID);
  int length = v.size();
  if(length) {
    aSeq->length(length);
    for(int i = 0; i<length; i++) aSeq[i] = CORBA::string_dup(v[i].c_str());
  }
  return aSeq._retn();
}
  
 
//=======================================================================
/*!
 * Function : IsSet
 * Purpose  : Returns true if for the ID of given type was assigned \n
 *            a value in the attribute
 */
//=======================================================================
CORBA::Boolean SALOMEDS_AttributeParameter_i::IsSet(const char* theID, CORBA::Long theType)
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->IsSet(theID, (Parameter_Types)theType);
}

//=======================================================================
/*!
 * Function : RemoveID
 * Purpose  : Removes a parameter with given ID
 */
//=======================================================================  
CORBA::Boolean SALOMEDS_AttributeParameter_i::RemoveID(const char* theID, CORBA::Long theType)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->RemoveID(theID, (Parameter_Types)theType);
}

//=======================================================================
/*!
 * Function : GetFather
 * Purpose  : Returns a father attribute for this attribute
 */
//=======================================================================
SALOMEDS::AttributeParameter_ptr SALOMEDS_AttributeParameter_i::GetFather()
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeParameter* impl = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl);
  SALOMEDS_AttributeParameter_i* attr = new SALOMEDS_AttributeParameter_i(impl, _orb);
  return attr->AttributeParameter::_this();
}

//=======================================================================
/*!
 * Function : HasFather
 * Purpose  : Returns True if the attribute has a father attribute
 */
//=======================================================================
CORBA::Boolean SALOMEDS_AttributeParameter_i::HasFather()
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->HasFather();
}

//=======================================================================
/*!
 * Function : IsRoot
 * Purpose  : Returns True is the attribute is highest in an hierachy
 */
//=======================================================================
CORBA::Boolean SALOMEDS_AttributeParameter_i::IsRoot()
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->IsRoot();
}

//=======================================================================
/*!
 * Function : IsRoot
 * Purpose  : Clears the content of the attribute
 */
//=======================================================================
void SALOMEDS_AttributeParameter_i::Clear()
{
  SALOMEDS::Locker lock; 
  dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->Clear();
}


//=======================================================================
/*!
 * Function : GetIDs
 * Purpose  : Returns an array of all ID's of the given type
 */
//=======================================================================
SALOMEDS::StringSeq* SALOMEDS_AttributeParameter_i::GetIDs(CORBA::Long theType)
{
  SALOMEDS::Locker lock; 
  SALOMEDS::StringSeq_var CorbaSeq = new SALOMEDS::StringSeq;
  std::vector<std::string> A = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_impl)->GetIDs((Parameter_Types)theType);

  if(A.size()) {
    int length = A.size();
    CorbaSeq->length(length);
    for (int i = 0; i < length; i++) CorbaSeq[i] = CORBA::string_dup(A[i].c_str());;
  }

  return CorbaSeq._retn();
}
