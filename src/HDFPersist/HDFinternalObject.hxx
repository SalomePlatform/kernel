#ifndef HDFINTERNALOBJECT_HXX
#define HDFINTERNALOBJECT_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFobject.hxx"

class HDFinternalObject : public HDFobject
{
private :
  HDFinternalObject *_previousbrother;
  HDFinternalObject *_nextbrother;
public :
  HDFinternalObject(char *name);

  HDFinternalObject *GetPreviousBrother();
  HDFinternalObject *GetNextBrother();
  void SetPreviousBrother(HDFinternalObject *previous);
  void SetNextBrother(HDFinternalObject *next);
};
#endif
