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
template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
class CorbaNCNoCopyReceiver : public Receiver<T,servForT,ptrForT>
{
private:
  CorbaSender _mySender;
public:
  CorbaNCNoCopyReceiver(CorbaSender mySender);
  ~CorbaNCNoCopyReceiver();
  T *getValue(long &size);
private:
  T *getDistValue(long &size);
};

/*!
  Receiver used for transfert with CORBA when copy is not required remotely but required locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
class CorbaNCWithCopyReceiver : public Receiver<T,servForT,ptrForT>
{
private:
  CorbaSender _mySender;
public:
  CorbaNCWithCopyReceiver(CorbaSender mySender);
  ~CorbaNCWithCopyReceiver();
  T *getValue(long &size);
private:
  T *getDistValue(long &size);
};

/*!
  Receiver used for transfert with CORBA when copy is required remotely but not required locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
class CorbaWCNoCopyReceiver : public Receiver<T,servForT,ptrForT>
{
private:
  CorbaSender  _mySender;
public:
  CorbaWCNoCopyReceiver(CorbaSender mySender);
  ~CorbaWCNoCopyReceiver();
  T *getValue(long &size);
private:
  T *getDistValue(long &size);
};

/*!
  Receiver used for transfert with CORBA when copy is required both remotely and locally.
 */
template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
class CorbaWCWithCopyReceiver : public Receiver<T,servForT,ptrForT>
{
private:
  CorbaSender _mySender;
public:
  CorbaWCWithCopyReceiver(CorbaSender mySender);
  ~CorbaWCWithCopyReceiver();
  T *getValue(long &size);
private:
  T *getDistValue(long &size);
};

#ifdef HAVE_MPI2
/*!
  Receiver for MPI transfert.
 */
template<class T,MPI_Datatype T2,class CorbaSender,class servForT,class ptrForT>
class MPIReceiver : public Receiver<T,servForT,ptrForT>
{
private:
  CorbaSender _mySender;
public:
  MPIReceiver(CorbaSender mySender);
  ~MPIReceiver();
  T *getValue(long &size);
private:
  T *getDistValue(long &size);
};
#endif

#ifdef HAVE_SOCKET

class XDR;

/*!
  Receiver for transfert with sockets.
 */
template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
class SocketReceiver : public Receiver<T,servForT,ptrForT>
{
private:
  CorbaSender _mySender;
  int _clientSockfd;
  bool _senderDestruc;
public:
  SocketReceiver(CorbaSender mySender);
  ~SocketReceiver();
  T *getValue(long &size);
private:
  T *getDistValue(long &size);
  void initCom();
  void connectCom(const char *, int);
  void closeCom();
};
#endif

#include "Receivers.cxx"

#endif

