using namespace std;
extern "C"
{
#include "hdfi.h"
#include <string.h>
}
#include "HDFgroup.hxx"
#include "HDFexception.hxx"

HDFgroup::HDFgroup(char *name, HDFcontainerObject *father)
  : HDFcontainerObject(name)
{
  _father = father;
  _fid = _father->GetId();
  _father->AddSon(this);
  _mid = -1;
};

void HDFgroup::CreateOnDisk()
{
  if ((_id = HDFgroupCreate(_fid,_name)) < 0)
    throw HDFexception("Can't create group");
}
  
void HDFgroup::OpenOnDisk()
{
  if ((_id = HDFgroupOpen(_fid,_name)) < 0)
    throw HDFexception("Can't open group");
}
  
void HDFgroup::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFgroupClose(_id)) < 0)
    throw HDFexception("Can't close group");
  _id = -1;
}

HDFcontainerObject *HDFgroup::GetFather()
{
  return _father;
}

hdf_object_type HDFgroup::GetObjectType()
{
  return HDF_GROUP;
}

int HDFgroup::nInternalObjects()
{
  int n = 0;
  hdf_err ret;   
  
  if ((ret = HDFnObjects(_fid,_name,&n)) < 0)
    throw HDFexception("Can't determine the number of internal objects");

  return  n;  
}

void HDFgroup::InternalObjectIndentify(int rank, char *object_name)
{
  hdf_err ret;

  if ((ret = HDFobjectIdentify(_fid,_name,rank,object_name)) < 0)
    throw HDFexception("Can't identify an internal object");
}

int HDFgroup::ExistInternalObject(char *object_name)
{
  int n,i;
  int ret = 0;
  char name[HDF_NAME_MAX_LEN+1];

  n = this->nInternalObjects(); 
  for (i=0;i<n;i++) 
    {
      this->InternalObjectIndentify(i,name);
      if (!strcmp(name,object_name))
	{
	  ret = 1;
	  break;
	}
    }  
  return ret;
}

hdf_object_type HDFgroup::InternalObjectType(char *object_name)
{
  hdf_object_type type;
  hdf_err ret;
  
  if ((ret = HDFobjectType(_id,object_name,&type)) < 0)
    throw HDFexception("Can't determine internal object type");

  return type;
}

void HDFgroup::FileMount(char *file,hdf_access_mode mode)
{
  hdf_err ret;

  if (_mid != -1)
    throw HDFexception("Can't mount the file");    

  if ((_mid = HDFfileOpen(file,mode)) < 0)
    throw HDFexception("Can't mount the file");

  if ((ret = HDFfileMount(_fid,_name,_mid)) < 0)
    throw HDFexception("Can't mount the file");
}

void HDFgroup::FileUnMount()
{
  hdf_err ret;

  if ((ret = HDFfileUmount(_fid,_name)) < 0)
        throw HDFexception("Can't unmount the file");

  if ((ret = HDFfileClose(_mid)) < 0)
        throw HDFexception("Can't unmount the file");

  _mid = -1;
}
