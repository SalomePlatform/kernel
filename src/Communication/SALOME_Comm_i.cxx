#include <rpc/xdr.h>
#include "SALOME_Comm_i.hxx"
#include "poa.h"
#include "omnithread.h"
#include "Utils_SINGLETON.hxx"
#include "Utils_ORB_INIT.hxx"
#include "utilities.h"

#include "SenderFactory.hxx"
using namespace std;

CORBA::ORB_var &getGlobalORB(){
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  CORBA::ORB_var &orb = init(0,0);
  return orb;
}

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
SALOME_Sender_i::SALOME_Sender_i(SALOME::TypeOfDataTransmitted type,const void *tabToSend,long lgrTabToSend,int sizeOf):_tabToSend(tabToSend),_lgrTabToSend(lgrTabToSend),_type(type),_sizeOf(sizeOf){
}

/*! Method to establish if the CORBA object refered by pCorba is collocalised.\n
  If it is, the pointer to the servant that incarnates the CORBA object is returned.
*/
SALOME_Sender_i *SALOME_Sender_i::find(SALOME::Sender_ptr pCorba){
  PortableServer::ServantBase *ret;
  try {
    ret=PortableServer::POA::_the_root_poa()->reference_to_servant(pCorba);
  }
  catch(...){
    return 0;
  }
  ret->_remove_ref();
  return dynamic_cast<SALOME_Sender_i *>(ret);
}

/*! Method for the remote destroy of the current servant. This method is used by the receiver to destroy the sender when the transfert is complete.
 */
void SALOME_Sender_i::release()
{
  PortableServer::ObjectId_var oid = _default_POA()->servant_to_id(this);
  _default_POA()->deactivate_object(oid);
  _remove_ref();
}

/*! Return the type of the element that compose the array. Used by receiverfactory to build the correct receiver.
 */
SALOME::TypeOfDataTransmitted SALOME_Sender_i::getTypeOfDataTransmitted()
{
  return _type;
}

/*! Return a new sender of the same array but with an another protocol.
 */
SALOME::Sender_ptr SALOME_Sender_i::buildOtherWithProtocol(SALOME::TypeOfCommunication type)
{
  return SenderFactory::buildSender(type,this);
}

SALOME_CorbaDoubleNCSender_i::SALOME_CorbaDoubleNCSender_i(const double *tabToSend,long lgrTabToSend):SALOME_Sender_i(SALOME::DOUBLE_,tabToSend,lgrTabToSend,sizeof(double)){
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

SALOME_CorbaDoubleCSender_i::SALOME_CorbaDoubleCSender_i(const double *tabToSend,long lgrTabToSend):SALOME_Sender_i(SALOME::DOUBLE_,tabToSend,lgrTabToSend,sizeof(double)){
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

SALOME_CorbaLongNCSender_i::SALOME_CorbaLongNCSender_i(const int *tabToSend,long lgrTabToSend):SALOME_Sender_i(SALOME::INT_,tabToSend,lgrTabToSend,sizeof(int)){
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

SALOME_CorbaLongCSender_i::SALOME_CorbaLongCSender_i(const int *tabToSend,long lgrTabToSend):SALOME_Sender_i(SALOME::INT_,tabToSend,lgrTabToSend,sizeof(int)){
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

SALOME_MPISender_i::SALOME_MPISender_i(SALOME::TypeOfDataTransmitted type,const void *tabToSend,long lgrTabToSend,int sizeOf):SALOME_Sender_i(type,tabToSend,lgrTabToSend,sizeOf){
  _portName=new char[MPI_MAX_PORT_NAME];
}

SALOME_MPISender_i::~SALOME_MPISender_i(){
  delete [] _portName;
}

SALOME::MPISender::param* SALOME_MPISender_i::getParam()
{
  char stag[12];
  int myproc,i=0;

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
  _argsForThr=new (void *)[8];
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
}

void SALOME_MPISender_i::close(const SALOME::MPISender::param& p)
{
  std::string service(p.service);
  const char *st=p.service;
  void *r;
  _newThr->join(&r);
  MPI_Comm_free(&_com); 
  MPI_Unpublish_name((char *)service.c_str(),MPI_INFO_NULL,_portName); 
  MPI_Close_port(_portName);
  delete [] _argsForThr;
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

SALOME_SocketSender_i::SALOME_SocketSender_i(SALOME::TypeOfDataTransmitted type,const void *tabToSend,long lgrTabToSend,int sizeOf):SALOME_Sender_i(type,tabToSend,lgrTabToSend,sizeOf){
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
  int new_fd;
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

//CCRT porting
#undef _LIBC_POLLUTION_H_
#undef _POSIX_PII_SOCKET

#endif
