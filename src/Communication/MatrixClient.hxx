#ifndef __MATRIXCLIENT_HXX__
#define __MATRIXCLIENT_HXX__

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

class MatrixClient
{
public:
  static double *getValue(SALOME::Matrix_ptr distMat, int& columnSize, int& rowSize);
};

#endif
