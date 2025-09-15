// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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
double *ReceiverFactory::getValue(SALOME_CMOD::SenderDouble_ptr sender,long &size)
{
  double *ret;
  try{
    ret=getValueOneShot(sender,size);
  }
  catch(MultiCommException&)
    {
      SALOME_CMOD::SenderDouble_ptr newSender=sender->buildOtherWithProtocol(SALOME_CMOD::CORBA_);
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
int *ReceiverFactory::getValue(SALOME_CMOD::SenderInt_ptr sender,long &size)
{
  int *ret;
  try{
    ret=getValueOneShot(sender,size);
  }
  catch(MultiCommException&)
    {
      SALOME_CMOD::SenderInt_ptr newSender=sender->buildOtherWithProtocol(SALOME_CMOD::CORBA_);
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
double *ReceiverFactory::getValueOneShot(SALOME_CMOD::SenderDouble_ptr sender,long &size)
{
  SALOME_CMOD::CorbaDoubleNCSender_ptr cncD_ptr;
  SALOME_CMOD::CorbaDoubleCSender_ptr cwcD_ptr;
#ifdef HAVE_MPI2
  SALOME_CMOD::MPISenderDouble_ptr mpi_ptr=SALOME_CMOD::MPISenderDouble::_narrow(sender);
#endif
#ifdef HAVE_SOCKET
  SALOME_CMOD::SocketSenderDouble_ptr sock_ptr=SALOME_CMOD::SocketSenderDouble::_narrow(sender);
#endif
  cncD_ptr=SALOME_CMOD::CorbaDoubleNCSender::_narrow(sender);
  cwcD_ptr=SALOME_CMOD::CorbaDoubleCSender::_narrow(sender);
  if(!CORBA::is_nil(cncD_ptr))
    {
      CORBA::release(sender);
      CorbaDNoCopyReceiver<double,CORBA::Double,SALOME_CMOD::vectorOfDouble_var,SALOME_CMOD::CorbaDoubleNCSender_ptr,SALOME_CMOD::SenderDouble_ptr,SALOME_SenderDouble_i> rec(cncD_ptr);
      return rec.getValue(size);
    }
  else if(!CORBA::is_nil(cwcD_ptr))
    {
      CORBA::release(sender);
      CorbaDWithCopyReceiver<double,CORBA::Double,SALOME_CMOD::vectorOfDouble_var,SALOME_CMOD::CorbaDoubleCSender_ptr,SALOME_CMOD::SenderDouble_ptr,SALOME_SenderDouble_i> rec(cwcD_ptr);
      return rec.getValue(size);
    }
#ifdef HAVE_MPI2
  else if(!CORBA::is_nil(mpi_ptr))
    {
      CORBA::release(sender);
      MPIReceiver<double,SALOME_CMOD::MPISenderDouble_ptr,SALOME_CMOD::SenderDouble_ptr,SALOME_SenderDouble_i> rec(mpi_ptr);
      return rec.getValue(size);
    }
#endif
#ifdef HAVE_SOCKET
  else if(!CORBA::is_nil(sock_ptr))
    {
      CORBA::release(sender);
      SocketReceiver<double,xdr_double,SALOME_CMOD::SocketSenderDouble_ptr,SALOME_CMOD::SenderDouble_ptr,SALOME_SenderDouble_i> rec(sock_ptr);
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
int *ReceiverFactory::getValueOneShot(SALOME_CMOD::SenderInt_ptr sender,long &size)
{
  SALOME_CMOD::CorbaLongNCSender_ptr cncL_ptr;
  SALOME_CMOD::CorbaLongCSender_ptr cwcL_ptr;
#ifdef HAVE_MPI2
  SALOME_CMOD::MPISenderInt_ptr mpi_ptr=SALOME_CMOD::MPISenderInt::_narrow(sender);
#endif
#ifdef HAVE_SOCKET
  SALOME_CMOD::SocketSenderInt_ptr sock_ptr=SALOME_CMOD::SocketSenderInt::_narrow(sender);
#endif
  cncL_ptr=SALOME_CMOD::CorbaLongNCSender::_narrow(sender);
  cwcL_ptr=SALOME_CMOD::CorbaLongCSender::_narrow(sender);
  if(!CORBA::is_nil(cncL_ptr))
    {
      CORBA::release(sender);
      CorbaINoCopyReceiver<int,CORBA::Long,SALOME_CMOD::vectorOfLong_var,SALOME_CMOD::CorbaLongNCSender_ptr,SALOME_CMOD::SenderInt_ptr,SALOME_SenderInt_i> rec(cncL_ptr);
      return rec.getValue(size);
    }
  else if(!CORBA::is_nil(cwcL_ptr))
    {
      CORBA::release(sender);
      CorbaIWithCopyReceiver<int,CORBA::Long,SALOME_CMOD::vectorOfLong_var,SALOME_CMOD::CorbaLongCSender_ptr,SALOME_CMOD::SenderInt_ptr,SALOME_SenderInt_i> rec(cwcL_ptr);
      return rec.getValue(size);
    }
#ifdef HAVE_MPI2
  else if(!CORBA::is_nil(mpi_ptr))
    {
      CORBA::release(sender);
      MPIReceiver<int,SALOME_CMOD::MPISenderInt_ptr,SALOME_CMOD::SenderInt_ptr,SALOME_SenderInt_i> rec(mpi_ptr);
      return rec.getValue(size);
    }
#endif
#ifdef HAVE_SOCKET
  else if(!CORBA::is_nil(sock_ptr))
    {
      CORBA::release(sender);
      SocketReceiver<int,xdr_int,SALOME_CMOD::SocketSenderInt_ptr,SALOME_CMOD::SenderInt_ptr,SALOME_SenderInt_i> rec(sock_ptr);
      return rec.getValue(size);
    }
#endif
  else
    {
      throw MultiCommException("Unknown sender protocol");
      return 0;
    }
}

SeqByteReceiver::SeqByteReceiver(SALOME_CMOD::SenderByte_ptr sender):_obj(SALOME_CMOD::SenderByte::_duplicate(sender))
{
}

char *SeqByteReceiver::data(unsigned long& size)
{
  size = _obj->getSize();
  if(size <= CHUNK_SIZE)
  {
    this->fetchOneShot( size );
    return reinterpret_cast<char *>(_data_one_shot->get_buffer());
  }
  else
  {
    this->fetchByChunks( size );
    return _data_for_split_case.get();
  }
}

void SeqByteReceiver::fetchOneShot(unsigned long size)
{
  _data_one_shot.reset( _obj->sendPart(0,size) );
}

void SeqByteReceiver::fetchByChunks(unsigned long size)
{
  _data_for_split_case.reset( new char[size] );
  char *destination = _data_for_split_case.get();
  constexpr unsigned long EFF_CHUNK_SIZE = CHUNK_SIZE / 8;
  unsigned long iStart = 0;
  unsigned long iEnd = EFF_CHUNK_SIZE;
  while( iStart!=iEnd && iEnd <= size )
  {
    std::unique_ptr<SALOME_CMOD::vectorOfByte> part( _obj->sendPart(iStart,iEnd) );
    const unsigned char *partC = part->get_buffer();
    std::copy(partC,partC+(iEnd-iStart),destination+iStart);
    iStart = iEnd; iEnd = std::min(iStart + EFF_CHUNK_SIZE,size);
  }
}

SeqByteReceiver::~SeqByteReceiver()
{
  _obj->UnRegister();
}
