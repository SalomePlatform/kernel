#ifndef HDFEXPLORER_HXX
#define HDFEXPLORER_HXX

#include "HDFinternalObject.hxx"
#include "HDFcontainerObject.hxx"

class HDFexplorer {
private :
  int _size;
  HDFcontainerObject *_container;
  HDFinternalObject *_head;
  HDFinternalObject *_queue;
  HDFinternalObject *_current;
public :
  HDFexplorer(HDFcontainerObject *container);
  ~HDFexplorer();

  void Reset(HDFcontainerObject *new_container);
  void Init();
  void Next();
  int More();
  HDFinternalObject *Value();
};

#endif
