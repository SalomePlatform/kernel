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

#ifndef DFATTRIBUTE_HXX
#define DFATTRIBUTE_HXX

#include "DF_definitions.hxx"
#include <string>

class DF_Label;
class DF_LabelNode;

//Class DF_Attribute is used to store some data defined by the DF_Attribute type
class DF_Attribute {
protected:
  DF_LabelNode* _node;

public:
  //Constructor
  Standard_EXPORT DF_Attribute();

  Standard_EXPORT virtual ~DF_Attribute();

  //Returns a Label on which this Attribute is located.
  Standard_EXPORT DF_Label Label() const;

  //Searches an Attribute with given ID located on the same Label as this Attribute.
  Standard_EXPORT DF_Attribute* FindAttribute(const std::string& theID) const;


  Standard_EXPORT virtual std::string Save() { return ""; } 
  Standard_EXPORT virtual void Load(const std::string&) {}

  //######## Virtual methods that must be redefined in descendants of the DF_Attribute 

  //This method must be redefined in all descendents of the DF_Attribute
  //ID is a std::string that uniquely identify the given type of Attributes within the Application.
  Standard_EXPORT virtual const std::string& ID() const = 0;

  //Restores a content of this Attribute from another Attribute
  Standard_EXPORT virtual  void Restore(DF_Attribute* theAttribute) = 0;

  //Creates a new empty copy oà this Attribute
  Standard_EXPORT virtual DF_Attribute* NewEmpty() const = 0;

  //Pastes a content of this Attribute into another Attribute 
  Standard_EXPORT virtual  void Paste(DF_Attribute* theIntoAttribute) = 0;

 
  //######## Callbacks

  Standard_EXPORT virtual void AfterAddition() {}
  Standard_EXPORT virtual void BeforeForget() {}

protected:
  void Backup() {}

  
friend class DF_Label;
  
};

#endif
