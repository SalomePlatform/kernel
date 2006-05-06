#include "MatrixClient.hxx"
#include "ReceiverFactory.hxx"

double *MatrixClient::getValue(SALOME::Matrix_ptr distMat, int& columnSize, int& rowSize)
{
  long totalSize;
  double *ret=ReceiverFactory::getValue(distMat->getData(),totalSize);
  columnSize=distMat->getSizeOfColumn();
  distMat->release();
  rowSize=totalSize/columnSize;
  return ret;
}
