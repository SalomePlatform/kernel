// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef _SALOME_COMM_I_HXX_
#define _SALOME_COMM_I_HXX_

#include "SALOME_Communication.hxx"

#ifdef HAVE_MPI2
#include "mpi.h"
#endif
#include <string>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

#define TIMEOUT 20


/*!
  Generic servant class for senders that factorizes all the common methods and attributes necessary to senders.
  All servant classes for senders have to inheritate from it.
 */
class COMMUNICATION_EXPORT SALOME_Sender_i : public virtual POA_SALOME::Sender
{
protected:
  /*! Pointer to the generic array to transmit*/
  const void *_tabToSend;
  /*! Length of the generic array to transmit*/
  long _lgrTabToSend;
  /*! it represents the sizeof() of each component of the generic array:\n
    Practically in terms of bytes the size to be transmitted is _lgrTabToSend*_sizeOf
  */
  int _sizeOf;
  /*! Indicates if _tabToSend has to be deallocated */ 
  bool _ownTabToSend;

  SALOME_Sender_i(const void *tabToSend,long lgrTabToSend,int sizeOf,bool ownTabToSend=false);
public:
  const void *getData(long &size) const;
  int getSizeOf() const;
  void setOwnerShip(bool own);
  bool getOwnerShip() const { return _ownTabToSend; }
  void release();
  virtual ~SALOME_Sender_i() {}
};

class COMMUNICATION_EXPORT SALOME_SenderDouble_i : public virtual POA_SALOME::SenderDouble,
                              public virtual SALOME_Sender_i
{
public:
  SALOME_SenderDouble_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
  SALOME::TypeOfDataTransmitted getTypeOfDataTransmitted() { return SALOME::DOUBLE_; }
  SALOME::SenderDouble_ptr buildOtherWithProtocol(SALOME::TypeOfCommunication type);
  virtual ~SALOME_SenderDouble_i();
  static SALOME_SenderDouble_i *find(SALOME::SenderDouble_ptr pCorba);
};

class COMMUNICATION_EXPORT SALOME_SenderInt_i : public virtual POA_SALOME::SenderInt,
                           public virtual SALOME_Sender_i
{
public:
  SALOME_SenderInt_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
  SALOME::TypeOfDataTransmitted getTypeOfDataTransmitted() { return SALOME::INT_; }
  SALOME::SenderInt_ptr buildOtherWithProtocol(SALOME::TypeOfCommunication type);
  virtual ~SALOME_SenderInt_i();
  static SALOME_SenderInt_i *find(SALOME::SenderInt_ptr pCorba);
};

/*! Servant class for CORBA sender for double* when no copy of array _tabToSend is required, that is to say double and CORBA::Double are binary equal.
 */
class COMMUNICATION_EXPORT SALOME_CorbaDoubleNCSender_i : public POA_SALOME::CorbaDoubleNCSender,
                                     public SALOME_SenderDouble_i
{
public:
  SALOME_CorbaDoubleNCSender_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
  ~SALOME_CorbaDoubleNCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfDouble* sendPart(CORBA::ULong offset, CORBA::ULong length);
  SALOME::vectorOfDouble* send();
};

/*! Servant class for CORBA sender for double* when copy of array _tabToSend is required, that is to say double and CORBA::Double are NOT binary equal.
 */
class COMMUNICATION_EXPORT SALOME_CorbaDoubleCSender_i : public POA_SALOME::CorbaDoubleCSender,
                                    public SALOME_SenderDouble_i
{
public:
  SALOME_CorbaDoubleCSender_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
  ~SALOME_CorbaDoubleCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfDouble* sendPart(CORBA::ULong offset, CORBA::ULong length);
};

/*! Servant class for CORBA sender for int* when no copy of array _tabToSend is required, that is to say int and CORBA::Long are binary equal.
 */
class COMMUNICATION_EXPORT SALOME_CorbaLongNCSender_i : public POA_SALOME::CorbaLongNCSender,
                                   public SALOME_SenderInt_i
{
public:
  SALOME_CorbaLongNCSender_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
  ~SALOME_CorbaLongNCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfLong* sendPart(CORBA::ULong offset, CORBA::ULong length);
  SALOME::vectorOfLong* send();
};

/*! Servant class for CORBA sender for int* when copy of array _tabToSend is required, that is to say int and CORBA::Long are NOT binary equal.
 */
class COMMUNICATION_EXPORT SALOME_CorbaLongCSender_i : public POA_SALOME::CorbaLongCSender,
                                  public SALOME_SenderInt_i
{
public:
  SALOME_CorbaLongCSender_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
  ~SALOME_CorbaLongCSender_i();
  CORBA::ULong getSize();
  SALOME::vectorOfLong* sendPart(CORBA::ULong offset, CORBA::ULong length);
};

#ifdef HAVE_MPI2

/*! Servant class of sender using MPI2.
 */
class COMMUNICATION_EXPORT SALOME_MPISender_i : public virtual POA_SALOME::MPISender,
                           public virtual SALOME_Sender_i
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
  /*! Type the component of the array*/
  SALOME::TypeOfDataTransmitted _type;
public:
  SALOME_MPISender_i(const void *tabToSend,long lgrTabToSend,int sizeOf,bool ownTabToSend=false);
  ~SALOME_MPISender_i();
  SALOME::MPISender::param* getParam();
  void send();
  void close(const SALOME::MPISender::param& p);
private:
  static void* myThread(void *args);
};

class COMMUNICATION_EXPORT SALOME_MPISenderDouble_i : public POA_SALOME::MPISenderDouble,
                                 public SALOME_SenderDouble_i,
                                 public SALOME_MPISender_i
{
public:
  SALOME_MPISenderDouble_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
};

class COMMUNICATION_EXPORT SALOME_MPISenderInt_i : public POA_SALOME::MPISenderInt,
                              public SALOME_SenderInt_i,
                              public SALOME_MPISender_i
{
public:
  SALOME_MPISenderInt_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
};

#endif

#ifdef HAVE_SOCKET

/*! Servant class of sender using Sockets.
 */
class COMMUNICATION_EXPORT SALOME_SocketSender_i : public virtual POA_SALOME::SocketSender,
                              public virtual SALOME_Sender_i
{
private:
  int _serverSockfd;
  int _clientSockfd;
  int _port;
  std::string _IPAddress;
  void **_argsForThr;
  omni_thread *_newThr;
  bool _errorFlag;
  /*! Type the component of the array*/
  SALOME::TypeOfDataTransmitted _type;
public:
  SALOME_SocketSender_i(const void *tabToSend,long lgrTabToSend,int sizeOf,bool ownTabToSend=false);
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

class COMMUNICATION_EXPORT SALOME_SocketSenderDouble_i : public POA_SALOME::SocketSenderDouble,
                                    public SALOME_SenderDouble_i,
                                    public SALOME_SocketSender_i
{
public:
  SALOME_SocketSenderDouble_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
};

class COMMUNICATION_EXPORT SALOME_SocketSenderInt_i : public POA_SALOME::SocketSenderInt,
                                 public SALOME_SenderInt_i,
                                 public SALOME_SocketSender_i
{
public:
  SALOME_SocketSenderInt_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend=false);
};

#endif

#endif

