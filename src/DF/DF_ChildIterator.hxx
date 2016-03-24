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

#ifndef DFCHILDITERATOR_HXX
#define DFCHILDITERATOR_HXX

#include "DF_definitions.hxx"
#include "DF_Label.hxx"

#include <string>

//Class DF_ChildIterator is used to iterate a tree of Labels in the Document
class DF_ChildIterator {
public:
  //Constructor
  Standard_EXPORT DF_ChildIterator(const DF_Label& theLabel, bool allLevels = false);

  Standard_EXPORT DF_ChildIterator();

  Standard_EXPORT ~DF_ChildIterator();

  //Initializes the iterator, if allLevels is true the iterator before iterating the next
  //brother of the current Label iterates the Label children
  Standard_EXPORT void Init(const DF_Label& theLabel, bool allLevels = false);

  //Returns a current Label
  Standard_EXPORT DF_Label Value();

  //Returns true if there is a current Label
  Standard_EXPORT bool More();

  //Moves to the next Label
  Standard_EXPORT void Next();

private:
  DF_LabelNode*    _root;
  DF_LabelNode*    _current;
  bool             _allLevels;
};

#endif
