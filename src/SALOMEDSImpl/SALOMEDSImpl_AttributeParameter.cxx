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

//  File   : SALOMEDSImpl_AttributeParameter.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeParameter.hxx"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <sstream>


// Purpose: Each character in the string is replaced by 3 characters: '%' and hex number 
//          of the character (2 characters)
std::string convertString(const std::string& S)
{
  int length = S.size();
  const char *s = S.c_str();
  char *c = new char[3], *buffer = new char[length*3+1];
  buffer[length*3] = (char)0;
  for(int i = 0, pos = 0; i<length; i++, pos+=3) {
    int val = (int)s[i];
    buffer[pos] = '%';
    sprintf(c, "%.2x", val);
    buffer[pos+1] = c[0]; 
    buffer[pos+2] = c[1];
  }

  std::string RS(buffer); 
  delete [] c;
  delete [] buffer;
  return RS;
}

//Restors a string converted by the function convertString
std::string restoreString(const std::string& S)
{
  int length = S.size();
  char *c = new char[3], *buffer = new char[length/3+1];
  buffer[length/3] = (char)0;
  const char *s = S.c_str();
  for(int i = 0, pos = 0; i<length; i+=3, pos++) {
    c[0] = s[i+1];
    c[1] = s[i+2];
    c[2] = (char)0;
    int val = strtol(c, NULL, 16);
    buffer[pos] = (char)val;
  }

  std::string RS(buffer); 
  delete [] c;
  delete [] buffer;
  return RS;
}

//=======================================================================
/*!
 * Function : GetID
 * Purpose  : Returns GUID of the attribute
 */
//=======================================================================
const std::string& SALOMEDSImpl_AttributeParameter::GetID ()
{
  static std::string ParemeterID ("BA75F3A1-E40B-46b8-8D24-B1D3C3BB1A8C");
  return ParemeterID;
}   

//=======================================================================
/*!
 * Function : Set
 * Purpose  : Adds an attribute to the label
 */
//=======================================================================
SALOMEDSImpl_AttributeParameter* SALOMEDSImpl_AttributeParameter::Set (const DF_Label& L) 
{
  SALOMEDSImpl_AttributeParameter* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeParameter*)L.FindAttribute(SALOMEDSImpl_AttributeParameter::GetID()))) {
    A = new  SALOMEDSImpl_AttributeParameter(); 
    L.AddAttribute(A);
  }

  return A;
}

//=======================================================================
/*!
 * Function : SetInt
 * Purpose  : Associates a integer value with the ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetInt(const std::string& theID, const int& theValue)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _ints[theID] = theValue;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetInt
 * Purpose  : Returns a int value associated with the given ID
 */
//=======================================================================
int SALOMEDSImpl_AttributeParameter::GetInt(const std::string& theID)
{
  if(!IsSet(theID, PT_INTEGER)) throw DFexception("Invalid ID");
  return _ints[theID];
}

//=======================================================================
/*!
 * Function : SetReal
 * Purpose  : Associates a double value with the ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetReal(const std::string& theID, const double& theValue)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _reals[theID] = theValue;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetReal
 * Purpose  : Returns a double value associated with the given ID
 */
//=======================================================================
double SALOMEDSImpl_AttributeParameter::GetReal(const std::string& theID)
{
  if(!IsSet(theID, PT_REAL)) throw DFexception("Invalid ID");
  return _reals[theID];
}

//=======================================================================
/*!
 * Function : SetString
 * Purpose  : Associates a string with the ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetString(const std::string& theID, const std::string& theValue)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _strings[theID] = theValue;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetString
 * Purpose  : Returns a string associated with the given ID
 */
//=======================================================================
std::string SALOMEDSImpl_AttributeParameter::GetString(const std::string& theID)
{
  if(!IsSet(theID, PT_STRING)) throw DFexception("Invalid ID");
  return _strings[theID];
}

//=======================================================================
/*!
 * Function : SetBool
 * Purpose  : Associates a bool value with the ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetBool(const std::string& theID, const bool& theValue)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _bools[theID] = theValue;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetBool
 * Purpose  : Returns a bool value associated with the ID
 */
//=======================================================================
bool SALOMEDSImpl_AttributeParameter::GetBool(const std::string& theID)
{
  if(!IsSet(theID, PT_BOOLEAN)) throw DFexception("Invalid ID");
  return _bools[theID];
}

//=======================================================================
/*!
 * Function : SetRealArray
 * Purpose  : Associates an array of double values with the given ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetRealArray(const std::string& theID, const std::vector<double>& theArray)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _realarrays[theID] = theArray;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetRealArray
 * Purpose  : Returns double values associated with the ID
 */
//=======================================================================
std::vector<double> SALOMEDSImpl_AttributeParameter::GetRealArray(const std::string& theID)
{
  if(!IsSet(theID, PT_REALARRAY)) throw DFexception("Invalid ID");
  return _realarrays[theID];
}
 

//=======================================================================
/*!
 * Function : SetIntArray
 * Purpose  : Associates an array of int values with the given ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetIntArray(const std::string& theID, const std::vector<int>& theArray)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _intarrays[theID] = theArray;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetIntArray
 * Purpose  : Returns int values associated with the ID
 */
//=======================================================================
std::vector<int> SALOMEDSImpl_AttributeParameter::GetIntArray(const std::string& theID)
{
  if(!IsSet(theID, PT_INTARRAY)) throw DFexception("Invalid ID");
  return _intarrays[theID];
}
 

//=======================================================================
/*!
 * Function : SetStrArray
 * Purpose  : Associates an array of string values with the given ID
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::SetStrArray(const std::string& theID, const std::vector<std::string>& theArray)
{
  CheckLocked();

  if(theID.size() == 0) return;

  Backup();

  _strarrays[theID] = theArray;

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetStrArray
 * Purpose  : Returns string values associated with the ID
 */
//=======================================================================
std::vector<std::string> SALOMEDSImpl_AttributeParameter::GetStrArray(const std::string& theID)
{
  if(!IsSet(theID, PT_STRARRAY)) throw DFexception("Invalid ID");
  return _strarrays[theID];
}
 

//=======================================================================
/*!
 * Function : IsSet
 * Purpose  : Returns true if for the ID of given type was assigned \n
 *            a value in the attribute
 */
//=======================================================================
bool SALOMEDSImpl_AttributeParameter::IsSet(const std::string& theID, const Parameter_Types theType)
{
  switch(theType) {
  case PT_INTEGER: {
    if(_ints.find(theID) != _ints.end()) return true;
    break;
  }
  case PT_REAL: {
    if(_reals.find(theID) != _reals.end()) return true;
    break;
  }
  case PT_BOOLEAN: {
    if(_bools.find(theID) != _bools.end()) return true;
    break;
  }
  case PT_STRING: {
    if(_strings.find(theID) != _strings.end()) return true;
    break;
  }
  case PT_REALARRAY: {
    if(_realarrays.find(theID) != _realarrays.end()) return true;
    break;
  }
  case PT_INTARRAY: {
    if(_intarrays.find(theID) != _intarrays.end()) return true;
    break;
  }
  case PT_STRARRAY: {
    if(_strarrays.find(theID) != _strarrays.end()) return true;
    break;
  }
  default: return false;
  }

  return false;
}

//=======================================================================
/*!
 * Function : RemoveID
 * Purpose  : Removes a parameter with given ID
 */
//=======================================================================
bool SALOMEDSImpl_AttributeParameter::RemoveID(const std::string& theID, const Parameter_Types theType)
{
  Backup();
  SetModifyFlag(); 

  switch(theType) {
  case PT_INTEGER: {
    if(_ints.erase(theID)) return true;
    break;
  }
  case PT_REAL: {
    if(_reals.erase(theID)) return true;
    break;
  }
  case PT_BOOLEAN: {
    if(_bools.erase(theID)) return true;
    break;
  }
  case PT_STRING: {
    if(_strings.erase(theID)) return true;
    break;
  }
  case PT_REALARRAY: {
    if(_realarrays.erase(theID)) return true;
    break;
  }
  case PT_INTARRAY: {
    if(_intarrays.erase(theID)) return true;
    break;
  }
  case PT_STRARRAY: {
    if(_strarrays.erase(theID)) return true;
    break;
  }
  default: return false;
  }


  return false;
}


//=======================================================================
/*!
 * Function : GetFather
 * Purpose  : Returns a father attribute for this attribute
 */
//=======================================================================
SALOMEDSImpl_AttributeParameter* SALOMEDSImpl_AttributeParameter::GetFather()
{
  SALOMEDSImpl_AttributeParameter* aFather;
  DF_Label L = Label();
  if(L.IsRoot()) return aFather;

  while(!L.IsRoot()) {
    L = L.Father();
    if((aFather=(SALOMEDSImpl_AttributeParameter*)L.FindAttribute(SALOMEDSImpl_AttributeParameter::GetID()))) break; 
  }

  return aFather;
}

//=======================================================================
/*!
 * Function : HasFather
 * Purpose  : Returns True if the attribute has a father attribute
 */
//=======================================================================
bool SALOMEDSImpl_AttributeParameter::HasFather()
{
  DF_Label L = Label();
  if(L.IsRoot()) return false;
  while(!L.IsRoot()) {
    L = L.Father();
    if(L.IsAttribute(SALOMEDSImpl_AttributeParameter::GetID())) return true; 
  }

  return false;
}

//=======================================================================
/*!
 * Function : IsRoot
 * Purpose  : Returns True is the attribute is highest in an hierachy
 */
//=======================================================================
bool SALOMEDSImpl_AttributeParameter::IsRoot()
{
  return ((HasFather())?false:true);
}           

//=======================================================================
/*!
 * Function : Clear
 * Purpose  : Clears the content of the attribute
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::Clear()
{
  Backup();

  _ints.clear();
  _reals.clear();
  _bools.clear();
  _strings.clear();
  _realarrays.clear();
  _intarrays.clear();
  _strarrays.clear();

  SetModifyFlag(); 
}

//=======================================================================
/*!
 * Function : GetIDs
 * Purpose  : Returns an array of all ID's of the given type
 */
//=======================================================================
std::vector<std::string> SALOMEDSImpl_AttributeParameter::GetIDs(const Parameter_Types theType)
{
  
  std::vector<std::string> anArray;
  int i = 0;

  switch(theType) {
  case PT_INTEGER: {
    if(_ints.size()) {
      anArray.resize(_ints.size());
      for(std::map<std::string,int>::const_iterator p = _ints.begin(); p != _ints.end(); p++, i++) 
        anArray[i] =  p->first;
    }
    break;
  }
  case PT_REAL: {
    if(_reals.size()) {
      anArray.resize(_reals.size());
      for(std::map<std::string,double>::const_iterator p = _reals.begin(); p != _reals.end(); p++, i++) 
        anArray[i] = p->first;
    }
    break;
  }
  case PT_BOOLEAN: {
    if(_bools.size()) {
      anArray.resize(_bools.size());
      for(std::map<std::string,bool>::const_iterator p = _bools.begin(); p != _bools.end(); p++, i++) 
        anArray[i] = p->first;
    }
    break;
  }
  case PT_STRING: {
    if(_strings.size()) {
      anArray.resize(_strings.size());
      for(std::map<std::string,std::string>::const_iterator p = _strings.begin(); p!= _strings.end(); p++, i++) 
        anArray[i] = p->first;
    }
    break;
  }
  case PT_REALARRAY: {
    if(_realarrays.size()) {
      anArray.resize(_realarrays.size());
      for(std::map< std::string, std::vector<double> >::const_iterator p = _realarrays.begin(); p!= _realarrays.end(); p++, i++) 
        anArray[i] = p->first;
    }
    break;
  }
  case PT_INTARRAY: {
    if(_intarrays.size()) {
      anArray.resize(_intarrays.size());
      for(std::map< std::string, std::vector<int> >::const_iterator p = _intarrays.begin(); p!= _intarrays.end(); p++, i++) 
        anArray[i] = p->first;
    }
    break;
  }
  case PT_STRARRAY: {
    if(_strarrays.size()) {
      anArray.resize(_strarrays.size());
      for(std::map< std::string, std::vector<std::string> >::const_iterator p = _strarrays.begin(); p!= _strarrays.end(); p++, i++) 
        anArray[i] = p->first;
    }
    break;
  }
  default: return anArray;
  }

  return anArray;
}

//=======================================================================
/*!
 * Function : ID
 * Purpose  : Rteurns an GUID of the attribute
 */
//=======================================================================
const std::string& SALOMEDSImpl_AttributeParameter::ID () const { return GetID(); } 


DF_Attribute* SALOMEDSImpl_AttributeParameter::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeParameter(); 
}

//=======================================================================
/*!
 * Function : Restore
 * Purpose  : Restors the content of the attribute from another
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::Restore(DF_Attribute* with) 
{
  SALOMEDSImpl_AttributeParameter* A = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(with);
  _ints.clear();
  _reals.clear();
  _bools.clear();
  _strings.clear();
  _realarrays.clear();
  _intarrays.clear();
  _strarrays.clear();
  
  for(std::map<std::string,int>::const_iterator p = A->_ints.begin(); p!= A->_ints.end(); p++)
    if(p->first.size()) _ints[p->first] = p->second;
  for(std::map<std::string,double>::const_iterator p = A->_reals.begin(); p!= A->_reals.end(); p++) 
    if(p->first.size()) _reals[p->first] = p->second;
  for(std::map<std::string,bool>::const_iterator p = A->_bools.begin(); p!= A->_bools.end(); p++) 
    if(p->first.size()) _bools[p->first] = p->second;
  for(std::map<std::string,std::string>::const_iterator p = A->_strings.begin(); p!= A->_strings.end(); p++) 
    if(p->first.size()) _strings[p->first] = p->second;
  for(std::map< std::string,std::vector<double> >::const_iterator p = A->_realarrays.begin(); p!= A->_realarrays.end(); p++) 
    if(p->first.size()) _realarrays[p->first] = p->second;  
  for(std::map< std::string,std::vector<int> >::const_iterator p = A->_intarrays.begin(); p!= A->_intarrays.end(); p++) 
    if(p->first.size()) _intarrays[p->first] = p->second;  
  for(std::map< std::string,std::vector<std::string> >::const_iterator p = A->_strarrays.begin(); p!= A->_strarrays.end(); p++) 
    if(p->first.size()) _strarrays[p->first] = p->second; 
}

//=======================================================================
/*!
 * Function : Paste
 * Purpose  : Pastes the content of attribute to another
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::Paste (DF_Attribute* into)
{
  into->Restore(this);
}

//=======================================================================
/*!
 * Function : Save
 * Purpose  : Saves a content of the attribute as a string
 */
//=======================================================================
std::string SALOMEDSImpl_AttributeParameter::Save() 
{ 
  std::ostringstream buffer;
  char *tmpBuffer = new char[255];

  buffer << _ints.size() << " ";
  for(std::map<std::string,int>::const_iterator p = _ints.begin(); p != _ints.end(); p++) {
    buffer << convertString(p->first) << " " << p->second << " ";
  }

  buffer << _reals.size() << " ";
  for(std::map<std::string,double>::const_iterator p =_reals.begin(); p != _reals.end(); p++) {
    sprintf(tmpBuffer, "%.64e", p->second);
    buffer << convertString(p->first) << " " << tmpBuffer << " ";
  }

  buffer << _bools.size() << " ";
  for(std::map<std::string,bool>::const_iterator p = _bools.begin(); p != _bools.end(); p++) {
     buffer << convertString(p->first) << " " << p->second << " ";
  }

  buffer << _strings.size() << " ";
  for(std::map<std::string,std::string>::const_iterator p = _strings.begin(); p != _strings.end(); p++) {
    buffer << convertString(p->first) << " " <<  convertString(p->second) << " ";
  }

  buffer << _realarrays.size() << " ";
  for(std::map< std::string,std::vector<double> >::const_iterator p = _realarrays.begin(); p != _realarrays.end(); p++) {
    std::vector<double> v(p->second);
    sprintf(tmpBuffer, " %s %d ", convertString(p->first).c_str(), v.size());
    buffer << tmpBuffer;
    for(int i = 0; i<v.size(); i++) {
      sprintf(tmpBuffer, " %.64e ", v[i]);
      buffer << tmpBuffer;
    }
  }

  buffer << _intarrays.size() << " ";
  for(std::map< std::string,std::vector<int> >::const_iterator p = _intarrays.begin(); p != _intarrays.end(); p++) {
    std::vector<int> v(p->second);
    sprintf(tmpBuffer, " %s %d ", convertString(p->first).c_str(), v.size());
    buffer << tmpBuffer;
    for(int i = 0; i<v.size(); i++) {
      sprintf(tmpBuffer, " %d ", v[i]);
      buffer << tmpBuffer;
    }
  }

  buffer << _strarrays.size() << " ";
  for(std::map< std::string,std::vector<std::string> >::const_iterator p = _strarrays.begin(); p != _strarrays.end(); p++) {
    std::vector<std::string> v(p->second);
    sprintf(tmpBuffer, " %s %d ", convertString(p->first).c_str(), v.size());
    buffer << tmpBuffer;
    for(int i = 0; i<v.size(); i++) {
      buffer << " " << convertString(v[i]) << " ";
    }
  }

  delete tmpBuffer;

  std::string AS = buffer.str();

  return AS; 
}

//=======================================================================
/*!
 * Function : Load
 * Purpose  : Restores the attribute from the string
 */
//=======================================================================
void SALOMEDSImpl_AttributeParameter::Load(const std::string& theValue) 
{ 
  Backup();

  _ints.clear();
  _reals.clear();
  _bools.clear();
  _strings.clear();
  _realarrays.clear();
  _intarrays.clear();

  std::istringstream buffer(theValue.c_str());

  int size, val, ival;
  double val2;
  std::string s, id;

  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> val;
    _ints[restoreString(id)] = val;
  }

  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> val2;
    _reals[restoreString(id)] = val2;
  }

  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> val;
    _bools[restoreString(id)] = val;
  }

  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> s;
    _strings[restoreString(id)] = restoreString(s);
  }
  
  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> val;
    std::vector<double> v;
    v.resize(val);
    for(int j = 0; j<val; j++) {
      buffer >> val2;
      v[j] = val2;
    }
    _realarrays[restoreString(id)] = v;
  }

  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> val;
    std::vector<int> v;
    v.resize(val);
    for(int j = 0; j<val; j++) {
      buffer >> ival;
      v[j] = ival;
    }
    _intarrays[restoreString(id)] = v;
  }
  
  buffer >> size;
  for(int i = 1; i<=size; i++) {
    buffer >> id >> val;
    std::vector<std::string> v;
    v.resize(val);
    for(int j = 0; j<val; j++) {
      buffer >> s;
      v[j] = restoreString(s);
    }
    _strarrays[restoreString(id)] = v;
  }
} 
