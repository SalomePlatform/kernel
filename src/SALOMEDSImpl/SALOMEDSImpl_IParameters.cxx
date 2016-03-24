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

#include "SALOMEDSImpl_IParameters.hxx"
#include <utilities.h>

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"

#define _AP_LISTS_LIST_ "AP_LISTS_LIST"
#define _AP_ENTRIES_LIST_ "AP_ENTRIES_LIST"
#define _AP_PROPERTIES_LIST_ "AP_PROPERTIES_LIST"
#define _AP_DUMP_PYTHON_ "AP_DUMP_PYTHON"

#define _PT_ID_ "_PT_OBJECT_ID_"

/*!
  Constructor
*/
SALOMEDSImpl_IParameters::SALOMEDSImpl_IParameters(SALOMEDSImpl_AttributeParameter* ap)
{
  if(!ap) return;
  _ap = ap;
  SALOMEDSImpl_SObject so = _ap->GetSObject();
  _study = so.GetStudy();
}

SALOMEDSImpl_IParameters::~SALOMEDSImpl_IParameters()
{
  _compNames.clear();
}

int SALOMEDSImpl_IParameters::append(const std::string& listName, const std::string& value)
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

int SALOMEDSImpl_IParameters::nbValues(const std::string& listName)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(listName, PT_STRARRAY)) return 0;
  std::vector<std::string> v = _ap->GetStrArray(listName);
  return v.size();
}

std::vector<std::string> SALOMEDSImpl_IParameters::getValues(const std::string& listName)
{
  std::vector<std::string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(listName, PT_STRARRAY)) return v;
  return _ap->GetStrArray(listName);
}


std::string SALOMEDSImpl_IParameters::getValue(const std::string& listName, int index)
{
  if(!_ap) return "";
  if(!_ap->IsSet(listName, PT_STRARRAY)) return "";
  std::vector<std::string> v = _ap->GetStrArray(listName);
  if(index >= v.size()) return ""; 
  return v[index];
}

std::vector<std::string> SALOMEDSImpl_IParameters::getLists()
{
  std::vector<std::string> v;
  if(!_ap->IsSet(_AP_LISTS_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_LISTS_LIST_);
}

void SALOMEDSImpl_IParameters::setParameter(const std::string& entry, const std::string& parameterName, const std::string& value)
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


std::string SALOMEDSImpl_IParameters::getParameter(const std::string& entry, const std::string& parameterName)
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


void SALOMEDSImpl_IParameters::setIdParameter(const std::string& entry, const std::string& value)
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


std::string SALOMEDSImpl_IParameters::getIdParameter(const std::string& entry)
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

std::vector<std::string> SALOMEDSImpl_IParameters::getAllParameterNames(const std::string& entry)
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

std::vector<std::string> SALOMEDSImpl_IParameters::getAllParameterValues(const std::string& entry)
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

int SALOMEDSImpl_IParameters::getNbParameters(const std::string& entry)
{
  if(!_ap) return -1;
  if(!_ap->IsSet(entry, PT_STRARRAY)) return -1;
  return  _ap->GetStrArray(entry).size()/2;
}

std::vector<std::string> SALOMEDSImpl_IParameters::getEntries()
{
  std::vector<std::string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(_AP_ENTRIES_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_ENTRIES_LIST_);
}

void SALOMEDSImpl_IParameters::setProperty(const std::string& name, const std::string& value)
{
  if(!_ap) return;
  if(!_ap->IsSet(name, PT_STRING)) {
    append(_AP_PROPERTIES_LIST_, name); //Add the property to the internal list of properties
  }
  _ap->SetString(name, value);
}

std::string SALOMEDSImpl_IParameters::getProperty(const std::string& name)
{
  if(!_ap) return "";
  if(!_ap->IsSet(name, PT_STRING)) return "";
  return _ap->GetString(name);
}

std::vector<std::string> SALOMEDSImpl_IParameters::getProperties()
{
  std::vector<std::string> v;
  if(!_ap) return v;
  if(!_ap->IsSet(_AP_PROPERTIES_LIST_, PT_STRARRAY)) return v;
  return _ap->GetStrArray(_AP_PROPERTIES_LIST_);
}

std::string SALOMEDSImpl_IParameters::decodeEntry(const std::string& entry)
{
  if(!_study) return entry;
  int pos = entry.rfind("_");
  if(pos < 0 || pos >= entry.size()) return entry;

  std::string compName(entry, 0, pos), compID, tail(entry, pos+1, entry.length()-1);
  
  if(_compNames.find(compName) == _compNames.end()) {
    SALOMEDSImpl_SObject so = _study->FindComponent(compName);
    if(!so) return entry;
    compID = so.GetID();
    _compNames[compName] = compID;
  }
  else compID = _compNames[compName];
 
  std::string newEntry(compID);
  newEntry += (":"+tail);
  
  return newEntry;
}


bool SALOMEDSImpl_IParameters::isDumpPython(SALOMEDSImpl_Study* study, const std::string& theID)
{
  std::string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  SALOMEDSImpl_AttributeParameter* ap = study->GetCommonParameters((char*)anID.c_str(), 0);
  if(!ap) return false;
  if(!ap->IsSet(_AP_DUMP_PYTHON_, PT_BOOLEAN)) return false;
  return (bool)ap->GetBool(_AP_DUMP_PYTHON_);
}


int SALOMEDSImpl_IParameters::getLastSavePoint(SALOMEDSImpl_Study* study, const std::string& theID)
{
  std::string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;


  SALOMEDSImpl_SObject so = study->FindComponent(anID);
  if(!so) return -1;

  SALOMEDSImpl_StudyBuilder* builder = study->NewBuilder();
  SALOMEDSImpl_ChildIterator anIter = study->NewChildIterator( so );
  int tag = -1;
  for(; anIter.More(); anIter.Next())
  {
    SALOMEDSImpl_SObject val( anIter.Value() );
    DF_Attribute* genAttr;
    if(builder->FindAttribute(val, genAttr, "AttributeParameter")) tag = val.Tag();
  }

  return tag;
}



std::string SALOMEDSImpl_IParameters::getStudyScript(SALOMEDSImpl_Study* study, int savePoint, const std::string& theID)
{
  std::string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  SALOMEDSImpl_AttributeParameter* ap = study->GetCommonParameters((char*)anID.c_str(), savePoint);
  SALOMEDSImpl_IParameters ip(ap);

  std::string dump("");

  dump += "import iparameters\n";
  dump += "ipar = iparameters.IParameters(salome.myStudy.GetCommonParameters(\""+anID+"\", 1), True)\n\n";
  
  
  std::vector<std::string> v = ip.getProperties();
  if(v.size() > 0) {
    dump += "#Set up visual properties:\n";
    for(int i = 0; i<v.size(); i++) {
      std::string prp = ip.getProperty(v[i]);
      dump += "ipar.setProperty(\""+v[i]+"\", \""+prp+"\")\n";
    }
  }

  v = ip.getLists();
  if(v.size() > 0) {
    dump += "#Set up lists:\n";
    for(int i = 0; i<v.size(); i++) {
      std::vector<std::string> lst = ip.getValues(v[i]);
      dump += "# fill list "+v[i]+"\n";
      for(int j = 0; j < lst.size(); j++) {
        if (lst[j].find('\"') == -1)
          dump += "ipar.append(\""+v[i]+"\", \""+lst[j]+"\")\n";
        else
          dump += "ipar.append(\""+v[i]+"\", \"\"\""+lst[j]+"\"\"\")\n";
      }
    }
  }

  return dump;
}

std::string SALOMEDSImpl_IParameters::getDefaultScript(SALOMEDSImpl_Study* study, 
                                                  const std::string& moduleName, 
                                                  const std::string& shift, 
                                                  const std::string& theID)
{
  std::string anID;
  if(theID == "") anID = getDefaultVisualComponent();
  else anID = theID;

  std::string dump("");

  int savePoint = SALOMEDSImpl_IParameters::getLastSavePoint(study, anID);
  if(savePoint < 0) return dump;
  SALOMEDSImpl_IParameters ip = SALOMEDSImpl_IParameters(study->GetCommonParameters(anID.c_str(), savePoint));
  if(!isDumpPython(study)) return dump;

  SALOMEDSImpl_AttributeParameter* ap = study->GetModuleParameters(anID.c_str(), moduleName.c_str(), savePoint);
  ip = SALOMEDSImpl_IParameters(ap);


  dump += shift +"import iparameters\n";
  dump += shift + "ipar = iparameters.IParameters(theStudy.GetModuleParameters(\""+anID+"\", \""+moduleName+"\", 1))\n\n";
  
  std::vector<std::string> v = ip.getProperties();
  if(v.size() > 0) {
    dump += shift +"#Set up visual properties:\n";
    for(int i = 0; i<v.size(); i++) {
      std::string prp = ip.getProperty(v[i]);
      dump += shift +"ipar.setProperty(\""+v[i]+"\", \""+prp+"\")\n";
    }
  }

  v = ip.getLists();
  if(v.size() > 0) {
    dump +=  shift +"#Set up lists:\n";
    for(int i = 0; i<v.size(); i++) {
      std::vector<std::string> lst = ip.getValues(v[i]);
      dump += shift +"# fill list "+v[i]+"\n";
      for(int j = 0; j < lst.size(); j++)
        dump += shift +"ipar.append(\""+v[i]+"\", \""+lst[j]+"\")\n";
    }
  }

  v = ip.getEntries();
  if(v.size() > 0) {
    dump += shift + "#Set up entries:\n";
    for(int i = 0; i<v.size(); i++) {
      std::vector<std::string> names = ip.getAllParameterNames(v[i]);
      std::vector<std::string> values = ip.getAllParameterValues(v[i]);
      std::string decodedEntry = ip.decodeEntry(v[i]);
      SALOMEDSImpl_SObject so = study->FindObjectID(decodedEntry);
      std::string so_name("");
      if(so) so_name = so.GetName();
      
      //Try to find id parameter 
      std::vector<std::string>::iterator it = std::find(names.begin(), names.end(), _PT_ID_ );
      bool hasId = it !=  names.end();
      bool onlyId = hasId && names.size() == 1;
      
      if(!onlyId) {
	dump += shift + "# set up entry " + v[i] +" ("+so_name+")" + " parameters" + "\n";
	if(hasId) {
	  int idIndex = std::distance(names.begin(), it);
	  dump += shift + "objId = " + values[idIndex] + "\n";
	}
	   
	for(int j = 0; j < names.size() && j < values.size(); j++) {
	  if(names[j] == _PT_ID_) continue;
	  if(hasId)
	    dump += shift + "ipar.setParameter(" + "objId" + ", \"" + names[j] + "\", \"" + values[j] + "\")\n";
	  else
	    dump += shift + "ipar.setParameter(\"" + v[i] + "\", \"" + names[j] + "\", \"" + values[j] + "\")\n";
	}
      }
    }
  }
  
  return dump;  
}


std::string SALOMEDSImpl_IParameters::getDefaultVisualComponent()
{
  return "Interface Applicative";
}


