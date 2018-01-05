// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
// Author : Anthony GEAY (EDF R&D)

#include "SALOMESDS_RequestSwitcher.hxx"

using namespace SALOMESDS;

RequestSwitcherBase::RequestSwitcherBase(CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj(orb->resolve_initial_references("RootPOA"));
  PortableServer::POA_var poa(PortableServer::POA::_narrow(obj));
  _poa_manager_under_control=poa->the_POAManager();
  //
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL));
  policies[0]=PortableServer::ThreadPolicy::_duplicate(threadPol);
  // all is in PortableServer::POAManager::_nil. By specifying _nil cf Advanced CORBA Programming with C++ p 506
  // a new POA manager is created. This POA manager is independent from POA manager of the son ones.
  _poa_for_request_control=poa->create_POA("4RqstSwitcher",PortableServer::POAManager::_nil(),policies);
  threadPol->destroy();
  PortableServer::POAManager_var mgr(_poa_for_request_control->the_POAManager());
  mgr->activate();
  //obj=orb->resolve_initial_references ("POACurrent");// agy : usage of POACurrent breaks the hold_requests. Why ?
  //PortableServer::Current_var current(PortableServer::Current::_narrow(obj));
}

void RequestSwitcherBase::holdRequests()
{
  _poa_manager_under_control->hold_requests(true);
}

void RequestSwitcherBase::activeRequests()
{
  _poa_manager_under_control->activate();
}
