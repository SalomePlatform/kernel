using namespace std;
extern "C"
{
#include "hdfi.h"
}
#include "HDFinternalObject.hxx"

HDFinternalObject::HDFinternalObject(char *name)
  : HDFobject(name)
{
  _previousbrother = NULL;
  _nextbrother = NULL;
}

HDFinternalObject *HDFinternalObject::GetPreviousBrother()
{
  return _previousbrother;
}
 
HDFinternalObject *HDFinternalObject::GetNextBrother()
{
  return _nextbrother;
}
  
void HDFinternalObject::SetPreviousBrother(HDFinternalObject *previous)
{
  _previousbrother = previous;
}
  
void HDFinternalObject::SetNextBrother(HDFinternalObject *next)
{
  _nextbrother = next;
}

