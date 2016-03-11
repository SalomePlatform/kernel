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

#include "SALOMEDS_IParameters.hxx"
#include <utilities.h>

#define PT_INTEGER   0
#define PT_REAL      1
#define PT_BOOLEAN   2
#define PT_STRING    3
#define PT_REALARRAY 4
#define PT_INTARRAY  5
#define PT_STRARRAY  6

#define _AP_LISTS_LIST_ "AP_LISTS_LIST"
#define _AP_ENTRIES_LIST_ "AP_ENTRIES_LIST"
#define _AP_PROPERTIES_LIST_ "AP_PROPERTIES_LIST"
#define _AP_DUMP_PYTHON_ "AP_DUMP_PYTHON"

#define _PT_ID_ "_PT_OBJECT_ID_"


/*!
  Constructor
*/
SALOMEDS_IParameters::SALOMEDS_IParameters(const _PTR(AttributeParameter)& ap)
{
  if(!ap) return;
  _ap = ap;
  _PTR(SObject) so = _ap->GetSObject();
  _study = so->GetStudy();
}

SALOMEDS_IParameters::~SALOMEDS_IParameters()
{
  _compNames.clear();
}

int SALOMEDS_IParameters::append(const std::string& listName, const std::string& value)
{
  if(!_ap) return -1;
  std::vector<std::string> v;
  if(!_ap->IsSet(listName, PT_STRARRAY)) {
    if(!_ap->IsSet(_AP_LISTS_LIST_, PT_STRARRAY)) _ap->SetStrArray(_AP_LISTS_LIST_, v);
    if(listName != _AP_ENTRIES_LIST_ && 
       listName != _AP_PROPERTIES_LIST_) {
      append(_AP_LISTS_LIST_, listName);
    }
    _ap->SetStrArray(listName, v);
  }
  v = _ap->GetStrArray(listName);
  v.push_back(value);
  _ap->SetStrArray(listName, v);
  return (v.size()-1);
}

int SALOMEDS_IParameters::nbValues(const std::string& listName)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(listName, PT_STRARRAY)) return 0;
  std::vector<std::string> v = _ap->GetStrArray(listName);
  return v.size();
}

std::vector<std::string> SALOMEDS_IParameters::getValues(const std::string& listName)
{
  std::vector<std::string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(listName, PT_STRARRAY)) return v;
  return _ap->GetStrArray(listName);
}


std::string SALOMEDS_IParameters::getValue(const std::string& listName, int index)
{
  if(!_ap) return "";
  if(!_ap->IsSet(listName, PT_STRARRAY)) return "";
  std::vector<std::string> v = _ap->GetStrArray(listName);
  if(index >= v.size()) return ""; 
  return v[index];
}

std::vector<std::string> SALOMEDS_IParameters::getLists()
{
  std::vector<std::string> v;
  if(!_ap->IsSet(_AP_LISTS_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_LISTS_LIST_);
}

void SALOMEDS_IParameters::setParameter(const std::string& entry, const std::string& parameterName, const std::string& value)
{
  if(!_ap) return;
  std::vector<std::string> v;
  if(!_ap->IsSet(entry, PT_STRARRAY)) {
    append(_AP_ENTRIES_LIST_, entry); //Add the entry to the internal list of entries
    _ap->SetStrArray(entry, v);
  }
  v = _ap->GetStrArray(entry);
  v.push_back(parameterName);
  v.push_back(value);
  _ap->SetStrArray(entry, v);
}


std::string SALOMEDS_IParameters::getParameter(const std::string& entry, const std::string& parameterName)
{
  if(!_ap) return "";
  if(!_ap->IsSet(entry, PT_STRARRAY)) return "";
  std::vector<std::string> v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 0; i<length; i+=1) {
    if(v[i] == parameterName) return v[i+1];
  }
  return "";
}


std::vector<std::string> SALOMEDS_IParameters::getAllParameterNames(const std::string& entry)
{
  std::vector<std::string> v, names;
  if(!_ap) return v; 
  if(!_ap->IsSet(entry, PT_STRARRAY)) return v;
  v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 0; i<length; i+=2) {
    names.push_back(v[i]);
  }
  return names;
}


std::string SALOMEDS_IParameters::getIdParameter(const std::string& entry)
{
  if(!_ap) return "";
  if(!_ap->IsSet(entry, PT_STRARRAY)) return "";
  std::vector<std::string> v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 0; i<length; i+=1) {
    if(v[i] == _PT_ID_) return v[i+1];
  }
  return "";
}

void SALOMEDS_IParameters::setIdParameter(const std::string& entry, const std::string& value)
{
  if(!_ap) return;
  std::vector<std::string> v;
  if(!_ap->IsSet(entry, PT_STRARRAY)) {
    append(_AP_ENTRIES_LIST_, entry); //Add the entry to the internal list of entries
    _ap->SetStrArray(entry, v);
  }
  v = _ap->GetStrArray(entry);
  v.push_back(_PT_ID_);
  v.push_back(value);
  _ap->SetStrArray(entry, v);
}

std::vector<std::string> SALOMEDS_IParameters::getAllParameterValues(const std::string& entry)
{
  std::vector<std::string> v, values;
  if(!_ap) return v; 
  if(!_ap->IsSet(entry, PT_STRARRAY)) return v;
  v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 1; i<length; i+=2) {
    values.push_back(v[i]);
  }
  return values; 
}

int SALOMEDS_IParameters::getNbParameters(const std::string& entry)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(entry, PT_STRARRAY)) return -1;
  return  _ap->GetStrArray(entry).size()/2;
}

std::vector<std::string> SALOMEDS_IParameters::getEntries()
{
  std::vector<std::string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(_AP_ENTRIES_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_ENTRIES_LIST_);
}

void SALOMEDS_IParameters::setProperty(const std::string& name, const std::string& value)
{
  if(!_ap) return;
  if(!_ap->IsSet(name, PT_STRING)) {
    append(_AP_PROPERTIES_LIST_, name); //Add the property to the internal list of properties
  }
  _ap->SetString(name, value);
}

std::string SALOMEDS_IParameters::getProperty(const std::string& name)
{
  if(!_ap) return "";
  if(!_ap->IsSet(name, PT_STRING)) return "";
  return _ap->GetString(name);
}

std::vector<std::string> SALOMEDS_IParameters::getProperties()
{
  std::vector<std::string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(_AP_PROPERTIES_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_PROPERTIES_LIST_);
}


std::vector<std::string> SALOMEDS_IParameters::parseValue(const std::string& value, const char separator, bool fromEnd)
{
  std::string val(value);
  std::vector<std::string> v;
  int pos;
  if(fromEnd) pos = val.rfind(separator);
  else pos = val.find(separator);

  if(pos < 0) {
    v.push_back(value);
    return v;
  }

  std::string part1, part2;
  part1 = val.substr(0, pos);
  part2 = val.substr(pos+1, val.size());
  v.push_back(part1);
  v.push_back(part2);
  return v;
}

std::string SALOMEDS_IParameters::encodeEntry(const std::string& entry, const std::string& compName)
{
  std::string tail(entry, 6, entry.length()-1);
  std::string newEntry(compName);
  newEntry+=("_"+tail);
  return newEntry;
}

std::string SALOMEDS_IParameters::decodeEntry(const std::string& entry)
{
  if(!_study) return entry;
  int pos = entry.rfind("_");
  if(pos < 0 || pos >= entry.length()) return entry;

  std::string compName(entry, 0, pos), compID, tail(entry, pos+1, entry.length()-1);
  
  if(_compNames.find(compName) == _compNames.end()) {
    _PTR(SObject) so = _study->FindComponent(compName);
    if(!so) return entry;
    compID = so->GetID();
    _compNames[compName] = compID;
  }
  else compID = _compNames[compName];
 
  std::string newEntry(compID);
  newEntry += (":"+tail);
  
  return newEntry;
}

void SALOMEDS_IParameters::setDumpPython(_PTR(Study) study, const std::string& theID)
{
  std::string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  _PTR(AttributeParameter) ap = study->GetCommonParameters(anID, 0);
  ap->SetBool(_AP_DUMP_PYTHON_, !isDumpPython(study, theID));
}

bool SALOMEDS_IParameters::isDumpPython(_PTR(Study) study, const std::string& theID)
{
  std::string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  _PTR(AttributeParameter) ap = study->GetCommonParameters(anID, 0);
  if(!ap) return false;
  if(!ap->IsSet(_AP_DUMP_PYTHON_, PT_BOOLEAN)) return false;
  return (bool)ap->GetBool(_AP_DUMP_PYTHON_);
}

std::string SALOMEDS_IParameters::getDefaultVisualComponent()
{
  return "Interface Applicative";
}



