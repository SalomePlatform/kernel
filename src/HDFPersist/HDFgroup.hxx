#ifndef HDFGROUP_HXX
#define HDFGROUP_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFcontainerObject.hxx"

class HDFgroup : public HDFcontainerObject
{
private :
  HDFcontainerObject *_father;
  hdf_idt _fid;
  hdf_idt _mid;
public :
  HDFgroup(char *name, HDFcontainerObject *father);

  void CreateOnDisk();
  void OpenOnDisk();
  void CloseOnDisk();

  void FileMount(char *file,hdf_access_mode mode);
  void FileUnMount();

  HDFcontainerObject *GetFather();
  hdf_object_type GetObjectType();

  int nInternalObjects();
  void InternalObjectIndentify(int rank, char *object_name);
  int ExistInternalObject(char *object_name);
  hdf_object_type InternalObjectType(char *object_name);
};

#endif /* HDFGROUP_HXX */ 
