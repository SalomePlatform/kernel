#ifndef HDFATTRIBUTE_HXX
#define HDFATTRIBUTE_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFinternalObject.hxx"
#include "HDFcontainerObject.hxx"

class HDFattribute : public HDFobject
{
private :
  HDFinternalObject *_father;
  hdf_idt _fid;
  hdf_type _type;
public :
  HDFattribute(char *name,HDFinternalObject *father,hdf_type type);

  void CreateOnDisk();
  void OpenOnDisk();
  void CloseOnDisk();

  void WriteOnDisk(void *values);
  void ReadFromDisk(void *values);

  HDFinternalObject *GetFather();
  hdf_type GetType();
};

#endif
