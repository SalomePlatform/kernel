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

#pragma once

#include "SALOME_Container.hxx"

#include <SALOMEconfig.h>

#include CORBA_SERVER_HEADER(SALOME_Locker)

#include <mutex>
#include <future>
#include <vector>

namespace SALOME
{
  class CONTAINER_EXPORT LockServantImpl : public virtual POA_Engines::LockServant
  {
  public:
    LockServantImpl() = default;
    void acquire() override;
    void release() override;
  private:
    std::mutex _mutex;
  };
  
  class CONTAINER_EXPORT RendezVousServantImpl : public virtual POA_Engines::RendezVousServant
  {
  public:
    RendezVousServantImpl(unsigned int nbOfClientsToWait);
    void acquire() override;
  private:
    std::vector< std::promise<void> > _promise;
    std::mutex _mutex;
    unsigned int _nb_clients;
  };
}
