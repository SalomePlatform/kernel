#include "ReceiverFactory.hxx"
#include "Receivers.hxx"

#ifdef COMP_CORBA_DOUBLE
#define CorbaDNoCopyReceiver CorbaNCNoCopyReceiver
#define CorbaDWithCopyReceiver CorbaNCWithCopyReceiver
#else
#define CorbaDNoCopyReceiver CorbaWCNoCopyReceiver
#define CorbaDWithCopyReceiver CorbaWCWithCopyReceiver
#endif

#ifdef COMP_CORBA_LONG
#define CorbaINoCopyReceiver CorbaNCNoCopyReceiver
#define CorbaIWithCopyReceiver CorbaNCWithCopyReceiver
#else
#define CorbaINoCopyReceiver CorbaWCNoCopyReceiver
#define CorbaIWithCopyReceiver CorbaWCWithCopyReceiver
#endif

#ifdef HAVE_SOCKET
#include <rpc/xdr.h>
#endif

/*!
  This method performs the transfert with the remote sender given. If it fails with this sender it tries with an another protocol (CORBA by default).
 */
void *ReceiverFactory::getValue(SALOME::Sender_ptr sender,long &size)throw(MultiCommException)
{
  void *ret;
  try{
    ret=getValueOneShot(sender,size);
  }
  catch(MultiCommException&)
    {
      SALOME::Sender_ptr newSender=sender->buildOtherWithProtocol(SALOME::CORBA_);
      MESSAGE("PROTOCOL CHANGED TO CORBA");
      sender->release();
      CORBA::release(sender);
      ret=getValueOneShot(newSender,size);
    }
  return ret;
}

/*!
  This method performs the transfert with the remote sender given. If it fails an exception is thrown.
 */
void *ReceiverFactory::getValueOneShot(SALOME::Sender_ptr sender,long &size)throw(MultiCommException)
{
  SALOME::CorbaDoubleNCSender_ptr cncD_ptr;
  SALOME::CorbaDoubleCSender_ptr cwcD_ptr;
  SALOME::CorbaLongNCSender_ptr cncL_ptr;
  SALOME::CorbaLongCSender_ptr cwcL_ptr;
#ifdef HAVE_MPI2
  SALOME::MPISender_ptr mpi_ptr=SALOME::MPISender::_narrow(sender);
#endif
#ifdef HAVE_SOCKET
  SALOME::SocketSender_ptr sock_ptr=SALOME::SocketSender::_narrow(sender);
#endif
 switch(sender->getTypeOfDataTransmitted())
    {
    case SALOME::DOUBLE_:
      cncD_ptr=SALOME::CorbaDoubleNCSender::_narrow(sender);
      cwcD_ptr=SALOME::CorbaDoubleCSender::_narrow(sender);
      if(!CORBA::is_nil(cncD_ptr))
	{
	  CORBA::release(sender);
	  CorbaDNoCopyReceiver<double,CORBA::Double,SALOME::vectorOfDouble_var,SALOME::CorbaDoubleNCSender_ptr> rec(cncD_ptr);
	  return rec.getValue(size);
	}
      else if(!CORBA::is_nil(cwcD_ptr))
	{
	  CORBA::release(sender);
	  CorbaDWithCopyReceiver<double,CORBA::Double,SALOME::vectorOfDouble_var,SALOME::CorbaDoubleCSender_ptr> rec(cwcD_ptr);
	  return rec.getValue(size);
	}
#ifdef HAVE_MPI2
      else if(!CORBA::is_nil(mpi_ptr))
	{
	  CORBA::release(sender);
	  MPIReceiver<double,MPI_DOUBLE> rec(mpi_ptr);
	  return rec.getValue(size);
	}
#endif
#ifdef HAVE_SOCKET
      else if(!CORBA::is_nil(sock_ptr))
	{
	  CORBA::release(sender);
	  SocketReceiver<double,xdr_double> rec(sock_ptr);
	  return rec.getValue(size);
	}
#endif
      else
	{
	  throw MultiCommException("Unknown sender protocol");
	  return 0;
	}
    case SALOME::INT_:
      cncL_ptr=SALOME::CorbaLongNCSender::_narrow(sender);
      cwcL_ptr=SALOME::CorbaLongCSender::_narrow(sender);
      if(!CORBA::is_nil(cncL_ptr))
	{
	  CORBA::release(sender);
	  CorbaINoCopyReceiver<int,CORBA::Long,SALOME::vectorOfLong_var,SALOME::CorbaLongNCSender_ptr> rec(cncL_ptr);
	  return rec.getValue(size);
	}
      else if(!CORBA::is_nil(cwcL_ptr))
	{
	  CORBA::release(sender);
	  CorbaIWithCopyReceiver<int,CORBA::Long,SALOME::vectorOfLong_var,SALOME::CorbaLongCSender_ptr> rec(cwcL_ptr);
	  return rec.getValue(size);
	}
#ifdef HAVE_MPI2
      else if(!CORBA::is_nil(mpi_ptr))
	{
	  CORBA::release(sender);
	  MPIReceiver<int,MPI_INT> rec(mpi_ptr);
	  return rec.getValue(size);
	}
#endif
#ifdef HAVE_SOCKET
      else if(!CORBA::is_nil(sock_ptr))
	{
	  CORBA::release(sender);
	  SocketReceiver<int,xdr_int> rec(sock_ptr);
	  return rec.getValue(size);
	}
#endif
      else
	{
	  throw MultiCommException("Unknown sender protocol");
	  return 0;
	}
    default:
      throw MultiCommException("unknown type of data transfered");
      return 0;
    }
}

