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
