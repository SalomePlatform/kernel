// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "DF_definitions.hxx"
#include "DF_Label.hxx"
#include "DF_Attribute.hxx"

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
	  std::map<std::string, DF_Attribute*>::iterator mi;
    for(mi =_node->_attributes.begin(); mi != _node->_attributes.end(); mi++) {
       if(mi->second == this) {
         _node->_attributes.erase(mi);
         return;
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
DF_Attribute* DF_Attribute::FindAttribute(const std::string& theID) const
{
  if(!_node) return NULL;
  return Label().FindAttribute(theID);
}


