#ifndef _RECEIVERS_HXX_
#define _RECEIVERS_HXX_

#include "SALOME_Comm_i.hxx"
#include "Receiver.hxx"
#ifdef HAVE_MPI2
#include "mpi.h"
#endif

/*!
  Receiver used for transfert with CORBA when no copy is required remotely and locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender>
class CorbaNCNoCopyReceiver : public Receiver
{
private:
  CorbaSender _mySender;
public:
  CorbaNCNoCopyReceiver(CorbaSender mySender);
  ~CorbaNCNoCopyReceiver();
  void *getValue(long &size);
private:
  void *getDistValue(long &size);
};

/*!
  Receiver used for transfert with CORBA when copy is not required remotely but required locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender>
class CorbaNCWithCopyReceiver : public Receiver
{
private:
  CorbaSender _mySender;
public:
  CorbaNCWithCopyReceiver(CorbaSender mySender);
  ~CorbaNCWithCopyReceiver();
  void *getValue(long &size);
private:
  void *getDistValue(long &size);
};

/*!
  Receiver used for transfert with CORBA when copy is required remotely but not required locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender>
class CorbaWCNoCopyReceiver : public Receiver
{
private:
  CorbaSender  _mySender;
public:
  CorbaWCNoCopyReceiver(CorbaSender mySender);
  ~CorbaWCNoCopyReceiver();
  void *getValue(long &size);
private:
  void *getDistValue(long &size);
};

/*!
  Receiver used for transfert with CORBA when copy is required both remotely and locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender>
class CorbaWCWithCopyReceiver : public Receiver
{
private:
  CorbaSender _mySender;
public:
  CorbaWCWithCopyReceiver(CorbaSender mySender);
  ~CorbaWCWithCopyReceiver();
  void *getValue(long &size);
private:
  void *getDistValue(long &size);
};

#ifdef HAVE_MPI2
/*!
  Receiver for MPI transfert.
 */
template<class T,MPI_Datatype T2>
class MPIReceiver : public Receiver
{
private:
  SALOME::MPISender_ptr _mySender;
public:
  MPIReceiver(SALOME::MPISender_ptr mySender);
  ~MPIReceiver();
  void *getValue(long &size);
private:
  void *getDistValue(long &size);
};
#endif

#ifdef HAVE_SOCKET

class XDR;

/*!
  Receiver for transfert with sockets.
 */
template<class T,int (*myFunc)(XDR*,T*)>
class SocketReceiver : public Receiver
{
private:
  SALOME::SocketSender_ptr _mySender;
  int _clientSockfd;
  bool _senderDestruc;
public:
  SocketReceiver(SALOME::SocketSender_ptr mySender);
  ~SocketReceiver();
  void *getValue(long &size);
private:
  void *getDistValue(long &size);
  void initCom();
  void connectCom(const char *, int);
  void closeCom();
};
#endif

#include "Receivers.cxx"

#endif

