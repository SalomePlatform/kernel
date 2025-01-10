// Copyright (C) 2024-2025  CEA, EDF
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

#include "SALOME_LockMasterImpl.hxx"
#include "SALOME_LockServantImpl.hxx"

#include "SALOME_KernelORB.hxx"

#include <string>
#include <map>

static std::map<std::string,Engines::LockServant_var> _uniqueLockObj;

Engines::LockServant_ptr SALOME::LockMasterImpl::getLockerFor(const char *key)
{
  std::string keyCpp(key);
  auto it = _uniqueLockObj.find( keyCpp );
  Engines::LockServant_var ret;
  if( it != _uniqueLockObj.end() )
    ret = it->second;
  else
  {
    SALOME::LockServantImpl *serv = new SALOME::LockServantImpl;
    ret = serv->_this();
    serv->_remove_ref();
    _uniqueLockObj[keyCpp] = ret;
  }
  return Engines::LockServant::_duplicate( ret );
}

Engines::RendezVousServant_ptr SALOME::LockMasterImpl::buildRendezVous(CORBA::Long nbClients)
{
  SALOME::RendezVousServantImpl *serv = new SALOME::RendezVousServantImpl(nbClients);
  Engines::RendezVousServant_var ret = serv->_this();
  serv->_remove_ref();
  return Engines::RendezVousServant::_duplicate( ret );
}
