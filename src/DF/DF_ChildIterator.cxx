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

#include "DF_ChildIterator.hxx"

//Constructor
DF_ChildIterator::DF_ChildIterator(const DF_Label& theLabel, bool allLevels)
  :_root(NULL), _current(NULL)
{
  Init(theLabel, allLevels);
}

DF_ChildIterator::DF_ChildIterator()
  :_root(NULL), _current(NULL)
{
}

DF_ChildIterator::~DF_ChildIterator() 
{
  _root = NULL;
  _current = NULL;
}

//Initializes the iterator
void DF_ChildIterator::Init(const DF_Label& theLabel, bool allLevels)
{
  _root = theLabel._node;
  _allLevels = allLevels;
  if(_root) _current = _root->_firstChild;
}

//Returns a current Label
DF_Label DF_ChildIterator::Value()
{
  return DF_Label(_current);
}

//Returns true if there is a current Label
bool DF_ChildIterator::More()
{
  return _current!=0;
}

//Moves to the next Label
void DF_ChildIterator::Next()
{
  if(!_allLevels) {
    _current = _current->_next; //Move to the next brother
    return;
  }
  else {
    if(_current->_firstChild) { //Go down to the first child
      _current = _current->_firstChild;
    }
    else {
      if(_current->_next) { //Next Brother
        _current = _current->_next;
      }
      else {
        if(_current->_father && _current->_father != _root) {
          DF_LabelNode *father = _current->_father;
          _current = father->_next;
          if(!_current) {
            while(father && father != _root) {
              father = father->_father;
              if(father->_next) break;
            } 
            if(father == _root) father = NULL;
            if(father) _current = father->_next;
            else _current = NULL;
          }
        }
        else {
          _current = NULL; //We iterate the whole sub tree
        }
      }
    }
  }
}

