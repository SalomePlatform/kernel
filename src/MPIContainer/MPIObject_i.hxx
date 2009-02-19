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
//  File   : MPIObject_i.hxx
//  Module : SALOME
//
#ifndef _SALOME_POBJECT_I_H_
#define _SALOME_POBJECT_I_H_ 

#include <mpi.h>
#include <string>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_MPIObject)
#define defaultService "SERVER"

class POException
{
public:
  const std::string msg;
  const int numproc;
  POException(const int ip,const std::string m) : numproc(ip),msg(m) {}
};

class MPIObject_i: public POA_Engines::MPIObject
{
 public:
  MPIObject_i();
  MPIObject_i(int nbproc, int numproc);
  ~MPIObject_i();
    
  Engines::IORTab* tior();
  void tior(const Engines::IORTab& ior);

 protected:
  // Numero du processus
  int _numproc;
  // Nombre de processus
  int _nbproc;
  // IOR des objets paralleles sur tous les process mpi
  Engines::IORTab* _tior;
  // Echange des IOR de l'objet entre process
  void BCastIOR(CORBA::ORB_ptr orb,Engines::MPIObject_ptr pobj,bool amiCont) throw(POException);
#ifdef HAVE_MPI2
  // MPI2 connection
  MPI_Comm remoteMPI2Connect(std::string service=defaultService) throw(POException);
  // MPI2 disconnection
  void remoteMPI2Disconnect(MPI_Comm gcom);
#endif

private:
  int _srv;
  char _port_name[MPI_MAX_PORT_NAME];
  std::string _service;

} ;

#endif
