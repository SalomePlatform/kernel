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

#include "omniORB4/poa.h"
#include "utilities.h"

#define TAILLE_SPLIT 100000
#define TIMEOUT 20

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaNCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::CorbaNCNoCopyReceiver(CorbaSender mySender):_mySender(mySender){
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaNCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::~CorbaNCNoCopyReceiver(){
  _mySender->release();
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaNCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getDistValue(long &size)
{
  TSeqCorba seq=_mySender->send();
  size=seq->length();
  return (T *)seq->get_buffer(1);
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaNCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getValue(long &size)
{
  return Receiver<T,servForT,ptrForT>::getValue(size,_mySender);
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaNCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::CorbaNCWithCopyReceiver(CorbaSender mySender):_mySender(mySender){
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaNCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::~CorbaNCWithCopyReceiver(){
  _mySender->release();
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaNCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getDistValue(long &size){
  size=_mySender->getSize();
  long n;
  T *ret=new T[size];
  T *iter=ret;
  for(long i=0;i<size;i+=TAILLE_SPLIT)
    {
      if(size-i>TAILLE_SPLIT)
        n=TAILLE_SPLIT;
      else
        n=size-i;
      TSeqCorba seq=_mySender->sendPart(i,n);
      T *seqd=(T *)seq->get_buffer(0);
      for(long j=0;j<n;j++)
        *iter++=*seqd++;
    }
  return ret;
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaNCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getValue(long &size)
{
  return Receiver<T,servForT,ptrForT>::getValue(size,_mySender);
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaWCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::CorbaWCNoCopyReceiver(CorbaSender mySender):_mySender(mySender){
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaWCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::~CorbaWCNoCopyReceiver(){
  _mySender->release();
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaWCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getDistValue(long &size){
  size=_mySender->getSize();
  long n;
  T *ret=new T[size];
  T *iter=ret;
  for(long i=0;i<size;i+=TAILLE_SPLIT)
    {
      if(size-i>TAILLE_SPLIT)
        n=TAILLE_SPLIT;
      else
        n=size-i;
      TSeqCorba seq=_mySender->sendPart(i,n);
      TCorba *seqd=seq->get_buffer(0);
      for(long j=0;j<n;j++)
        *iter++=*seqd++;
    }
  return ret;
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaWCNoCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getValue(long &size)
{
  return Receiver<T,servForT,ptrForT>::getValue(size,_mySender);
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaWCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::CorbaWCWithCopyReceiver(CorbaSender mySender):_mySender(mySender){
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
CorbaWCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::~CorbaWCWithCopyReceiver(){
  _mySender->release();
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaWCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getDistValue(long &size){
  size=_mySender->getSize();
  long n;
  T *ret=new T[size];
  T *iter=ret;
  for(long i=0;i<size;i+=TAILLE_SPLIT)
    {
      if(size-i>TAILLE_SPLIT)
        n=TAILLE_SPLIT;
      else
        n=size-i;
      TSeqCorba seq=_mySender->sendPart(i,n);
      TCorba *seqd=seq->get_buffer(0);
      for(long j=0;j<n;j++)
      *iter++=*seqd++;
    }
  return ret;
}

template<class T,class TCorba,class TSeqCorba,class CorbaSender,class servForT,class ptrForT>
T *CorbaWCWithCopyReceiver<T,TCorba,TSeqCorba,CorbaSender,servForT,ptrForT>::getValue(long &size)
{
  return Receiver<T,servForT,ptrForT>::getValue(size,_mySender);
}

#ifdef HAVE_MPI2

template<class T,class CorbaSender,class servForT,class ptrForT>
MPIReceiver<T,CorbaSender,servForT,ptrForT>::MPIReceiver(CorbaSender mySender):_mySender(mySender){
}

template<class T,class CorbaSender,class servForT,class ptrForT>
MPIReceiver<T,CorbaSender,servForT,ptrForT>::~MPIReceiver(){
  _mySender->release();
}

template<class T,class CorbaSender,class servForT,class ptrForT>
T *MPIReceiver<T,CorbaSender,servForT,ptrForT>::getDistValue(long &size){
  int i=0;
  int myproc;
  int sproc;
  MPI_Status status;
  MPI_Comm com; 
  char   port_name_clt [MPI_MAX_PORT_NAME];
  T *_v;
  long _n;

  
  CORBA::Any a; 
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);
  SALOME::MPISender::param_var p =_mySender->getParam();
  _mySender->send();
  sproc = p->myproc;
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  while ( i != TIMEOUT  && MPI_Lookup_name((char*)p->service,MPI_INFO_NULL,port_name_clt) != MPI_SUCCESS) { 
    i++;
  }       
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);
  if ( i == TIMEOUT  ) { 
    MPI_Finalize();
    exit(-1);
  }
  else{
    //       Connect to service, get the inter-communicator server
    //      Attention MPI_Comm_connect est un appel collectif :
    //  - Si lancement mpirun -c n -----> uniquement     MPI_COMM_SELF fonctionne
    //  - Si lancement client_server&client_server ----> MPI_COMM_WORLD fonctionne
    
    //      TIMEOUT is inefficient since MPI_Comm_Connect doesn't return if we asked for
    //        a service that has been unpublished !
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    i = 0;
    while ( i != TIMEOUT  &&  MPI_Comm_connect(port_name_clt, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &com)!=MPI_SUCCESS ) { 
      i++; 
    } 
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);
    if ( i == TIMEOUT ) {
      MPI_Finalize(); 
      exit(-1);
    }
  }
  MPI_Recv( &_n, 1, MPI_LONG, sproc,p->tag1,com,&status);
  _v = new T[_n];
  MPI_Recv( _v, _n, MPITRAITS<T>::MpiType, sproc,p->tag2,com,&status);
  _mySender->close(p);
  MPI_Comm_disconnect( &com );  
  size=_n;
  return _v;
}

template<class T,class CorbaSender,class servForT,class ptrForT>
T *MPIReceiver<T,CorbaSender,servForT,ptrForT>::getValue(long &size)
{
  return Receiver<T,servForT,ptrForT>::getValue(size,_mySender);
}

#endif

#ifdef HAVE_SOCKET
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <rpc/xdr.h>

template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::SocketReceiver(CorbaSender mySender) : _mySender(mySender)
{
  _clientSockfd = -1;
  _senderDestruc=true;
}

template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::~SocketReceiver()
{
  if(_senderDestruc)
    {
      _mySender->release();
    }
}

template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
T *SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::getValue(long &size)
{
  return Receiver<T,servForT,ptrForT>::getValue(size,_mySender);
}

template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
T* SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::getDistValue(long &size)
{
  int n=0, m;
  T *v;
  XDR xp; /* pointeur sur le decodeur XDR */

  try{
    initCom();

    SALOME::SocketSender::param_var p = _mySender->getParam();

    size = p->lend - p->lstart + 1;
    v = new T[size];

    connectCom(p->internet_address, p->myport);
  
    _mySender->send();

    xdrmem_create(&xp,(char*)v,size*sizeof(T),XDR_DECODE );
    while( n < size*sizeof(T) ){
      m = read(_clientSockfd, (char*)v+n, size*sizeof(T)-n);
      if( m < 0 ){
        closeCom();
        delete [] v;
        SALOME::ExceptionStruct es;
        es.type = SALOME::COMM;
        es.text = "error read Socket exception";
        throw SALOME::SALOME_Exception(es);
      }
      n += m;
    }
    xdr_vector( &xp, (char*)v, size, sizeof(T), (xdrproc_t)myFunc);
    xdr_destroy( &xp );
    
    _mySender->endOfCom();
    closeCom();
  }
  catch(SALOME::SALOME_Exception &ex){
    if( ex.details.type == SALOME::COMM )
      {
        _senderDestruc=false;
		std::cout << ex.details.text << std::endl;
        throw MultiCommException("Unknown sender protocol");
      }
    else
      throw ex;
  }
 
  return v;
}

template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
void SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::initCom()
{
  try{
    _mySender->initCom();

    /* Ouverture de la socket */
    _clientSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_clientSockfd < 0) {
      closeCom();
      SALOME::ExceptionStruct es;
      es.type = SALOME::COMM;
      es.text = "error Socket exception";
      throw SALOME::SALOME_Exception(es);
    }
  }
  catch(SALOME::SALOME_Exception &ex){
    if( ex.details.type == SALOME::COMM )
      {
        _senderDestruc=false;
		std::cout << ex.details.text << std::endl;
        throw MultiCommException("Unknown sender protocol");
      }
    else
      throw ex;
  }

}

template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
void SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::connectCom(const char *dest_address, int port)
{
  struct sockaddr_in serv_addr;
  struct hostent * server;
  SALOME::ExceptionStruct es;

  try{
    /* reception of the host structure on the remote process */
    server = gethostbyname(dest_address);
    if( server == NULL ) {
      closeCom();
      es.type = SALOME::COMM;
      es.text = "error unknown host Socket exception";
      _senderDestruc=false;
      throw SALOME::SALOME_Exception(es);
    }

    /* Initialisation of the socket structure */
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    
    if( connect(_clientSockfd, (struct sockaddr *) & serv_addr, sizeof(struct sockaddr)) < 0 ){
      closeCom();
      es.type = SALOME::COMM;
      es.text = "error connect Socket exception";
      _senderDestruc=false;
      throw SALOME::SALOME_Exception(es);
    }

    _mySender->acceptCom();

  }
  catch(SALOME::SALOME_Exception &ex){
    if( ex.details.type == SALOME::COMM )
      {
        _senderDestruc=false;
        std::cout << ex.details.text << std::endl;
        throw MultiCommException("Unknown sender protocol");
      }
    else
      throw ex;
  }

}


template<class T,int (*myFunc)(XDR*,T*),class CorbaSender,class servForT,class ptrForT>
void SocketReceiver<T,myFunc,CorbaSender,servForT,ptrForT>::closeCom()
{
  _mySender->closeCom();
  if( _clientSockfd >= 0 ){
    close(_clientSockfd);
    _clientSockfd = -1;
  }
 
}

#endif
