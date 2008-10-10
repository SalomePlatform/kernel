//  SALOME_GenericObj_i_CC
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//  File   : SALOME_GenericObj_i.cc
//  Author : Alexey PETROV
//  Module : SALOME

#include "SALOME_GenericObj_i.hh"
#include "utilities.h"

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

using namespace SALOME;
using namespace std;

GenericObj_i::GenericObj_i(PortableServer::POA_ptr thePOA): myRefCounter(1){
  if(MYDEBUG) 
    MESSAGE("GenericObj_i::GenericObj_i() - this = "<<this<<
	    "; CORBA::is_nil(thePOA) = "<<CORBA::is_nil(thePOA));
  if(CORBA::is_nil(thePOA))
#ifndef WIN32
    myPOA = PortableServer::ServantBase::_default_POA();
#else
    myPOA = ServantBase::_default_POA();
#endif
  else
    myPOA = PortableServer::POA::_duplicate(thePOA);
}


PortableServer::POA_ptr GenericObj_i::_default_POA(){
  return PortableServer::POA::_duplicate(myPOA);
}


void GenericObj_i::Register(){
  if(MYDEBUG)
    MESSAGE("GenericObj_i::Register "<<this<<"; myRefCounter = "<<myRefCounter)
  ++myRefCounter;
}


void GenericObj_i::Destroy(){
  if(MYDEBUG)
    MESSAGE("GenericObj_i::Destroy "<<this<<"; myRefCounter = "<<myRefCounter)
  if(--myRefCounter <= 0){
    PortableServer::ObjectId_var anObjectId = myPOA->servant_to_id(this);
    myPOA->deactivate_object(anObjectId.in());
    _remove_ref();
  }
}
