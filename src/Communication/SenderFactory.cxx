#include "SALOME_Comm_i.hxx"
#include "SenderFactory.hxx"
#include "utilities.h"
#include "SALOMEMultiComm.hxx"
using namespace std;

#ifdef COMP_CORBA_DOUBLE
#define SALOME_CorbaDoubleSender SALOME_CorbaDoubleNCSender_i
#else
#define SALOME_CorbaDoubleSender SALOME_CorbaDoubleCSender_i
#endif

#ifdef COMP_CORBA_LONG
#define SALOME_CorbaLongSender SALOME_CorbaLongNCSender_i
#else
#define SALOME_CorbaLongSender SALOME_CorbaLongCSender_i
#endif

SALOME::SenderDouble_ptr SenderFactory::buildSender(SALOMEMultiComm &multiCommunicator,const double *tab,long lgr,bool ownTab)throw(MultiCommException){
  switch(multiCommunicator.getProtocol())
    {
    case SALOME::CORBA_:
      {
	SALOME_CorbaDoubleSender * retc=new SALOME_CorbaDoubleSender(tab,lgr,ownTab);
	return retc->_this();
      }
#ifdef HAVE_MPI2
    case SALOME::MPI_:
      {
	SALOME_MPISenderDouble_i* retm=new SALOME_MPISenderDouble_i(tab,lgr,ownTab);
	return retm->_this();
      }
#endif
#ifdef HAVE_SOCKET
    case SALOME::SOCKET_:
      {
	SALOME_SocketSenderDouble_i* rets=new SALOME_SocketSenderDouble_i(tab,lgr,ownTab);
	return rets->_this();
      }
#endif
    default:
      {
	multiCommunicator.setProtocol(SALOME::CORBA_);
	MESSAGE("PROTOCOL CHANGED TO CORBA");
	SALOME_CorbaDoubleSender * retc=new SALOME_CorbaDoubleSender(tab,lgr,ownTab);
	return retc->_this();
      }
//       throw MultiCommException("Communication protocol not implemented");
    }
}

SALOME::SenderInt_ptr SenderFactory::buildSender(SALOMEMultiComm &multiCommunicator,const int *tab,long lgr,bool ownTab)throw(MultiCommException){
  switch(multiCommunicator.getProtocol())
    {
    case SALOME::CORBA_:
      {
	SALOME_CorbaLongSender * retc=new SALOME_CorbaLongSender(tab,lgr,ownTab);
	return retc->_this();
      }
#ifdef HAVE_MPI2
    case SALOME::MPI_:
      {
	SALOME_MPISenderInt_i* retm=new SALOME_MPISenderInt_i(tab,lgr,ownTab);
	return retm->_this();
      }
#endif
#ifdef HAVE_SOCKET
    case SALOME::SOCKET_:
      {
	SALOME_SocketSenderInt_i* rets=new SALOME_SocketSenderInt_i(tab,lgr,ownTab);
	return rets->_this();
      }
#endif
    default:
      {
	multiCommunicator.setProtocol(SALOME::CORBA_);
	SALOME_CorbaLongSender * retc=new SALOME_CorbaLongSender(tab,lgr,ownTab);
	return retc->_this();
      }
//       throw MultiCommException("Communication protocol not implemented"); 
    }
  }

SALOME::SenderDouble_ptr SenderFactory::buildSender(SALOME::TypeOfCommunication NewType,SALOME_SenderDouble_i *src)
{
  SALOMEMultiComm mc(NewType);
  long n;
  const double *data=(const double *)src->getData(n);
  bool own=src->getOwnerShip();
  src->setOwnerShip(false);
  return buildSender(mc,data,n,own);
}

SALOME::SenderInt_ptr SenderFactory::buildSender(SALOME::TypeOfCommunication NewType,SALOME_SenderInt_i *src)
{
  SALOMEMultiComm mc(NewType);
  long n;
  const int *data=(const int *)src->getData(n);
  bool own=src->getOwnerShip();
  src->setOwnerShip(false);
  return buildSender(mc,data,n,own);
}
