#ifndef _SALOME_COMM_I_HXX_
#define _SALOME_COMM_I_HXX_

#include <string>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)
#ifdef HAVE_MPI2
#include "mpi.h"
#endif

#define TIMEOUT 20


/*!
  Generic servant class for senders that factorizes all the common methods and attributes necessary to senders.
  All servant classes for senders have to inheritate from it.
 */
class SALOME_Sender_i : public virtual POA_SALOME::Sender,
			public PortableServer::RefCountServantBase {
protected:
  /*! Pointer to the generic array to transmit*/
  const void *_tabToSend;
  /*! Length of the generic array to transmit*/
  long _lgrTabToSend;
  /*! it represents the sizeof() of each component of the generic array:\n
    Practically in terms of bytes the size to be transmitted is _lgrTabToSend*_sizeOf
  */
  int _sizeOf;
  /*! Type the component of the array*/
  SALOME::TypeOfDataTransmitted _type;

  SALOME_Sender_i(SALOME::TypeOfDataTransmitted type,const void *tabToSend,long lgrTabToSend,int sizeOf);
public:
  const void *getData(long &size) const;
  int getSizeOf() const;
  void release();
  SALOME::TypeOfDataTransmitted getTypeOfDataTransmitted();
  SALOME::Sender_ptr buildOtherWithProtocol(SALOME::TypeOfCommunication type);
  static SALOME_Sender_i *find(SALOME::Sender_ptr pCorba);
};

/*! Servant class for CORBA sender for double* when no copy of array _tabToSend is required, that is to say double and CORBA::Double are binary equal.
 */
class SALOME_CorbaDoubleNCSender_i : public POA_SALOME::CorbaDoubleNCSender,
				     public SALOME_Sender_i
{
public:
  SALOME_CorbaDoubleNCSender_i(const double *tabToSend,long lgrTabToSend);
  ~SALOME_CorbaDoubleNCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfDouble* sendPart(CORBA::ULong offset, CORBA::ULong length);
  SALOME::vectorOfDouble* send();
};

/*! Servant class for CORBA sender for double* when copy of array _tabToSend is required, that is to say double and CORBA::Double are NOT binary equal.
 */
class SALOME_CorbaDoubleCSender_i : public POA_SALOME::CorbaDoubleCSender,
				    public SALOME_Sender_i
{
public:
  SALOME_CorbaDoubleCSender_i(const double *tabToSend,long lgrTabToSend);
  ~SALOME_CorbaDoubleCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfDouble* sendPart(CORBA::ULong offset, CORBA::ULong length);
};

/*! Servant class for CORBA sender for int* when no copy of array _tabToSend is required, that is to say int and CORBA::Long are binary equal.
 */
class SALOME_CorbaLongNCSender_i : public POA_SALOME::CorbaLongNCSender,
				   public SALOME_Sender_i
{
public:
  SALOME_CorbaLongNCSender_i(const int *tabToSend,long lgrTabToSend);
  ~SALOME_CorbaLongNCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfLong* sendPart(CORBA::ULong offset, CORBA::ULong length);
  SALOME::vectorOfLong* send();
};

/*! Servant class for CORBA sender for int* when copy of array _tabToSend is required, that is to say int and CORBA::Long are NOT binary equal.
 */
class SALOME_CorbaLongCSender_i : public POA_SALOME::CorbaLongCSender,
				  public SALOME_Sender_i
{
public:
  SALOME_CorbaLongCSender_i(const int *tabToSend,long lgrTabToSend);
  ~SALOME_CorbaLongCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfLong* sendPart(CORBA::ULong offset, CORBA::ULong length);
  SALOME::CorbaLongCSender_ptr _this();
};

#ifdef HAVE_MPI2

/*! Servant class of sender using MPI2.
 */
class SALOME_MPISender_i : public POA_SALOME::MPISender,
			   public SALOME_Sender_i
{
private:
  static unsigned long _tag1;
  static unsigned long _tag2;
  /*! Name of the port opened*/
  char *_portName;
  int _cproc;
  /*! Tag 1 that identifies the transfert*/
  int _tag1Inst;
  /*! Tag 2 that identifies the transfert*/
  int _tag2Inst;
  /*! MPI communicator*/
  MPI_Comm _com;
  /*! Array of pointer for asynchronous invocation with omnithread*/
  void **_argsForThr;
  /*! Pointer to thread created on asynchronous invocation*/
  omni_thread *_newThr;
public:
  SALOME_MPISender_i(SALOME::TypeOfDataTransmitted type,const void *tabToSend,long lgrTabToSend,int sizeOf);
  ~SALOME_MPISender_i();
  SALOME::MPISender::param* getParam();
  void send();
  void close(const SALOME::MPISender::param& p);
private:
  static void* myThread(void *args);
};

#endif

#ifdef HAVE_SOCKET

/*! Servant class of sender using Sockets.
 */
class SALOME_SocketSender_i : public POA_SALOME::SocketSender,
			      public SALOME_Sender_i
{
private:
  int _serverSockfd;
  int _clientSockfd;
  int _port;
  std::string _IPAddress;
  void **_argsForThr;
  omni_thread *_newThr;
  bool _errorFlag;
public:
  SALOME_SocketSender_i(SALOME::TypeOfDataTransmitted type,const void *tabToSend,long lgrTabToSend,int sizeOf);
  ~SALOME_SocketSender_i();
  SALOME::SocketSender::param* getParam();
  void send();
  void initCom() throw(SALOME::SALOME_Exception);
  void acceptCom() throw(SALOME::SALOME_Exception);
  void endOfCom();
  void closeCom();
private:
  static void* myThread(void *args);
  std::string inetAddress();
};

#endif

#endif

