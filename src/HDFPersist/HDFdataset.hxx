#ifndef HDFDATASET_HXX
#define HDFDATASET_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFinternalObject.hxx"
#include "HDFcontainerObject.hxx"

class HDFdataset : public HDFinternalObject
{
private :
  HDFcontainerObject *_father;
  hdf_idt _fid;
  hdf_type _type;
  hdf_size *_dim;
  int _size;
  int _ndim;

public:
  HDFdataset(char *name, HDFcontainerObject *father,hdf_type type, 
	     hdf_size dim[],int dimsize);
  HDFdataset(char *name,HDFcontainerObject *father);
  virtual ~HDFdataset();

  void CreateOnDisk();
  void OpenOnDisk();
  void CloseOnDisk();

  void WriteOnDisk(void *values);
  void ReadFromDisk(void *values);

  HDFcontainerObject *GetFather();
  hdf_type GetType();
  int nDim();
  void GetDim(hdf_size dim[]);
  int GetSize();
  hdf_object_type GetObjectType();
};

#endif
