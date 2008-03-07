//  SALOME_ParallelContainerProxy : implementation of container and engine for Parallel Kernel
//
//  Copyright (C) 2008 OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : SALOME_ParallelContainerProxy_i.cxx
//  Author : André RIBES, EDF

#include "SALOME_ParallelContainerProxy_i.hxx"

Container_proxy_impl_final::Container_proxy_impl_final(CORBA::ORB_ptr orb, 
						       paco_fabrique_thread * fab_thread, 
						       bool is_a_return_proxy) :
  Engines::Container_proxy_impl(orb, fab_thread, is_a_return_proxy),
  InterfaceManager_impl(orb, fab_thread, is_a_return_proxy)
{}

Container_proxy_impl_final:: ~Container_proxy_impl_final() {}

void
Container_proxy_impl_final::Shutdown()
{
  INFOS("Shutdown Parallel Proxy");
  if(!CORBA::is_nil(_orb))
    _orb->shutdown(0);
}

