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

#ifdef HAVE_MPI2
#include "mpi.h"
#endif
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
  This method performs the transfert of double array with the remote SenderDouble given. If it fails with this SenderDouble it tries with an another protocol (CORBA by default).
 */
double *ReceiverFactory::getValue(SALOME::SenderDouble_ptr sender,long &size)throw(MultiCommException)
{
  double *ret;
  try{
    ret=getValueOneShot(sender,size);
  }
  catch(MultiCommException&)
    {
      SALOME::SenderDouble_ptr newSender=sender->buildOtherWithProtocol(SALOME::CORBA_);
      MESSAGE("PROTOCOL CHANGED TO CORBA");
      sender->release();
      ret=getValueOneShot(newSender,size);
      CORBA::release(newSender);
    }
  return ret;
}

/*!
  This method performs the transfert of int array with the remote SenderInt given. If it fails with this SenderInt it tries with an another protocol (CORBA by default).
 */
int *ReceiverFactory::getValue(SALOME::SenderInt_ptr sender,long &size)throw(MultiCommException)
{
  int *ret;
  try{
    ret=getValueOneShot(sender,size);
  }
  catch(MultiCommException&)
    {
      SALOME::SenderInt_ptr newSender=sender->buildOtherWithProtocol(SALOME::CORBA_);
      MESSAGE("PROTOCOL CHANGED TO CORBA");
      sender->release();
      ret=getValueOneShot(newSender,size);
      CORBA::release(newSender);
    }
  return ret;
}

/*!
  This method performs the transfert with the remote SenderDouble given. If it fails an exception is thrown.
 */
double *ReceiverFactory::getValueOneShot(SALOME::SenderDouble_ptr sender,long &size)throw(MultiCommException)
{
  SALOME::CorbaDoubleNCSender_ptr cncD_ptr;
  SALOME::CorbaDoubleCSender_ptr cwcD_ptr;
#ifdef HAVE_MPI2
  SALOME::MPISenderDouble_ptr mpi_ptr=SALOME::MPISenderDouble::_narrow(sender);
#endif
#ifdef HAVE_SOCKET
  SALOME::SocketSenderDouble_ptr sock_ptr=SALOME::SocketSenderDouble::_narrow(sender);
#endif
  cncD_ptr=SALOME::CorbaDoubleNCSender::_narrow(sender);
  cwcD_ptr=SALOME::CorbaDoubleCSender::_narrow(sender);
  if(!CORBA::is_nil(cncD_ptr))
    {
      CORBA::release(sender);
      CorbaDNoCopyReceiver<double,CORBA::Double,SALOME::vectorOfDouble_var,SALOME::CorbaDoubleNCSender_ptr,SALOME::SenderDouble_ptr,SALOME_SenderDouble_i> rec(cncD_ptr);
      return rec.getValue(size);
    }
  else if(!CORBA::is_nil(cwcD_ptr))
    {
      CORBA::release(sender);
      CorbaDWithCopyReceiver<double,CORBA::Double,SALOME::vectorOfDouble_var,SALOME::CorbaDoubleCSender_ptr,SALOME::SenderDouble_ptr,SALOME_SenderDouble_i> rec(cwcD_ptr);
      return rec.getValue(size);
    }
#ifdef HAVE_MPI2
  else if(!CORBA::is_nil(mpi_ptr))
    {
      CORBA::release(sender);
      MPIReceiver<double,SALOME::MPISenderDouble_ptr,SALOME::SenderDouble_ptr,SALOME_SenderDouble_i> rec(mpi_ptr);
      return rec.getValue(size);
    }
#endif
#ifdef HAVE_SOCKET
  else if(!CORBA::is_nil(sock_ptr))
    {
      CORBA::release(sender);
      SocketReceiver<double,xdr_double,SALOME::SocketSenderDouble_ptr,SALOME::SenderDouble_ptr,SALOME_SenderDouble_i> rec(sock_ptr);
      return rec.getValue(size);
    }
#endif
  else
    {
      throw MultiCommException("Unknown sender protocol");
      return 0;
    }
}

/*!
  This method performs the transfert with the remote SenderInt given. If it fails an exception is thrown.
 */
int *ReceiverFactory::getValueOneShot(SALOME::SenderInt_ptr sender,long &size)throw(MultiCommException)
{
  SALOME::CorbaLongNCSender_ptr cncL_ptr;
  SALOME::CorbaLongCSender_ptr cwcL_ptr;
#ifdef HAVE_MPI2
  SALOME::MPISenderInt_ptr mpi_ptr=SALOME::MPISenderInt::_narrow(sender);
#endif
#ifdef HAVE_SOCKET
  SALOME::SocketSenderInt_ptr sock_ptr=SALOME::SocketSenderInt::_narrow(sender);
#endif
  cncL_ptr=SALOME::CorbaLongNCSender::_narrow(sender);
  cwcL_ptr=SALOME::CorbaLongCSender::_narrow(sender);
  if(!CORBA::is_nil(cncL_ptr))
    {
      CORBA::release(sender);
      CorbaINoCopyReceiver<int,CORBA::Long,SALOME::vectorOfLong_var,SALOME::CorbaLongNCSender_ptr,SALOME::SenderInt_ptr,SALOME_SenderInt_i> rec(cncL_ptr);
      return rec.getValue(size);
    }
  else if(!CORBA::is_nil(cwcL_ptr))
    {
      CORBA::release(sender);
      CorbaIWithCopyReceiver<int,CORBA::Long,SALOME::vectorOfLong_var,SALOME::CorbaLongCSender_ptr,SALOME::SenderInt_ptr,SALOME_SenderInt_i> rec(cwcL_ptr);
      return rec.getValue(size);
    }
#ifdef HAVE_MPI2
  else if(!CORBA::is_nil(mpi_ptr))
    {
      CORBA::release(sender);
      MPIReceiver<int,SALOME::MPISenderInt_ptr,SALOME::SenderInt_ptr,SALOME_SenderInt_i> rec(mpi_ptr);
      return rec.getValue(size);
    }
#endif
#ifdef HAVE_SOCKET
  else if(!CORBA::is_nil(sock_ptr))
    {
      CORBA::release(sender);
      SocketReceiver<int,xdr_int,SALOME::SocketSenderInt_ptr,SALOME::SenderInt_ptr,SALOME_SenderInt_i> rec(sock_ptr);
      return rec.getValue(size);
    }
#endif
  else
    {
      throw MultiCommException("Unknown sender protocol");
      return 0;
    }
}

