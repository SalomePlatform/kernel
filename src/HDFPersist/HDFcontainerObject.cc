using namespace std;
extern "C"
{
#include "hdfi.h"
}
#include "HDFcontainerObject.hxx"
#include "HDFexception.hxx"
#include "utilities.h"

HDFcontainerObject::HDFcontainerObject(char *name)
  : HDFinternalObject(name)
{
  _nsons = 0;
  _firstson = NULL;
  _lastson = NULL;
}

HDFcontainerObject::~HDFcontainerObject()
{
  HDFinternalObject *sonToDelete = 0;
  while(_firstson)
    {
      sonToDelete = _firstson;
      _firstson = _firstson->GetNextBrother();
//       MESSAGE("son to delete: " << sonToDelete);
      delete sonToDelete;
    }
//   MESSAGE("destruction: " << this);
}

int HDFcontainerObject::nInternalObjects()
{
  return 0;
}

void HDFcontainerObject::InternalObjectIndentify(int rank, char *object_name)
{
  object_name = NULL;
}

void HDFcontainerObject::AddSon(HDFinternalObject *son)
{
//   MESSAGE("add son ")  MESSAGE("add to this" << this);
//   MESSAGE("add son " << son);
//   MESSAGE("firstson " << _firstson);
//   MESSAGE("lastson  " << _lastson);

  if (_nsons == 0)
    {
      _firstson = son;
      _lastson = _firstson;
    }
  else
    {
      _lastson->SetNextBrother(son);
      son->SetPreviousBrother(_lastson);
      _lastson = son;
    }
  _nsons ++;
//   MESSAGE("firstson " << _firstson);
//   MESSAGE("lastson  " << _lastson);
}

HDFinternalObject *HDFcontainerObject::GetFirstSon()
{
  return _firstson;
}

HDFinternalObject *HDFcontainerObject::GetLastSon()
{
  return _lastson;
}

int HDFcontainerObject::nSons()
{
  return _nsons;
}
