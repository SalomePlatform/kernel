//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  SALOME MPIContainer : implemenation of container based on MPI libraries
//  File   : MPIObject_i.cxx
//  Module : SALOME
//
#include "MPIObject_i.hxx"
#include "utilities.h"
using namespace std;
#define TIMEOUT 5

MPIObject_i::MPIObject_i()
{
  MPI_Comm_size( MPI_COMM_WORLD, &_nbproc );
  MPI_Comm_rank( MPI_COMM_WORLD, &_numproc );
  _tior=NULL;
}

MPIObject_i::MPIObject_i(int nbproc, int numproc)
{
  _nbproc = nbproc;
  _numproc = numproc;
  _tior=NULL;
}

MPIObject_i::~MPIObject_i()
{
  if(_tior) delete _tior;
}

Engines::IORTab* MPIObject_i::tior()
{
  Engines::IORTab_var tior = new Engines::IORTab;
  tior->length(_tior->length());
  for(unsigned int ip=0;ip<tior->length();ip++)
    tior[ip] = (*_tior)[ip];
  return tior._retn(); 
};

void MPIObject_i::tior(const Engines::IORTab& ior)
{
  _tior = new Engines::IORTab;
  _tior->length(ior.length());
  for(unsigned int ip=0;ip<ior.length();ip++)
    (*_tior)[ip] = ior[ip];
}

void MPIObject_i::BCastIOR(CORBA::ORB_ptr orb, Engines::MPIObject_ptr pobj, bool amiCont)
{
  int err, ip, n;
  char *ior;
  MPI_Status status; /* status de reception de message MPI */

  if( _numproc == 0 ){

    //Allocation du tableau des IOR
    Engines::IORTab_var iort = new Engines::IORTab;
    iort->length(_nbproc);

    iort[0] = pobj;

    // Process 0 recupere les ior de l'object sur les autres process
    for(ip=1;ip<_nbproc;ip++){
      err = MPI_Recv(&n,1,MPI_INT,ip,ip,MPI_COMM_WORLD,&status);
      if(err){
        MESSAGE("[" << _numproc << "] MPI_RECV error");
        exit(1);
      }
      // Allocation de la chaine de longueur n
      ior = new char[n];
      err = MPI_Recv(ior,n,MPI_CHAR,ip,2*ip,MPI_COMM_WORLD,&status);
      if(err){
        MESSAGE("[" << _numproc << "] MPI_RECV error");
        exit(1);
      }
      iort[ip] = orb->string_to_object(ior);
      delete [] ior;
      if(CORBA::is_nil(iort[ip]))
        throw POException(ip,"MPI Component not loaded");
    }
    // On donne le tableau des ior a l'objet Corba du process 0
    if( amiCont )
      tior(*(iort._retn()));
    else
      pobj->tior(*(iort._retn()));
  }
  else{
    // Conversion IOR vers string
    ior = orb->object_to_string(pobj);
    n = strlen(ior) + 1;
    // On envoie l'IOR au process 0
    err = MPI_Send(&n,1,MPI_INT,0,_numproc,MPI_COMM_WORLD);
    if(err){
      MESSAGE("[" << _numproc << "] MPI_SEND error");
      exit(1);
    }
    err = MPI_Send(ior,n,MPI_CHAR,0,2*_numproc,MPI_COMM_WORLD);
    if(err){
      MESSAGE("[" << _numproc << "] MPI_SEND error");
      exit(1);
    }
    CORBA::string_free(ior);
  }

}

#ifdef HAVE_MPI2
void MPIObject_i::remoteMPI2Connect(string service)
{
  int i;
  char port_name[MPI_MAX_PORT_NAME];
  char port_name_clt[MPI_MAX_PORT_NAME];

  if( service.size() == 0 ){
    MESSAGE("[" << _numproc << "] You have to give a service name !");
    throw POException(_numproc,"You have to give a service name !");
  }

  if( _srv.find(service) != _srv.end() ){
    MESSAGE("[" << _numproc << "] service " << service << " already exist !");
    throw POException(_numproc,"service " + service + " already exist !");
  }

  _srv[service] = false;

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if( _numproc == 0 ){ 
    /* rank 0 try to be a server. If service is already published, try to be a cient */
    MPI_Open_port(MPI_INFO_NULL, port_name); 
    if ( MPI_Publish_name((char*)service.c_str(), MPI_INFO_NULL, port_name) == MPI_SUCCESS )  {
      _srv[service] = true;
      _port_name[service] = port_name;
      MESSAGE("[" << _numproc << "] service " << service << " available at " << port_name << endl);
    }      
    else if ( MPI_Lookup_name((char*)service.c_str(), MPI_INFO_NULL, port_name_clt) == MPI_SUCCESS ){
      MESSAGE("[" << _numproc << "] I get the connection with " << service << " at " << port_name_clt << endl);
      MPI_Close_port( port_name );
    }
    else{
      /* Throw exception */
      MESSAGE("[" << _numproc << "] Error on connection with " << service << " at " << port_name_clt << endl);
      throw POException(_numproc,"Error on connection with " + service + " at " + port_name_clt);
    }
  }
  else{
    i=0;
    /* Waiting rank 0 publish name and try to be a client */
    while ( i != TIMEOUT  ) {
      sleep(1);
      if ( MPI_Lookup_name((char*)service.c_str(), MPI_INFO_NULL, port_name_clt) == MPI_SUCCESS ){
        MESSAGE("[" << _numproc << "] I get the connection with " << service << " at " << port_name_clt << endl);
        break;
      }
      i++;
    }
    if(i==TIMEOUT){
      /* Throw exception */
      MESSAGE("[" << _numproc << "] Error on connection with " << service << " at " << port_name_clt << endl);
      throw POException(_numproc,"Error on connection with " + service + " at " + port_name_clt);
    }
  }
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);
  
  /* If rank 0 is server, all processes call MPI_Comm_accept */
  /* If rank 0 is not server, all processes call MPI_Comm_connect */
  int srv = (int)_srv[service];
  MPI_Bcast(&srv,1,MPI_INT,0,MPI_COMM_WORLD);
  _srv[service] = (bool)srv;
  if ( _srv[service] )
    MPI_Comm_accept( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(_icom[service]) );
  else
    MPI_Comm_connect(port_name_clt, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(_icom[service]) );

  /* create global communicator: servers have low index in global communicator*/
  MPI_Intercomm_merge(_icom[service],!_srv[service],&(_gcom[service]));

  /* only rank 0 can be server for unpublish name */
  if(_numproc != 0) _srv[service] = false;

}

void MPIObject_i::remoteMPI2Disconnect(std::string service)
{

  if( service.size() == 0 ){
    MESSAGE("[" << _numproc << "] You have to give a service name !");
    throw POException(_numproc,"You have to give a service name !");
  }

  if( _srv.find(service) == _srv.end() ){
    MESSAGE("[" << _numproc << "] service " << service << " don't exist !");
    throw POException(_numproc,"service " + service + " don't exist !");
  }

  MPI_Comm_disconnect( &(_gcom[service]) ); 
  if ( _srv[service] ) {

    char port_name[MPI_MAX_PORT_NAME];
    strcpy(port_name,_port_name[service].c_str());

    MPI_Unpublish_name((char*)service.c_str(), MPI_INFO_NULL, port_name); 
    MESSAGE("[" << _numproc << "] " << service << ": close port " << _port_name[service] << endl);
    MPI_Close_port( port_name ); 
    _port_name.erase(service);
  }

  _gcom.erase(service);
  _icom.erase(service);
  _srv.erase(service);

}
#endif

