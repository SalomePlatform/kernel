#ifndef HDFCONTAINEROBJECT_HXX
#define HDFCONTAINEROBJECT_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFobject.hxx"
#include "HDFinternalObject.hxx"

class HDFcontainerObject : public HDFinternalObject
{ 
private :
  HDFinternalObject *_firstson;
  HDFinternalObject *_lastson;
  int _nsons;
public :
  HDFcontainerObject(char *name);
  virtual ~HDFcontainerObject();

  virtual int nInternalObjects();
  virtual void InternalObjectIndentify(int rank, char *object_name);
  
  void AddSon(HDFinternalObject *son);
  HDFinternalObject *GetFirstSon();
  HDFinternalObject *GetLastSon();
  int nSons();
};
#endif
