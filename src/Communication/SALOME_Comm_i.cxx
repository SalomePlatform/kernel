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

#include "SALOME_Comm_i.hxx"
#ifndef WIN32
#ifndef __APPLE__
#include <rpc/xdr.h>
#endif
#endif
#include "omniORB4/poa.h"
#include "omnithread.h"
#include "Utils_SINGLETON.hxx"
#include "Utils_ORB_INIT.hxx"
#include "utilities.h"

#include "SenderFactory.hxx"

#ifndef WIN32
CORBA::ORB_var &getGlobalORB(){
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  CORBA::ORB_var &orb = init(0,0);
  return orb;
}
#endif

/*! Return the C++ data associated to the array to transmit.
  Used when sender and receiver are collocalized.
 */
const void *SALOME_Sender_i::getData(long &size) const{
  size=_lgrTabToSend;
  return _tabToSend;
}

/*! Return the sizeof() of each component of the generic array
 */
int SALOME_Sender_i::getSizeOf() const {
  return _sizeOf;
}

/*! Unique constructor */
SALOME_Sender_i::SALOME_Sender_i(const void *tabToSend,long lgrTabToSend,int sizeOf,bool ownTabToSend):_tabToSend(tabToSend),_lgrTabToSend(lgrTabToSend),_sizeOf(sizeOf),_ownTabToSend(ownTabToSend){
}

/*! To force ownerShip status */
void SALOME_Sender_i::setOwnerShip(bool own)
{
  _ownTabToSend=own;
}

/*! Method for the remote destroy of the current servant. This method is used by the receiver to destroy the sender when the transfert is complete.
 */
void SALOME_Sender_i::release()
{
  PortableServer::ObjectId_var oid = _default_POA()->servant_to_id(this);
  _default_POA()->deactivate_object(oid);
  _remove_ref();
}

SALOME_SenderDouble_i::SALOME_SenderDouble_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend):SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend)
{
}

/*! Destructor.
 */
SALOME_SenderDouble_i::~SALOME_SenderDouble_i()
{
  if(_ownTabToSend)
    delete [] (double *)_tabToSend;
}

/*! Return a new sender of the same array but with an another protocol and delegates to the returned sender the ownership of array.
 */
SALOME::SenderDouble_ptr SALOME_SenderDouble_i::buildOtherWithProtocol(SALOME::TypeOfCommunication type)
{
  return SenderFactory::buildSender(type,this);
}

/*! Method to establish if the CORBA object refered by pCorba is collocalised.\n
  If it is, the pointer to the servant that incarnates the CORBA object is returned.
*/
SALOME_SenderDouble_i *SALOME_SenderDouble_i::find(SALOME::SenderDouble_ptr pCorba){
  PortableServer::ServantBase *ret;
  try {
    ret=PortableServer::POA::_the_root_poa()->reference_to_servant(pCorba);
  }
  catch(...){
    return 0;
  }
  ret->_remove_ref();
  return dynamic_cast<SALOME_SenderDouble_i *>(ret);
}

SALOME_SenderInt_i::SALOME_SenderInt_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend):SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend)
{
}

/*! Destructor.
 */
SALOME_SenderInt_i::~SALOME_SenderInt_i()
{
  if(_ownTabToSend)
    delete [] (int *)_tabToSend;
}

/*! Return a new sender of the same array but with an another protocol.
 */
SALOME::SenderInt_ptr SALOME_SenderInt_i::buildOtherWithProtocol(SALOME::TypeOfCommunication type)
{
  return SenderFactory::buildSender(type,this);
}

/*! Method to establish if the CORBA object refered by pCorba is collocalised.\n
  If it is, the pointer to the servant that incarnates the CORBA object is returned.
*/
SALOME_SenderInt_i *SALOME_SenderInt_i::find(SALOME::SenderInt_ptr pCorba){
  PortableServer::ServantBase *ret;
  try {
    ret=PortableServer::POA::_the_root_poa()->reference_to_servant(pCorba);
  }
  catch(...){
    return 0;
  }
  ret->_remove_ref();
  return dynamic_cast<SALOME_SenderInt_i *>(ret);
}

SALOME_CorbaDoubleNCSender_i::SALOME_CorbaDoubleNCSender_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend):SALOME_SenderDouble_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend){
}

SALOME_CorbaDoubleNCSender_i::~SALOME_CorbaDoubleNCSender_i(){
}

CORBA::ULong SALOME_CorbaDoubleNCSender_i::getSize(){
  CORBA::ULong ret=_lgrTabToSend;
  return ret;
}

SALOME::vectorOfDouble* SALOME_CorbaDoubleNCSender_i::sendPart(CORBA::ULong offset, CORBA::ULong length){
  SALOME::vectorOfDouble_var c1 = new SALOME::vectorOfDouble(length,length,(CORBA::Double *)((double *)_tabToSend+(long)offset),0);
  return c1._retn();
}

SALOME::vectorOfDouble* SALOME_CorbaDoubleNCSender_i::send(){
  SALOME::vectorOfDouble_var c1 = new SALOME::vectorOfDouble(_lgrTabToSend,_lgrTabToSend,(CORBA::Double *)_tabToSend,0);
  return c1._retn();
}

SALOME_CorbaDoubleCSender_i::SALOME_CorbaDoubleCSender_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend):SALOME_SenderDouble_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend){
}

SALOME_CorbaDoubleCSender_i::~SALOME_CorbaDoubleCSender_i(){
}

CORBA::ULong SALOME_CorbaDoubleCSender_i::getSize(){
  CORBA::ULong ret=_lgrTabToSend;
  return ret;
}

SALOME::vectorOfDouble* SALOME_CorbaDoubleCSender_i::sendPart(CORBA::ULong offset, CORBA::ULong length){
  SALOME::vectorOfDouble_var c1 = new SALOME::vectorOfDouble;
  c1->length(length);
  for (long i=0; i<length; i++)
    c1[i] = ((double *)_tabToSend)[i+offset];
  return c1._retn();
}

////////////////////////

SALOME_CorbaLongNCSender_i::SALOME_CorbaLongNCSender_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend):SALOME_SenderInt_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend){
}

SALOME_CorbaLongNCSender_i::~SALOME_CorbaLongNCSender_i(){
}

CORBA::ULong SALOME_CorbaLongNCSender_i::getSize(){
  CORBA::ULong ret=_lgrTabToSend;
  return ret;
}

SALOME::vectorOfLong* SALOME_CorbaLongNCSender_i::sendPart(CORBA::ULong offset, CORBA::ULong length){
  SALOME::vectorOfLong_var c1 = new SALOME::vectorOfLong(length,length,(CORBA::Long *)((long *)_tabToSend+(long)offset),0);
  return c1._retn();
}

SALOME::vectorOfLong* SALOME_CorbaLongNCSender_i::send(){
  SALOME::vectorOfLong_var c1 = new SALOME::vectorOfLong(_lgrTabToSend,_lgrTabToSend,(CORBA::Long *)_tabToSend,0);
  return c1._retn();
}

SALOME_CorbaLongCSender_i::SALOME_CorbaLongCSender_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend):SALOME_SenderInt_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend){
}

SALOME_CorbaLongCSender_i::~SALOME_CorbaLongCSender_i(){
}

CORBA::ULong SALOME_CorbaLongCSender_i::getSize(){
  CORBA::ULong ret=_lgrTabToSend;
  return ret;
}

SALOME::vectorOfLong* SALOME_CorbaLongCSender_i::sendPart(CORBA::ULong offset, CORBA::ULong length){
  SALOME::vectorOfLong_var c1 = new SALOME::vectorOfLong;
  c1->length(length);
  for (long i=0; i<length; i++)
    c1[i] = ((long *)_tabToSend)[i+offset];
  return c1._retn();
}

#ifdef HAVE_MPI2

unsigned long SALOME_MPISender_i::_tag1=0;

unsigned long SALOME_MPISender_i::_tag2=1;

SALOME_MPISender_i::SALOME_MPISender_i(const void *tabToSend,long lgrTabToSend,int sizeOf,bool ownTabToSend):SALOME_Sender_i(tabToSend,lgrTabToSend,sizeOf,ownTabToSend){
  _portName=new char[MPI_MAX_PORT_NAME];
}

SALOME_MPISender_i::~SALOME_MPISender_i(){
  delete [] _portName;
}

SALOME::MPISender::param* SALOME_MPISender_i::getParam()
{
  char stag[12];
  int i=0;

  SALOME::MPISender::param_var p = new SALOME::MPISender::param;
  MPI_Comm_rank(MPI_COMM_WORLD,&_cproc);
  p->myproc = _cproc;
  p->tag1 = _tag1;
  _tag1Inst=_tag1;
  p->tag2 =_tag2;
  _tag2Inst=_tag2;
  std::string service("toto_");
  sprintf(stag,"%d_",_tag1);
  service += stag;
  sprintf(stag,"%d_",p->tag2);
  service += stag;
  p->service = CORBA::string_dup(service.c_str());
  MPI_Open_port(MPI_INFO_NULL, _portName);
  MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
  while ( i != TIMEOUT  && MPI_Publish_name((char*)service.c_str(),MPI_INFO_NULL,_portName) != MPI_SUCCESS) {
    i++;
  } 
  MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_ARE_FATAL);
  if ( i == TIMEOUT  ) { 
    MPI_Close_port(_portName);
    MPI_Finalize();
    exit(-1);
    }
  _tag1 += 2;
  _tag2 += 2;
  return p._retn();
}

void SALOME_MPISender_i::send()
{
  _type=getTypeOfDataTransmitted();
  _argsForThr=new void *[8];
  _argsForThr[0]=_portName;
  _argsForThr[1]=&_lgrTabToSend;
  _argsForThr[2]=(void *)_tabToSend;
  _argsForThr[3]=&_cproc;
  _argsForThr[4]=&_tag1Inst;
  _argsForThr[5]=&_tag2Inst;
  _argsForThr[6]=&_com;
  _argsForThr[7]=&_type;

  _newThr=new omni_thread(SALOME_MPISender_i::myThread,_argsForThr);
  _newThr->start();
}

void* SALOME_MPISender_i::myThread(void *args)
{
  void **argsTab=(void **)args;
  long *lgrTabToSend=(long *)argsTab[1];
  int *cproc=(int *)argsTab[3];
  int *tag1=(int *)argsTab[4];
  int *tag2=(int *)argsTab[5];
  MPI_Comm *com=(MPI_Comm *)argsTab[6];
  SALOME::TypeOfDataTransmitted *type=(SALOME::TypeOfDataTransmitted *)argsTab[7];

  MPI_Comm_accept((char *)argsTab[0],MPI_INFO_NULL,0,MPI_COMM_SELF,com);
  MPI_Send(lgrTabToSend,1,MPI_LONG,*cproc,*tag1,*com);
  switch(*type)
    { 
    case SALOME::DOUBLE_:
      MPI_Send(argsTab[2],*lgrTabToSend,MPI_DOUBLE,*cproc,*tag2,*com);
      break;
    case SALOME::INT_:
      MPI_Send(argsTab[2],*lgrTabToSend,MPI_INT,*cproc,*tag2,*com);
    }
  omni_thread::exit();
  return args;
}

void SALOME_MPISender_i::close(const SALOME::MPISender::param& p)
{
  std::string service(p.service);
  void *r;
  _newThr->join(&r);
  MPI_Comm_free(&_com); 
  MPI_Unpublish_name((char *)service.c_str(),MPI_INFO_NULL,_portName); 
  MPI_Close_port(_portName);
  delete [] _argsForThr;
}

SALOME_MPISenderDouble_i::SALOME_MPISenderDouble_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend)
  :SALOME_SenderDouble_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_MPISender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend)
  ,SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend)
{
}

SALOME_MPISenderInt_i::SALOME_MPISenderInt_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend)
  :SALOME_SenderInt_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_MPISender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend)
  ,SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend)
{
}

#endif

#ifdef HAVE_SOCKET

//CCRT porting
#define _POSIX_PII_SOCKET
#define _LIBC_POLLUTION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

SALOME_SocketSender_i::SALOME_SocketSender_i(const void *tabToSend,long lgrTabToSend,int sizeOf,bool ownTabToSend):SALOME_Sender_i(tabToSend,lgrTabToSend,sizeOf,ownTabToSend){
  _IPAddress = inetAddress();
  _serverSockfd = -1;
  _clientSockfd = -1;
}

SALOME_SocketSender_i::~SALOME_SocketSender_i(){
}

std::string SALOME_SocketSender_i::inetAddress()
{
   char s[256];
   char t[INET_ADDRSTRLEN+1];
   struct hostent *host;
   struct in_addr saddr;

   gethostname(s, 255);

   *t = '\0';

   saddr.s_addr = inet_addr(s);
   if (saddr.s_addr != -1)
      inet_ntop(AF_INET, &saddr, t, INET_ADDRSTRLEN);
   else {
      host = gethostbyname(s);
      if (host != NULL)
         inet_ntop(AF_INET, (struct in_addr *) *host->h_addr_list, 
                   t, INET_ADDRSTRLEN);
   }
   return std::string(t);
}

SALOME::SocketSender::param * SALOME_SocketSender_i::getParam()
{

  SALOME::SocketSender::param_var p = new SALOME::SocketSender::param;

  p->lstart = 0;
  p->lend = _lgrTabToSend - 1;
  p->myport = _port;
  p->internet_address = CORBA::string_dup(_IPAddress.c_str());

  return p._retn();
}

void SALOME_SocketSender_i::send()
{
  _type=getTypeOfDataTransmitted();
  _argsForThr=new void *[6];
  _argsForThr[0]=&_serverSockfd;
  _argsForThr[1]=&_clientSockfd;
  _argsForThr[2]=&_lgrTabToSend;
  _argsForThr[3]=(void *)_tabToSend;
  _argsForThr[4]=&_errorFlag;
  _argsForThr[5]=&_type;

  _newThr=new omni_thread(SALOME_SocketSender_i::myThread,_argsForThr);
  _newThr->start();
}

void* SALOME_SocketSender_i::myThread(void *args)
{
  int n=0, m;
  void **argsTab=(void **)args;
  int *serverSockfd=(int *)argsTab[0];
  int *clientSockfd=(int *)argsTab[1];
  long *lgrTabToSend=(long *)argsTab[2];
  void *tabToSend=argsTab[3];
  bool *errorFlag=(bool*)argsTab[4];
  SALOME::TypeOfDataTransmitted *type=(SALOME::TypeOfDataTransmitted *)argsTab[5];
  
  XDR xp; /* pointeur sur le decodeur XDR */
  
  switch(*type)
    { 
    case SALOME::DOUBLE_:
      xdrmem_create(&xp,(char*)tabToSend,(*lgrTabToSend)*sizeof(double),XDR_ENCODE );
      xdr_vector( &xp, (char*)tabToSend, *lgrTabToSend, sizeof(double), (xdrproc_t)xdr_double );

      *errorFlag = false;
      while( n < *lgrTabToSend*sizeof(double) ){
        m = write(*clientSockfd, (char*)tabToSend+n, *lgrTabToSend*sizeof(double)-n);
        if( m < 0 ){
          if( *clientSockfd >= 0 ){
            ::close(*clientSockfd);
            *clientSockfd = -1;
          }
          if( *serverSockfd >= 0 ){
            ::close(*serverSockfd);
            *serverSockfd = -1;
          }
          *errorFlag = true;
        }
        n += m;
      }
      xdr_destroy( &xp );

      xdrmem_create(&xp,(char*)tabToSend,(*lgrTabToSend)*sizeof(double),XDR_DECODE );
      xdr_vector( &xp, (char*)tabToSend, *lgrTabToSend, sizeof(double), (xdrproc_t)xdr_double );
      xdr_destroy( &xp );
      break;
    case SALOME::INT_:
      xdrmem_create(&xp,(char*)tabToSend,(*lgrTabToSend)*sizeof(int),XDR_ENCODE );
      xdr_vector( &xp, (char*)tabToSend, *lgrTabToSend, sizeof(int), (xdrproc_t)xdr_int );

      *errorFlag = false;
      while( n < *lgrTabToSend*sizeof(int) ){
        m = write(*clientSockfd, (char*)tabToSend+n, *lgrTabToSend*sizeof(int)-n);
        if( m < 0 ){
          if( *clientSockfd >= 0 ){
            ::close(*clientSockfd);
            *clientSockfd = -1;
          }
          if( *serverSockfd >= 0 ){
            ::close(*serverSockfd);
            *serverSockfd = -1;
          }
          *errorFlag = true;
        }
        n += m;
      }
      xdr_destroy( &xp );

      xdrmem_create(&xp,(char*)tabToSend,(*lgrTabToSend)*sizeof(int),XDR_DECODE );
      xdr_vector( &xp, (char*)tabToSend, *lgrTabToSend, sizeof(int), (xdrproc_t)xdr_int );
      xdr_destroy( &xp );
    }
  return args;
}

void SALOME_SocketSender_i::initCom() throw(SALOME::SALOME_Exception)
{
  struct sockaddr_in serv_addr;
  socklen_t n;
  SALOME::ExceptionStruct es;

  /* Ouverture de la socket */
  _serverSockfd = socket(AF_INET , SOCK_STREAM , 0);
  if(_serverSockfd < 0) {
    es.type = SALOME::COMM;
    es.text = "error Socket exception";
    throw SALOME::SALOME_Exception(es);
  }
  /* Socket structure initialisation*/
  bzero((char*)&serv_addr,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = 0; /* asking for a free port */
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  /* Association of socket with a port */
  if( ::bind(_serverSockfd, (struct sockaddr *) & serv_addr, 
           sizeof(struct sockaddr)) < 0 ) {
    closeCom();
    es.type = SALOME::COMM;
    es.text = "error bind Socket exception";
    throw SALOME::SALOME_Exception(es);
  }
  /* Listening to the allocated port */
  if( listen(_serverSockfd, 10) < 0 ) {
    closeCom();
    es.type = SALOME::COMM;
    es.text = "error listen Socket exception";
    throw SALOME::SALOME_Exception(es);
  }
  /* Retrieving port number*/
  if( getsockname(_serverSockfd, (struct sockaddr *) & serv_addr, &n) < 0 ){
    closeCom();
    es.type = SALOME::COMM;
    es.text = "error getName Socket exception";
    throw SALOME::SALOME_Exception(es);
  }
  _port = htons(serv_addr.sin_port);
  SCRUTE(_port);
}

void SALOME_SocketSender_i::acceptCom() throw(SALOME::SALOME_Exception)
{
  socklen_t sin_size;
  struct sockaddr_in client_addr;
  SALOME::ExceptionStruct es;

  sin_size = sizeof(struct sockaddr_in);
  
  _clientSockfd = accept(_serverSockfd, (struct sockaddr *)&client_addr, &sin_size);
  if( _clientSockfd < 0 ){
    closeCom();
    es.type = SALOME::COMM;
    es.text = "error accept Socket exception";
    throw SALOME::SALOME_Exception(es);
  }
}

void SALOME_SocketSender_i::closeCom()
{
  if( _clientSockfd >= 0 ){
    ::close(_clientSockfd);
    _clientSockfd = -1;
  }
  if( _serverSockfd >= 0 ){
    ::close(_serverSockfd);
    _serverSockfd = -1;
  }

}

void SALOME_SocketSender_i::endOfCom()
{
  void *r;
  _newThr->join(&r);
  if(_errorFlag)
    {
      SALOME::ExceptionStruct es;
      es.type = SALOME::COMM;
      es.text = "error write Socket exception";
      throw SALOME::SALOME_Exception(es);
    }
  delete [] _argsForThr;
}

SALOME_SocketSenderDouble_i::SALOME_SocketSenderDouble_i(const double *tabToSend,long lgrTabToSend,bool ownTabToSend)
  :SALOME_SenderDouble_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_SocketSender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend)
  ,SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(double),ownTabToSend)
{
}

SALOME_SocketSenderInt_i::SALOME_SocketSenderInt_i(const int *tabToSend,long lgrTabToSend,bool ownTabToSend)
  :SALOME_SenderInt_i(tabToSend,lgrTabToSend,ownTabToSend),SALOME_SocketSender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend)
  ,SALOME_Sender_i(tabToSend,lgrTabToSend,sizeof(int),ownTabToSend)
{
}

//CCRT porting
#undef _LIBC_POLLUTION_H_
#undef _POSIX_PII_SOCKET

#endif
