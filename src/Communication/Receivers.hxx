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

#ifndef _RECEIVERS_HXX_
#define _RECEIVERS_HXX_

#ifdef HAVE_MPI2
#include "mpi.h"
#endif
#include "SALOME_Comm_i.hxx"
#include "Receiver.hxx"

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
template<class T>
struct MPITRAITS
{
  static MPI_Datatype MpiType;
};

template<>
MPI_Datatype MPITRAITS<double>::MpiType=MPI_DOUBLE;

template<>
MPI_Datatype MPITRAITS<int>::MpiType=MPI_INT;

template<class T,class CorbaSender,class servForT,class ptrForT>
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

