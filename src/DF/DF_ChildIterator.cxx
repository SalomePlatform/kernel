#include "DF_ChildIterator.hxx"

using namespace std;


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
  return bool(_current);
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

