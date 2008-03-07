#include "DF_definitions.hxx"
#include "DF_Label.hxx"
#include "DF_Attribute.hxx"

using namespace std;

//Class DF_Attribute is used to store some data defined by the DF_Attribute type

//Constructor
DF_Attribute::DF_Attribute()
{
  _node = NULL;
}

DF_Attribute::~DF_Attribute()
{
  //Remove an attribute from a map of the node's attributes to 
  //avoid double deletion on the node destruction
  if(_node) {
    map<string, DF_Attribute*>::iterator mi;
    for(mi =_node->_attributes.begin(); mi != _node->_attributes.end(); mi++) {
       if(mi->second == this) {
	 _node->_attributes.erase(mi);
       }	 
    }
  }
}

 //Returns a Label on which this Attribute is located.
DF_Label DF_Attribute::Label() const
{
   return DF_Label(_node);
}

 //Searches an Attribute with given ID located on the same Label as this Attribute.
DF_Attribute* DF_Attribute::FindAttribute(const string& theID) const
{
  if(!_node) return NULL;
  return Label().FindAttribute(theID);
}


