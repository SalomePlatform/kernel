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

//  SALOME MPIContainer : implemenation of container based on MPI libraries
//  File   : MPIObject_i.hxx
//  Module : SALOME
//
#ifndef _SALOME_POBJECT_I_H_
#define _SALOME_POBJECT_I_H_ 

#include <mpi.h>
#include <string>
#include <map>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_MPIObject)

class MPIObject_i: public virtual POA_Engines::MPIObject
{
 public:
  MPIObject_i();
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
  void BCastIOR(CORBA::ORB_ptr orb,Engines::MPIObject_ptr pobj,bool amiCont);
#ifdef HAVE_MPI2
  // MPI2 connection
  void remoteMPI2Connect(std::string service);
  // MPI2 disconnection
  void remoteMPI2Disconnect(std::string service);
#endif

protected:
  std::map<std::string, MPI_Comm> _gcom;

private:
  std::map<std::string, MPI_Comm> _icom;
  std::map<std::string, bool> _srv;
  std::map<std::string, std::string> _port_name;

} ;

#endif
