using namespace std;
extern "C"
{
#include "hdfi.h"
#include <unistd.h>
#include <string.h>
}
#include <iostream.h>
#include "HDFfile.hxx"
#include "HDFexception.hxx"

HDFfile::HDFfile(char *name)
  : HDFcontainerObject(name) 
{}

void HDFfile::CreateOnDisk()
{
  if ((_id = HDFfileCreate(_name)) < 0) 
    throw HDFexception("Can't create HDF file");
}

void HDFfile::OpenOnDisk(hdf_access_mode access_mode)
{
  _access_mode = access_mode;

  switch (_access_mode)
    {
    case HDF_RDWR :
      if (access(_name,F_OK))
	{
	  if ((_id = HDFfileCreate(_name)) < 0) 
	    throw HDFexception("Can't open HDF file");
	}
      else
	if ((_id = HDFfileOpen(_name,_access_mode)) < 0)
	  throw HDFexception("Can't open HDF file");
      break;
      
    case HDF_RDONLY :
      if ((_id = HDFfileOpen(_name,_access_mode)) < 0)
	throw HDFexception("Can't open HDF file");
      break;
      
    default :
      throw HDFexception("Can't open HDF file : bad acces option");
    }
}

void HDFfile::CloseOnDisk()
{
  hdf_err ret;

  if ((ret = HDFfileClose(_id)) < 0) 
    throw HDFexception("Can't close HDF file");
  _id = -1;
}
  

hdf_access_mode HDFfile::GetAccessMode()
{
  return _access_mode;
}

hdf_object_type HDFfile::GetObjectType()
{
  return HDF_FILE;
}

int HDFfile::nInternalObjects()
{
  int n;
  hdf_err ret;   

  if ((ret = HDFnObjects(_id,"/",&n)) < 0)
    throw HDFexception("Can't determine the number of internal objects");

  return  n;
}

void HDFfile::InternalObjectIndentify(int rank, char *object_name)
{
  hdf_err ret;

  if ((ret = HDFobjectIdentify(_id,"/",rank,object_name)) < 0)
    throw HDFexception("Can't identify an internal object");
}

int HDFfile::ExistInternalObject(char *object_name)
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

hdf_object_type HDFfile::InternalObjectType(char *object_name)
{
  hdf_object_type type;
  hdf_err ret;
  
  if ((ret = HDFobjectType(_id,object_name,&type)) < 0)
    throw HDFexception("Can't determine internal object type");

  return type;
}
