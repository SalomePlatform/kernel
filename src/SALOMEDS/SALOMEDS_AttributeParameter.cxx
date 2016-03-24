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

//  File   : SALOMEDS_AttributeParameter.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeParameter.hxx"
#include "SALOMEDS.hxx"

#include <string>

//=======================================================================
/*!
 * Function : Constructor
 * Purpose  : Creates a new instance of SALOMEDS_AttributeParameter
 */
//=======================================================================
SALOMEDS_AttributeParameter::SALOMEDS_AttributeParameter(SALOMEDSImpl_AttributeParameter* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

//=======================================================================
/*!
 * Function : Constructor
 * Purpose  : Creates a new instance of SALOMEDS_AttributeParameter
 */
//=======================================================================
SALOMEDS_AttributeParameter::SALOMEDS_AttributeParameter(SALOMEDS::AttributeParameter_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

//=======================================================================
/*!
 * Function : Destructor
 * Purpose  : Destroys the instance of SALOMEDS_AttributeParameter
 */
//=======================================================================
SALOMEDS_AttributeParameter::~SALOMEDS_AttributeParameter()
{}

//=======================================================================
/*!
 * Function : SetInt
 * Purpose  : Associates a integer value with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetInt(const std::string& theID, const int theValue) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetInt(theID, theValue);
  }
  else
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetInt(theID.c_str(), theValue);
}

//=======================================================================
/*!
 * Function : GetInt
 * Purpose  : Returns a int value associated with the given ID
 */
//=======================================================================
int SALOMEDS_AttributeParameter::GetInt(const std::string& theID) 
{
  int aValue;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetInt(theID);
  }
  else
    aValue = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetInt(theID.c_str());
  return aValue;
}

//=======================================================================
/*!
 * Function : SetReal
 * Purpose  : Associates a double value with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetReal(const std::string& theID, const double& theValue) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetReal(theID, theValue);
  }
  else
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetReal(theID.c_str(), theValue);
}
 
//=======================================================================
/*!
 * Function : GetReal
 * Purpose  : Returns a double value associated with the given ID
 */
//=======================================================================
double SALOMEDS_AttributeParameter::GetReal(const std::string& theID) 
{
  double aValue;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetReal(theID);
  }
  else
    aValue = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetReal(theID.c_str());
  return aValue;
}

//=======================================================================
/*!
 * Function : SetString
 * Purpose  : Associates a string with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetString(const std::string& theID, const std::string& theValue) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetString(theID, theValue);
  }
  else
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetString(theID.c_str(), theValue.c_str());
}
 
//=======================================================================
/*!
 * Function : GetString
 * Purpose  : Returns a string associated with the given ID
 */
//=======================================================================
std::string SALOMEDS_AttributeParameter::GetString(const std::string& theID) 
{
  std::string aValue;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetString(theID);
  }
  else
    aValue = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetString(theID.c_str());
  return aValue;
}
  
//=======================================================================
/*!
 * Function : SetBool
 * Purpose  : Associates a bool value with the ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetBool(const std::string& theID, const bool& theValue) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetBool(theID, theValue);
  }
  else
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetBool(theID.c_str(), theValue);
}

//=======================================================================
/*!
 * Function : GetBool
 * Purpose  : Returns a bool value associated with the ID
 */
//=======================================================================
bool SALOMEDS_AttributeParameter::GetBool(const std::string& theID) 
{
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetBool(theID);
  }
  else
    return SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetBool(theID.c_str());
}
  
//=======================================================================
/*!
 * Function : SetRealArray
 * Purpose  : Associates an array of double values with the given ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetRealArray(const std::string& theID, const std::vector<double>& theArray) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetRealArray(theID, theArray);
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq;
    int length = theArray.size();
    if(length) {
      aSeq->length(length);
      for(int i = 0; i<length; i++) aSeq[i] = theArray[i];
    }
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetRealArray(theID.c_str(), aSeq);
  }
}
 
//=======================================================================
/*!
 * Function : GetRealArray
 * Purpose  : Returns an array of double values associated with the ID
 */
//=======================================================================
std::vector<double> SALOMEDS_AttributeParameter::GetRealArray(const std::string& theID) 
{
  std::vector<double> v;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetRealArray(theID);
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetRealArray(theID.c_str());    
    int length = aSeq->length();
    if(length) {
      v.resize(length);
      for(int i = 0; i < length; i++) v[i] = aSeq[i];
    }
  }
  return v;
}

//=======================================================================
/*!
 * Function : SetIntArray
 * Purpose  : Associates an array of int values with the given ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetIntArray(const std::string& theID, const std::vector<int>& theArray) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetIntArray(theID, theArray);
  }
  else {
    SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq;
    int length = theArray.size();
    if(length) {
      aSeq->length(length);
      for(int i = 0; i<length; i++) aSeq[i] = theArray[i];
    }
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetIntArray(theID.c_str(), aSeq);
  }
}
 
//=======================================================================
/*!
 * Function : GetIntArray
 * Purpose  : Returns an array of int values associated with the ID
 */
//=======================================================================
std::vector<int> SALOMEDS_AttributeParameter::GetIntArray(const std::string& theID) 
{
  std::vector<int> v;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetIntArray(theID);
  }
  else {
    SALOMEDS::LongSeq_var aSeq = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetIntArray(theID.c_str());    
    int length = aSeq->length();
    if(length) {
      v.resize(length);
      for(int i = 0; i < length; i++) v[i] = aSeq[i];
    }
  }
  return v;
}

//=======================================================================
/*!
 * Function : SetStrArray
 * Purpose  : Associates an array of string values with the given ID
 */
//=======================================================================
void SALOMEDS_AttributeParameter::SetStrArray(const std::string& theID, const std::vector<std::string>& theArray) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->SetStrArray(theID, theArray);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq;
    int length = theArray.size();
    if(length) {
      aSeq->length(length);
      for(int i = 0; i<length; i++) aSeq[i] = CORBA::string_dup(theArray[i].c_str());
    }
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->SetStrArray(theID.c_str(), aSeq);
  }
}
 
//=======================================================================
/*!
 * Function : GetStrArray
 * Purpose  : Returns an array of string values associated with the ID
 */
//=======================================================================
std::vector<std::string> SALOMEDS_AttributeParameter::GetStrArray(const std::string& theID) 
{
  std::vector<std::string> v;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetStrArray(theID);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetStrArray(theID.c_str());    
    int length = aSeq->length();
    if(length) {
      v.resize(length);
      for(int i = 0; i < length; i++) v[i] = std::string(aSeq[i].in());
    }
  }
  return v;
}


//=======================================================================
/*!
 * Function : IsSet
 * Purpose  : Returns true if for the ID of given type was assigned \n
 *            a value in the attribute
 */
//======================================================================= 
bool SALOMEDS_AttributeParameter::IsSet(const std::string& theID, const int theType) 
{
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->IsSet(theID, (Parameter_Types)theType);
  }
  else 
    return SALOMEDS::AttributeParameter::_narrow(_corba_impl)->IsSet(theID.c_str(), theType);
}

//=======================================================================
/*!
 * Function : RemoveID
 * Purpose  : Removes a parameter with given ID
 */
//======================================================================= 
bool SALOMEDS_AttributeParameter::RemoveID(const std::string& theID, const int theType) 
{
  CheckLocked();

  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->RemoveID(theID, (Parameter_Types)theType);
  }
  else
    return SALOMEDS::AttributeParameter::_narrow(_corba_impl)->RemoveID(theID.c_str(), theType);
}

//=======================================================================
/*!
 * Function : GetFather
 * Purpose  : Returns a father attribute for this attribute
 */
//=======================================================================
_PTR(AttributeParameter) SALOMEDS_AttributeParameter::GetFather() 
{
  SALOMEDSClient_AttributeParameter* AP = NULL;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeParameter* AP_impl = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->GetFather();
    if(!AP_impl) return _PTR(AttributeParameter)(AP);
    AP = new SALOMEDS_AttributeParameter(AP_impl);
  }
  else {
    SALOMEDS::AttributeParameter_var AP_impl = SALOMEDS::AttributeParameter::_narrow(_corba_impl)->GetFather();
    if(CORBA::is_nil(AP_impl)) return _PTR(AttributeParameter)(AP);
    AP = new SALOMEDS_AttributeParameter(AP_impl);
  }
  
  return _PTR(AttributeParameter)(AP);
}

//=======================================================================
/*!
 * Function : HasFather
 * Purpose  : Returns True if the attribute has a father attribute
 */
//======================================================================= 
bool SALOMEDS_AttributeParameter::HasFather() 
{
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->HasFather();
  }
  else 
    return SALOMEDS::AttributeParameter::_narrow(_corba_impl)->HasFather();
}

//=======================================================================
/*!
 * Function : IsRoot
 * Purpose  : Returns True is the attribute is highest in an hierachy
 */
//=======================================================================
bool SALOMEDS_AttributeParameter::IsRoot()
{
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    return dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->IsRoot();
  }
  else 
    return SALOMEDS::AttributeParameter::_narrow(_corba_impl)->IsRoot();
}

//=======================================================================
/*!
 * Function : Clear
 * Purpose  : Clears the content of the attribute
 */
//======================================================================= 
void SALOMEDS_AttributeParameter::Clear() 
{
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl)->Clear();
  }
  else 
    SALOMEDS::AttributeParameter::_narrow(_corba_impl)->Clear();
}

//=======================================================================
/*!
 * Function : GetIDs
 * Purpose  : Returns an array of all ID's of the given type
 */
//=======================================================================
std::vector<std::string> SALOMEDS_AttributeParameter::GetIDs(const int theType)
{
  std::vector<std::string> v;
  if(_isLocal) {
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeParameter* AP_impl = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(_local_impl);
    return AP_impl->GetIDs((Parameter_Types)theType);
  }
  else {
    SALOMEDS::AttributeParameter_var AP_impl = SALOMEDS::AttributeParameter::_narrow(_corba_impl);
    SALOMEDS::StringSeq_var CorbaSeq = AP_impl->GetIDs(theType);
    int length = CorbaSeq->length();
    if(length) {
      v.resize(length);
      for(int i = 0; i <length; i++) v[i] = std::string(CorbaSeq[i].in());
    }
  }

  return v;
}
