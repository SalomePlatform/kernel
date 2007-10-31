#include "DF_definitions.hxx"
#include "DF_Label.hxx"
#include "DF_Container.hxx"

using namespace std;

//Static method that returns an ID of the give type of attributes
const string& DF_Container::GetID()
{
  static string id = "DF_Container_srn";
  return id;
}

//Creates if not exists a Container attribute and places if is not placed it the Label
DF_Container* DF_Container::Set(DF_Label& theLabel) 
{
  DF_Attribute* attr = NULL;
  if(!(attr = theLabel.FindAttribute(DF_Container::GetID()))) {
    attr = new DF_Container;
    theLabel.AddAttribute(attr);
  }
  
  return dynamic_cast<DF_Container*>(attr);
}

//Constructor
DF_Container::DF_Container() 
{
  _ints.clear();
  _doubles.clear();
  _bools.clear();
  _strings.clear();
}

//Destructor
DF_Container::~DF_Container() 
{
  _ints.clear();
  _doubles.clear();
  _bools.clear();
  _strings.clear();
}

//Sets an integer value of the attribute with given ID
void DF_Container::SetInt(const string& theID, int theValue) 
{
  _ints[theID] = theValue;
}

//Returns an integer value of the attribute with given ID
int DF_Container::GetInt(const string& theID) 
{
  if(!HasIntID(theID)) 
    return 0;
  return _ints[theID];
}

//Returns True if there is an integer with given ID
bool DF_Container::HasIntID(const string& theID) 
{
  if(_ints.find(theID) != _ints.end()) return true;
  return false;
}

//Sets a double value of the attribute with given ID
void DF_Container::SetDouble(const string& theID, const double& theValue) 
{
  _doubles[theID] = theValue;
}


//Returns a double value of the attribute with given ID
double DF_Container::GetDouble(const string& theID) 
{
  if(!HasDoubleID(theID)) return 0.0;
  return _doubles[theID];
}

//Returns True if there is a double with given ID
bool DF_Container::HasDoubleID(const string& theID) 
{
  if(_doubles.find(theID) != _doubles.end()) return true;
  return false;
}
 
//Sets a string value of the attribute with given ID
void DF_Container::SetString(const string& theID, const string& theValue) 
{
  _strings[theID] = theValue;
}

//Returns a string  value of the attribute with given ID
string DF_Container::GetString(const string& theID) 
{
  if(!HasStringID(theID)) return "";
  return _strings[theID];
}

//Returns True if there is a string with given ID
bool DF_Container::HasStringID(const string& theID) 
{
  if(_strings.find(theID) != _strings.end()) return true;
  return false;
}

//Sets a boolean value of the attribute with given ID
void DF_Container::SetBool(const string& theID, bool theValue) 
{
  _bools[theID] = theValue;
}

//Returns a boolean value of the attribute with given ID
bool DF_Container::GetBool(const string& theID) 
{
  if(!HasBoolID(theID)) return false;
  return _bools[theID];
}

//Returns True if there is a boolean value with given ID
bool DF_Container::HasBoolID(const string& theID) 
{
  if(_bools.find(theID) != _bools.end()) return true;
  return false;
}

//Clears a content of the attribute
void DF_Container::Clear()
{
  _ints.clear();
  _doubles.clear();
  _strings.clear();
  _bools.clear();
}

//ID is a string that uniquely identify the given type of Attributes within the Application.
const string& DF_Container::ID() const
{
  return GetID();
}

//Restores a content of this Attribute from another Attribute
void DF_Container::Restore(DF_Attribute* theAttribute) 
{
  Clear();

  DF_Container* attr = dynamic_cast<DF_Container*>(theAttribute);
  if(!attr) return;

  typedef map<string, int>::const_iterator SI;
  for(SI p = attr->_ints.begin(); p != attr->_ints.end(); p++) 
    _ints[p->first] = p->second;

  typedef map<string, double>::const_iterator SD;
  for(SD p = attr->_doubles.begin(); p != attr->_doubles.end(); p++) 
    _doubles[p->first] = p->second;

  typedef map<string, string>::const_iterator SS;
  for(SS p = attr->_strings.begin(); p != attr->_strings.end(); p++) 
    _strings[p->first] = p->second;

  typedef map<string, bool>::const_iterator SB;
  for(SB p = attr->_bools.begin(); p != attr->_bools.end(); p++) 
    _bools[p->first] = p->second;
}

//Creates a new empty copy oà this Attribute
DF_Attribute* DF_Container::NewEmpty() const 
{
  return new DF_Container();
}

//Pastes a content of this Attribute into another Attribute 
void DF_Container::Paste(DF_Attribute* theIntoAttribute) 
{
  DF_Container* attr = dynamic_cast<DF_Container*>(theIntoAttribute);
  if(!attr) return;  

  attr->Clear();

  typedef map<string, int>::const_iterator SI;
  for(SI p = _ints.begin(); p != _ints.end(); p++) 
    attr->_ints[p->first] = p->second;

  typedef map<string, double>::const_iterator SD;
  for(SD p = _doubles.begin(); p != _doubles.end(); p++) 
    attr->_doubles[p->first] = p->second;

  typedef map<string, string>::const_iterator SS;
  for(SS p = _strings.begin(); p != _strings.end(); p++) 
    attr->_strings[p->first] = p->second;

  typedef map<string, bool>::const_iterator SB;
  for(SB p = _bools.begin(); p != _bools.end(); p++) 
   attr-> _bools[p->first] = p->second;
}

