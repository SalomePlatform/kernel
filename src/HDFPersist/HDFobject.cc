using namespace std;
#include "HDFobject.hxx"
extern "C"
{
#include "hdfi.h"
#include <string.h>
}
#include "utilities.h"

HDFobject::HDFobject(char *name)
{
  //  MESSAGE("-------- constructor " << name << " " << this);
  HDFerrorModeLock();
  _name = new char[strlen(name)+1];
  strcpy(_name,name);
  _id = -1;
}

HDFobject::~HDFobject()
{
  //delete [] _name;
  delete _name;
  //  MESSAGE("-------- destructor ------" << this);
}

hdf_idt HDFobject::GetId()
{
  return _id;
}

char *HDFobject::GetName()
{
  return _name;
}

hdf_object_type HDFobject::GetObjectType()
{
  return HDF_OBJECT;
}
