#ifndef HDFOBJECT_HXX
#define HDFOBJECT_HXX

extern "C"
{
#include "HDFtypes.h"
}

class HDFobject {
protected :
  char *_name;
  hdf_idt _id;
public :
  HDFobject(char *name);
  virtual ~HDFobject();

  hdf_idt GetId();
  char *GetName();
  virtual hdf_object_type GetObjectType();
  
};

#endif /* HDFOBJECT_HXX */ 

