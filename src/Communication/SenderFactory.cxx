#include "SenderFactory.hxx"
#include "utilities.h"
#include "SALOMEMultiComm.hxx"
#include "SALOME_Comm_i.hxx"

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

SALOME::Sender_ptr SenderFactory::buildSender(SALOMEMultiComm &multiCommunicator,const double *tab,long lgr)throw(MultiCommException){
  switch(multiCommunicator.getProtocol())
    {
    case SALOME::CORBA_:
      {
	SALOME_CorbaDoubleSender * retc=new SALOME_CorbaDoubleSender(tab,lgr);
	return retc->_this();
      }
#ifdef HAVE_MPI2
    case SALOME::MPI_:
      {
	SALOME_MPISender_i* retm=new SALOME_MPISender_i(SALOME::DOUBLE_,tab,lgr,sizeof(double));
	return retm->_this();
      }
#endif
#ifdef HAVE_SOCKET
    case SALOME::SOCKET_:
      {
	SALOME_SocketSender_i* rets=new SALOME_SocketSender_i(SALOME::DOUBLE_,tab,lgr,sizeof(double));
	return rets->_this();
      }
#endif
    default:
      {
	multiCommunicator.setProtocol(SALOME::CORBA_);
	MESSAGE("PROTOCOL CHANGED TO CORBA");
	SALOME_CorbaDoubleSender * retc=new SALOME_CorbaDoubleSender(tab,lgr);
	return retc->_this();
      }
//       throw MultiCommException("Communication protocol not implemented");
    }
}

SALOME::Sender_ptr SenderFactory::buildSender(SALOMEMultiComm &multiCommunicator,const int *tab,long lgr)throw(MultiCommException){
  switch(multiCommunicator.getProtocol())
    {
    case SALOME::CORBA_:
      {
	SALOME_CorbaLongSender * retc=new SALOME_CorbaLongSender(tab,lgr);
	return retc->_this();
      }
#ifdef HAVE_MPI2
    case SALOME::MPI_:
      {
	SALOME_MPISender_i* retm=new SALOME_MPISender_i(SALOME::INT_,tab,lgr,sizeof(int));
	return retm->_this();
      }
#endif
#ifdef HAVE_SOCKET
    case SALOME::SOCKET_:
      {
	SALOME_SocketSender_i* rets=new SALOME_SocketSender_i(SALOME::INT_,tab,lgr,sizeof(int));
	return rets->_this();
      }
#endif
    default:
      {
	multiCommunicator.setProtocol(SALOME::CORBA_);
	SALOME_CorbaLongSender * retc=new SALOME_CorbaLongSender(tab,lgr);
	return retc->_this();
      }
//       throw MultiCommException("Communication protocol not implemented"); 
    }
  }

SALOME::Sender_ptr SenderFactory::buildSender(SALOME::TypeOfCommunication NewType,SALOME_Sender_i *src)
{
  SALOMEMultiComm mc(NewType);
  long n;
  const void *data=src->getData(n);
  switch(src->getTypeOfDataTransmitted())
    {
    case SALOME::DOUBLE_:
      return buildSender(mc,(const double *)data,n);
    case SALOME::INT_:
      return buildSender(mc,(const int *)data,n);
    }
  
}


