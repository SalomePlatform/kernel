#ifndef HDFCONVERT_HXX
#define HDFCONVERT_HXX

extern "C"
{
#include "HDFtypes.h"
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
}
#include "HDFcontainerObject.hxx"
#include "HDFdataset.hxx"
#include "HDFfile.hxx"
#include <string>

class HDFConvert 
{
private:

  HDFConvert();
  ~HDFConvert();

public:

static int FromAscii(const string& file, const  HDFcontainerObject& hdf_container, const string& nomdataset);

};

#endif /* HDFCONVERT_HXX */ 
