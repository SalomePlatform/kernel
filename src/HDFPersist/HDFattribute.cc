using namespace std;
extern "C"
{
#include "hdfi.h"
}
#include "HDFexception.hxx"
#include "HDFattribute.hxx"
#include "HDFinternalObject.hxx"

HDFattribute::HDFattribute(char *name,HDFinternalObject *father,hdf_type type)
  : HDFobject(name)
{
  _father = father;
  _fid = _father->GetId();
  _type = type;
}

void HDFattribute::CreateOnDisk()
{
  if ((_id = HDFattrCreate(_fid,_name,_type)) < 0)
    throw HDFexception("Can't create attribute");
}
void HDFattribute::OpenOnDisk()
{
  if ((_id = HDFattrOpen(_fid,_name)) < 0)
    throw HDFexception("Can't open attribute");
}
void HDFattribute::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFattrClose(_id)) < 0)
      throw HDFexception("Can't close dataset");
}

void HDFattribute::WriteOnDisk(void *values)
{
  hdf_err ret;

  if ((ret = HDFattrWrite(_id,values)) < 0)
    throw HDFexception("Can't create group");
}

void HDFattribute::ReadFromDisk(void *values)
{
  hdf_err ret; 

  if ((ret = HDFattrRead(_id,values)) < 0)
    throw HDFexception("Can't read attribute");
}

HDFinternalObject * HDFattribute::GetFather()
{
  return _father;
}

hdf_type HDFattribute::GetType()
{
  return _type;
}
