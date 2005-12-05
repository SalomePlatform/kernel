//  SALOME MPIContainer : implemenation of container based on MPI libraries
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : MPIObject_i.cxx
//  Module : SALOME

#include <mpi.h>
#include "MPIObject_i.hxx"
#include "utilities.h"
using namespace std;

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

void MPIObject_i::BCastIOR(CORBA::ORB_ptr orb, Engines::MPIObject_ptr pobj, 
			   bool amiCont)
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

