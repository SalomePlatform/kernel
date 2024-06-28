// Copyright (C) 2024  CEA, EDF
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

namespace SALOME
{
  class CONTAINER_EXPORT LockMasterImpl : public virtual POA_Engines::LockMaster
  {
  public:
    LockMasterImpl() = default;
    Engines::LockServant_ptr getLockerFor(const char *key) override;
    Engines::RendezVousServant_ptr buildRendezVous(CORBA::Long nbClients) override;
  };
}
