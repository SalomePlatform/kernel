// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
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
#include "SALOMEDS_IParameters.hxx"
#include <utilities.h>

#include <TCollection_AsciiString.hxx>

using namespace std;

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

int SALOMEDS_IParameters::append(const string& listName, const string& value)
{
  if(!_ap) return -1;
  vector<string> v;
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

int SALOMEDS_IParameters::nbValues(const string& listName)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(listName, PT_STRARRAY)) return 0;
  vector<string> v = _ap->GetStrArray(listName);
  return v.size();
}

vector<string> SALOMEDS_IParameters::getValues(const string& listName)
{
  vector<string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(listName, PT_STRARRAY)) return v;
  return _ap->GetStrArray(listName);
}


string SALOMEDS_IParameters::getValue(const string& listName, int index)
{
  if(!_ap) return "";
  if(!_ap->IsSet(listName, PT_STRARRAY)) return "";
  vector<string> v = _ap->GetStrArray(listName);
  if(index >= v.size()) return ""; 
  return v[index];
}

vector<string> SALOMEDS_IParameters::getLists()
{
  vector<string> v;
  if(!_ap->IsSet(_AP_LISTS_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_LISTS_LIST_);
}

void SALOMEDS_IParameters::setParameter(const string& entry, const string& parameterName, const string& value)
{
  if(!_ap) return;
  vector<string> v;
  if(!_ap->IsSet(entry, PT_STRARRAY)) {
    append(_AP_ENTRIES_LIST_, entry); //Add the entry to the internal list of entries
    _ap->SetStrArray(entry, v);
  }
  v = _ap->GetStrArray(entry);
  v.push_back(parameterName);
  v.push_back(value);
  _ap->SetStrArray(entry, v);
}


string SALOMEDS_IParameters::getParameter(const string& entry, const string& parameterName)
{
  if(!_ap) return "";
  if(!_ap->IsSet(entry, PT_STRARRAY)) return "";
  vector<string> v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 0; i<length; i+=1) {
    if(v[i] == parameterName) return v[i+1];
  }
  return "";
}


vector<string> SALOMEDS_IParameters::getAllParameterNames(const string& entry)
{
  vector<string> v, names;
  if(!_ap) return v; 
  if(!_ap->IsSet(entry, PT_STRARRAY)) return v;
  v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 0; i<length; i+=2) {
    names.push_back(v[i]);
  }
  return names;
}

vector<string> SALOMEDS_IParameters::getAllParameterValues(const string& entry)
{
  vector<string> v, values;
  if(!_ap) return v; 
  if(!_ap->IsSet(entry, PT_STRARRAY)) return v;
  v = _ap->GetStrArray(entry);
  int length = v.size();
  for(int i = 1; i<length; i+=2) {
    values.push_back(v[i]);
  }
  return values; 
}

int SALOMEDS_IParameters::getNbParameters(const string& entry)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(entry, PT_STRARRAY)) return -1;
  return  _ap->GetStrArray(entry).size()/2;
}

vector<string> SALOMEDS_IParameters::getEntries()
{
  vector<string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(_AP_ENTRIES_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_ENTRIES_LIST_);
}

void SALOMEDS_IParameters::setProperty(const string& name, const std::string& value)
{
  if(!_ap) return;
  if(!_ap->IsSet(name, PT_STRING)) {
    append(_AP_PROPERTIES_LIST_, name); //Add the property to the internal list of properties
  }
  _ap->SetString(name, value);
}

string SALOMEDS_IParameters::getProperty(const string& name)
{
  if(!_ap) return "";
  if(!_ap->IsSet(name, PT_STRING)) return "";
  return _ap->GetString(name);
}

vector<string> SALOMEDS_IParameters::getProperties()
{
  vector<string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(_AP_PROPERTIES_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_PROPERTIES_LIST_);
}


vector<string> SALOMEDS_IParameters::parseValue(const string& value, const char separator, bool fromEnd)
{
  TCollection_AsciiString val((char*)value.c_str());
  vector<string> v;
  int pos;
  if(fromEnd) pos = val.SearchFromEnd(separator);
  else pos = val.Search(separator);

  if(pos < 0) {
    v.push_back(value);
    return v;
  }

  TCollection_AsciiString part1, part2;
  part1 = val.SubString(1, pos-1);
  part2 = val.SubString(pos+1, val.Length());
  v.push_back(part1.ToCString());
  v.push_back(part2.ToCString());
  return v;
}

string SALOMEDS_IParameters::encodeEntry(const string& entry, const string& compName)
{
  string tail(entry, 6, entry.length()-1);
  string newEntry(compName);
  newEntry+=("_"+tail);
  return newEntry;
}

string SALOMEDS_IParameters::decodeEntry(const string& entry)
{
  if(!_study) return entry;
  int pos = entry.rfind("_");
  if(pos < 0 || pos >= entry.length()) return entry;

  string compName(entry, 0, pos), compID, tail(entry, pos+1, entry.length()-1);
  
  if(_compNames.find(compName) == _compNames.end()) {
    _PTR(SObject) so = _study->FindComponent(compName);
    if(!so) return entry;
    compID = so->GetID();
    _compNames[compName] = compID;
  }
  else compID = _compNames[compName];
 
  string newEntry(compID);
  newEntry += (":"+tail);
  
  return newEntry;
}

void SALOMEDS_IParameters::setDumpPython(_PTR(Study) study, const string& theID)
{
  string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  _PTR(AttributeParameter) ap = study->GetCommonParameters(anID, 0);
  ap->SetBool(_AP_DUMP_PYTHON_, true);
}

bool SALOMEDS_IParameters::isDumpPython(_PTR(Study) study, const string& theID)
{
  string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  _PTR(AttributeParameter) ap = study->GetCommonParameters(anID, 0);
  if(!ap) return false;
  if(!ap->IsSet(_AP_DUMP_PYTHON_, PT_BOOLEAN)) return false;
  return (bool)ap->GetBool(_AP_DUMP_PYTHON_);
}

string SALOMEDS_IParameters::getDefaultVisualComponent()
{
  return "Interface Applicative";
}



