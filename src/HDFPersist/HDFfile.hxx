#ifndef HDFFILE_HXX
#define HDFFILE_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFcontainerObject.hxx"

class HDFfile : public HDFcontainerObject
{
private :
  hdf_access_mode _access_mode;
public :
  HDFfile(char *name);

  void CreateOnDisk();
  void OpenOnDisk(hdf_access_mode acess_mode);
  void CloseOnDisk();

  hdf_access_mode GetAccessMode();
  hdf_object_type GetObjectType();

  int nInternalObjects();
  void InternalObjectIndentify(int rank, char *object_name);
  int ExistInternalObject(char *object_name);
  hdf_object_type InternalObjectType(char *object_name);
};

#endif /* HDFFILE_HXX */ 


