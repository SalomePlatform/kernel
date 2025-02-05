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

#include "SALOME_LockServantImpl.hxx"

#include <stdexcept>

#include <iostream>

void SALOME::LockServantImpl::acquire()
{
  _mutex.lock();
}

void SALOME::LockServantImpl::release()
{
  _mutex.unlock();
}

SALOME::RendezVousServantImpl::RendezVousServantImpl(unsigned int nbOfClientsToWait):_nb_clients(nbOfClientsToWait)
{
  if( _nb_clients == 0 )
    throw std::runtime_error("RendezVousServantImpl is supposed to be >= 1!");
  _promise.resize( _nb_clients-1 );
}

void SALOME::RendezVousServantImpl::acquire()
{
  bool waitOrRelease = false;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    waitOrRelease = _nb_clients <= 1;
    if( _nb_clients != 0)
      _nb_clients--;
  }
  if( !waitOrRelease )//wait
    _promise[_nb_clients-1].get_future().wait();
  else//relase
  {
    for(auto& it : _promise)
      it.set_value();
  }
}
