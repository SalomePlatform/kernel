#include "SALOME_Matrix_i.hxx"
#include "SenderFactory.hxx"

SALOME_Matrix_i::SALOME_Matrix_i(const SALOMEMultiComm& multiCommunicator,const double *tabToSend,int nbOfRow,int nbOfColumn,bool ownTabToSend):_tabToSend(tabToSend),
																		_nbOfRow(nbOfRow),
																		_nbOfColumn(nbOfColumn),
																		_ownTabToSend(ownTabToSend),
																		_type(multiCommunicator)
{
}

SALOME_Matrix_i::~SALOME_Matrix_i()
{
  if(_ownTabToSend)
    delete [] _tabToSend;
}

SALOME::SenderDouble_ptr SALOME_Matrix_i::getData()
{
  return SenderFactory::buildSender(_type,_tabToSend,_nbOfRow*_nbOfColumn,_ownTabToSend);
}

CORBA::Long SALOME_Matrix_i::getSizeOfColumn()
{
  return _nbOfColumn;
}

void SALOME_Matrix_i::release()
{
  PortableServer::ObjectId_var oid = _default_POA()->servant_to_id(this);
  _default_POA()->deactivate_object(oid);
  _remove_ref();
}
