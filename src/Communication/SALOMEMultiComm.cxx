#include "SALOMEMultiComm.hxx"
using namespace std;

SALOMEMultiComm::SALOMEMultiComm():_type(SALOME::CORBA_)
{
}

SALOMEMultiComm::SALOMEMultiComm(SALOME::TypeOfCommunication type):_type(type)
{
}

void SALOMEMultiComm::setProtocol(SALOME::TypeOfCommunication type)
{
  _type=type;
}

SALOME::TypeOfCommunication SALOMEMultiComm::getProtocol() const
{
  return _type;
}


